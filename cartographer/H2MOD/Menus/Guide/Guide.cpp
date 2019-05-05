#include "../../../dllMain.h"
#include "Guide.h"
#include "../CustomMenuFunctions.h"
#include "../../Language/CustomLabels.h"
#include "../../Language/CustomLanguage.h"
#include "../../../Utils/Utils.h"
#include "../../Config/Config.h"
#include "../Credits/Credits.h"
#include "../AdvSettings/AdvSettings.h"
#include "../Error/Error.h"
#include "../Accounts/AccountManagement.h"
#include "../Accounts/AccountHelper.h"
#include "../../../xlive/xlive.h"
#include <Shellapi.h>

static void __fastcall c_list_widget_label_buttons(DWORD _ECX, DWORD _EDX, int a1, int a2)
{
	int(__thiscall *sub_211909)(int, int, int, int) = (int(__thiscall*)(int, int, int, int))(GetOffsetAddress(CLIENT_11122, 0x00211909));

	__int16 button_id = *(WORD*)(a1 + 112);
	int v3 = sub_211909(a1, 6, 0, 0);
	if (v3) {
		sub_21bf85_CMLTD(v3, button_id + 1, CMLabelMenuId_Guide);
	}
}

static int __fastcall widget_title_description(int a1, DWORD _EDX, char a2)
{
	return sub_2111ab_CMLTD(a1, a2, CMLabelMenuId_Guide, 0xFFFFFFF0, 0xFFFFFFF1);
}

static int __fastcall widget_button_handler(void *thisptr, DWORD _EDX, int a2, DWORD *a3)
{
	unsigned __int16 button_id = *a3 & 0xFFFF;

	if (button_id == 0) {
		CustomMenuCall_AdvSettings();
	}
	else if (button_id == 1) {
		CustomMenuCall_AccountManagement(0);
	}
	else if (button_id == 2) {
		ShellExecuteA(NULL, "open", "https://cartographer.online/", NULL, NULL, SW_SHOWDEFAULT);
	}
	else if (button_id == 3) {
		//CustomMenuCall_Credits();
		return CM_CloseMenu(thisptr);
	}
	else if (button_id == 4) {
		if (XShowGuideUI) {
			XShowGuideUI(0);
		}
		else {
			CustomMenuCall_Error_Inner(CMLabelMenuId_Error, 0xA, 0xB);
		}
	}
	else if (button_id == 5) {
		//TODO CustomMenuCall_Update();
		CustomMenuCall_Error_Inner(CMLabelMenuId_Error, 0x8, 0x9);
	}
	return CM_PressButtonAnimation(thisptr);
}

static int __fastcall widget_preselected_button(DWORD a1, DWORD _EDX)
{
	return sub_20F790_CM(a1, 0);//selected button id
}

static DWORD* menu_vftable_1 = 0;
static DWORD* menu_vftable_2 = 0;

static int __cdecl widget_call_head(int a1)
{
	char* guide_desc_base = H2CustomLanguageGetLabel(CMLabelMenuId_Guide, 0xFFFFFFF2);
	DWORD guide_desc_buflen = strlen(guide_desc_base) + 50;
	char* guide_description = (char*)malloc(guide_desc_buflen);
	char hotkeyname[20];
	GetVKeyCodeString(H2Config_hotkeyIdGuide, hotkeyname, 20);
	snprintf(guide_description, guide_desc_buflen, guide_desc_base, hotkeyname);
	add_cartographer_label(CMLabelMenuId_Guide, 0xFFFFFFF1, guide_description, true);
	free(guide_description);
	return CustomMenu_CallHead(a1, menu_vftable_1, menu_vftable_2, (DWORD)widget_button_handler, 6, 272);
}

static int(__cdecl *widget_function_ptr_helper())(int)
{
	return widget_call_head;
}

void CMSetupVFTables_Guide()
{
	CMSetupVFTables(&menu_vftable_1, &menu_vftable_2, (DWORD)c_list_widget_label_buttons, (DWORD)widget_title_description, (DWORD)widget_function_ptr_helper, (DWORD)widget_preselected_button, false, 0);
}

void CustomMenuCall_Guide()
{
	if (!isAccountingActiveHandle()) {
		CallWidget(widget_call_head);
	}
}
