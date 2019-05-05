#include "../../../dllMain.h"
#include "AccountList.h"
#include "../CustomMenuFunctions.h"
#include "../../Language/CustomLabels.h"
#include "../../Language/CustomLanguage.h"
#include "../Error/Error.h"
#include "AccountCreate.h"
#include "AccountAdd.h"
#include "../../Config/Accounts.h"
#include "AccountManagement.h"
#include "AccountHelper.h"
#include "../../../xlive/xlive.h"

static bool mode_remove_account = false;
static DWORD signin_user_id = 0;

static void setup_button_labels()
{
	mode_remove_account = false;

	for (int i = 0; i < H2AccountCount; i++) {
		add_cartographer_label(CMLabelMenuId_AccountList, 1 + i, H2AccountBufferUsername[i] ? H2AccountBufferUsername[i] : H2CustomLanguageGetLabel(CMLabelMenuId_AccountList, 0xFFFF0005), true);
	}

	add_cartographer_label(CMLabelMenuId_AccountList, 1 + H2AccountCount, H2CustomLanguageGetLabel(CMLabelMenuId_AccountList, 0xFFFF0004), true);
	add_cartographer_label(CMLabelMenuId_AccountList, 1 + H2AccountCount + 1, H2CustomLanguageGetLabel(CMLabelMenuId_AccountList, 0xFFFF0003), true);
	add_cartographer_label(CMLabelMenuId_AccountList, 1 + H2AccountCount + 2, H2CustomLanguageGetLabel(CMLabelMenuId_AccountList, 0xFFFF0000), true);
	add_cartographer_label(CMLabelMenuId_AccountList, 1 + H2AccountCount + 3, H2CustomLanguageGetLabel(CMLabelMenuId_AccountList, 0xFFFF0001 + (mode_remove_account ? 1 : 0)), true);
}

static void __fastcall c_list_widget_label_buttons(DWORD _ECX, DWORD _EDX, int a1, int a2)
{
	int(__thiscall *sub_211909)(int, int, int, int) = (int(__thiscall*)(int, int, int, int))(GetOffsetAddress(CLIENT_11122, 0x00211909));

	__int16 button_id = *(WORD*)(a1 + 112);
	int v3 = sub_211909(a1, 6, 0, 0);
	if (v3) {
		sub_21bf85_CMLTD(v3, button_id + 1, CMLabelMenuId_AccountList);
	}
}

static int __fastcall widget_title_description(int a1, DWORD _EDX, char a2)
{
	return sub_2111ab_CMLTD(a1, a2, CMLabelMenuId_AccountList, 0xFFFFFFF0, 0xFFFFFFF1);
}

static int __fastcall widget_button_handler(void *thisptr, DWORD _EDX, int a2, DWORD *a3)
{
	unsigned __int16 button_id = *a3 & 0xFFFF;
	
	if (button_id == H2AccountCount + 1) {
		if (!mode_remove_account) {
			CustomMenuCall_AccountCreate();
		}
	}
	else if (button_id == H2AccountCount + 2) {
		if (!mode_remove_account) {
			CustomMenuCall_AccountAdd();
		}
	}
	else if (button_id == H2AccountCount + 3) {
		// remove next selected account.
		mode_remove_account = !mode_remove_account;
		add_cartographer_label(CMLabelMenuId_AccountList, 1 + H2AccountCount + 3, H2CustomLanguageGetLabel(CMLabelMenuId_AccountList, 0xFFFF0001 + (mode_remove_account ? 1 : 0)), true);
	}
	else if (H2AccountCount > 0 && button_id >= 0 && button_id < H2AccountCount) {
		int account_id = button_id;
		if (mode_remove_account) {
			if (H2AccountBufferLoginToken && H2AccountBufferLoginToken[account_id]) {
				if (H2AccountBufferUsername[account_id]) {
					free(H2AccountBufferUsername[account_id]);
				}
				if (H2AccountBufferLoginToken[account_id]) {
					free(H2AccountBufferLoginToken[account_id]);
				}
				for (int i = account_id + 1; i < H2AccountCount; i++) {
					H2AccountBufferUsername[i - 1] = H2AccountBufferUsername[i];
					H2AccountBufferLoginToken[i - 1] = H2AccountBufferLoginToken[i];
				}
				H2AccountCount--;
				H2AccountBufferI--;
			}
			H2AccountLastUsed = 0;
			//FIXME Reload menu instead.
			CM_CloseMenu(thisptr);
			return false;
		}
		else {
			// login button_id
			if (XLLNLogin(signin_user_id, TRUE, 0, H2AccountBufferUsername[account_id]) == ERROR_SUCCESS) {
				CM_CloseMenuStack();
				return false;
			}
			else {
				CustomMenuCall_Error_Inner(CMLabelMenuId_Error, 0xFFFFF02E, 0xFFFFF02F);
			}
		}
	}
	else if (button_id == H2AccountCount) {
		if (!mode_remove_account) {
			// play offline
			if (XLLNLogin(signin_user_id, FALSE, 0, 0) == ERROR_SUCCESS) {
				CM_CloseMenuStack();
				return false;
			}
			else {
				CustomMenuCall_Error_Inner(CMLabelMenuId_Error, 0xFFFFF02E, 0xFFFFF02F);
			}
		}
	}
	return CM_PressButtonAnimation(thisptr);
}

static int __fastcall widget_preselected_button(DWORD a1, DWORD _EDX)
{
	return sub_20F790_CM(a1, 0);// selected button id
}

static DWORD* menu_vftable_1 = 0;
static DWORD* menu_vftable_2 = 0;

static int __cdecl widget_call_head(int a1)
{
	setup_button_labels();
	return CustomMenu_CallHead(a1, menu_vftable_1, menu_vftable_2, (DWORD)widget_button_handler, 4 + H2AccountCount + (H2AccountCount <= 0 ? -1 : 0), 272);
}

static int(__cdecl *widget_function_ptr_helper())(int)
{
	return widget_call_head;
}

static void* __fastcall widget_deconstructor(LPVOID lpMem, DWORD _EDX, char a2)
{
	return sub_248beb_deconstructor(lpMem, _EDX, a2);
}

void CMSetupVFTables_AccountList()
{
	CMSetupVFTables(&menu_vftable_1, &menu_vftable_2, (DWORD)c_list_widget_label_buttons, (DWORD)widget_title_description, (DWORD)widget_function_ptr_helper, (DWORD)widget_preselected_button, true, (DWORD)widget_deconstructor);
}

bool CustomMenuCall_AccountList(DWORD dwUserIndex)
{
	if (XUserGetSigninState(dwUserIndex) == eXUserSigninState_NotSignedIn) {
		signin_user_id = dwUserIndex;
		CallWidget(widget_call_head);
		return true;
	}
	else {
		DWORD result = XLLNLogout(dwUserIndex);
		return false;
	}
}
