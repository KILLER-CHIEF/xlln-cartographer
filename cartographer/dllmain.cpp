// dllmain.cpp : Defines the entry point for the DLL application.
#include "dllMain.h"
#include "xlive/xlive.h"
#include "H2MOD/TitlePatches.h"

HMODULE hTitleModule = 0;
DWORD Title_Version = 0;

DWORD GetOffsetAddress(DWORD dwTitleVersion, DWORD dwOffset)
{
	if (Title_Version != dwTitleVersion)
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
