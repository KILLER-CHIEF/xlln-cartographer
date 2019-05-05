#include "../../dllMain.h"
#include "CustomMenuFunctions.h"
#include <chrono>

static DWORD H2BaseAddr = 0;

BOOL InitCMFunctions()
{
	H2BaseAddr = GetOffsetAddress(CLIENT_11122, 0);
	return TRUE;
}

void __stdcall sub_2101a4_CMLTD(int thisptr, int label_id, wchar_t* rtn_label, int label_menu_id);
char __stdcall sub_21bb0b_CMLTD(void* thisptr, __int16 a2, int* aa3, int label_menu_id, int label_id_description);

#pragma pack(push) // Save current alignment setting.
#pragma pack(1) // Specify single-byte alignment.
typedef struct {
	WORD Ukn1;
	WORD Ukn2;
	DWORD Ukn3;
	DWORD Ukn4;
	DWORD Ukn5;
	DWORD Ukn6;
	DWORD Ukn7;
	DWORD Ukn8;
	void*(__cdecl *WgitFnPtr)(void*);
} MENU_SETUP;
#pragma pack(pop) // Return to original alignment setting.

VOID CallWidget(void *wgitFnPtr, bool history)
{
	//CM_CloseMenu function doesn't work with history off
	MENU_SETUP menu_setup;
	menu_setup.Ukn1 = history ? 0 : 3; // 0,1 remember history, 2,3,4,-1 don't remember
	menu_setup.Ukn2 = 1;
	menu_setup.Ukn3 = 4;
	//menu_setup.Ukn3 = 5;// 5 ingame, 5 mainmenu options, 4 press any key / pregame
	menu_setup.Ukn4 = 4;// 0 ingame, 4 mainmenu
	menu_setup.Ukn5 = 0;
	menu_setup.Ukn6 = -1;
	menu_setup.Ukn7 = -1;
	menu_setup.Ukn8 = -1;
	menu_setup.WgitFnPtr = (void*(__cdecl*)(void*))wgitFnPtr;

	int GameGlobals = (int)*(int*)((char*)H2BaseAddr + 0x482D3C);
	if (GameGlobals) {
		BYTE& EngineMode = *(BYTE*)(GameGlobals + 0x8);
		if (EngineMode != 3) {
			menu_setup.Ukn4 = 0;
		}
	}

	void* menu_struct = menu_setup.WgitFnPtr(&menu_setup);
	return;
}

VOID CallWidget(void *wgitFnPtr)
{
	return CallWidget(wgitFnPtr, true);
}

static int nak_keyHandler_CM_thisptr;
static int nak_keyHandler_CM_a2;
static int nak_keyHandler_CM_result;
static int nak_keyHandler_CM_func;

static __declspec(naked) void CM_keyHandler_inner_nak() {
	__asm {
		mov     nak_keyHandler_CM_thisptr, ecx
		mov     nak_keyHandler_CM_a2, edi
	}
	nak_keyHandler_CM_func = *(DWORD*)((int)*(int*)((DWORD)H2BaseAddr + 0x482D3C) + 0x8);
	if (nak_keyHandler_CM_func == 3) {
		nak_keyHandler_CM_func = 0x20EEBE;
	}
	else {
		nak_keyHandler_CM_func = 0x24DC0D;
	}
	nak_keyHandler_CM_func = (int)((char*)H2BaseAddr + nak_keyHandler_CM_func);
	__asm {
		mov     ecx, nak_keyHandler_CM_thisptr
		mov     edi, nak_keyHandler_CM_a2
		jmp     nak_keyHandler_CM_func
	}
}

static __declspec(naked) void CM_keyHandler_outer_nak() {
	__asm {
		mov     nak_keyHandler_CM_thisptr, ecx
		mov     nak_keyHandler_CM_a2, edi
	}
	nak_keyHandler_CM_func = *(DWORD*)((int)*(int*)((DWORD)H2BaseAddr + 0x482D3C) + 0x8);
	if (nak_keyHandler_CM_func == 3) {
		nak_keyHandler_CM_func = 0x20EEBE;
	}
	else {
		nak_keyHandler_CM_func = 0x23D8AE;
	}
	nak_keyHandler_CM_func = (int)((char*)H2BaseAddr + nak_keyHandler_CM_func);
	__asm {
		mov     ecx, nak_keyHandler_CM_thisptr
		mov     edi, nak_keyHandler_CM_a2
		jmp     nak_keyHandler_CM_func
	}
}

