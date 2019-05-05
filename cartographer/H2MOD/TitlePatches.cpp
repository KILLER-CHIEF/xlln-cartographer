#include "../dllMain.h"
#include "TitlePatches.h"
#include "../Utils/UtilHook.h"
#include "../xlive/xlive.h"
#include "Menus/CustomMenu.h"
#include "Menus/NetworkType/NetworkType.h"
#include "Language/CustomLanguage.h"
#include "Config/Config.h"
#include "Language/CustomLanguage.h"
#include "RunLoop/RunLoop.h"

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


BOOL __stdcall sub_0000a1d6(int a1)
{
	return a1 == 2 || a1 == 1;
}

BOOL __stdcall sub_0000A276(int a1)
{
	//BOOL(__stdcall* sub_0000a1d6)(int a1) = (BOOL(__stdcall*)(int))GetOffsetAddress(CLIENT_11122, 0x0000a1d6);
	HRESULT(WINAPI* XLivePBufferGetByte)(DWORD *xebBuffer, DWORD dwOffset, BYTE *pucValue) = (HRESULT(WINAPI*)(DWORD*, DWORD, BYTE*))GetOffsetAddress(CLIENT_11122, 0x0000e898);
	DWORD*& var_c00479e78 = *(DWORD**)(GetOffsetAddress(CLIENT_11122, 0x00479e78));

	bool v4 = sub_0000a1d6(a1) == 0;

	return a1 != 1 || OnlineModeFunctional;

	BYTE a5 = 0;
	HRESULT rawr = XLivePBufferGetByte(var_c00479e78, 0, &a5);
	return !a5 || v4;
}

void __stdcall sub_1F98258(int a1, int a2)
{
	int(__thiscall* sub_EA1973)(DWORD *thisptr, WORD a2) = (int(__thiscall*)(DWORD*, WORD))GetOffsetAddress(CLIENT_11122, 0x00211973);
	//bool(__stdcall* sub_0000A276)(int a1) = (bool(__stdcall*)(int))GetOffsetAddress(CLIENT_11122, 0x0000A276);
	void(__thiscall* sub_EABF85)(void *thisptr, int a2) = (void(__thiscall*)(void*, int))GetOffsetAddress(CLIENT_11122, 0x0021BF85);

	__int16 v2; // bp
	int v3; // esi
	char v4; // al
	char v5; // bl
	BYTE *v6; // ebp

	v2 = *(WORD*)(a1 + 112);
	*(BYTE*)(sub_EA1973((DWORD*)a1, 0) + 110) = 0;
	v3 = v2;
	v5 = v4 = sub_0000A276(v2);
	//v5 = v4 = true;
	v6 = (BYTE*)sub_EA1973((DWORD*)a1, v4 == 0);
	*(BYTE*)(a1 + 120) = v5;
	if (v6)
	{
		switch (v3)
		{
		case 0:
			sub_EABF85(v6, 0x800010B);
			v6[110] = 1;
			break;
		case 1:
			sub_EABF85(v6, 0x9000284);
			v6[110] = 1;
			break;
		case 2:
			sub_EABF85(v6, 0xA000285);
			v6[110] = 1;
			break;
		case 3:
			sub_EABF85(v6, 0x8000286);
			v6[110] = 1;
			break;
		case 4:
			sub_EABF85(v6, 0x5000986);
			v6[110] = 1;
			break;
		case 5:
			sub_EABF85(v6, 0x40002ED);
			v6[110] = 1;
			break;
		default:
			sub_EABF85(v6, -1);
			v6[110] = 1;
			break;
		}
	}
}

int __stdcall sub_15FD028(int a1, DWORD *a2)
{
	//bool(__stdcall* sub_0000A276)(int a1) = (bool(__stdcall*)(int))GetOffsetAddress(CLIENT_11122, 0x0000A276);
	int(__cdecl* sub_CC23EA)(int a1) = (int(__cdecl*)(int))GetOffsetAddress(CLIENT_11122, 0x000323EA);
	BOOL(__cdecl* sub_C94544)(int a1) = (BOOL(__cdecl*)(int))GetOffsetAddress(CLIENT_11122, 0x00004544);

	int(__stdcall* sub_C9B198)(int a1) = (int(__stdcall*)(int))GetOffsetAddress(CLIENT_11122, 0x0000b198);
	int(__stdcall* sub_C9B257)(int a1) = (int(__stdcall*)(int))GetOffsetAddress(CLIENT_11122, 0x0000b257);
	int(__stdcall* sub_C9A978)(int a1) = (int(__stdcall*)(int))GetOffsetAddress(CLIENT_11122, 0x0000a978);
	int(__stdcall* sub_C9B32B)(int a1) = (int(__stdcall*)(int))GetOffsetAddress(CLIENT_11122, 0x0000B32B);
	DWORD(WINAPI* XShowGuideUI)(DWORD dwUserIndex) = (DWORD(WINAPI*)(DWORD))GetOffsetAddress(CLIENT_11122, 0x0000e8a4);
	int(__stdcall* sub_C9A307)(int a1) = (int(__stdcall*)(int))GetOffsetAddress(CLIENT_11122, 0x0000A307);

	int v2; // esi
	int result; // eax

	v2 = *a2 & 0xFFFF;
	result = sub_0000A276(v2);
	if ((BYTE)result)
	{
		// I don't think result gets altered from here on?
		// Honestly IDK if this function returns int, bool or void.
		// if void just integrate the if above and delete all the writes to result.
		// and change the return to all those (a1) funcs below to void probs.
		if (v2 != 5)
			sub_CC23EA(v2);
		sub_C94544(20);
		result = sub_C94544(19);
		switch (v2)
		{
		case 0:
			result = sub_C9B198(a1);
			break;
		case 1:
			CustomMenuCall_NetworkType(true);
			//result = sub_C9B257(0);
			break;
		case 2:
			CustomMenuCall_NetworkType(false);
			//result = sub_C9A978(a1);
			break;
		case 3:
			result = sub_C9B32B(a1);
			break;
		case 4:
			result = XShowGuideUI(0);
			break;
		case 5:
			result = sub_C9A307(a1);
			break;
		default:
			return result;
		}
		return 0;
	}
	return result;
}

