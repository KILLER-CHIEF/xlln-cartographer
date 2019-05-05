#include "../../dllMain.h"
#include "CustomMenu.h"
#include "CustomMenuFunctions.h"
#include "../Language/CustomLabels.h"
#include "Error/Error.h"
#include "VirtualKB/VirtualKB.h"
#include "Guide/Guide.h"
#include "Credits/Credits.h"
#include "AdvSettings/AdvSettings.h"
#include "AdvSettings/Language/LanguageMain.h"
#include "AdvSettings/Language/LanguageSub.h"
#include "Accounts/AccountManagement.h"
#include "Accounts/AccountList.h"
#include "Accounts/AccountCreate.h"
#include "Accounts/AccountAdd.h"
#include "NetworkType/NetworkType.h"

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
		CMSetupVFTables_AccountManagement();
		CMSetupVFTables_AccountList();
		CMSetupVFTables_AccountCreate();
		CMSetupVFTables_AccountAdd();
		CMSetupVFTables_NetworkType();
	}

	return TRUE;
}

BOOL UninitCustomMenu()
{
	return TRUE;
}
