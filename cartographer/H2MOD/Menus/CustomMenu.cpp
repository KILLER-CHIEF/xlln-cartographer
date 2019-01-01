#include "../../dllMain.h"
#include "CustomMenu.h"
#include "CustomMenuFunctions.h"
#include "../../Utils/UtilHook.h"
#include "../Language/CustomLabels.h"
#include "Error/Error.h"
#include "VirtualKB/VirtualKB.h"
#include "Guide/Guide.h"
#include "Credits/Credits.h"
#include "AdvSettings/AdvSettings.h"
#include "AdvSettings/Language/LanguageMain.h"
#include "AdvSettings/Language/LanguageSub.h"

typedef int(__stdcall *tbtnhandler)(void* thisptr, int a2, int a3);
tbtnhandler pbtnHandler;
int __stdcall BtnHandlerCaller(void* thisptr, int a2, int a3) {
	int result = 0;//doesn't /always/ matter
	//addDebugText("Hooked Button Press Handler.");
	//result = pbtnHandler(thisptr, a2, a3);
	int firstFuncPtr = *(int*)(*(DWORD*)thisptr - 4);
	if (!firstFuncPtr) {
		bool(__cdecl* customHandlerFunc)(int) = (bool(__cdecl*)(int))(*(int*)(*(DWORD*)thisptr + 16));
		int button_id = *(int*)a3 & 0xFFFF;
		bool success = customHandlerFunc(button_id);

		if (success) {
			int v3 = ((int)thisptr) - 0xac;
			int v4 = (*(int(__thiscall**)(int))(*(DWORD*)v3 + 56))(v3);//__thiscall
			int v5 = (*(int(__thiscall**)(int))(*(DWORD*)v3 + 52))(v3);//__thiscall

			int(__cdecl* sub_5A96DA)(int a1, int a2);
			sub_5A96DA = (int(__cdecl*)(int, int))GetOffsetAddress(CLIENT_11122, 0x002096da);
			result = sub_5A96DA(v5, v4);
		}
	}
	else {
		for (int i = *(DWORD*)thisptr; i; i = *(DWORD*)(i + 4)) {
			result = (**(int(__thiscall ***)(DWORD, DWORD, DWORD))(i - 4))(i - 4, a2, a3);
		}
	}
	return result;
}

BOOL CMGuideMenu()
{
	if (Title_Version == CLIENT_11122) {
		CustomMenuCall_Guide();
		//Override XLLN GUIDE Menu.
		return TRUE;
	}
	return FALSE;
}

BOOL InitCustomMenu()
{
	if (Title_Version == CLIENT_11122) {
		InitCMFunctions();
		CMSetupVFTables_Error_Inner();
		CMSetupVFTables_Error_Outer();
		CMSetupVFTablesVKb_VKeyTest();
		CMSetupVFTables_Guide();
		CMSetupVFTables_Credits();
		CMSetupVFTables_AdvSettings();
		CMSetupVFTables_LanguageMain();
		CMSetupVFTables_LanguageSub();

		DWORD dwBack;
		pbtnHandler = (tbtnhandler)DetourClassFunc((BYTE*)GetOffsetAddress(CLIENT_11122, 0x00213af2), (BYTE*)BtnHandlerCaller, 8);
		VirtualProtect(pbtnHandler, 4, PAGE_EXECUTE_READWRITE, &dwBack);
	}

	return TRUE;
}

BOOL UninitCustomMenu()
{
	return TRUE;
}
