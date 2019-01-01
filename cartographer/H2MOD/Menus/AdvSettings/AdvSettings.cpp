#include "../../../dllMain.h"
#include "AdvSettings.h"
#include "../CustomMenuFunctions.h"
#include "../../Language/CustomLabels.h"
#include "Language/LanguageMain.h"

static void __fastcall c_list_widget_label_buttons(DWORD _ECX, DWORD _EDX, int a1, int a2)
{
	int(__thiscall *sub_211909)(int, int, int, int) = (int(__thiscall*)(int, int, int, int))(GetOffsetAddress(CLIENT_11122, 0x00211909));

	__int16 button_id = *(WORD*)(a1 + 112);
	int v3 = sub_211909(a1, 6, 0, 0);
	if (v3) {
		sub_21bf85_CMLTD(v3, button_id + 1, CMLabelMenuId_AdvSettings);
	}
}

static int __fastcall widget_title_description(int a1, DWORD _EDX, char a2)
{
	return sub_2111ab_CMLTD(a1, a2, CMLabelMenuId_AdvSettings, 0xFFFFFFF0, 0xFFFFFFF1);
}

static bool widget_button_handler(int button_id)
{
	if (button_id == 0) {
		CustomMenuCall_LanguageMain();
	}
	else if (button_id == 1) {
		//CustomMenuCall_EditHudGui();
	}
	else if (button_id == 2) {
		//CustomMenuCall_OtherSettings();
	}
	else if (button_id == 3) {
		//CustomMenuCall_ToggleSkulls();
	}
	else if (button_id == 4) {
		//CustomMenuCall_AdvLobbySettings();
	}
	return false;
}

static int __fastcall widget_preselected_button(DWORD a1, DWORD _EDX)
{
	return sub_20F790_CM(a1, 0);//selected button id
}

static DWORD* menu_vftable_1 = 0;
static DWORD* menu_vftable_2 = 0;

static int __cdecl widget_call_head(int a1)
{
	//TODO AdvSettings button count AdvLobbySettings
	return CustomMenu_CallHead(a1, menu_vftable_1, menu_vftable_2, (DWORD)widget_button_handler, 5, 272);
}

static int(__cdecl *widget_function_ptr_helper())(int)
{
	return widget_call_head;
}

void CMSetupVFTables_AdvSettings()
{
	CMSetupVFTables(&menu_vftable_1, &menu_vftable_2, (DWORD)c_list_widget_label_buttons, (DWORD)widget_title_description, (DWORD)widget_function_ptr_helper, (DWORD)widget_preselected_button, true, 0);
}

void CustomMenuCall_AdvSettings()
{
	int WgitScreenfunctionPtr = (int)(widget_call_head);
	CallWgit(WgitScreenfunctionPtr);
}