void CMSetupVFTables(DWORD** menu_vftable_1, DWORD** menu_vftable_2, DWORD CM_LabelButtons, DWORD sub_2111ab_CMLTD_nak, DWORD CM_FuncPtrHelper, DWORD CM_ButtonPreselection, bool isInternalMenuWindow, DWORD sub_248beb_nak_deconstructor)
{
	//clone a brightness menu_vftable_1
	*menu_vftable_1 = (DWORD*)malloc(0x98);
	memcpy(*menu_vftable_1, (BYTE*)H2BaseAddr + 0x3d96fc, 0x98);//Brightness

	//Button Labels
	if (CM_LabelButtons)
		*(DWORD*)((DWORD)*menu_vftable_1 + 0x90) = (DWORD)CM_LabelButtons;


	//clone a brightness menu_vftable_2
	*menu_vftable_2 = (DWORD*)malloc(0xA8);
	memcpy(*menu_vftable_2, (BYTE*)H2BaseAddr + 0x3D7ABC, 0xA8);//Brightness

	if (sub_248beb_nak_deconstructor)
		*(DWORD*)((DWORD)*menu_vftable_2 + 0x0) = (DWORD)sub_248beb_nak_deconstructor;

	if (isInternalMenuWindow) {
		//*(DWORD*)((DWORD)*menu_vftable_2 + 0x30) = (DWORD)H2BaseAddr + 0x24DC0D;//inner ingame
		//*(DWORD*)((DWORD)*menu_vftable_2 + 0x30) = (DWORD)H2BaseAddr + 0x20EEBE;//both main-mainmenu
		*(DWORD*)((DWORD)*menu_vftable_2 + 0x30) = (DWORD)CM_keyHandler_inner_nak;
	}
	else {
		//*(DWORD*)((DWORD)*menu_vftable_2 + 0x30) = (DWORD)H2BaseAddr + 0x23D8AE;//outer ingame
		//*(DWORD*)((DWORD)*menu_vftable_2 + 0x30) = (DWORD)H2BaseAddr + 0x20EEBE;//both main-mainmenu
		*(DWORD*)((DWORD)*menu_vftable_2 + 0x30) = (DWORD)CM_keyHandler_outer_nak;

		//*(DWORD*)((DWORD)*menu_vftable_2 + 0x30) = (DWORD)sub_23D8AE_CM_nak;

		//*(DWORD*)((DWORD)*menu_vftable_2 + 0xC) = (DWORD)H2BaseAddr + 0x212CD8;//outer main-mainmenu?
		//*(DWORD*)((DWORD)*menu_vftable_2 + 0x30) = (DWORD)H2BaseAddr + 0x24fe49;//gameoptions
	}
	//i have a feeling it could be because 1,3,4 doesn't work. need 1, 5, 4 somehow.

	//*(DWORD*)((DWORD)*menu_vftable_2 + 0x30) = (DWORD)H2BaseAddr + 0x24DC0D;
	//*(DWORD*)((DWORD)*menu_vftable_2 + 0x30) = (DWORD)H2BaseAddr + 0x20EEBE;
	//*(DWORD*)((DWORD)*menu_vftable_2 + 0x30) = (DWORD)H2BaseAddr + 0x23D8AE;
	//*(DWORD*)((DWORD)*menu_vftable_2 + 0x30) = (DWORD)H2BaseAddr + 0x23E76C;//same as 0x23D8AE

	//*(DWORD*)((DWORD)*menu_vftable_2 + 0x30) = (DWORD)H2BaseAddr + 0x24FE49;
	//*(DWORD*)((DWORD)*menu_vftable_2 + 0x30) = (DWORD)H2BaseAddr + 0xD04C;
	//*(DWORD*)((DWORD)*menu_vftable_2 + 0x30) = (DWORD)H2BaseAddr + 0x21FCEE;
	//*(DWORD*)((DWORD)*menu_vftable_2 + 0x30) = (DWORD)H2BaseAddr + 0x23D060;
	//*(DWORD*)((DWORD)*menu_vftable_2 + 0x30) = (DWORD)H2BaseAddr + 0x23F0BA;
	//*(DWORD*)((DWORD)*menu_vftable_2 + 0x30) = (DWORD)H2BaseAddr + 0x2415C4;
	//*(DWORD*)((DWORD)*menu_vftable_2 + 0x30) = (DWORD)H2BaseAddr + 0x245BB1;
	//*(DWORD*)((DWORD)*menu_vftable_2 + 0x30) = (DWORD)H2BaseAddr + 0x246B55;
	//*(DWORD*)((DWORD)*menu_vftable_2 + 0x30) = (DWORD)H2BaseAddr + 0x24A5D9;
	//*(DWORD*)((DWORD)*menu_vftable_2 + 0x30) = (DWORD)H2BaseAddr + 0x24C714;
	//*(DWORD*)((DWORD)*menu_vftable_2 + 0x30) = (DWORD)H2BaseAddr + 0x2531AC;
	//*(DWORD*)((DWORD)*menu_vftable_2 + 0x30) = (DWORD)H2BaseAddr + 0x25C9E8;
	//*(DWORD*)((DWORD)*menu_vftable_2 + 0x30) = (DWORD)H2BaseAddr + 0x25D2D0;


	//*(DWORD*)((DWORD)*menu_vftable_2 + 0xC) = (DWORD)H2BaseAddr + 0x212CD8;//text related?
	//*(DWORD*)((DWORD)*menu_vftable_2 + 0xC) = (DWORD)H2BaseAddr + 0x23D64E;//text related?
	//*(DWORD*)((DWORD)*menu_vftable_2 + 0xC) = (DWORD)H2BaseAddr + 0x24f0eb;//gameoptions breaks text

	//CALL for Title & Description
	if (sub_2111ab_CMLTD_nak)
		*(DWORD*)((DWORD)*menu_vftable_2 + 0x58) = (DWORD)sub_2111ab_CMLTD_nak;
	//Menu Button Preselected Option
	if (CM_ButtonPreselection)
		*(DWORD*)((DWORD)*menu_vftable_2 + 0x5C) = (DWORD)CM_ButtonPreselection;
	//Menu function pointer helper
	if (CM_FuncPtrHelper)
		*(DWORD*)((DWORD)*menu_vftable_2 + 0x98) = (DWORD)CM_FuncPtrHelper;
}

int __stdcall CM_CloseMenu(void *thisptr)
{
	int(__cdecl* sub_002096da)(int a1, int a2) = (int(__cdecl*)(int, int))GetOffsetAddress(CLIENT_11122, 0x002096da);
	int v4 = (*(int(__thiscall **)(void *))(*(DWORD *)thisptr + 56))(thisptr);
	int v5 = (*(int(__thiscall **)(void *))(*(DWORD *)thisptr + 52))(thisptr);
	int result = sub_002096da(v5, v4);
	return result;
}

int __stdcall CM_PressButtonAnimation(void *thisptr)
{
	WORD*(__thiscall* sub_00211ba1)(DWORD *a1) = (WORD*(__thiscall*)(DWORD*))GetOffsetAddress(CLIENT_11122, 0x00211ba1);
	int(__thiscall* sub_00212604)(unsigned __int16 *a1, int) = (int(__thiscall*)(unsigned __int16 *, int))GetOffsetAddress(CLIENT_11122, 0x00212604);
	unsigned __int16 *dumb = sub_00211ba1((DWORD*)thisptr);
	int result = sub_00212604(dumb, 4);
	// 0,1 - reload menu
	// 2,3 - close/hide all menus (at the mainmenu if you open another menu then go back everything from the previous menu stack comes back)
	// 4 - button press animation
	// 5,6,7 - highlight all
	return result;
}

void __stdcall CM_CloseMenuStack()
{
	DWORD d = GetOffsetAddress(CLIENT_11122, 0x00971d24);
	DWORD &d8 = *(DWORD*)(d + (0x44 * 4) + 0x8);
	DWORD &dc = *(DWORD*)(d + (0x44 * 4) + 0xc);
	DWORD &d30 = *(DWORD*)(d + (0x44 * 4) + 0x30);

	int(__cdecl* sub_002096da)(int a1, int a2) = (int(__cdecl*)(int, int))GetOffsetAddress(CLIENT_11122, 0x002096da);
	if (d8 || d30) {
		while (d8 || d30) {
			sub_002096da(4, 4);
		}
		d8 = dc;
		while (d8 || dc || d30) {
			sub_002096da(4, 4);
		}
	}
}

