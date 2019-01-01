#pragma once
#include "windows.h"

extern HMODULE hTitleModule;
#define CLIENT_11081 1
#define CLIENT_11091 2
#define CLIENT_11122 3
#define SERVER_11081 4
#define SERVER_11091 5
#define SERVER_11122 6
extern DWORD Title_Version;
extern wchar_t lpwszTitleModulePath[MAX_PATH];

DWORD GetOffsetAddress(DWORD dwTitleVersion, DWORD dwOffset);
DWORD GetOffsetAddress(DWORD dwOffsetC_11081, DWORD dwOffsetC_11091, DWORD dwOffsetC_11122);
