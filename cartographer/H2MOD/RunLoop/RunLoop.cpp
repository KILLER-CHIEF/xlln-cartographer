#include "../../dllMain.h"
#include "RunLoop.h"
#include "../../Utils/UtilHook.h"
#include "../../xlive/xlive.h"
#include "../Menus/CustomMenuFunctions.h"

static BYTE* Quit_Exit_Game;
static bool QuitRunLoop = false;

static int H2Config_hotkeyIdGuide = VK_HOME;
static void hotkeyFuncGuide()
{
	if (Title_Version == CLIENT_11081 || Title_Version == CLIENT_11091|| Title_Version == CLIENT_11122) {
		// #5215
		typedef DWORD(WINAPI *tXShowGuideUI)(DWORD dwUserIndex);
		tXShowGuideUI AnyXShowGuideUI = (tXShowGuideUI)**(DWORD**)(GetOffsetAddress(0x0000e8a4 + 2, 0x0000e8a4 + 2, 0x0000e8a4 + 2));
		AnyXShowGuideUI(0);
	}
}

static int H2Config_hotkeyIdTestA = VK_F6;
static void hotkeyFuncTestA()
{
	if (Title_Version == CLIENT_11122) {
		//CallWidget((void*)GetOffsetAddress(CLIENT_11122, 0x0021f681));// About dialog.
		//CallWidget((void*)GetOffsetAddress(CLIENT_11122, 0x0025692c));// keyboard layout.
		//CallWidget((void*)GetOffsetAddress(CLIENT_11122, 0x00259d05));// thumbstick layout.
		//CallWidget((void*)GetOffsetAddress(CLIENT_11122, 0x00259dc0));// button layout.
		//CallWidget(CustomMenu_Obscure);
		//CallWidget((void*)GetOffsetAddress(CLIENT_11122, 0x0021f5f8));// Network Adapter list.
		//CallWidget((void*)GetOffsetAddress(CLIENT_11122, 0x00258c8c));// Brightness.
		int result;
		
		

		DWORD d = GetOffsetAddress(CLIENT_11122, 0x00971d24);
		DWORD &d8 = *(DWORD*)(d + (0x44 * 4) + 0x8);
		DWORD &dc = *(DWORD*)(d + (0x44 * 4) + 0xc);
		DWORD &d30 = *(DWORD*)(d + (0x44 * 4) + 0x30);

		int(__cdecl* sub_002096da)(int a1, int a2) = (int(__cdecl*)(int, int))GetOffsetAddress(CLIENT_11122, 0x002096da);
		if (d8 || d30) {
			while (d8 || d30) {
				result = sub_002096da(4, 4);
				XLLNDebugLog(0, "A");
			}
			XLLNDebugLog(0, "SWAP");
			d8 = dc;
			while (d8 || dc || d30) {
				result = sub_002096da(4, 4);
				XLLNDebugLog(0, "B");
			}
		}

		XLLNDebugLog(0, "Dumb");
	}
}

static const int hotkeyLen = 2;
static int hotkeyListenLen = 2;
static int* hotkeyId[hotkeyLen] = { &H2Config_hotkeyIdGuide, &H2Config_hotkeyIdTestA };
static bool hotkeyPressed[hotkeyLen] = { false, false };
static void(*hotkeyFunc[hotkeyLen])(void) = { hotkeyFuncGuide, hotkeyFuncTestA };


static void RunLoop()
{
	if ((Title_Version == CLIENT_11122 || Title_Version == CLIENT_11081 || Title_Version == CLIENT_11091) && (GetFocus() == H2hWnd || GetForegroundWindow() == H2hWnd)) {

		for (int i = 0; i < hotkeyListenLen; i++) {
			//& 0x8000 is pressed
			//& 0x1 Key just transitioned from released to pressed.
			if (GetAsyncKeyState(*hotkeyId[i]) & 0x8000) {
				hotkeyPressed[i] = true;
			}
			else if (!(GetAsyncKeyState(*hotkeyId[i]) & 0x8000) && hotkeyPressed[i]) {
				hotkeyPressed[i] = false;
				hotkeyFunc[i]();
			}
		}
	}
}

static signed int(*sub_287a1)();
static signed int HookedClientRandFunc()
{
	if (!QuitRunLoop)
		RunLoop();

	signed int result = sub_287a1();
	return result;
}

static char HookedServerShutdownCheck()
{
	if (!QuitRunLoop)
		RunLoop();

	if (*Quit_Exit_Game) {
		//DeinitH2Startup();
	}

	//original test - if game should shutdown
	return *Quit_Exit_Game;
}

BOOL InitRunLoop()
{
	if (Title_Version == SERVER_11122) {
		Quit_Exit_Game = (BYTE*)GetOffsetAddress(CLIENT_11122, 0x004a7083);
		PatchCall(GetOffsetAddress(CLIENT_11122, 0x0000c6cb), HookedServerShutdownCheck);
	}
	else if (Title_Version == CLIENT_11122) {
		Quit_Exit_Game = (BYTE*)GetOffsetAddress(CLIENT_11122, 0x0048220b);
		sub_287a1 = (signed int(*)())GetOffsetAddress(CLIENT_11122, 0x000287a1);
		PatchCall(GetOffsetAddress(CLIENT_11122, 0x000399f3), HookedClientRandFunc);
	}

	return TRUE;
}

BOOL UninitRunLoop()
{
	return TRUE;
}
