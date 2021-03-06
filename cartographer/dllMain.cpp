// dllmain.cpp : Defines the entry point for the DLL application.
#include "dllMain.h"
#include "xlive/xlive.h"
#include "H2MOD/TitlePatches.h"
#include "Utils/Utils.h"
#include <sys/stat.h>

HMODULE hModModule = 0;
HMODULE hTitleModule = 0;
DWORD Title_Version = 0;
wchar_t lpwszTitleModulePath[MAX_PATH];
wchar_t *H2ProcessFilePath = 0;
wchar_t *H2AppDataLocal = 0;
wchar_t* FlagFilePathConfig = 0;

DWORD GetOffsetAddress(DWORD dwTitleVersion, DWORD dwOffset)
{
	if (Title_Version != dwTitleVersion && dwTitleVersion != TITLEMODULEANY)
		__debugbreak();
	return (DWORD)hTitleModule + dwOffset;
}

DWORD GetOffsetAddress(DWORD dwOffsetC_11081, DWORD dwOffsetC_11091, DWORD dwOffsetC_11122)
{
	if (Title_Version == CLIENT_11081)
		return (DWORD)hTitleModule + dwOffsetC_11081;
	if (Title_Version == CLIENT_11091)
		return (DWORD)hTitleModule + dwOffsetC_11091;
	if (Title_Version == CLIENT_11122)
		return (DWORD)hTitleModule + dwOffsetC_11122;
	__debugbreak();
	return 0;
}

static BOOL TitleIsHalo2(LPWSTR lpwszFilePath)
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

static void initLocalAppData()
{
	//addDebugText("Find AppData Local.");

	if (H2AppDataLocal != 0) {
		free(H2AppDataLocal);
		H2AppDataLocal = 0;
	}

	wchar_t* userprofile = _wgetenv(L"USERPROFILE");

	wchar_t local2[1024];

	swprintf(local2, 1024, L"%ws\\AppData\\Local\\", userprofile);
	struct _stat64i32 sb;
	if (_wstat(local2, &sb) == 0 && sb.st_mode & S_IFDIR) {
		swprintf(local2, 1024, L"%ws\\AppData\\Local\\Microsoft\\", userprofile);
		CreateDirectoryW(local2, NULL);
		int fperrno1 = GetLastError();
		if (fperrno1 == ERROR_ALREADY_EXISTS || fperrno1 == ERROR_SUCCESS) {
			swprintf(local2, 1024, L"%ws\\AppData\\Local\\Microsoft\\Halo 2\\", userprofile);
			CreateDirectoryW(local2, NULL);
			int fperrno1 = GetLastError();
			if (fperrno1 == ERROR_ALREADY_EXISTS || fperrno1 == ERROR_SUCCESS) {
				int appdatabuflen = wcslen(local2) + 1;
				H2AppDataLocal = (wchar_t*)malloc(sizeof(wchar_t) * appdatabuflen);
				wcscpy_s(H2AppDataLocal, appdatabuflen, local2);
			}
		}
	}
	else if (swprintf(local2, 1024, L"%ws\\Local Settings\\Application Data\\", userprofile), _wstat(local2, &sb) == 0 && sb.st_mode & S_IFDIR)
	{
		swprintf(local2, 1024, L"%ws\\Local Settings\\Application Data\\Microsoft\\", userprofile);
		CreateDirectoryW(local2, NULL);
		int fperrno1 = GetLastError();
		if (fperrno1 == ERROR_ALREADY_EXISTS || fperrno1 == ERROR_SUCCESS) {
			swprintf(local2, 1024, L"%ws\\Local Settings\\Application Data\\Microsoft\\Halo 2\\", userprofile);
			CreateDirectoryW(local2, NULL);
			int fperrno1 = GetLastError();
			if (fperrno1 == ERROR_ALREADY_EXISTS || fperrno1 == ERROR_SUCCESS) {
				int appdatabuflen = wcslen(local2) + 1;
				H2AppDataLocal = (wchar_t*)malloc(sizeof(wchar_t) * appdatabuflen);
				wcscpy_s(H2AppDataLocal, appdatabuflen, local2);
			}
		}
	}

	if (H2AppDataLocal == 0) {
		int appdatabuflen = wcslen(H2ProcessFilePath) + 1;
		H2AppDataLocal = (wchar_t*)malloc(sizeof(wchar_t) * appdatabuflen);
		swprintf(H2AppDataLocal, appdatabuflen, L"%ws", H2ProcessFilePath);
		//addDebugText("ERROR: Could not find AppData Local. Using Process File Path:");
	}
	else {
		//addDebugText("Found AppData Local:");
	}
	//addDebugText(H2AppDataLocal);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {

		hTitleModule = GetModuleHandle(NULL);
		if (!hTitleModule)
			return FALSE;

		GetModuleFileName(NULL, lpwszTitleModulePath, sizeof(lpwszTitleModulePath));
		DWORD dwLastError = GetLastError();
		if (dwLastError != ERROR_SUCCESS)
			return FALSE;

		if (!TitleIsHalo2(lpwszTitleModulePath))
			return FALSE;

		H2ProcessFilePath = (wchar_t*)malloc((wcslen(lpwszTitleModulePath) + 1) * sizeof(wchar_t));
		int rtncodepath = GetWidePathFromFullWideFilename(lpwszTitleModulePath, H2ProcessFilePath);
		if (rtncodepath == -1) {
			swprintf(H2ProcessFilePath, 2, L"");
		}
		initLocalAppData();

		int nArgs;
		LPWSTR* lpwszArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
		if (lpwszArglist != NULL) {
			for (int i = 1; i < nArgs; i++) {
				if (wcsstr(lpwszArglist[i], L"-pcartoconfig=") != NULL) {
					const DWORD flaglen = 14;
					if (wcslen(lpwszArglist[i] + flaglen) < 255) {
						int pfcbuflen = wcslen(lpwszArglist[i] + flaglen) + 1;
						FlagFilePathConfig = (wchar_t*)malloc(sizeof(wchar_t) * pfcbuflen);
						swprintf(FlagFilePathConfig, pfcbuflen, lpwszArglist[i] + flaglen);
					}
					else {
						//addDebugText("ERROR -pcartoconfig= filepath is too long.");
					}
				}
			}
		}

		if (!InitXLive()) {
			return FALSE;
		}
		if (!InitTitlePatches()) {
			UninitXLive();
			return FALSE;
		}
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
		UninitTitlePatches();
		UninitXLive();
	}
	else if (ul_reason_for_call == DLL_THREAD_ATTACH) {

	}
	else if (ul_reason_for_call == DLL_THREAD_DETACH) {

	}
	return TRUE;
}
