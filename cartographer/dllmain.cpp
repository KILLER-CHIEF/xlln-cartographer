// dllmain.cpp : Defines the entry point for the DLL application.
#include <windows.h>

HMODULE hTitleModule = 0;
#define CLIENT_11081 1
#define CLIENT_11091 2
#define CLIENT_11122 3
#define SERVER_11081 4
#define SERVER_11091 5
#define SERVER_11122 6
DWORD Title_Version = 0;

DWORD GetOffsetAddress(DWORD dwTitleVersion, DWORD dwOffset) {
	if (Title_Version != dwTitleVersion)
		__debugbreak();
	return (DWORD)hTitleModule + dwOffset;
}

DWORD GetOffsetAddress(DWORD dwOffset_11081, DWORD dwOffset_11091, DWORD dwOffset_11122) {
	if (Title_Version == CLIENT_11081)
		return (DWORD)hTitleModule + dwOffset_11081;
	if (Title_Version == CLIENT_11091)
		return (DWORD)hTitleModule + dwOffset_11091;
	if (Title_Version == CLIENT_11122)
		return (DWORD)hTitleModule + dwOffset_11122;
	__debugbreak();
	return 0;
}

// #41140
typedef DWORD(WINAPI *tXLLNLogin)(DWORD dwUserIndex, BOOL bLiveEnabled, DWORD dwUserId, const CHAR *szUsername);
static tXLLNLogin XLLNLogin;

// #41141
typedef DWORD(WINAPI *tXLLNLogout)(DWORD dwUserIndex);
static tXLLNLogout XLLNLogout;

void WriteBytes(DWORD destAddress, LPVOID bytesToWrite, int numBytes)
{
	DWORD OldProtection;
	DWORD temp;

	VirtualProtect((LPVOID)destAddress, numBytes, PAGE_EXECUTE_READWRITE, &OldProtection);
	memcpy((LPVOID)destAddress, bytesToWrite, numBytes);
	VirtualProtect((LPVOID)destAddress, numBytes, OldProtection, &temp); //quick fix for exception that happens here
}

void WritePointer(DWORD offset, void *ptr) {
	BYTE* pbyte = (BYTE*)&ptr;
	BYTE assmNewFuncRel[0x4] = { pbyte[0], pbyte[1], pbyte[2], pbyte[3] };
	WriteBytes(offset, assmNewFuncRel, 0x4);
}

// #5019
typedef HRESULT(WINAPI *tXLivePBufferSetByte)(void *xebBuffer, DWORD dwOffset, BYTE ucValue);
static tXLivePBufferSetByte XLivePBufferSetByte;
static HRESULT WINAPI XLivePBufferSetByteHook(void *xebBuffer, DWORD dwOffset, BYTE ucValue)
{
	ucValue = 0;
	HRESULT result = XLivePBufferSetByte(xebBuffer, dwOffset, ucValue);
	return result;
}
static DWORD XLivePBufferSetByteHookHelper = (DWORD)XLivePBufferSetByteHook;


typedef struct XLIVE_INPUT_INFO {
	UINT cbSize;
	HWND hWnd;
	UINT uMSG;
	WPARAM wParam;
	LPARAM lParam;
	BOOL fHandled;
	LRESULT lRet;
} XLIVE_INPUT_INFO;

// #5001
typedef HRESULT (WINAPI *tXLiveInput)(XLIVE_INPUT_INFO *pPii);
static tXLiveInput XLiveInput;
static HRESULT WINAPI XLiveInputHook(XLIVE_INPUT_INFO *pPii);
static DWORD XLiveInputHookHelper = (DWORD)XLiveInputHook;
static HRESULT WINAPI XLiveInputHook(XLIVE_INPUT_INFO *pPii)
{
	// Undo hook.
	WritePointer(GetOffsetAddress(0x0000E892 + 2, 0x0000e892 + 2, 0x0000E892 + 2), &XLiveInput);
	XLiveInputHookHelper = 0;

	// Set game window as resizable.
	SetWindowLong(pPii->hWnd, GWL_STYLE, GetWindowLong(pPii->hWnd, GWL_STYLE) | WS_SIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);

	HRESULT result = XLiveInput(pPii);
	return result;
}