//int __fastcall sub_250DD8(void *thisptr, DWORD _EDX, int a2, DWORD *a3)//__thiscall
int __stdcall sub_250DD8_ClickHandler2(void *thisptr, bool closeMenu)
{
	int(__cdecl* sub_002096da)(int a1, int a2) = (int(__cdecl*)(int, int))GetOffsetAddress(CLIENT_11122, 0x002096da);
	int result = closeMenu;
	if (closeMenu) {
		int v4 = (*(int(__thiscall **)(void *))(*(DWORD *)thisptr + 56))(thisptr);
		int v5 = (*(int(__thiscall **)(void *))(*(DWORD *)thisptr + 52))(thisptr);
		result = sub_002096da(v5, v4);
	}
	return result;
	/*
	WORD*(__thiscall* sub_00211ba1)(DWORD *a1) = (WORD*(__thiscall*)(DWORD*))GetOffsetAddress(CLIENT_11122, 0x00211ba1);
	int(__thiscall* sub_00212604)(unsigned __int16 *a1, int) = (int(__thiscall*)(unsigned __int16 *, int))GetOffsetAddress(CLIENT_11122, 0x00212604);

	int result = closeMenu;
	int GameGlobals = (int)*(int*)((char*)H2BaseAddr + 0x482D3C);
	if (result && GameGlobals) {
		BYTE& EngineMode = *(BYTE*)(GameGlobals + 0x8);
		if (EngineMode != 3) {
			
		}
		else {
			
		}
	}
	return result;*/
}

static int __cdecl sub_250E22_CM(int thisptr, int a2, DWORD* menu_vftable_1, DWORD menu_button_handler, int number_of_buttons)
{
	void*(__thiscall* sub_213B1C)(int, int) = (void*(__thiscall*)(int, int))((char*)H2BaseAddr + 0x213B1C);
	void*(__thiscall* sub_21FFC9)(void*) = (void*(__thiscall*)(void*))((char*)H2BaseAddr + 0x21FFC9);
	void(__stdcall* sub_28870B)(int, int, int, void(__thiscall*)(DWORD), int) = (void(__stdcall*)(int, int, int, void(__thiscall*)(DWORD), int))((char*)H2BaseAddr + 0x28870B);
	int(__thiscall* sub_2113C6)(int) = (int(__thiscall*)(int))((char*)H2BaseAddr + 0x2113C6);
	int(__cdecl* sub_20D1FD)(char*, int numberOfButtons, int) = (int(__cdecl*)(char*, int, int))((char*)H2BaseAddr + 0x20D1FD);
	int(__cdecl* sub_66B33)(int) = (int(__cdecl*)(int))((char*)H2BaseAddr + 0x66B33);
	int(__cdecl* sub_667A0)(int) = (int(__cdecl*)(int))((char*)H2BaseAddr + 0x667A0);
	void*(__thiscall* sub_2113D3)(int, int) = (void*(__thiscall*)(int, int))((char*)H2BaseAddr + 0x2113D3);
	DWORD*& var_003d9700 = *(DWORD**)(GetOffsetAddress(CLIENT_11122, 0x003d9700));

	int v5; // eax@4
	int v2 = thisptr;

	sub_213B1C(thisptr, a2);

	//cloned vftable
	*(DWORD*)v2 = (DWORD)menu_vftable_1 + 0x30;

	int(__thiscall* sub_E5A551)(void* thisptr) = (int(__thiscall*)(void*))((char*)H2BaseAddr + 0xA551);

	sub_28870B((int)((char*)v2 + 176), 132, 4, (void(__thiscall*)(DWORD))sub_21FFC9, (int)sub_E5A551);

	*((BYTE*)v2 + 704) = 0;

	sub_2113C6((int)((char*)v2 + 712));

	*((DWORD*)v2 + 177) = (DWORD)&var_003d9700;

	*((DWORD*)v2 + 181) = (DWORD)v2;

	*((DWORD*)v2 + 182) = (DWORD)menu_button_handler;
	DWORD* safds = ((DWORD*)v2 + 182);

	char editliststr[] = "brightness level settings edit list";

	int v3 = sub_20D1FD(editliststr, number_of_buttons, 4);

	*((DWORD*)v2 + 28) = v3;

	sub_66B33(v3);

	for (unsigned int i = 0; i < *(DWORD*)(*((DWORD*)v2 + 28) + 32); ++i) {
		sub_667A0(*((DWORD*)v2 + 28));
	}
	if (v2 == -708) {
		v5 = 0;
	}
	else {
		v5 = v2 + 712;
	}

	sub_2113D3(v2 + 172, v5);

	return (int)v2;
}

static int __cdecl sub_248B17_CM(int thisptr, int a2, int a3, int a4, DWORD* menu_vftable_1, DWORD* menu_vftable_2, DWORD menu_button_handler, int number_of_buttons, int menu_wgit_type)
{
	void*(__thiscall* sub_211159)(void*, int wgit_id, int, int, int, int) = (void*(__thiscall*)(void*, int, int, int, int, int))((char*)H2BaseAddr + 0x211159);
	//int(__thiscall* sub_250E22)(int, int) = (int(__thiscall*)(int, int))((char*)H2BaseAddr + 0x250E22);

	//The GUI type and the Labels on it are pre-written from that WgitID
	//272, 259
	sub_211159((void*)thisptr, menu_wgit_type, a2, a3, a4, thisptr + 2656);

	*(DWORD*)thisptr = (DWORD)menu_vftable_2;

	sub_250E22_CM(thisptr + 2656, a4, menu_vftable_1, menu_button_handler, number_of_buttons);

	return thisptr;
}

int __cdecl CustomMenu_CallHead(int a1, DWORD* menu_vftable_1, DWORD* menu_vftable_2, DWORD menu_button_handler, int number_of_buttons, int menu_wgit_type)
{
	int(__cdecl* Allocator)(int) = (int(__cdecl*)(int))((char*)H2BaseAddr + 0x20D2D8);
	int(__cdecl* sub_20B8C3)(int, int) = (int(__cdecl*)(int, int))((char*)H2BaseAddr + 0x20B8C3);

	int menu_struct = Allocator(3388);
	int& menu_id = ((int*)menu_struct)[28];
	if (menu_struct) {
		menu_struct = sub_248B17_CM(menu_struct, *(DWORD*)(a1 + 4), *(DWORD*)(a1 + 8), *(WORD*)(a1 + 2), menu_vftable_1, menu_vftable_2, menu_button_handler, number_of_buttons, menu_wgit_type);
	}
	*(BYTE *)(menu_struct + 0x6C) = 1;
	//*(BYTE *)(menu_struct + 0xd20) = 1;
	// If you crash here then you probably forgot to initialise the custom menu.
	sub_20B8C3(menu_struct, a1);

	return menu_struct;
}

int __stdcall sub_20F790_CM(int thisptr, __int16 selected_button_id)
{
	int(__thiscall* sub_213F50)(int, __int16) = (int(__thiscall*)(int, __int16))((char*)H2BaseAddr + 0x213F50);
	int(__thiscall* sub_20F790)(int) = (int(__thiscall*)(int))((char*)H2BaseAddr + 0x20F790);

	sub_213F50(thisptr + 2656, selected_button_id);
	return sub_20F790(thisptr);
}

