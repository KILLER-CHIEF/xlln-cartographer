#include "../../../dllMain.h"
#include "NetworkType.h"
#include "../CustomMenuFunctions.h"
#include "../../Language/CustomLabels.h"
#include "../Error/Error.h"
#include "../../Language/CustomLanguage.h"
#include "../VirtualKB/VirtualKB.h"
#include "../../../xlive/xlive.h"

static bool Online_Mode = false;

static void __fastcall c_list_widget_label_buttons(DWORD _ECX, DWORD _EDX, int a1, int a2)
{
	int(__thiscall *sub_211909)(int, int, int, int) = (int(__thiscall*)(int, int, int, int))(GetOffsetAddress(CLIENT_11122, 0x00211909));

	__int16 button_id = *(WORD*)(a1 + 112);
	int v3 = sub_211909(a1, 6, 0, 0);
	if (v3) {
		sub_21bf85_CMLTD(v3, button_id + 1 + (Online_Mode ? 0 : 10), CMLabelMenuId_NetworkType);
	}
}

static int __fastcall widget_title_description(int a1, DWORD _EDX, char a2)
{
	return sub_2111ab_CMLTD(a1, a2, CMLabelMenuId_NetworkType, Online_Mode ? 0xFFFFFFF2 : 0xFFFFFFF0, Online_Mode ? 0xFFFFFFF3 : 0xFFFFFFF1);
}

static int __fastcall widget_button_handler(void *thisptr, DWORD _EDX, int a2, DWORD *a3)
{
	unsigned __int16 button_id = *a3 & 0xFFFF;
	
	if (button_id == 0) {
		int result = CM_CloseMenu(thisptr);
		int(__stdcall* sub_C9B257)(int a1) = (int(__stdcall*)(int))GetOffsetAddress(CLIENT_11122, 0x0000b257);
		sub_C9B257((int)a2);
		return result;
	}
	else if (button_id == 1) {
		int(__stdcall* sub_C9A978)(int a1) = (int(__stdcall*)(int))GetOffsetAddress(CLIENT_11122, 0x0000a978);
		sub_C9A978((int)a2);
		return CM_CloseMenu(thisptr);
	}
	else if (button_id == 2) {
		wchar_t* bufferhost = (wchar_t*)H2CustomLanguageGetLabel(CMLabelMenuId_NetworkType, 0xFFFFF001);
		CustomMenuCall_VKeyboard_Inner(bufferhost, 22, 0b10, CMLabelMenuId_NetworkType, 0xFFFFFF02, CMLabelMenuId_NetworkType, 0xFFFFFF03);
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
	return CustomMenu_CallHead(a1, menu_vftable_1, menu_vftable_2, (DWORD)widget_button_handler, Online_Mode ? 3 : 2, 272);
}

static int(__cdecl *widget_function_ptr_helper())(int)
{
	return widget_call_head;
}

void CMSetupVFTables_NetworkType()
{
	CMSetupVFTables(&menu_vftable_1, &menu_vftable_2, (DWORD)c_list_widget_label_buttons, (DWORD)widget_title_description, (DWORD)widget_function_ptr_helper, (DWORD)widget_preselected_button, true, 0);
}

void CustomMenuCall_NetworkType(bool onlineMode)
{
	DWORD online_mode = onlineMode ? 1 : 0;
	if (XLLNOnlineSetMode) {
		DWORD result = XLLNOnlineSetMode(&online_mode);
		OnlineModeFunctional = !result;
	}
	else {
		online_mode = 0;
		OnlineModeFunctional = FALSE;
	}
	Online_Mode = online_mode != 0;
	
	CallWidget(widget_call_head);
}
