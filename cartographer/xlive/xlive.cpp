#include "../dllMain.h"
#include "xlive.h"
#include "../Utils/UtilHook.h"
#include "../H2MOD/TitlePatches.h"

// #41140
typedef DWORD(WINAPI *tXLLNLogin)(DWORD dwUserIndex, BOOL bLiveEnabled, DWORD dwUserId, const CHAR *szUsername);
static tXLLNLogin XLLNLogin;

// #41141
typedef DWORD(WINAPI *tXLLNLogout)(DWORD dwUserIndex);
static tXLLNLogout XLLNLogout;

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

// #5001
typedef HRESULT(WINAPI *tXLiveInput)(XLIVE_INPUT_INFO *pPii);
static tXLiveInput XLiveInput;
static HRESULT WINAPI XLiveInputHook(XLIVE_INPUT_INFO *pPii);
static DWORD XLiveInputHookHelper = (DWORD)XLiveInputHook;
static HRESULT WINAPI XLiveInputHook(XLIVE_INPUT_INFO *pPii)
{
	// Undo hook.
	WritePointer(GetOffsetAddress(0x0000E892 + 2, 0x0000e892 + 2, 0x0000E892 + 2), &XLiveInput);
	XLiveInputHookHelper = 0;

	TPSetWindowResizable(pPii->hWnd);

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

BOOL InitXLive()
{
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

	}

	return TRUE;
}

BOOL UninitXLive()
{
	if (Title_Version == CLIENT_11081 || Title_Version == CLIENT_11091 || Title_Version == CLIENT_11122) {
		// Undo hooks.
		WritePointer(GetOffsetAddress(0x0000E880 + 2, 0x0000E880 + 2, 0x0000E880 + 2), &XLivePBufferSetByte);
		if (XLiveInputHookHelper)
			WritePointer(GetOffsetAddress(0x0000E892 + 2, 0x0000e892 + 2, 0x0000E892 + 2), &XLiveInput);
		WritePointer(GetOffsetAddress(0x002a7acd + 2, 0x002a861d + 2, 0x002a8769 + 2), &XShowSigninUI);
		WritePointer(GetOffsetAddress(0x0000e8a4 + 2, 0x0000e8a4 + 2, 0x0000e8a4 + 2), &XShowGuideUI);
	}
	return TRUE;
}