void* __fastcall sub_248beb_deconstructor(LPVOID lpMem, DWORD _EDX, char a2)
{
	int(__thiscall* sub_248b90)(void*) = (int(__thiscall*)(void*))GetOffsetAddress(CLIENT_11122, 0x248b90);
	int(__cdecl* sub_287c23)(void*) = (int(__cdecl*)(void*))GetOffsetAddress(CLIENT_11122, 0x287c23);

	sub_248b90((void*)lpMem);
	if (a2 & 1) {
		sub_287c23((void*)lpMem);
	}
	return (void*)lpMem;
}

static void* __stdcall sub_20f8ae_CMLTD(void* thisptr, __int16 a2, int* a3, int label_menu_id, int label_id_description)
{
	int(__cdecl* sub_20D2D8)(int) = (int(__cdecl*)(int))((char*)H2BaseAddr + 0x20D2D8);
	void*(__thiscall* sub_20F576)(void*, int) = (void*(__thiscall*)(void*, int))((char*)H2BaseAddr + 0x20F576);
	void*(__thiscall* sub_20F65D)(void*, __int16) = (void*(__thiscall*)(void*, __int16))((char*)H2BaseAddr + 0x20F65D);
	int(__thiscall* sub_21208E)(void*, int) = (int(__thiscall*)(void*, int))((char*)H2BaseAddr + 0x21208E);
	//char(__thiscall* sub_21bb0b)(void*, __int16, int*) = (char(__thiscall*)(void*, __int16, int*))((char*)H2BaseAddr + 0x21bb0b);

	void* v3; // edi@1
	int v4; // eax@2
	int v5; // eax@3
	int v6; // eax@4
	void* v7; // esi@7

	v3 = thisptr;
	v5 = 0;
	if (*(BYTE*)a3 & 0x10)
	{
		v4 = sub_20D2D8(252);
		if (v4)
		{
			v5 = (int)sub_20F576((void*)v4, *((WORD*)v3 + 4));
		}
	}
	else
	{
		v6 = sub_20D2D8(1212);
		if (v6)
		{
			v5 = (int)sub_20F65D((void*)v6, *((WORD*)v3 + 4));
		}
	}
	v7 = (void*)v5;
	if (v5)
	{
		*(BYTE*)(v5 + 108) = 1;
		sub_21208E(v3, v5);
		sub_21bb0b_CMLTD(v7, a2, a3, label_menu_id, label_id_description);
	}
	return v7;
}

static char __stdcall sub_21bb0b_CMLTD(void* thisptr, __int16 a2, int* aa3, int label_menu_id, int label_id_description)
{
	int(__thiscall* sub_211BA1)(int) = (int(__thiscall*)(int))((char*)H2BaseAddr + 0x211BA1);
	//void(__thiscall* sub_2101a4)(int, int, wchar_t*) = (void(__thiscall*)(int, int label_id, wchar_t* rtn_label))((char*)H2BaseAddr + 0x2101A4);
	int(__thiscall* sub_2116D2)(int, int) = (int(__thiscall*)(int, int))((char*)H2BaseAddr + 0x2116D2);
	int(__thiscall* sub_22DF53)(void*, wchar_t*, int, int, int, __int16, int, int, int) = (int(__thiscall*)(void*, wchar_t*, int, int, int, __int16, int, int, int))((char*)H2BaseAddr + 0x22DF53);
	int(*sub_20BB89)() = (int(*)())((char*)H2BaseAddr + 0x20BB89);
	char(__cdecl* sub_99D1F)(int, int) = (char(__cdecl*)(int, int))((char*)H2BaseAddr + 0x99D1F);

	int a3 = (int)aa3;
	int v3; // esi@1
	int v4; // eax@1
	int v5; // edx@1
	__int16 v6; // bx@1
	bool v7; // zf@1
	int v8; // ebp@6
	int v9; // ecx@8
	int v10; // edx@8
	void *v11; // eax@8
	int v12; // edi@9
	__int16 v13; // bx@9
	int v14; // ST30_4@10
	int v15; // eax@12
	int v16; // eax@12
	int v18; // [sp+10h] [bp-414h]@2
	int v19[3]; // [sp+14h] [bp-410h]@8
	wchar_t v20[512]; // [sp+20h] [bp-404h]@1

	v3 = (int)thisptr;
	v4 = sub_211BA1((int)thisptr);
	*(WORD *)(v3 + 10) = a2;
	*(DWORD *)(v3 + 112) = *(WORD *)(a3 + 6);
	v5 = *(DWORD *)a3;
	v6 = 0;
	v7 = (*(DWORD *)a3 & 1) == 0;
	v20[0] = 0;
	if (v7)
		v18 = 2 - ((v5 & 2) != 0);
	else
		v18 = 0;
	if (v5 & 4)
		v6 = 1;
	*(WORD *)(v3 + 104) = *(WORD *)(a3 + 4) - 1;
	*(WORD *)(v3 + 106) = *(WORD *)(a3 + 40);
	v8 = 1;
	if (!(*(BYTE *)a3 & 8))
		v8 = *(WORD *)(a3 + 10);
	v9 = *(DWORD *)(a3 + 20);
	v19[0] = *(DWORD *)(a3 + 16);
	v10 = *(DWORD *)(a3 + 24);
	v19[1] = v9;
	v19[2] = v10;
	//sub_2101a4(v4, *(DWORD *)(a3 + 36), v20);//description label_id 2nd par
	sub_2101a4_CMLTD(v4, label_id_description, v20, label_menu_id);
	sub_2116D2(v3, (int)&aa3[7]);
	v11 = (void *)(*(int(__thiscall **)(int))(*(DWORD *)v3 + 76))(v3);
	sub_22DF53(v11, v20, -1, v8, (int)v19, v6, -1, v18, -1);
	if (*(BYTE *)a3 & 8)
	{
		v12 = sub_20BB89();
		v13 = *(WORD *)((*(int(__thiscall **)(int))(*(DWORD *)v3 + 76))(v3) + 20) | 4;
		*(WORD *)((*(int(__thiscall **)(DWORD))(*(DWORD *)v3 + 76))(v3) + 20) = v13;
		if (v12)
		{
			v14 = *(DWORD *)(v12 + 76);
			*(DWORD *)((*(int(__thiscall **)(DWORD))(*(DWORD *)v3 + 76))(v3) + 32) = v14;
		}
		*(WORD *)((*(int(__thiscall **)(DWORD))(*(DWORD *)v3 + 76))(v3) + 60) = 0;
	}
	v15 = (*(int(__thiscall **)(DWORD))(*(DWORD *)v3 + 76))(v3);
	v16 = (*(int(__thiscall **)(int))(*(DWORD *)v15 + 12))(v15);
	return sub_99D1F(v16, v8);
}

