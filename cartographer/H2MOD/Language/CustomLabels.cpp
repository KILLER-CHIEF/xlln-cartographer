#include "../../dllMain.h"
#include "CustomLabels.h"
#include "CustomLanguage.h"

const int CMLabelMenuId_Error =					0xFF000001;
const int CMLabelMenuId_Language =				0xFF000002;
const int CMLabelMenuId_Guide =					0xFF000003;
const int CMLabelMenuId_EscSettings =			0xFF000004;
const int CMLabelMenuId_AdvSettings =			0xFF000005;
const int CMLabelMenuId_Credits =				0xFF000006;
const int CMLabelMenuId_EditHudGui =			0xFF000007;
const int CMLabelMenuId_ToggleSkulls =			0xFF000008;
const int CMLabelMenuId_AccountList =			0xFF000009;
const int CMLabelMenuId_AccountEdit =			0xFF00000A;
const int CMLabelMenuId_VKeyTest =				0xFF00000B;
const int CMLabelMenuId_AccountCreate =			0xFF00000C;
const int CMLabelMenuId_OtherSettings =			0xFF00000D;
const int CMLabelMenuId_EditFPS =				0xFF00000E;
const int CMLabelMenuId_EditFOV =				0xFF00000F;
const int CMLabelMenuId_EditCrosshair =			0xFF000010;
const int CMLabelMenuId_Update =				0xFF000011;
const int CMLabelMenuId_Update_Note =			0xFF000012;
const int CMLabelMenuId_Login_Warn =			0xFF000013;
const int CMLabelMenuId_EditStaticLoD =			0xFF000014;
const int CMLabelMenuId_EditCrosshairSize =		0xFF000015;
const int CMLabelMenuId_AdvLobbySettings =		0xFF000016;
const int CMLabelMenuId_Invalid_Login_Token =	0xFF000017;