static int __fastcall sub_250DD8(void *thisptr, DWORD _EDX, int a2, DWORD *a3)//__thiscall
{
	signed int(__cdecl* sub_00263d0e)(signed int* a1) = (signed int(__cdecl*)(signed int*))GetOffsetAddress(CLIENT_11122, 0x00263d0e);
	int(__cdecl* sub_002096da)(int a1, int a2) = (int(__cdecl*)(int, int))GetOffsetAddress(CLIENT_11122, 0x002096da);

	void *v3; // esi
	int v4; // ST04_4
	int v5; // eax

	v3 = thisptr;
	a3 = (DWORD *)(*a3 & 0xFFFF);
	sub_00263d0e((signed int *)&a3);
	v4 = 4;// (*(int(__thiscall **)(void *))(*(DWORD *)v3 + 56))(v3);
	v5 = 3;// (*(int(__thiscall **)(void *))(*(DWORD *)v3 + 52))(v3);
	return sub_002096da(v5, v4);
}

typedef int(__cdecl *tfn_0020B8C3)(DWORD*, DWORD*);
tfn_0020B8C3 pfn_0020B8C3;
static int __cdecl fn_0020B8C3(DWORD *a1, DWORD *a2)
{
	char menu_details[150];
	snprintf(menu_details, 150, "MENU l0:%hX h0:%hX 1:%X 2:%X 3:%X 4:%X 5:%X 6:%X 7:%X", ((WORD*)a2)[0], ((WORD*)a2)[1], a2[1], a2[2], a2[3], a2[4], a2[5], a2[6], a2[7]);
	XLLNDebugLog(0, menu_details);
	int result = pfn_0020B8C3(a1, a2);
	return result;
}

static int __fastcall widget_button_handler_matchmaking(void *thisptr, DWORD _EDX, int a2, DWORD *a3)
{
	int(__thiscall* sub_00212604)(unsigned __int16 *a1, int) = (int(__thiscall*)(unsigned __int16 *, int))GetOffsetAddress(CLIENT_11122, 0x00212604);
	/*WORD*(__thiscall* sub_00211ba1)(DWORD *a1) = (WORD*(__thiscall*)(DWORD*))GetOffsetAddress(CLIENT_11122, 0x00211ba1);
	//unsigned __int16 *dumb = sub_00211ba1((DWORD*)thisptr);
	int result = sub_00212604((unsigned __int16 *)thisptr, 3);
	return result;*/

	unsigned __int16 *v3; // esi
	int(__cdecl *v4)(DWORD); // eax
	int result; // eax

	v3 = (unsigned __int16 *)thisptr;
	v4 = (int(__cdecl *)(DWORD))((DWORD*)thisptr)[1760];
	if (!v4 || (result = v4(*(DWORD *)(*(DWORD *)a2 + 4)), (BYTE)result))
		result = sub_00212604(v3, 3);
	return result;
}

BOOL InitTitlePatches()
{
	if (Title_Version == CLIENT_11081 || Title_Version == CLIENT_11091 || Title_Version == CLIENT_11122) {
		wchar_t* var_c0039bcf0 = (wchar_t*)(GetOffsetAddress(0x0039bce8, 0x0039bce8, 0x0039bcf0));//"Global\\Halo2AlreadyRunning"
		WriteBytes((DWORD)var_c0039bcf0, L"", 2);
	}

	if (Title_Version == CLIENT_11122) {
		DWORD dwBack;

		//pfn_0020B8C3 = (tfn_0020B8C3)DetourFunc((BYTE*)GetOffsetAddress(CLIENT_11122, 0x0020B8C3), (BYTE*)fn_0020B8C3, 7);
		//VirtualProtect(pfn_0020B8C3, 4, PAGE_EXECUTE_READWRITE, &dwBack);

		//WritePointer(GetOffsetAddress(CLIENT_11122, 0x0020dd5d), widget_button_handler_matchmaking);

		// Brightness Button Click Handler.
		WritePointer(GetOffsetAddress(CLIENT_11122, 0x00250ea7), sub_250DD8);
		
		// Mainmenu button press handler.
		WritePointer(GetOffsetAddress(CLIENT_11122, 0x0000b626), sub_15FD028);

		// Mainmenu button enabled handler.
		WritePointer(GetOffsetAddress(CLIENT_11122, 0x0039c09c), sub_1F98258);

		// Licence Check XLivePBufferGetByte
		PatchCall(GetOffsetAddress(CLIENT_11122, 0x0000a276), sub_0000A276);

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
	InitRunLoop();

	return TRUE;
}

BOOL UninitTitlePatches()
{
	UninitRunLoop();
	UninitCustomMenu();
	UninitCustomLanguage();
	return TRUE;
}