static int __stdcall sub_20fb1b_CMLTD(void* thisptr, int label_menu_id, int label_id_description)
{
	int& dword_482290 = *(int*)((char*)H2BaseAddr + 0x482290);

	int(__thiscall* sub_20F815)(int) = (int(__thiscall*)(int))((char*)H2BaseAddr + 0x20F815);
	int(__thiscall* sub_20E8C9)(void*, int) = (int(__thiscall*)(void*, int))((char*)H2BaseAddr + 0x20E8C9);
	int(__thiscall* sub_212604)(int, int) = (int(__thiscall*)(int, int))((char*)H2BaseAddr + 0x212604);
	int(__thiscall* sub_20E9CE)(void*, int) = (int(__thiscall*)(void*, int))((char*)H2BaseAddr + 0x20E9CE);
	int(__thiscall* sub_20EA52)(void*, int, int) = (int(__thiscall*)(void*, int, int))((char*)H2BaseAddr + 0x20EA52);
	//void*(__thiscall* sub_20f8ae)(void*, __int16, int*) = (void*(__thiscall*)(void*, __int16, int*))((char*)H2BaseAddr + 0x20f8ae);
	int(__thiscall* sub_20E94D)(void*, int) = (int(__thiscall*)(void*, int))((char*)H2BaseAddr + 0x20E94D);

	BYTE* v1; // esi@1
	int v2; // ebx@1
	unsigned int v3; // edi@5
	int v4; // ebp@6
	int v5; // ecx@7
	int v6; // eax@7
	int v7; // eax@9
	int v8; // esi@9
	unsigned int v9; // edi@14
	int v10; // ebp@15
	int v11; // ecx@17
	int v12; // eax@17
	int v13; // esi@19
	int v14; // ebp@24
	int v15; // ecx@26
	int v16; // eax@26
	int v17; // ebp@28
	int i; // edi@28
	int v19; // esi@29
	bool v20; // sf@34
			  //unsigned __int8 v21; // of@34
	unsigned int v22; // esi@35
	int v23; // edi@36
	int v24; // ecx@37
	int v25; // eax@37
	void* v26; // eax@39
	unsigned int v27; // edi@43
	int v28; // ecx@45
	int v29; // eax@45
	int v30; // eax@47
	int v31; // esi@47
	char v33; // [sp+Bh] [bp-Dh]@3
	BYTE* v34; // [sp+Ch] [bp-Ch]@1
	int v35; // [sp+10h] [bp-8h]@24
	int v36; // [sp+10h] [bp-8h]@44
	int v37; // [sp+14h] [bp-4h]@25

	v1 = (BYTE*)thisptr;
	v34 = (BYTE*)thisptr;
	v2 = sub_20F815((int)thisptr);//thisptr is from beginning :)
	if (v2)
	{
		if ((v1[66] >> 1) & 1 || (v33 = 0, v1[66] & 1))
			v33 = 1;
		v3 = 0;
		if (*(DWORD*)(v2 + 36) > 0)
		{
			v4 = 0;
			do
			{
				v5 = *(DWORD*)(v2 + 40);
				v6 = 0;
				if (v5 != -1)
					v6 = dword_482290 + v5;
				v7 = sub_20E8C9(v1, v4 + v6);
				v8 = v7;
				if (v7)
				{
					if (v33)
						sub_212604(v7, *((DWORD*)v34 + 13));
					*(WORD*)(v8 + 10) = v3;
				}
				v1 = v34;
				++v3;
				v4 += 56;
			} while (v3 < *(DWORD*)(v2 + 36));
		}
		v9 = 0;
		if (*(DWORD*)(v2 + 60) > 0)
		{
			v10 = 0;
			while (1)
			{
				v11 = *(DWORD*)(v2 + 64);
				v12 = 0;
				if (v11 != -1)
					v12 = v11 + dword_482290;
				v13 = sub_20E9CE(v1, v12 + v10);
				if (v13)
				{
					if (v33)
						sub_212604(v13, *((DWORD*)v34 + 13));
					*(WORD*)(v13 + 10) = v9;
				}
				++v9;
				v10 += 36;
				if (v9 >= *(DWORD*)(v2 + 60))
					break;
				v1 = v34;
			}
		}
		v14 = 0;
		v35 = 0;
		if (*(DWORD*)(v2 + 68) > 0)
		{
			v37 = 0;
			bool varesult = 0;
			do
			{
				v15 = *(DWORD*)(v2 + 72);
				v16 = 0;
				if (v15 != -1)
					v16 = dword_482290 + v15;
				v17 = v16 + v14;
				for (i = 0; i < *(BYTE*)(v17 + 17); ++i)
				{
					v19 = sub_20EA52(v34, i, v17);
					if (v19)
					{
						if (v33)
							sub_212604(v19, *((DWORD*)v34 + 13));
						*(WORD*)(v19 + 10) = v35;
					}
				}
				v14 = v37 + 24;

				//varesult = __OFSUB__(v35 + 1, *(DWORD*)(v2 + 68));

				int va1 = v35 + 1;
				int va2 = *(DWORD*)(v2 + 68);
				int vreax;
				varesult = 0;
				__asm {
					//save register
					mov vreax, eax

					mov eax, va1
					cmp eax, va2
					//jump if Overflow Flag is not set.
					JNO toEnd
					mov al, 1
					mov varesult, al

					toEnd :
					//restore original register
					mov eax, vreax
				}

				v20 = v35++ + 1 - *(DWORD*)(v2 + 68) < 0;
				v37 += 24;
			} while (v20 ^ varesult);
		}
		v22 = 0;
		if (*(DWORD*)(v2 + 28) > 0)
		{
			v23 = 0;
			do
			{
				v24 = *(DWORD*)(v2 + 32);
				v25 = 0;
				if (v24 != -1)
					v25 = dword_482290 + v24;
				v26 = sub_20f8ae_CMLTD(v34, v22, (int*)(v23 + v25), label_menu_id, label_id_description);//(int*)(v23 + v25) contains description
				if (v26 && v33)
					sub_212604((int)v26, *((DWORD*)v34 + 13));
				++v22;
				v23 += 44;
			} while (v22 < *(DWORD*)(v2 + 28));
		}
		v27 = 0;
		if (*(DWORD*)(v2 + 44) <= 0)
		{
			v1 = v34;
		}
		else
		{
			v36 = 0;
			do
			{
				v28 = *(DWORD*)(v2 + 48);
				v29 = 0;
				if (v28 != -1)
					v29 = dword_482290 + v28;
				v30 = sub_20E94D(v34, v29 + v36);
				v31 = v30;
				if (v30)
				{
					if (v33)
						sub_212604(v30, *((DWORD*)v34 + 13));
					*(WORD*)(v31 + 10) = v27;
				}
				v36 += 76;
				++v27;
			} while (v27 < *(DWORD*)(v2 + 44));
			v1 = v34;
		}
	}
	return (*(int(__thiscall **)(BYTE*))(*(DWORD*)v1 + 96))(v1);
}

