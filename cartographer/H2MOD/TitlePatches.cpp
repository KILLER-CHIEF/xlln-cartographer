#include "../dllMain.h"
#include "TitlePatches.h"
#include "../Utils/UtilHook.h"
#include "Menus/CustomMenu.h"
#include "Language/CustomLanguage.h"
#include "Config/Config.h"
#include "Language/CustomLanguage.h"

VOID TPSetWindowResizable(HWND hWindow)
{
	// Set game window as resizable.
	SetWindowLong(hWindow, GWL_STYLE, GetWindowLong(hWindow, GWL_STYLE) | WS_SIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
}

#pragma region Custom_Language_And_Font

typedef char*(__stdcall *tH2GetLabel)(int, int, int, int);
tH2GetLabel pH2GetLabel;
char* __stdcall H2GetLabel(int a1, int label_id, int a3, int a4)
{ //sub_3defd
	//int label_menu_id = *(int*)(*(int*)a1 + 8 * a3 + 4);
	int label_menu_id = a3;
	char* label = get_cartographer_label(label_menu_id, label_id, 0b10);
	if (label)
		return label;
	label = get_custom_label(current_language, label_menu_id, label_id, 0);
	if (label)
		return label;
	label = get_cartographer_label(label_menu_id, label_id, 0b01);
	if (!label && a1)
		label = pH2GetLabel(a1, label_id, a3, a4);
	//if (strcmp(label, "PROFILE NAME") == 0) {
	//	return label;//in order to breakpoint and get label_id's.
	//}
	if (!H2Config_custom_labels_capture_missing) {
		return label;
	}
	return get_custom_label(current_language, label_menu_id, label_id, label);
}

char* __cdecl cave_c00031b97(char* result, int buff_len)//Font Table Filename Override
{
	strcpy_s(result, buff_len, current_language->font_table_filename);
	return result;
}

char*(__cdecl* pfn_c00031b97)(int, int) = 0;
//__usercall - edi a1, stack a2
__declspec(naked) char* nak_c00031b97()
{
	__asm {
		push ebp
		push edi
		push esi
		push ecx
		push ebx

		mov eax, [esp + 18h]
		push eax//buff_len

		push eax
		call pfn_c00031b97
		add esp, 4h

		push eax//result
		call cave_c00031b97
		add esp, 8h

		pop ebx
		pop ecx
		pop esi
		pop edi
		pop ebp

		retn
	}
}

DWORD langAfterJmpAddr;
__declspec(naked) void getSystemLanguageMethodJmp() {
	CLSetGameLanguage();
	__asm {
		jmp langAfterJmpAddr
	}
}

#pragma endregion


BOOL InitTitlePatches()
{
	if (Title_Version == CLIENT_11081 || Title_Version == CLIENT_11091 || Title_Version == CLIENT_11122) {
		wchar_t* var_c0039bcf0 = (wchar_t*)(GetOffsetAddress(0x0039bce8, 0x0039bce8, 0x0039bcf0));//"Global\\Halo2AlreadyRunning"
		WriteBytes((DWORD)var_c0039bcf0, L"", 2);
	}

	if (Title_Version == CLIENT_11122) {
		DWORD dwBack;

		pH2GetLabel = (tH2GetLabel)DetourClassFunc((BYTE*)GetOffsetAddress(CLIENT_11122, 0x0003defd), (BYTE*)H2GetLabel, 8);
		VirtualProtect(pH2GetLabel, 4, PAGE_EXECUTE_READWRITE, &dwBack);

		//Hook the function that sets the font table filename.
		pfn_c00031b97 = (char*(__cdecl*)(int, int))(GetOffsetAddress(CLIENT_11122, 0x00031b97));
		PatchCall(GetOffsetAddress(CLIENT_11122, 0x00031e89), nak_c00031b97);

		//Hook the part where it sets the global language id.
		langAfterJmpAddr = (DWORD)(GetOffsetAddress(CLIENT_11122, 0x0003828c));
		DetourFunc((BYTE*)GetOffsetAddress(CLIENT_11122, 0x0003820d), (BYTE*)getSystemLanguageMethodJmp, 6);
	}

	InitCustomLanguage();
	InitCustomMenu();

	return TRUE;
}

BOOL UninitTitlePatches()
{
	UninitCustomMenu();
	UninitCustomLanguage();
	return TRUE;
}
