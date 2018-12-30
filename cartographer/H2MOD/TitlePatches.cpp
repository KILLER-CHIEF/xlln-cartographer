#include "../dllMain.h"
#include "TitlePatches.h"
#include "../Utils/UtilHook.h"

VOID TPSetWindowResizable(HWND hWindow)
{
	// Set game window as resizable.
	SetWindowLong(hWindow, GWL_STYLE, GetWindowLong(hWindow, GWL_STYLE) | WS_SIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
}

BOOL InitTitlePatches()
{
	if (Title_Version == CLIENT_11081 || Title_Version == CLIENT_11091 || Title_Version == CLIENT_11122) {
		wchar_t* var_c0039bcf0 = (wchar_t*)(GetOffsetAddress(0x0039bce8, 0x0039bce8, 0x0039bcf0));//"Global\\Halo2AlreadyRunning"
		WriteBytes((DWORD)var_c0039bcf0, L"", 2);
	}

	return TRUE;
}

BOOL UninitTitlePatches()
{
	return TRUE;
}