static int __stdcall sub_2107df_CMLTD(int thisptr, int* a2, char a3, int label_menu_id, int label_id_description)
{
	int(__cdecl* sub_20C701)(int) = (int(__cdecl*)(int))((char*)H2BaseAddr + 0x20C701);
	int(__cdecl* sub_239623)(unsigned __int16) = (int(__cdecl*)(unsigned __int16))((char*)H2BaseAddr + 0x239623);
	int(__thiscall* sub_20F815)(int) = (int(__thiscall*)(int))((char*)H2BaseAddr + 0x20F815);
	int(__thiscall* sub_21208E)(void*, int) = (int(__thiscall*)(void*, int))((char*)H2BaseAddr + 0x21208E);
	int(__thiscall* sub_214990)(void*, char) = (int(__thiscall*)(void*, char))((char*)H2BaseAddr + 0x214990);
	int(__thiscall* sub_2113D3)(void*, int) = (int(__thiscall*)(void*, int))((char*)H2BaseAddr + 0x2113D3);
	//int(__thiscall* sub_20fb1b)(void*) = (int(__thiscall*)(void*))((char*)H2BaseAddr + 0x20fb1b);
	int(__cdecl* sub_20F402)(void*, int) = (int(__cdecl*)(void*, int))((char*)H2BaseAddr + 0x20F402);

	int *v3; // edi@1
	int v4; // esi@1
	int result; // eax@2
	int v6; // ebp@4
	int v7; // eax@4
	int v8; // ebx@6
	unsigned int v9; // ebp@9
	int v10; // eax@10
	bool v11; // zf@10
	int *v12; // eax@10
	void *v13; // edi@14
	int v14; // ecx@17
	int v15; // eax@17
	int v16; // ebx@18
	int v17; // ebp@20
	int v18; // ecx@23
	int v19; // [sp+Ch] [bp+4h]@4

	v3 = a2;
	v4 = thisptr;
	if (a3
		|| (result = *(WORD*)(thisptr + 2552), (signed __int16)result >= 0)
		&& (result = (signed __int16)result, (signed __int16)result < a2[1]))
	{
		v6 = 0;
		v19 = 0;
		v7 = sub_20C701(*(DWORD*)(thisptr + 112));
		if (v7 != -1)
		{
			v19 = sub_239623(v7);
			v6 = v19;
		}
		result = sub_20F815(v4);
		v8 = result;
		if (v6)
		{
			if (result)
			{
				*(WORD*)(v4 + 104) = *(WORD*)(result + 2) - 1;
				if (a3)
				{
					v9 = 0;
					if (*(DWORD*)(result + 4) > 0)
					{
						do
						{
							v10 = v3[4 * *(WORD*)(v4 + 2552) + 3];
							v11 = *(DWORD*)(v10 + 4 * v9) == 0;
							v12 = (int*)(v10 + 4 * v9);
							if (v11)
								break;
							sub_21208E((void*)v4, *v12);
							DWORD aa = **(DWORD**)(v3[4 * *(WORD*)(v4 + 2552) + 3] + 4 * v9++) + 84;
							//(*(void(**)(void))(aa))();
							(*(int(__thiscall**)(int))(aa))(*v12);//wasn't able to trigger and check but assume works due to similar case below.
						} while (v9 < *(DWORD*)(v8 + 4));
					}
					if (*(DWORD*)(v8 + 12) > 0)
					{
						sub_21208E((void*)v4, v3[4 * (*(WORD*)(v4 + 2552) + 1)]);
						sub_214990((void*)v3[4 * (*(WORD*)(v4 + 2552) + 1)], a3);
						if (*(BYTE*)v19 & 2)
						{
							*(BYTE*)(v3[4 * (*(WORD*)(v4 + 2552) + 1)] + 166) = 1;
							v13 = (void*)(v3[4 * (*(WORD*)(v4 + 2552) + 1)] + 168);
							if (v4 == -2628)
							{
								sub_2113D3(v13, 0);
								result = sub_20fb1b_CMLTD((void*)0xFFFFF5BC, label_menu_id, label_id_description);
							}
							else
							{
								sub_2113D3(v13, v4 + 2632);
								result = sub_20fb1b_CMLTD((void*)v4, label_menu_id, label_id_description);
							}
							return result;
						}
					}
					return sub_20fb1b_CMLTD((void*)v4, label_menu_id, label_id_description);
				}
				v14 = (int)&v3[4 * *(WORD*)(v4 + 2552)];
				v15 = *(DWORD*)(v14 + 8);
				if (v15 <= 0)
				{
					v18 = *(DWORD*)(v14 + 16);
					if (v18)
					{
						sub_21208E((void*)v4, v18);
						sub_214990((void*)v3[4 * (*(WORD*)(v4 + 2552) + 1)], 0);
						if (*(BYTE*)v6 & 2)
						{
							*(BYTE*)(v3[4 * (*(WORD*)(v4 + 2552) + 1)] + 166) = 1;
							sub_20F402((void *)(v3[4 * (*(WORD*)(v4 + 2552) + 1)] + 168), v4 + 2628);
						}
					}
					return sub_20fb1b_CMLTD((void*)v4, label_menu_id, label_id_description);
				}
				v16 = *(DWORD*)(v8 + 4);
				if (v16 > v15)
					v16 = *(DWORD*)(v14 + 8);
				v17 = 0;
				if (v16 <= 0)
					return sub_20fb1b_CMLTD((void*)v4, label_menu_id, label_id_description);
				do
				{
					sub_21208E((void*)v4, *(DWORD*)(v3[4 * *(WORD*)(v4 + 0x9F8) + 3] + 4 * v17));
					v10 = v3[4 * *(WORD*)(v4 + 2552) + 3];
					DWORD* ab = (DWORD*)(v10 + 4 * v17++);
					if (*ab == 0)
						break;
					DWORD aa = **(DWORD**)ab + 84;
					(*(int(__thiscall**)(int))(aa))(*ab);
				} while (v17 < v16);
				result = sub_20fb1b_CMLTD((void*)v4, label_menu_id, label_id_description);
			}
		}
	}
	return result;
}

static void __cdecl sub_3e3ac_CMLTD(int a1, int label_id, wchar_t* rtn_label, int label_menu_id)
{
	int& dword_479e70 = *(int*)((char*)H2BaseAddr + 0x479e70);
	int& dword_47cd54 = *(int*)((char*)H2BaseAddr + 0x47cd54);
	int& dword_47cd50 = *(int*)((char*)H2BaseAddr + 0x47cd50);

	int(__cdecl* sub_381fd)() = (int(__cdecl*)())((char*)H2BaseAddr + 0x381fd);
	void(__thiscall* sub_3e332)(int, int, wchar_t*, int, int) = (void(__thiscall*)(int, int, wchar_t*, int, int))((char*)H2BaseAddr + 0x3e332);

	if (a1 != -1) {
		int v3 = sub_381fd();
		int v4 = dword_47cd54 + *(DWORD*)(dword_47cd50 + 16 * (unsigned __int16)a1 + 8);

		sub_3e332(
			(int)dword_479e70 + 28 * (v3 + 14),
			label_id,
			rtn_label,
			label_menu_id,//*(WORD*)(v4 + 4 * (v3 + 14) - 40),
			*(WORD*)(v4 + 4 * (v3 + 14) - 38));
	}
}


