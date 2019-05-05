#include "../../../dllMain.h"
#include "AccountCreate.h"
#include "../CustomMenuFunctions.h"
#include "../../Language/CustomLabels.h"
#include "../../Language/CustomLanguage.h"
#include "../VirtualKB/VirtualKB.h"
#include "../Error/Error.h"
#include "AccountHelper.h"
#include "AccountList.h"

static void setup_button_labels()
{
	char* bufferUsername = H2CustomLanguageGetLabel(CMLabelMenuId_AccountCreate, 1);
	if (strlen(bufferUsername) <= 0) {
		char* bufferUsername2 = H2CustomLanguageGetLabel(CMLabelMenuId_AccountCreate, 0xFFFFFFF2);
		strcpy_s(bufferUsername, 32, bufferUsername2);
	}
	char* bufferEmail = H2CustomLanguageGetLabel(CMLabelMenuId_AccountCreate, 2);
	if (strlen(bufferEmail) <= 0) {
		char* bufferEmail2 = H2CustomLanguageGetLabel(CMLabelMenuId_AccountCreate, 0xFFFFFFF3);
		strcpy_s(bufferEmail, 255, bufferEmail2);
	}
	char* bufferPassword = H2CustomLanguageGetLabel(CMLabelMenuId_AccountCreate, 3);
	if (strlen(bufferPassword) <= 0) {
		char* bufferPassword2 = H2CustomLanguageGetLabel(CMLabelMenuId_AccountCreate, 0xFFFFFFF4);
		strcpy_s(bufferPassword, 255, bufferPassword2);
	}
}

static void __fastcall c_list_widget_label_buttons(DWORD _ECX, DWORD _EDX, int a1, int a2)
{
	int(__thiscall *sub_211909)(int, int, int, int) = (int(__thiscall*)(int, int, int, int))(GetOffsetAddress(CLIENT_11122, 0x00211909));

	__int16 button_id = *(WORD*)(a1 + 112);
	int v3 = sub_211909(a1, 6, 0, 0);
	if (v3) {
		sub_21bf85_CMLTD(v3, button_id + 1, CMLabelMenuId_AccountCreate);
	}
}

static int __fastcall widget_title_description(int a1, DWORD _EDX, char a2)
{
	return sub_2111ab_CMLTD(a1, a2, CMLabelMenuId_AccountCreate, 0xFFFFFFF0, 0xFFFFFFF1);
}

static int __fastcall widget_button_handler(void *thisptr, DWORD _EDX, int a2, DWORD *a3)
{
	unsigned __int16 button_id = *a3 & 0xFFFF;
	
	if (button_id == 0) {
		char* textBuffer = H2CustomLanguageGetLabel(CMLabelMenuId_AccountCreate, 1);
		CustomMenuCall_VKeyboard_Inner((wchar_t*)textBuffer, 16, 0b10010, CMLabelMenuId_AccountCreate, 0xFFFFF002, CMLabelMenuId_AccountCreate, 0xFFFFF003);
	}
	else if (button_id == 1) {
		char* textBuffer = H2CustomLanguageGetLabel(CMLabelMenuId_AccountCreate, 2);
		CustomMenuCall_VKeyboard_Inner((wchar_t*)textBuffer, 255, 0b10000, CMLabelMenuId_AccountCreate, 0xFFFFF004, CMLabelMenuId_AccountCreate, 0xFFFFF005);
	}
	else if (button_id == 2) {
		char* textBuffer = H2CustomLanguageGetLabel(CMLabelMenuId_AccountCreate, 3);
		CustomMenuCall_VKeyboard_Inner((wchar_t*)textBuffer, 255, 0b10000, CMLabelMenuId_AccountCreate, 0xFFFFF006, CMLabelMenuId_AccountCreate, 0xFFFFF007);
	}
	else if (button_id == 3) {
		CustomMenuCall_Error_Inner(CMLabelMenuId_Error, 0xFFFFF02C, 0xFFFFF02D);
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
	setup_button_labels();
	return CustomMenu_CallHead(a1, menu_vftable_1, menu_vftable_2, (DWORD)widget_button_handler, 4, 272);
}

static int(__cdecl *widget_function_ptr_helper())(int)
{
	return widget_call_head;
}

static void* __fastcall widget_deconstructor(LPVOID lpMem, DWORD _EDX, char a2)
{
	return sub_248beb_deconstructor(lpMem, _EDX, a2);
}

void CMSetupVFTables_AccountCreate()
{
	CMSetupVFTables(&menu_vftable_1, &menu_vftable_2, (DWORD)c_list_widget_label_buttons, (DWORD)widget_title_description, (DWORD)widget_function_ptr_helper, (DWORD)widget_preselected_button, true, (DWORD)widget_deconstructor);
}

void CustomMenuCall_AccountCreate()
{
	CallWidget(widget_call_head);
}
