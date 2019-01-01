#include "../../../../dllMain.h"
#include "LanguageMain.h"
#include "../../CustomMenuFunctions.h"
#include "../../../Language/CustomLabels.h"
#include "../../../Language/CustomLanguage.h"
#include "../../../Config/Config.h"
#include "../../Error/Error.h"
#include "LanguageSub.h"

static void languageCaptureSetLabel()
{
	if (H2Config_custom_labels_capture_missing) {
		add_cartographer_label(CMLabelMenuId_Language, 13, H2CustomLanguageGetLabel(CMLabelMenuId_Language, 0xFFFFF002), true);
	}
	else {
		char* asdg = H2CustomLanguageGetLabel(CMLabelMenuId_Language, 0xFFFFF003);
		add_cartographer_label(CMLabelMenuId_Language, 13, asdg, true);
	}
}

static void toggleLanguageCapture()
{
	H2Config_custom_labels_capture_missing = !H2Config_custom_labels_capture_missing;
	languageCaptureSetLabel();
}

static void __fastcall c_list_widget_label_buttons(DWORD _ECX, DWORD _EDX, int a1, int a2)
{
	int(__thiscall *sub_211909)(int, int, int, int) = (int(__thiscall*)(int, int, int, int))(GetOffsetAddress(CLIENT_11122, 0x00211909));

	__int16 button_id = *(WORD*)(a1 + 112);
	int v3 = sub_211909(a1, 6, 0, 0);
	if (v3) {
		sub_21bf85_CMLTD(v3, button_id + 1, CMLabelMenuId_Language);
	}
}

static int __fastcall widget_title_description(int a1, DWORD _EDX, char a2)
{
	return sub_2111ab_CMLTD(a1, a2, CMLabelMenuId_Language, 0xFFFFFFF0, 0xFFFFFFF1);
}

static bool widget_button_handler(int button_id)
{
	if (button_id == 0) {
		setCustomLanguage(-1);
		return true;
	}
	else if (button_id > 0 && button_id < 2 + 8) {
		CustomMenuCall_LanguageSub(button_id - 2);
		return false;
	}
	else if (button_id == 2 + 8 + 0) {
		if (!CLReloadCustomLanguages()) {
			CustomMenuCall_Error_Inner(CMLabelMenuId_Error, 6, 7);
		}
		return false;
	}
	else if (button_id == 2 + 8 + 1) {
		CLSaveCustomLanguages();
		return true;
	}
	else if (button_id == 2 + 8 + 2) {
		toggleLanguageCapture();
		return false;
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
	languageCaptureSetLabel();
	return CustomMenu_CallHead(a1, menu_vftable_1, menu_vftable_2, (DWORD)widget_button_handler, 13, 272);
}

static int(__cdecl *widget_function_ptr_helper())(int)
{
	return widget_call_head;
}

void CMSetupVFTables_LanguageMain()
{
	CMSetupVFTables(&menu_vftable_1, &menu_vftable_2, (DWORD)c_list_widget_label_buttons, (DWORD)widget_title_description, (DWORD)widget_function_ptr_helper, (DWORD)widget_preselected_button, true, 0);
}

void CustomMenuCall_LanguageMain()
{
	int WgitScreenfunctionPtr = (int)(widget_call_head);
	CallWgit(WgitScreenfunctionPtr);
}