static char __stdcall sub_20fd41_CMLTD(void* thisptr, int label_menu_id, int label_id_title)
{
	int& dword_482290 = *(int*)((char*)H2BaseAddr + 0x482290);

	int(__cdecl* sub_20bb89)() = (int(__cdecl*)())((char*)H2BaseAddr + 0x20bb89);
	int(__cdecl* sub_20c701)(int) = (int(__cdecl*)(int))((char*)H2BaseAddr + 0x20c701);
	int(__cdecl* sub_239623)(int) = (int(__cdecl*)(int))((char*)H2BaseAddr + 0x239623);
	int(__thiscall* sub_21208e)(int, int) = (int(__thiscall*)(int, int))((char*)H2BaseAddr + 0x21208e);
	void(__thiscall* sub_21bf85)(int, int) = (void(__thiscall*)(int, int))((char*)H2BaseAddr + 0x21bf85);
	int(__cdecl* sub_4bd27)(wchar_t*, wchar_t*, int) = (int(__cdecl*)(wchar_t*, wchar_t*, int))((char*)H2BaseAddr + 0x4bd27);
	char(__thiscall* sub_21bc81)(void*, __int16, __int16, int*, __int16, int*) = (char(__thiscall*)(void*, __int16, __int16, int*, __int16, int*))((char*)H2BaseAddr + 0x21bc81);
	int(__thiscall* sub_21baa9)(DWORD, wchar_t*) = (int(__thiscall*)(DWORD, wchar_t*))((char*)H2BaseAddr + 0x21baa9);

	void* v1; // ebx@1
	int v2; // edi@1
	int v3; // eax@1
	int v4; // ST10_4@3
	int v5; // esi@3
	int v6; // eax@3
	int v7; // esi@7
	int v8; // eax@7
	__int16 v9; // bp@10
	char result; // al@11
	DWORD v11; // esi@12
	DWORD v112;
	int v12; // eax@14
			 //int v13; // edx@15
	int v14; // eax@21
	wchar_t* v15; // eax@21
	int v16; // eax@22
	int v17; // [sp+10h] [bp-424h]@1
	__int16 v18; // [sp+14h] [bp-420h]@5
	int var41C[6]; // [sp+18h] [bp-41Ch]@3
	wchar_t label[512];

	v1 = thisptr;
	v2 = sub_20bb89();
	v17 = 0;

	DWORD menu_id = ((DWORD*)v1)[0x1C];
	v3 = sub_20c701(menu_id);

	if (v3 != -1) {
		v17 = sub_239623(v3);
	}
	v4 = ((DWORD*)v1)[0x1C];
	label[0] = 0;
	v5 = 0;
	v6 = sub_20c701(v4);

	if (v6 != -1) {
		v5 = sub_239623(v6);
	}
	v18 = 0;
	if (v5 && *(DWORD*)(v5 + 0x20) > 0)
	{
		v7 = *(DWORD*)(v5 + 0x24);
		v8 = 0;
		if (v7 != -1) {
			v8 = v7 + dword_482290;
		}
		v18 = *(WORD*)(v8 + 2);
	}
	var41C[0] = 1065353216;
	var41C[1] = 1060320051;
	var41C[2] = 1060320051;
	var41C[3] = 1060320051;
	v9 = 1;
	if (!v17 || (result = ~(unsigned __int8)(*(DWORD*)v17 >> 2) & 1) != 0)
	{
		v11 = (int)((char*)v1 + 0x80);
		v112 = *(DWORD*)v11 + 0x4C;
		var41C[4] = -6553500;
		var41C[5] = 6553680;

		sub_21208e((int)v1, v11);

		if (v2)
		{
			var41C[0] = *(DWORD*)(v2 + 0x168);
			var41C[1] = *(DWORD*)(v2 + 0x16C);
			var41C[2] = *(DWORD*)(v2 + 0x170);
			var41C[3] = *(DWORD*)(v2 + 0x174);
			if (v17) {
				v12 = *(DWORD*)v17;
				if (*(DWORD*)v17 & 1)
				{
					v9 = *(WORD*)(v2 + 0x166);
					var41C[4] = *(DWORD*)(v2 + 0x1A8);
					var41C[5] = *(DWORD*)(v2 + 0x1AC);
				}
				else if (v12 & 8)
				{
					v9 = *(WORD*)(v2 + 0x164);
					var41C[4] = *(DWORD*)(v2 + 0x198);
					var41C[5] = *(DWORD*)(v2 + 0x19C);
				}
				else if (v12 & 0x10)
				{
					v9 = *(WORD*)(v2 + 0x162);
					var41C[4] = *(DWORD*)(v2 + 0x188);
					var41C[5] = *(DWORD*)(v2 + 0x18C);
				}
				else
				{
					v9 = *(WORD*)(v2 + 0x160);
					var41C[4] = *(DWORD*)(v2 + 0x178);
					var41C[5] = *(DWORD*)(v2 + 0x17C);
				}

				//sub_21bf85(v11, *(DWORD*)(v17 + 0x2C)); //title label_id
				sub_21bf85_CMLTD(v11, label_id_title, label_menu_id);

				v14 = (*(int(__thiscall**)(DWORD))(v112))(v11);
				v15 = (*(wchar_t*(__thiscall**)(int))(*(DWORD*)v14 + 0xC))(v14);

				sub_4bd27(label, v15, 0x200);
			}
		}

		sub_21bc81((void*)v11, 1, v18, var41C, v9, &var41C[4]);

		v16 = (*(int(__thiscall**)(int))(v112))(v11);
		*(WORD*)(v16 + 0x14) |= 2u;

		result = sub_21baa9(v11, label);

		*((WORD*)v1 + 0xEA) = 0x7FFF;
	}
	return result;
}