// #5260
typedef DWORD(WINAPI *tXShowSigninUI)(DWORD cPanes, DWORD dwFlags);
static tXShowSigninUI XShowSigninUI;
static DWORD WINAPI XShowSigninUIHook(DWORD cPanes, DWORD dwFlags)
{
	//DWORD result = XShowSigninUI(cPanes, dwFlags);
	DWORD result = XLLNLogin(0, TRUE, 0, 0);
	return result;
}
static DWORD XShowSigninUIHookHelper = (DWORD)XShowSigninUIHook;

// #5215
typedef DWORD(WINAPI *tXShowGuideUI)(DWORD dwUserIndex);
static tXShowGuideUI XShowGuideUI;
static DWORD WINAPI XShowGuideUIHook(DWORD dwUserIndex)
{
	DWORD result = XShowGuideUI(dwUserIndex);
	//DWORD result = XLLNLogout(0);
	return result;
}
static DWORD XShowGuideUIHookHelper = (DWORD)XShowGuideUIHook;


BOOL TitleIsHalo2(LPWSTR lpwszFilePath)
{
	DWORD dwDummyHandle;
	DWORD dwSize = GetFileVersionInfoSize(lpwszFilePath, &dwDummyHandle);
	if (!dwSize)
		return FALSE;
	LPVOID buf = (void*)malloc(dwSize);
	if (!GetFileVersionInfo(lpwszFilePath, dwDummyHandle, dwSize, buf)) {
		free(buf);
		return FALSE;
	}
	UINT uLen;
	wchar_t *pwszOriginalFilename;
	if (!VerQueryValue(buf, L"\\StringFileInfo\\040904b0\\OriginalFilename", (LPVOID*)&pwszOriginalFilename, &uLen) || !uLen) {
		free(buf);
		return FALSE;
	}
	BOOL bIsDedi = FALSE;
	if (wcscmp(pwszOriginalFilename, L"Halo2.exe") != 0) {
		bIsDedi = TRUE;
		if (wcscmp(pwszOriginalFilename, L"h2server.exe") != 0) {
			free(buf);
			return FALSE;
		}
	}
	LPBYTE lpBuffer = NULL;
	if (!VerQueryValue(buf, L"\\", (LPVOID*)&lpBuffer, &uLen) || !uLen) {
		free(buf);
		return FALSE;
	}
	VS_FIXEDFILEINFO *verInfo = (VS_FIXEDFILEINFO*)lpBuffer;
	if (verInfo->dwSignature != 0xfeef04bd) {
		free(buf);
		return FALSE;
	}

	WORD Version_Major = (verInfo->dwFileVersionMS >> 16) & 0xffff;
	WORD Version_Minor = (verInfo->dwFileVersionMS >> 0) & 0xffff;
	WORD Version_Revision = (verInfo->dwFileVersionLS >> 16) & 0xffff;
	WORD Version_Build = (verInfo->dwFileVersionLS >> 0) & 0xffff;
	free(buf);

	if (Version_Major != 1 || Version_Minor != 0 || Version_Revision != 0)
		return FALSE;
	if (Version_Build == 11081)
		if (bIsDedi)
			Title_Version = SERVER_11081;
		else
			Title_Version = CLIENT_11081;
	else if (Version_Build == 11091)
		if (bIsDedi)
			Title_Version = SERVER_11091;
		else
			Title_Version = CLIENT_11091;
	else if (Version_Build == 11122)
		if (bIsDedi)
			Title_Version = SERVER_11122;
		else
			Title_Version = CLIENT_11122;
	else
		return FALSE;
	return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {

		hTitleModule = GetModuleHandle(NULL);
		if (!hTitleModule)
			return FALSE;

		wchar_t lpwszTitleModulePath[MAX_PATH];
		GetModuleFileName(NULL, lpwszTitleModulePath, sizeof(lpwszTitleModulePath));
		DWORD dwLastError = GetLastError();
		if (dwLastError != ERROR_SUCCESS)
			return FALSE;

		if (!TitleIsHalo2(lpwszTitleModulePath))
			return FALSE;

		// Get XLLN exports.
		HMODULE hModDll = GetModuleHandle(L"xlive.dll");
		if (!hModDll)
			return FALSE;
		XLLNLogin = (tXLLNLogin)GetProcAddress(hModDll, (PCSTR)41140);
		if (!XLLNLogin)
			return FALSE;
		XLLNLogout = (tXLLNLogout)GetProcAddress(hModDll, (PCSTR)41141);
		if (!XLLNLogout)
			return FALSE;

		if (Title_Version == CLIENT_11081 || Title_Version == CLIENT_11091 || Title_Version == CLIENT_11122) {
			// Hook imports.
			XLivePBufferSetByte = (tXLivePBufferSetByte)**(DWORD**)(GetOffsetAddress(0x0000E880 + 2, 0x0000E880 + 2, 0x0000E880 + 2));
			WritePointer(GetOffsetAddress(0x0000E880 + 2, 0x0000E880 + 2, 0x0000E880 + 2), &XLivePBufferSetByteHookHelper);

			XLiveInput = (tXLiveInput)**(DWORD**)(GetOffsetAddress(0x0000E892 + 2, 0x0000e892 + 2, 0x0000E892 + 2));
			WritePointer(GetOffsetAddress(0x0000E892 + 2, 0x0000e892 + 2, 0x0000E892 + 2), &XLiveInputHookHelper);

			XShowSigninUI = (tXShowSigninUI)**(DWORD**)(GetOffsetAddress(0x002a7acd + 2, 0x002a861d + 2, 0x002a8769 + 2));
			WritePointer(GetOffsetAddress(0x002a7acd + 2, 0x002a861d + 2, 0x002a8769 + 2), &XShowSigninUIHookHelper);

			XShowGuideUI = (tXShowGuideUI)**(DWORD**)(GetOffsetAddress(0x0000e8a4 + 2, 0x0000e8a4 + 2, 0x0000e8a4 + 2));
			WritePointer(GetOffsetAddress(0x0000e8a4 + 2, 0x0000e8a4 + 2, 0x0000e8a4 + 2), &XShowGuideUIHookHelper);

			// Patches.
			wchar_t* var_c0039bcf0 = (wchar_t*)(GetOffsetAddress(0x0039bce8, 0x0039bce8, 0x0039bcf0));//"Global\\Halo2AlreadyRunning"
			WriteBytes((DWORD)var_c0039bcf0, L"", 2);
		}

		return TRUE;
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
		if (Title_Version == CLIENT_11081 || Title_Version == CLIENT_11091 || Title_Version == CLIENT_11122) {
			// Undo hooks.
			WritePointer(GetOffsetAddress(0x0000E880 + 2, 0x0000E880 + 2, 0x0000E880 + 2), &XLivePBufferSetByte);
			if (XLiveInputHookHelper)
				WritePointer(GetOffsetAddress(0x0000E892 + 2, 0x0000e892 + 2, 0x0000E892 + 2), &XLiveInput);
			WritePointer(GetOffsetAddress(0x002a7acd + 2, 0x002a861d + 2, 0x002a8769 + 2), &XShowSigninUI);
			WritePointer(GetOffsetAddress(0x0000e8a4 + 2, 0x0000e8a4 + 2, 0x0000e8a4 + 2), &XShowGuideUI);
		}
	}
	else if (ul_reason_for_call == DLL_THREAD_ATTACH) {

	}
	else if (ul_reason_for_call == DLL_THREAD_DETACH) {

	}
	return TRUE;
}
