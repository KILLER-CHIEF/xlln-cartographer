#include "../../../dllMain.h"
#include "AccountManagement.h"
#include "../CustomMenuFunctions.h"
#include "../../Language/CustomLabels.h"
#include "../../Language/CustomLanguage.h"
#include "AccountList.h"
#include "../Error/Error.h"
#include "../../Config/Accounts.h"
#include "../../../xlive/xlive.h"
#include "AccountHelper.h"

static DWORD setup_button_labels()
{
	DWORD i;
	for (i = 0; i < 4; i++) {
		CHAR szUserName[XUSER_NAME_SIZE]{""};
		DWORD dwResultXUserGetName = XUserGetName(i, szUserName, XUSER_NAME_SIZE);
		if (dwResultXUserGetName == ERROR_NO_SUCH_USER) {
			break;
		}
		else if (dwResultXUserGetName != ERROR_SUCCESS && dwResultXUserGetName != ERROR_NOT_LOGGED_ON) {
			__debugbreak();
		}
		XUSER_SIGNIN_STATE dwResultXUserGetSigninState = dwResultXUserGetName == ERROR_NOT_LOGGED_ON ? eXUserSigninState_SignedInLocally : XUserGetSigninState(i);
		if (!(dwResultXUserGetSigninState & (eXUserSigninState_SignedInLocally | eXUserSigninState_SignedInToLive))) {
			// Potential race condition resolved.
			dwResultXUserGetName = ERROR_NOT_LOGGED_ON;
		}
		const char *label_player_base = H2CustomLanguageGetLabel(CMLabelMenuId_AccountManagement, dwResultXUserGetName == ERROR_NOT_LOGGED_ON ? 0xFFFF0002 : 0xFFFF0001);
		const char *label_signin_state = H2CustomLanguageGetLabel(CMLabelMenuId_AccountManagement, dwResultXUserGetSigninState == eXUserSigninState_SignedInToLive ? 0xFFFF0003 : 0xFFFF0004);
		DWORD combined_label_buflen = strlen(label_player_base) + strlen(label_signin_state) + strlen(szUserName) + 20;
		char *combined_label = (char*)malloc(combined_label_buflen);
		snprintf(combined_label, combined_label_buflen, label_player_base, i + 1, label_signin_state, szUserName);
		add_cartographer_label(CMLabelMenuId_AccountManagement, 1 + i, combined_label, true);
		free(combined_label);
	}
	return i;
}

static void __fastcall c_list_widget_label_buttons(DWORD _ECX, DWORD _EDX, int a1, int a2)
{
	int(__thiscall *sub_211909)(int, int, int, int) = (int(__thiscall*)(int, int, int, int))(GetOffsetAddress(CLIENT_11122, 0x00211909));

	__int16 button_id = *(WORD*)(a1 + 112);
	int v3 = sub_211909(a1, 6, 0, 0);
	if (v3) {
		sub_21bf85_CMLTD(v3, button_id + 1, CMLabelMenuId_AccountManagement);
	}
}

static int __fastcall widget_title_description(int a1, DWORD _EDX, char a2)
{
	return sub_2111ab_CMLTD(a1, a2, CMLabelMenuId_AccountManagement, 0xFFFFFFF0, 0xFFFFFFF1);
}

static int __fastcall widget_button_handler(void *thisptr, DWORD _EDX, int a2, DWORD *a3)
{
	unsigned __int16 button_id = *a3 & 0xFFFF;
	if (CustomMenuCall_AccountList(button_id)) {
		return CM_PressButtonAnimation(thisptr);
	}
	//FIXME Reload menu instead.
	CM_CloseMenu(thisptr);
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
	XLLNDebugLog(0, "PCarto: Constructor AccountManagement.");
	//if (getAccountingActiveHandle() < 2) {
		updateAccountingActiveHandle(true);
	//}
	DWORD num_of_buttons = setup_button_labels();
	return CustomMenu_CallHead(a1, menu_vftable_1, menu_vftable_2, (DWORD)widget_button_handler, num_of_buttons, 272);
}
static bool cancelit = false;
static int(__cdecl *widget_function_ptr_helper())(int)
{
	cancelit = true;
	return widget_call_head;
}
static BYTE menuAccountingMode = 0;

static void* __fastcall widget_deconstructor(LPVOID lpMem, DWORD _EDX, char a2)
{
	XLLNDebugLog(0, "PCarto: Deconstructor AccountManagement.");
	if (!cancelit) {
		closeAccountingActiveHandle();
	}
	cancelit = false;
	return sub_248beb_deconstructor(lpMem, _EDX, a2);
}

void CMSetupVFTables_AccountManagement()
{
	CMSetupVFTables(&menu_vftable_1, &menu_vftable_2, (DWORD)c_list_widget_label_buttons, (DWORD)widget_title_description, (DWORD)widget_function_ptr_helper, (DWORD)widget_preselected_button, true, (DWORD)widget_deconstructor);
}

void CustomMenuCall_AccountManagement(BYTE mode)
{
	if (!isAccountingActiveHandle()) {
		if (ReadH2Accounts()) {
			menuAccountingMode = mode;
			CallWidget(widget_call_head);
		}
		else {
			CustomMenuCall_Error_Inner(CMLabelMenuId_Error, 0xFFFFF016, 0xFFFFF017);
		}
	}
}