char __stdcall sub_210a44_CMLTD(int thisptr, int a2, int* a3, int label_menu_id, int label_id_title, int label_id_description)
{
	int& dword_482290 = *(int*)((char*)H2BaseAddr + 0x482290);

	int(__cdecl* sub_20c701)(int) = (int(__cdecl*)(int))((char*)H2BaseAddr + 0x20c701);
	int(__cdecl* sub_239623)(int) = (int(__cdecl*)(int))((char*)H2BaseAddr + 0x239623);
	//char(__thiscall* sub_20fd41)(int) = (char(__thiscall*)(int))((char*)H2BaseAddr + 0x20fd41);
	int(__thiscall* sub_20ff73)(int, char) = (int(__thiscall*)(int, char))((char*)H2BaseAddr + 0x20ff73);
	int(__thiscall* sub_20f975)(int, WORD) = (int(__thiscall*)(int, WORD))((char*)H2BaseAddr + 0x20f975);
	//int(__thiscall* sub_2107df)(int, int*, char) = (int(__thiscall*)(int, int*, char))((char*)H2BaseAddr + 0x2107df);
	int(__thiscall* sub_21208e)(int thisptr, int a2) = (int(__thiscall*)(int, int))((char*)H2BaseAddr + 0x21208e);

	int v3 = thisptr;
	int v5 = 0;
	int v6 = sub_20c701(*(DWORD*)(thisptr + 0x70));
	if (v6 != -1) {
		v5 = sub_239623(v6);
	}
	sub_20fd41_CMLTD((void*)v3, label_menu_id, label_id_title);
	char result = sub_20ff73(v3, 1);
	if (v5)
	{
		result = a3[1] == -1;
		*(BYTE*)(v3 + 0x9fc) = (*(DWORD*)v5 >> 5) & 1;
		signed int v8 = *(DWORD*)(v5 + 0x20);
		char v13 = result;
		if (v8 > 0)
		{
			int v9 = *(DWORD*)(v5 + 0x24);
			int v10 = 0;
			if (v9 != -1) {
				v10 = v9 + dword_482290;
			}
			bool v11 = v8 == 1;
			if (v8 > 1)
			{
				if (*a3)
				{
					int v12 = *(DWORD*)(*a3 + 0x14);
					*(WORD*)(v3 + 0x9f8) = 0;
					*(WORD*)(v3 + 0x68) = *(WORD*)(v10 + 2) - 1;
					sub_21208e(v3, *a3);
					if (*(DWORD*)(v5 + 0x20) > 0)
					{
						__int16 v4 = 0;
						do
						{
							*(WORD*)(v12 + 0x9f8) = v4;
							sub_2107df_CMLTD(v12, a3, v13, label_menu_id, label_id_description);
							v12 = *(DWORD*)(v12 + 0x18);
							++v4;
						} while ((unsigned int)v4 < *(DWORD*)(v5 + 0x20));
					}
					return sub_20f975(v3, *(WORD*)(v5 + 0x28) - 1);
				}
				v11 = v8 == 1;
			}
			if (v11 || *(BYTE*)v5 & 2)
			{
				*(WORD*)(v3 + 0x9f8) = 0;
				sub_2107df_CMLTD(v3, a3, v13, label_menu_id, label_id_description);
			}
			return sub_20f975(v3, *(WORD*)(v5 + 0x28) - 1);
		}
	}
	return result;
}

int __stdcall sub_2111ab_CMLTD(int thisptr, int a2, int label_menu_id, int label_id_title, int label_id_description)
{
	int(__cdecl* sub_20c701)(int) = (int(__cdecl*)(int))((char*)H2BaseAddr + 0x20c701);
	void*(__cdecl* sub_287BA9)(void* a1, int a2, unsigned int a3) = (void*(__cdecl*)(void*, int, unsigned int))((char*)H2BaseAddr + 0x287BA9);
	//int(__thiscall* sub_210a44)(int, int, int*) = (int(__thiscall*)(int, int, int*))((char*)H2BaseAddr + 0x210a44);
	int(__cdecl* sub_239623)(int) = (int(__cdecl*)(int))((char*)H2BaseAddr + 0x239623);
	int(__thiscall* sub_211e23)(int) = (int(__thiscall*)(int))((char*)H2BaseAddr + 0x211e23);

	int v2 = thisptr;
	int v3 = sub_20c701(*(DWORD*)(thisptr + 0x70));
	int v4 = *(DWORD*)(v2 + 0xA5C);

	int var68[0x50];
	var68[0] = 0;
	var68[1] = 1;
	var68[2] = 0;
	var68[3] = 0;
	var68[4] = v4;
	var68[5] = 0;

	sub_287BA9(&var68[6], 0, 0x50u);
	sub_210a44_CMLTD(v2, v3, var68, label_menu_id, label_id_title, label_id_description);
	int v6 = sub_20c701(*(DWORD*)(v2 + 0x70));

	if (v6 != -1)
	{
		int v7 = sub_239623(v6);
		if (v7)
		{
			if (!(*(BYTE*)v7 & 2))
			{
				int v8 = *(DWORD*)(v2 + 0xA5C);
				if (v8) {
					*(BYTE*)(v8 + 0xA2) = 1;
				}
			}
		}
	}
	return sub_211e23(v2);
}

static void __stdcall sub_2101a4_CMLTD(int thisptr, int label_id, wchar_t* rtn_label, int label_menu_id)
{
	int(__cdecl* sub_20c701)(int) = (int(__cdecl*)(int))((char*)H2BaseAddr + 0x20c701);
	int(__cdecl* sub_15C9623)(int) = (int(__cdecl*)(int))((char*)H2BaseAddr + 0x239623);
	//void(__cdecl* sub_3e3ac)(int, int, wchar_t*) = (void(__cdecl*)(int, int, wchar_t*))((char*)H2BaseAddr + 0x3e3ac);

	*rtn_label = 0;
	if (label_id != -1) {
		int menu_id = *(DWORD*)(thisptr + 112);
		int v3 = sub_20c701(menu_id);
		if (v3 != -1) {
			int v4 = sub_15C9623(v3);
			if (v4) {
				sub_3e3ac_CMLTD(*(DWORD*)(v4 + 28), label_id, rtn_label, label_menu_id);
			}
		}
	}
}

//void(__thiscall *sub_21bf85)(int, int label_id) = (void(__thiscall*)(int, int))(GetOffsetAddress(CLIENT_11122, 0x0021bf85));
void __stdcall sub_21bf85_CMLTD(int thisptr, int label_id, int label_menu_id)
{
	int(__thiscall* sub_391BA1)(int) = (int(__thiscall*)(int))((char*)H2BaseAddr + 0x211ba1);
	void(__thiscall* sub_2101a4)(int, int, wchar_t*) = (void(__thiscall*)(int, int, wchar_t*))((char*)H2BaseAddr + 0x2101a4);

	int v2 = thisptr;
	if (label_id != -1) {
		int v3 = sub_391BA1(thisptr);

		if (v3) {
			wchar_t tmp[512];

			sub_2101a4_CMLTD(v3, label_id, tmp, label_menu_id);

			int v4 = (*(int(__thiscall **)(int))(*(DWORD*)v2 + 76))(v2);
			(*(int(__thiscall **)(int, int))(*(DWORD*)v4 + 4))(v4, (int)tmp);
		}
	}
}
