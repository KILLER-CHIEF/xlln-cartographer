#include "../../../dllMain.h"
#include "Error.h"
#include "../CustomMenuFunctions.h"
#include "../../Language/CustomLabels.h"
#include "../../Language/CustomLanguage.h"
#include "../VirtualKB/VirtualKB.h"

static void __fastcall c_list_widget_label_buttons(DWORD _ECX, DWORD _EDX, int a1, int a2)
{
	int(__thiscall* sub_211909)(int, int, int, int) = (int(__thiscall*)(int, int, int, int))(GetOffsetAddress(CLIENT_11122, 0x00211909));

	__int16 button_id = *(WORD*)(a1 + 112);
	int v3 = sub_211909(a1, 6, 0, 0);
	if (v3) {
		sub_21bf85_CMLTD(v3, button_id + 1, CMLabelMenuId_Error);
	}
}

static int __fastcall widget_title_description(int a1, DWORD _EDX, char a2)
{
	return sub_2111ab_CMLTD(a1, a2, CMLabelMenuId_Error, 0xFFFFFFF0, 0xFFFFFFF1);
}

void CustomMenuCall_Error_Inner();
static int __fastcall widget_button_handler(void *thisptr, DWORD _EDX, int a2, DWORD *a3)
{
	unsigned __int16 button_id = *a3 & 0xFFFF;
	if (button_id == 0) {
		CustomMenuCall_Error_Inner();
	}
	else if (button_id == 1) {
		CM_CloseMenuStack();
		return false;
	}
	else if (button_id == 2) {
		wchar_t* bufferLobbyName = (wchar_t*)H2CustomLanguageGetLabel(CMLabelMenuId_AdvLobbySettings, 0xFFFFF001);
		CustomMenuCall_VKeyboard_Inner(bufferLobbyName, 32, 0b11, CMLabelMenuId_AdvLobbySettings, 0xFFFFFF02, CMLabelMenuId_AdvLobbySettings, 0xFFFFFF03);
	}
	return CM_PressButtonAnimation(thisptr);
}

static int __fastcall widget_preselected_button(DWORD a1, DWORD _EDX)
{
	return sub_20F790_CM(a1, 0);//selected button id
}

int __cdecl CustomMenu_Error_Inner(int);

static int(__cdecl *CustomMenuFuncPtrHelp_Error_Inner())(int) {
	return CustomMenu_Error_Inner;
}

static DWORD* menu_vftable_1_Error_Inner = 0;
static DWORD* menu_vftable_2_Error_Inner = 0;

void CMSetupVFTables_Error_Inner()
{
	CMSetupVFTables(&menu_vftable_1_Error_Inner, &menu_vftable_2_Error_Inner, (DWORD)c_list_widget_label_buttons, (DWORD)widget_title_description, (DWORD)CustomMenuFuncPtrHelp_Error_Inner, (DWORD)widget_preselected_button, true, 0);
}

static int __cdecl CustomMenu_Error_Inner(int a1) {
	return CustomMenu_CallHead(a1, menu_vftable_1_Error_Inner, menu_vftable_2_Error_Inner, (DWORD)&widget_button_handler, 3, 272);
}

static void CustomMenuCall_Error_Inner() {
	if (Title_Version == CLIENT_11122) {
		CallWidget(CustomMenu_Error_Inner);
	}
	char* lblTitle = H2CustomLanguageGetLabel(CMLabelMenuId_Error, 0xFFFFFFF0);
	char* lblDesc = H2CustomLanguageGetLabel(CMLabelMenuId_Error, 0xFFFFFFF1);
	if (!lblTitle || !lblDesc) {
		if (lblTitle) {
			//addDebugText(lblTitle);
		}
		if (lblDesc) {
			//addDebugText(lblDesc);
		}
	}
	else {
		int debugTextBuflen = (strlen(lblTitle) + strlen(lblDesc) + 6) * sizeof(char);
		char* debugText = (char*)malloc(debugTextBuflen);
		snprintf(debugText, debugTextBuflen, "%s - %s", lblTitle, lblDesc);
		//addDebugText(debugText);
		free(debugText);
	}
}

static void CustomMenuCall_Error_Inner(char* title, char* description) {
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFFFF0, title, true);
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFFFF1, description, true);
	CustomMenuCall_Error_Inner();
}

void CustomMenuCall_Error_Inner(int menuIdTitle, int title, int menuIdDesc, int description)
{
	char* lblTitle = H2CustomLanguageGetLabel(menuIdTitle, title);
	char* lblDesc = H2CustomLanguageGetLabel(menuIdDesc, description);
	CustomMenuCall_Error_Inner(lblTitle, lblDesc);
}

void CustomMenuCall_Error_Inner(int menuId, int title, int description)
{
	CustomMenuCall_Error_Inner(menuId, title, menuId, description);
}

int __cdecl CustomMenu_Error_Outer(int);

static int(__cdecl *CustomMenuFuncPtrHelp_Error_Outer())(int) {
	return CustomMenu_Error_Outer;
}

static DWORD* menu_vftable_1_Error_Outer = 0;
static DWORD* menu_vftable_2_Error_Outer = 0;

void CMSetupVFTables_Error_Outer()
{
	CMSetupVFTables(&menu_vftable_1_Error_Outer, &menu_vftable_2_Error_Outer, (DWORD)c_list_widget_label_buttons, (DWORD)widget_title_description, (DWORD)CustomMenuFuncPtrHelp_Error_Outer, (DWORD)widget_preselected_button, false, 0);
}

static int __cdecl CustomMenu_Error_Outer(int a1) {
	return CustomMenu_CallHead(a1, menu_vftable_1_Error_Outer, menu_vftable_2_Error_Outer, (DWORD)&widget_button_handler, 2, 272);
}

static void CustomMenuCall_Error_Outer() {
	if (Title_Version == CLIENT_11122) {
		CallWidget(CustomMenu_Error_Outer);
	}
	char* lblTitle = H2CustomLanguageGetLabel(CMLabelMenuId_Error, 0xFFFFFFF0);
	char* lblDesc = H2CustomLanguageGetLabel(CMLabelMenuId_Error, 0xFFFFFFF1);
	int debugTextBuflen = (strlen(lblTitle) + strlen(lblDesc) + 6) * sizeof(char);
	char* debugText = (char*)malloc(debugTextBuflen);
	snprintf(debugText, debugTextBuflen, "%s - %s", lblTitle, lblDesc);
	//addDebugText(debugText);
	free(debugText);
}

static void CustomMenuCall_Error_Outer(char* title, char* description) {
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFFFF0, title, true);
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFFFF1, description, true);
	CustomMenuCall_Error_Outer();
}

void CustomMenuCall_Error_Outer(int menuIdTitle, int title, int menuIdDesc, int description)
{
	char* lblTitle = H2CustomLanguageGetLabel(menuIdTitle, title);
	char* lblDesc = H2CustomLanguageGetLabel(menuIdDesc, description);
	CustomMenuCall_Error_Outer(lblTitle, lblDesc);
}

void CustomMenuCall_Error_Outer(int menuId, int title, int description)
{
	CustomMenuCall_Error_Outer(menuId, title, menuId, description);
}