BOOL InitCustomLabels()
{
	add_cartographer_label(CMLabelMenuId_EscSettings, 0xFFFFFFF0, "Esc Settings");
	add_cartographer_label(CMLabelMenuId_EscSettings, 0xFFFFFFF1, "Esc Settings Here.");
	add_cartographer_label(CMLabelMenuId_EscSettings, 1, "btn 1");
	add_cartographer_label(CMLabelMenuId_EscSettings, 2, "btn 2");

	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFFFF0, "Error!", true);
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFFFF1, "Generic Error.", true);
	add_cartographer_label(CMLabelMenuId_Error, 1, "ERROR");
	add_cartographer_label(CMLabelMenuId_Error, 2, "You... didn't listen did you!?");
	add_cartographer_label(CMLabelMenuId_Error, 3, "Good! You are part of the #dev_preview group (or you got this from the asshole who leaked it). I want you to try and break things and find bugs; since that's what your damn job is.");
	add_cartographer_label(CMLabelMenuId_Error, 4, "None");
	add_cartographer_label(CMLabelMenuId_Error, 5, "There are no custom languages catergorised as Other.");
	add_cartographer_label(CMLabelMenuId_Error, 6, "Error");
	add_cartographer_label(CMLabelMenuId_Error, 7, "An error occured when trying to read the custom language file.\r\nNo Changes have been made.\r\nReview the on screen debug log for more details.");
	add_cartographer_label(CMLabelMenuId_Error, 0x8, "Incomplete Feature");
	add_cartographer_label(CMLabelMenuId_Error, 0x9, "This feature is incomplete.");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFFF02, "Glitchy Scripts");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFFF03, "Created/reversed custom GUIs.\r\nCoded the entire account creation/login system.\r\nCreated Custom Languages.\r\nIs the Halo 2 Master Server overlord!");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFFF04, "PermaNull");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFFF05, "Wrote/reversed all the functionality for online play! And created GunGame & Zombies.\r\nNow that's no easy feat!");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF004, "Outdated Version!");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF005, "You are using an outdated version of Project Cartographer! Please install the latest version.");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF006, "Invalid Login Token!");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF007, "Login Again.");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF008, "Invalid Account ID!");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF009, "The Username or Email Address you entered is not one of an existing account. Please check your spelling or create a new account if you don't have one already.");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF00A, "Incorrect Password!");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF00B, "The password you entered is incorrect!");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF00C, "BANNED!");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF00D, "The computer you are currently using is banned! If you think this is a mistake, post on the online forum pleading your innocence; if you have any you cheater!");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF00E, "BANNED!");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF00F, "Your account is banned! If you think this is a mistake, post on the online forum pleading your innocence; if you have any you cheater!");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF010, "Account Disabled!");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF011, "Your account is currently disabled, closed or deleted! Please contact an administrator if this was a mistake.");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF012, "Unknown Error!?");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF013, "The error that has occured has not been handled. Please contact an admin including any log files you have and what you did.");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF014, "Something's Broken!");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF015, "An internal error has occured. We've probably noticed this already but contact an admin if this persists.");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF016, "Accounts in Use");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF017, "Another instance of Halo 2 / H2Server is currently signing in, please try again after it finishes.");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF018, "Insufficient PC Identification!");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF019, "Your PC does not have sufficient unique identifiers available. Please contact an admin about linking this PC with another that you own that does have sufficient info. This is to ensure fair online play.");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF01A, "Invalid Email!");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF01B, "The Email address you have entered is invalid! Please double check your spelling.");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF01C, "Invalid Username!");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF01D, "The Username you have entered is invalid! Please ensure you have formed it correctly using only allowed symbols!");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF01E, "Invalid Password!");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF01F, "The Password you have entered is invalid! Please ensure you have formed it correctly using only allowed symbols!");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF020, "Email Already in Use!");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF021, "The Email Address you have entered is already in use! You cannot use an email for multiple accounts. Please use a different email, sign in to that account or reset its password if you have forgotton it.");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF022, "Username Taken!");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF023, "The Username you have entered is already in use!");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF024, "BANNED Email Provider!");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF025, "The Email Address you have entered is using a domain name that has been banned! We do not allow disposable email addresses! If this is a mistake please contact an admin.");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF026, "Account Successfully Created!");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF027, "The account you just entered has been successfully created! You may now use those details to login.");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF028, "Verification Email Sent!");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF029, "An email has been sent to the email address submitted. Please follow the instuctions in the email to activate your account.");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF02A, "Restart Required");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF02B, "The setting you have just changed requires that you restart your game for it to take effect.");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF02C, "Creating Account...");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF02D, "Processing your new account...\r\nPlease wait.");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF02E, "Logging in...");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF02F, "Please wait while you are being logged in.");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF030, "Connection Failed!");
	add_cartographer_label(CMLabelMenuId_Error, 0xFFFFF031, "Please check your connection to:\r\nhttps://cartographer.online/\r\nthen try again.");


	add_cartographer_label(CMLabelMenuId_Language, 0xFFFFFFF0, "Select Language");
	add_cartographer_label(CMLabelMenuId_Language, 0xFFFFFFF1, "Select the language you would like to play the game in.");
	add_cartographer_label(CMLabelMenuId_Language, 1, "System Default");
	add_cartographer_label(CMLabelMenuId_Language, 2, "Other >");
	add_cartographer_label(CMLabelMenuId_Language, 3, "English >");
	add_cartographer_label(CMLabelMenuId_Language, 4, "Japanese >");
	add_cartographer_label(CMLabelMenuId_Language, 5, "German >");
	add_cartographer_label(CMLabelMenuId_Language, 6, "French >");
	add_cartographer_label(CMLabelMenuId_Language, 7, "Spanish >");
	add_cartographer_label(CMLabelMenuId_Language, 8, "Italian >");
	add_cartographer_label(CMLabelMenuId_Language, 9, "Korean >");
	add_cartographer_label(CMLabelMenuId_Language, 10, "Chinese >");
	add_cartographer_label(CMLabelMenuId_Language, 11, "Reload Language File");
	add_cartographer_label(CMLabelMenuId_Language, 12, "Save Language File");
	add_cartographer_label(CMLabelMenuId_Language, 0xFFFFF002, "Ignore New Labels");
	add_cartographer_label(CMLabelMenuId_Language, 0xFFFFF003, "Capture New Labels");
	add_cartographer_label(CMLabelMenuId_Language, 0xFFFFFFF2, "--- Base %s Variant ---");
	add_cartographer_label(CMLabelMenuId_Language, 0xFFFFFFF3, "Select Language Variant");
	add_cartographer_label(CMLabelMenuId_Language, 0xFFFFFFF4, "Select the variant of the language you would like to play the game in.");


	add_cartographer_label(CMLabelMenuId_EditCrosshair, 0xFFFFFFF0, "Edit Crosshair Offset");
	add_cartographer_label(CMLabelMenuId_EditCrosshair, 0xFFFFFFF1, "Use the buttons below to modify the in-game Crosshair Offset.");
	add_cartographer_label(CMLabelMenuId_EditCrosshair, 1, "+0.02");
	add_cartographer_label(CMLabelMenuId_EditCrosshair, 2, "+0.001");
	add_cartographer_label(CMLabelMenuId_EditCrosshair, 0xFFFF0003, "Offset: %f");
	add_cartographer_label(CMLabelMenuId_EditCrosshair, 0xFFFF0013, "Offset Alteration Disabled");
	add_cartographer_label(CMLabelMenuId_EditCrosshair, 4, "-0.001");
	add_cartographer_label(CMLabelMenuId_EditCrosshair, 5, "-0.02");


	add_cartographer_label(CMLabelMenuId_EditFOV, 0xFFFFFFF0, "Edit Field of View");
	add_cartographer_label(CMLabelMenuId_EditFOV, 0xFFFFFFF1, "Use the buttons below to modify the in-game Field of View (FoV).");
	add_cartographer_label(CMLabelMenuId_EditFOV, 1, "+10");
	add_cartographer_label(CMLabelMenuId_EditFOV, 2, "+1");
	add_cartographer_label(CMLabelMenuId_EditFOV, 0xFFFF0003, "FoV: %d");
	add_cartographer_label(CMLabelMenuId_EditFOV, 0xFFFF0013, "FoV Alteration Disabled");
	add_cartographer_label(CMLabelMenuId_EditFOV, 4, "-1");
	add_cartographer_label(CMLabelMenuId_EditFOV, 5, "-10");


	add_cartographer_label(CMLabelMenuId_EditFPS, 0xFFFFFFF0, "Edit FPS Limit");
	add_cartographer_label(CMLabelMenuId_EditFPS, 0xFFFFFFF1, "Use the buttons below to modify the FPS limit of Halo 2.");
	add_cartographer_label(CMLabelMenuId_EditFPS, 1, "+10");
	add_cartographer_label(CMLabelMenuId_EditFPS, 2, "+1");
	add_cartographer_label(CMLabelMenuId_EditFPS, 0xFFFF0003, "FPS Limit: %d");
	add_cartographer_label(CMLabelMenuId_EditFPS, 0xFFFF0013, "Xlive FPS Limiter Disabled");
	add_cartographer_label(CMLabelMenuId_EditFPS, 4, "-1");
	add_cartographer_label(CMLabelMenuId_EditFPS, 5, "-10");


	add_cartographer_label(CMLabelMenuId_EditStaticLoD, 0xFFFFFFF0, "Static Model Level of Detail");
	add_cartographer_label(CMLabelMenuId_EditStaticLoD, 0xFFFFFFF1, "Use the buttons below to set a static level on a model's Level of Detail.");
	add_cartographer_label(CMLabelMenuId_EditStaticLoD, 1, "Disabled");
	add_cartographer_label(CMLabelMenuId_EditStaticLoD, 2, "L1 - Very Low");
	add_cartographer_label(CMLabelMenuId_EditStaticLoD, 3, "L2 - Low");
	add_cartographer_label(CMLabelMenuId_EditStaticLoD, 4, "L3 - Medium");
	add_cartographer_label(CMLabelMenuId_EditStaticLoD, 5, "L4 - High");
	add_cartographer_label(CMLabelMenuId_EditStaticLoD, 6, "L5 - Very High");
	add_cartographer_label(CMLabelMenuId_EditStaticLoD, 7, "L6 - Cinematic");


	add_cartographer_label(CMLabelMenuId_EditCrosshairSize, 0xFFFFFFF0, "Crosshair Settings");
	add_cartographer_label(CMLabelMenuId_EditCrosshairSize, 0xFFFFFFF1, "Use the buttons below to set a preset crosshair size. Use the config file to modify crosshairs in more detail.");
	add_cartographer_label(CMLabelMenuId_EditCrosshairSize, 1, "Default");
	add_cartographer_label(CMLabelMenuId_EditCrosshairSize, 2, "Disabled");
	add_cartographer_label(CMLabelMenuId_EditCrosshairSize, 3, "Very Small");
	add_cartographer_label(CMLabelMenuId_EditCrosshairSize, 4, "Small");
	add_cartographer_label(CMLabelMenuId_EditCrosshairSize, 5, "Large");


	add_cartographer_label(CMLabelMenuId_Update, 0xFFFFFFF0, "Update");
	add_cartographer_label(CMLabelMenuId_Update, 0xFFFFFFF1, "Update Project Cartographer.");
	add_cartographer_label(CMLabelMenuId_Update, 1, (char*)0, true);
	add_cartographer_label(CMLabelMenuId_Update, 0xFFFF0001, "Check for Updates");
	add_cartographer_label(CMLabelMenuId_Update, 0xFFFFFF01, "Checking For Updates...");
	add_cartographer_label(CMLabelMenuId_Update, 2, (char*)0, true);
	add_cartographer_label(CMLabelMenuId_Update, 0xFFFF0002, "Download Updates");
	add_cartographer_label(CMLabelMenuId_Update, 0xFFFFFF02, "Downloading Updates...");
	add_cartographer_label(CMLabelMenuId_Update, 3, (char*)0, true);
	add_cartographer_label(CMLabelMenuId_Update, 0xFFFF0003, "Install Updates");
	add_cartographer_label(CMLabelMenuId_Update, 0xFFFFFF03, "Installing Updates...");
	add_cartographer_label(CMLabelMenuId_Update, 0xFFFFF003, "Failed to run updater app!");
	add_cartographer_label(CMLabelMenuId_Update, 4, "Cancel");
	add_cartographer_label(CMLabelMenuId_Update, 0xFFFFFFF2, "Download the following:\n");
	add_cartographer_label(CMLabelMenuId_Update, 0xFFFFFFF3, "Install the following:\n");
	add_cartographer_label(CMLabelMenuId_Update, 0xFFFFFFF4, "Up to date!");


	add_cartographer_label(CMLabelMenuId_Update_Note, 0xFFFFFFF0, "Outdated Version!");
	add_cartographer_label(CMLabelMenuId_Update_Note, 0xFFFFFFF1, "You are using an outdated version of Project Cartographer! Would you like to go install the latest version?");
	add_cartographer_label(CMLabelMenuId_Update_Note, 1, "Yes");
	add_cartographer_label(CMLabelMenuId_Update_Note, 2, "No");


	add_cartographer_label(CMLabelMenuId_Login_Warn, 0xFFFFFFF0, "NO CHEATING!");
	add_cartographer_label(CMLabelMenuId_Login_Warn, 0xFFFFFFF1, "DO NOT CHEAT/HACK ONLINE.\r\nWe have a complex system made to keep you banned if you do. Don't risk it. We will catch you otherwise!");


	add_cartographer_label(CMLabelMenuId_EditHudGui, 0xFFFFFFF0, "Customise HUD / GUI");
	add_cartographer_label(CMLabelMenuId_EditHudGui, 0xFFFFFFF1, "Customise your heads up display and user interface with the following settings.");
	add_cartographer_label(CMLabelMenuId_EditHudGui, 0xFFFFFFF2, "Enable %s");
	add_cartographer_label(CMLabelMenuId_EditHudGui, 0xFFFFFFF3, "Disable %s");
	add_cartographer_label(CMLabelMenuId_EditHudGui, 0xFFFFFFF4, "Show %s");
	add_cartographer_label(CMLabelMenuId_EditHudGui, 0xFFFFFFF5, "Hide %s");
	add_cartographer_label(CMLabelMenuId_EditHudGui, 1, "> Field of View (FOV)");
	add_cartographer_label(CMLabelMenuId_EditHudGui, 2, "> Crosshair Offset");
	add_cartographer_label(CMLabelMenuId_EditHudGui, 3, "> Crosshair Size");
	add_cartographer_label(CMLabelMenuId_EditHudGui, 0xFFFF0004, "Ingame Chat");
	add_cartographer_label(CMLabelMenuId_EditHudGui, 0xFFFF0005, "HUD");
	add_cartographer_label(CMLabelMenuId_EditHudGui, 0xFFFF0006, "First Person Model");


	add_cartographer_label(CMLabelMenuId_ToggleSkulls, 0xFFFFFFF0, "Toggle Skulls");
	add_cartographer_label(CMLabelMenuId_ToggleSkulls, 0xFFFFFFF1, "Enable and disable Halo 2's Skulls.\r\nWhat ice cream flavor would you like to choose today?");
	add_cartographer_label(CMLabelMenuId_ToggleSkulls, 0xFFFFFFF2, "Enable %s");
	add_cartographer_label(CMLabelMenuId_ToggleSkulls, 0xFFFFFFF3, "Disable %s");
	add_cartographer_label(CMLabelMenuId_ToggleSkulls, 0xFFFFFFF4, "???");
	add_cartographer_label(CMLabelMenuId_ToggleSkulls, 0xFFFF0000, "Anger");
	add_cartographer_label(CMLabelMenuId_ToggleSkulls, 0xFFFF0001, "Assassins");
	add_cartographer_label(CMLabelMenuId_ToggleSkulls, 0xFFFF0002, "Black Eye");
	add_cartographer_label(CMLabelMenuId_ToggleSkulls, 0xFFFF0003, "Blind");
	add_cartographer_label(CMLabelMenuId_ToggleSkulls, 0xFFFF0004, "Catch");
	add_cartographer_label(CMLabelMenuId_ToggleSkulls, 0xFFFF0005, "Envy");
	add_cartographer_label(CMLabelMenuId_ToggleSkulls, 0xFFFF0006, "Famine");
	add_cartographer_label(CMLabelMenuId_ToggleSkulls, 0xFFFF0007, "Ghost");
	add_cartographer_label(CMLabelMenuId_ToggleSkulls, 0xFFFF0008, "Grunt Birthday Party");
	add_cartographer_label(CMLabelMenuId_ToggleSkulls, 0xFFFF0009, "Iron");
	add_cartographer_label(CMLabelMenuId_ToggleSkulls, 0xFFFF000A, "IWHBYD");
	add_cartographer_label(CMLabelMenuId_ToggleSkulls, 0xFFFF000B, "Mythic");
	add_cartographer_label(CMLabelMenuId_ToggleSkulls, 0xFFFF000C, "Sputnik");
	add_cartographer_label(CMLabelMenuId_ToggleSkulls, 0xFFFF000D, "Thunderstorm");
	add_cartographer_label(CMLabelMenuId_ToggleSkulls, 0xFFFF000E, "Whuppopotamus");


	add_cartographer_label(CMLabelMenuId_OtherSettings, 0xFFFFFFF0, "Other Settings");
	add_cartographer_label(CMLabelMenuId_OtherSettings, 0xFFFFFFF1, "Customise other settings and features of Halo 2 / Project Cartographer.");
	add_cartographer_label(CMLabelMenuId_OtherSettings, 0xFFFFFFF2, "Enable %s");
	add_cartographer_label(CMLabelMenuId_OtherSettings, 0xFFFFFFF3, "Disable %s");
	add_cartographer_label(CMLabelMenuId_OtherSettings, 0xFFFFFFF4, "Show %s");
	add_cartographer_label(CMLabelMenuId_OtherSettings, 0xFFFFFFF5, "Hide %s");
	add_cartographer_label(CMLabelMenuId_OtherSettings, 0xFFFFFFF6, "Play %s");
	add_cartographer_label(CMLabelMenuId_OtherSettings, 0xFFFFFFF7, "Skip %s");
	add_cartographer_label(CMLabelMenuId_OtherSettings, 1, "> FPS Limit");
	add_cartographer_label(CMLabelMenuId_OtherSettings, 2, "> Static Model LoD");
	add_cartographer_label(CMLabelMenuId_OtherSettings, 0xFFFF0003, "Controller Aim-Assist");
	add_cartographer_label(CMLabelMenuId_OtherSettings, 0xFFFF0004, "Discord Rich Presence");
	add_cartographer_label(CMLabelMenuId_OtherSettings, 0xFFFF0005, "xDelay");
	add_cartographer_label(CMLabelMenuId_OtherSettings, 0xFFFF0006, "Game Intro Video");
	add_cartographer_label(CMLabelMenuId_OtherSettings, 0xFFFF0007, "In-game Keyb. CTRLs");
	add_cartographer_label(CMLabelMenuId_OtherSettings, 0xFFFF0008, "Raw Mouse Input");
	add_cartographer_label(CMLabelMenuId_OtherSettings, 0xFFFF0009, "Hitmarker Sound Effect");


	add_cartographer_label(CMLabelMenuId_AdvSettings, 0xFFFFFFF0, "Advanced Settings");
	add_cartographer_label(CMLabelMenuId_AdvSettings, 0xFFFFFFF1, "Alter additional settings for the game.");
	add_cartographer_label(CMLabelMenuId_AdvSettings, 1, "Change Language");
	add_cartographer_label(CMLabelMenuId_AdvSettings, 2, "Customise HUD/GUI");
	add_cartographer_label(CMLabelMenuId_AdvSettings, 3, "Other Settings");
	add_cartographer_label(CMLabelMenuId_AdvSettings, 4, "Toggle Skulls");
	add_cartographer_label(CMLabelMenuId_AdvSettings, 5, "Extra Game Settings");


	add_cartographer_label(CMLabelMenuId_AdvLobbySettings, 0xFFFFFFF0, "Extra Game Settings");
	add_cartographer_label(CMLabelMenuId_AdvLobbySettings, 0xFFFFFFF1, "Customise the game with some extra features / hacks.");
	add_cartographer_label(CMLabelMenuId_AdvLobbySettings, 0xFFFFFFF2, "Enable %s");
	add_cartographer_label(CMLabelMenuId_AdvLobbySettings, 0xFFFFFFF3, "Disable %s");
	add_cartographer_label(CMLabelMenuId_AdvLobbySettings, 0xFFFFFFF4, "Show %s");
	add_cartographer_label(CMLabelMenuId_AdvLobbySettings, 0xFFFFFFF5, "Hide %s");
	add_cartographer_label(CMLabelMenuId_AdvLobbySettings, 0xFFFFFFF6, "Play %s");
	add_cartographer_label(CMLabelMenuId_AdvLobbySettings, 0xFFFFFFF7, "Skip %s");
	add_cartographer_label(CMLabelMenuId_AdvLobbySettings, 0xFFFFFF02, "Change Server Name");
	add_cartographer_label(CMLabelMenuId_AdvLobbySettings, 0xFFFFFF03, "Enter the new name for your lobby.");
	add_cartographer_label(CMLabelMenuId_AdvLobbySettings, 0xFFFFF001, 32 * sizeof(wchar_t), true);
	add_cartographer_label(CMLabelMenuId_AdvLobbySettings, 1, ">Change Server Name");
	add_cartographer_label(CMLabelMenuId_AdvLobbySettings, 0xFFFF0002, "Vehicle Flip Eject");
	add_cartographer_label(CMLabelMenuId_AdvLobbySettings, 0xFFFF0003, "Kill Volumes");
	add_cartographer_label(CMLabelMenuId_AdvLobbySettings, 0xFFFF0004, "MP Explosion Physics");
	add_cartographer_label(CMLabelMenuId_AdvLobbySettings, 0xFFFF0005, "MP Sputnik");
	add_cartographer_label(CMLabelMenuId_AdvLobbySettings, 0xFFFF0006, "MP Grunt B-Day Party");
	add_cartographer_label(CMLabelMenuId_AdvLobbySettings, 0xFFFF0007, "Grenade Chain React");
	add_cartographer_label(CMLabelMenuId_AdvLobbySettings, 0xFFFF0008, "Banshee Bomb");
	add_cartographer_label(CMLabelMenuId_AdvLobbySettings, 0xFFFF0009, "HUD");
	add_cartographer_label(CMLabelMenuId_AdvLobbySettings, 0xFFFF000A, "First Person Model");
	add_cartographer_label(CMLabelMenuId_AdvLobbySettings, 0xFFFF000B, "Flashlight");
	//add_cartographer_label(CMLabelMenuId_AdvLobbySettings, 7, "Zombie Movement Speed");


	add_cartographer_label(CMLabelMenuId_Credits, 0xFFFFFFF0, "Credits");
	add_cartographer_label(CMLabelMenuId_Credits, 0xFFFFFFF1, "Praise the Following.");
	add_cartographer_label(CMLabelMenuId_Credits, 1, "--- The Devs ---");
	add_cartographer_label(CMLabelMenuId_Credits, 2, "PermaNull");
	add_cartographer_label(CMLabelMenuId_Credits, 3, "Glitchy Scripts");
	add_cartographer_label(CMLabelMenuId_Credits, 4, "Himanshu01");
	add_cartographer_label(CMLabelMenuId_Credits, 5, "Hootspa");
	add_cartographer_label(CMLabelMenuId_Credits, 6, "NukeULater");
	add_cartographer_label(CMLabelMenuId_Credits, 7, "Num005");
	add_cartographer_label(CMLabelMenuId_Credits, 8, "Rude Yoshi");
	add_cartographer_label(CMLabelMenuId_Credits, 9, "Supersniper");
	add_cartographer_label(CMLabelMenuId_Credits, 10, "--- Content Creators ---");
	add_cartographer_label(CMLabelMenuId_Credits, 11, "General_101");
	add_cartographer_label(CMLabelMenuId_Credits, 12, "--- Additional 7hanks ---");
	add_cartographer_label(CMLabelMenuId_Credits, 13, "Project Cartographer Staff");
	add_cartographer_label(CMLabelMenuId_Credits, 14, "Dev Preview Members");
	add_cartographer_label(CMLabelMenuId_Credits, 15, "And the many many more");
	add_cartographer_label(CMLabelMenuId_Credits, 16, "from the Halo 2 Community!");


	add_cartographer_label(CMLabelMenuId_Invalid_Login_Token, 0xFFFFFFF0, "Invalid Login Token!");
	add_cartographer_label(CMLabelMenuId_Invalid_Login_Token, 0xFFFFFFF1, "Login Again.");


	add_cartographer_label(CMLabelMenuId_AccountCreate, 0xFFFFFFF0, "Create Account");
	add_cartographer_label(CMLabelMenuId_AccountCreate, 0xFFFFFFF1, "Enter a username, email and password for your new account.");
	add_cartographer_label(CMLabelMenuId_AccountCreate, 0xFFFFFFF2, "[Username]");
	add_cartographer_label(CMLabelMenuId_AccountCreate, 0xFFFFFFF3, "[Email]");
	add_cartographer_label(CMLabelMenuId_AccountCreate, 0xFFFFFFF4, "[Password]");
	add_cartographer_label(CMLabelMenuId_AccountCreate, 0xFFFFF002, "Create a Username");
	add_cartographer_label(CMLabelMenuId_AccountCreate, 0xFFFFF003, "Create the Username for your new account below.\r\nHowever beware! Not all special characters are supported!");
	add_cartographer_label(CMLabelMenuId_AccountCreate, 0xFFFFF004, "Enter an Email Address");
	add_cartographer_label(CMLabelMenuId_AccountCreate, 0xFFFFF005, "Enter the Email Address to be linked to your new account below.");
	add_cartographer_label(CMLabelMenuId_AccountCreate, 0xFFFFF006, "Create a Password");
	add_cartographer_label(CMLabelMenuId_AccountCreate, 0xFFFFF007, "Create the Password for your new account below.");
	add_cartographer_label(CMLabelMenuId_AccountCreate, 1, 255 * 2, true);
	add_cartographer_label(CMLabelMenuId_AccountCreate, 2, 255 * 2, true);
	add_cartographer_label(CMLabelMenuId_AccountCreate, 3, 255 * 2, true);
	add_cartographer_label(CMLabelMenuId_AccountCreate, 4, "Create Account");


	add_cartographer_label(CMLabelMenuId_AccountEdit, 0xFFFFFFF0, "Add Account");
	add_cartographer_label(CMLabelMenuId_AccountEdit, 0xFFFFFFF1, "Enter your account's Username\r\n[or Email] and Password to Login.");
	add_cartographer_label(CMLabelMenuId_AccountEdit, 0xFFFFFFF2, "-Remember me");
	add_cartographer_label(CMLabelMenuId_AccountEdit, 0xFFFFFFF3, "-Don't remember me");
	add_cartographer_label(CMLabelMenuId_AccountEdit, 0xFFFFFFF4, "[Username]");
	add_cartographer_label(CMLabelMenuId_AccountEdit, 0xFFFFFFF5, "[Password]");
	add_cartographer_label(CMLabelMenuId_AccountEdit, 0xFFFFF002, "Enter Account Username or Email Address");
	add_cartographer_label(CMLabelMenuId_AccountEdit, 0xFFFFF003, "Enter the Username or Email Address of your account below.");
	add_cartographer_label(CMLabelMenuId_AccountEdit, 0xFFFFF004, "Enter Account Password");
	add_cartographer_label(CMLabelMenuId_AccountEdit, 0xFFFFF005, "Enter the Password of your account below.");
	add_cartographer_label(CMLabelMenuId_AccountEdit, 1, 255 * 2, true);
	add_cartographer_label(CMLabelMenuId_AccountEdit, 2, 255 * 2, true);
	add_cartographer_label(CMLabelMenuId_AccountEdit, 4, "Login");


	add_cartographer_label(CMLabelMenuId_AccountList, 0xFFFFFFF0, "Online Accounts");
	add_cartographer_label(CMLabelMenuId_AccountList, 0xFFFFFFF1, "Select an Account to Sign in to or use options to create/add/remove them.");
	add_cartographer_label(CMLabelMenuId_AccountList, 0xFFFF0000, ">Add Account");
	add_cartographer_label(CMLabelMenuId_AccountList, 0xFFFF0001, "-Remove Account");
	add_cartographer_label(CMLabelMenuId_AccountList, 0xFFFF0002, "-Cancel Remove");
	add_cartographer_label(CMLabelMenuId_AccountList, 0xFFFF0003, ">Create Account");
	add_cartographer_label(CMLabelMenuId_AccountList, 0xFFFF0004, ">Play Offline");
	add_cartographer_label(CMLabelMenuId_AccountList, 0xFFFF0005, "<Unnamed>");


	add_cartographer_label(CMLabelMenuId_Guide, 0xFFFFFFF0, "Project Cartographer Guide");
	add_cartographer_label(CMLabelMenuId_Guide, 0xFFFFFFF2, "Press the %s Key to open this guide from anywhere.");
	add_cartographer_label(CMLabelMenuId_Guide, 1, "Advanced Settings");
	add_cartographer_label(CMLabelMenuId_Guide, 2, "Website");
	add_cartographer_label(CMLabelMenuId_Guide, 3, "Credits");
	add_cartographer_label(CMLabelMenuId_Guide, 4, "Update");

	return TRUE;
}

BOOL UninitCustomLabels()
{
	return TRUE;
}