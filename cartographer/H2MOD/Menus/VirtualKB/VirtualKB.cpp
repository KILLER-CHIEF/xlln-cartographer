#include "../../../dllMain.h"
#include "VirtualKB.h"
#include "../CustomMenuFunctions.h"
#include "../../Language/CustomLabels.h"
#include "../../Language/CustomLanguage.h"
#include "../../../Utils/Utils.h"

static DWORD H2BaseAddr = 0;

static char __stdcall sub_23CC18_CM(int thisptr)//__thiscall
{
	int(__cdecl* sub_4BEB3)(int, int, int, int) = (int(__cdecl*)(int, int, int, int))((char*)H2BaseAddr + 0x4BEB3);
	signed int(__cdecl* sub_287567)(__int16*, unsigned int, __int16*) = (signed int(__cdecl*)(__int16*, unsigned int, __int16*))((char*)H2BaseAddr + 0x287567);
	char(__thiscall* sub_23C7B2)(int) = (char(__thiscall*)(int))((char*)H2BaseAddr + 0x23C7B2);
	char(__thiscall* sub_23B251)(int) = (char(__thiscall*)(int))((char*)H2BaseAddr + 0x23B251);
	char(__thiscall* sub_23C8F3)(int) = (char(__thiscall*)(int))((char*)H2BaseAddr + 0x23C8F3);
	char(__thiscall* sub_23B38C)(int) = (char(__thiscall*)(int))((char*)H2BaseAddr + 0x23B38C);
	char(__thiscall* sub_23B4A9)(int) = (char(__thiscall*)(int))((char*)H2BaseAddr + 0x23B4A9);
	char(__thiscall* sub_23CBA3)(int) = (char(__thiscall*)(int))((char*)H2BaseAddr + 0x23CBA3);
	char(__thiscall* sub_23B543)(int) = (char(__thiscall*)(int))((char*)H2BaseAddr + 0x23B543);
	char(__thiscall* sub_23B583)(int) = (char(__thiscall*)(int))((char*)H2BaseAddr + 0x23B583);
	char(__thiscall* sub_23B597)(int) = (char(__thiscall*)(int))((char*)H2BaseAddr + 0x23B597);
	int(__thiscall* sub_212604)(int, int) = (int(__thiscall*)(int, int))((char*)H2BaseAddr + 0x212604);

	BYTE& byte_978DFE = *((BYTE*)H2BaseAddr + 0x978DFE);

	int v1; // esi
	char result; // al
	//char v3[1024]; // [esp+4h] [ebp-804h]

	v1 = thisptr;
	int VKbMenuType = *(DWORD*)(thisptr + 2652);
	/*if (VKbMenuType != 15)//type: enter message text. hmm something interesting is happening here
	{
		sub_4BEB3((int)v3, 1024, *(DWORD*)(thisptr + 3172), *(signed __int16*)(thisptr + 3180));
		sub_287567(*(__int16**)(v1 + 3172), *(signed __int16*)(v1 + 3180), (__int16*)v3);
	}*/

	result = sub_212604(v1, 3);

	if (VKbMenuType >= 32 && VKbMenuType < 64 && VKbMenuType & 0b10000) {
		wchar_t* returnString = (wchar_t*)*(DWORD*)(thisptr + 0xC64);
		DWORD returnStrBuflen = wcslen(returnString) + 1;
		char* end = (char*)malloc(sizeof(char) * returnStrBuflen);
		wcstombs2(end, returnString, returnStrBuflen);
		strcpy_s((char*)returnString, returnStrBuflen, end);
		free(end);
	}

	return result;
}

//typedef void(__stdcall *tsub_23CD58)(void*, __int16);
//tsub_23CD58 psub_23CD58;
static void __stdcall sub_23CD58_CM(void* thisptr, __int16 a2)//__thiscall
{//virtual key press handler
 //return psub_23CD58(thisptr, a2);

 //void(__thiscall* sub_23CD58)(void*, __int16) = (void(__thiscall*)(void*, __int16))((char*)H2BaseAddr + 0x23CD58);
 //return sub_23CD58(thisptr, a2);

	char(__cdecl*sub_4C6E0)(__int16) = (char(__cdecl*)(__int16))((char*)H2BaseAddr + 0x4C6E0);
	char(__thiscall*sub_23C4C9)(int, __int16) = (char(__thiscall*)(int, __int16))((char*)H2BaseAddr + 0x23C4C9);
	int(__thiscall*sub_23B9DE)(int) = (int(__thiscall*)(int))((char*)H2BaseAddr + 0x23B9DE);
	int(__cdecl*sub_21DD04)(signed int) = (int(__cdecl*)(signed int))((char*)H2BaseAddr + 0x21DD04);
	signed int(__thiscall*sub_23B8F1)(void*) = (signed int(__thiscall*)(void*))((char*)H2BaseAddr + 0x23B8F1);
	int(__cdecl*sub_20E1D8)(int, int, int, int, int, int) = (int(__cdecl*)(int, int, int, int, int, int))((char*)H2BaseAddr + 0x20E1D8);
	//char(__thiscall*sub_23CC18)(int) = (char(__thiscall*)(int))((char*)H2BaseAddr + 0x23CC18);
	int(__thiscall*sub_23BADA)(void*) = (int(__thiscall*)(void*))((char*)H2BaseAddr + 0x23BADA);
	void(__thiscall*sub_23BB13)(void*) = (void(__thiscall*)(void*))((char*)H2BaseAddr + 0x23BB13);
	void(__thiscall*sub_23BB4C)(void*) = (void(__thiscall*)(void*))((char*)H2BaseAddr + 0x23BB4C);
	void(__thiscall*sub_23BB85)(void*) = (void(__thiscall*)(void*))((char*)H2BaseAddr + 0x23BB85);

	int v4; // eax
	__int16 *v5; // eax
	__int16 *v6; // ebx
	int v7; // edi
	signed __int16 v8; // bp
	unsigned int v9; // eax
	bool v10; // cf
	bool v11; // zf
	int v12; // eax
	WORD* v13; // eax
//	int v14; // edi
	//signed int v15; // eax

	int VKbMenuType = *((DWORD*)thisptr + 663);
	if (VKbMenuType != 12 || *((DWORD*)thisptr + 3806) == -1)
	{
		switch (a2)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		case 29:
		case 30:
		case 31:
		case 32:
		case 33:
		case 34:
		case 35://all normal/alphabet vkeyboard keys
			v4 = (*(int(__thiscall**)(void*))(*((DWORD*)thisptr + 64 * a2 + 798) + 76))((void*)((a2 << 8) + (DWORD)thisptr + 0xc78));
			v5 = (__int16 *)(*(int(__thiscall**)(int))(*(DWORD*)v4 + 12))(v4);
			v6 = v5;
			v7 = 0;
			if (*v5)
			{
				do
				{
					v8 = *v5;
					if (sub_4C6E0(*v5))
						v8 = 9633;
					if (!sub_23C4C9((int)thisptr, v8))
						break;
					v5 = &v6[++v7];
				} while (v6[v7]);
			}
			break;
		case 36://backspace
			sub_23B9DE((int)thisptr);
			break;
		case 37://space
			sub_23C4C9((int)thisptr, 32);
			break;
		case 38://move cursor left
			v9 = *((DWORD*)thisptr + 794);
			v10 = v9 < *((DWORD*)thisptr + 793);
			v11 = v9 == *((DWORD*)thisptr + 793);
			*((BYTE*)thisptr + 3188) = 0;
			if (v10 || v11)
				goto LABEL_17;
			v12 = v9 - 2;
			goto LABEL_16;
		case 39://move cursor right
			v13 = (WORD*)*((DWORD*)thisptr + 794);
			*((BYTE*)thisptr + 3188) = 0;
			if (*v13)
			{
				v12 = (int)(v13 + 1);
			LABEL_16:
				*((DWORD*)thisptr + 794) = v12;
			}
		LABEL_17:
			sub_21DD04(6);
			break;
		case 40://Done button
			/*if (VKbMenuType != -1 && (v14 = 0, (v15 = sub_23B8F1(thisptr)) != 0))
			{
				switch (VKbMenuType)//error with message type
				{
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
					if (v15 == 1)
					{
						v14 = 88;
					}
					else if (v15 == 2)
					{
						v14 = 70;
					}
					break;
				case 5:
				case 6:
				case 7:
					if (v15 == 1)
					{
						v14 = 89;
					}
					else if (v15 == 2)
					{
						v14 = 71;
					}
					break;
				default:
					break;
				}
				sub_20E1D8(1, v14, 4, *((unsigned __int16 *)thisptr + 4), 0, 0);//can get rid of this and do custom message
			}
			else*/
		{//success. however it will open other menus like customise new variant
			sub_23CC18_CM((int)thisptr);
		}
		break;
		case 41://shift button
			sub_23BADA(thisptr);
			break;
		case 42://caps lock
			sub_23BB13(thisptr);
			break;
		case 43://"ACCENTS" button
			if (!(VKbMenuType >= 32 && VKbMenuType < 64) && (VKbMenuType == 12 || VKbMenuType == 15 || VKbMenuType == 16))
				sub_23BB4C(thisptr);
			if (VKbMenuType >= 32 && VKbMenuType < 64 && VKbMenuType & 0b01)
				sub_23BB4C(thisptr);
			break;
		case 44://"SYMBOLS" button
			if (!(VKbMenuType >= 32 && VKbMenuType < 64) && (VKbMenuType == 12 || VKbMenuType == 15 || VKbMenuType == 16))
				sub_23BB85(thisptr);
			if (VKbMenuType >= 32 && VKbMenuType < 64 && VKbMenuType & 0b10)
				sub_23BB85(thisptr);
			break;
		default:
			return;
		}
	}
}

//typedef char(__stdcall *tsub_23D060)(void*, int*);
//tsub_23D060 psub_23D060;
static char __stdcall sub_23D060_CM(void* thisptr, int* a2) //__thiscall
{
	//return psub_23D060(thisptr, a2);

	char(__thiscall* sub_23AF4E)(void*, int) = (char(__thiscall*)(void*, int))((char*)H2BaseAddr + 0x23AF4E);
	void(__thiscall* sub_23BBBE)(void*, int, int) = (void(__thiscall*)(void*, int, int))((char*)H2BaseAddr + 0x23BBBE);
	int(__thiscall*sub_23B080)(void*) = (int(__thiscall*)(void*))((char*)H2BaseAddr + 0x23B080);
	int(__cdecl*sub_21DD04)(signed int) = (int(__cdecl*)(signed int))((char*)H2BaseAddr + 0x21DD04);
	int(__thiscall*sub_23B9DE)(int) = (int(__thiscall*)(int))((char*)H2BaseAddr + 0x23B9DE);
	int(__thiscall*sub_23BADA)(void*) = (int(__thiscall*)(void*))((char*)H2BaseAddr + 0x23BADA);
	char(__thiscall*sub_23C4C9)(int, __int16) = (char(__thiscall*)(int, __int16))((char*)H2BaseAddr + 0x23C4C9);
	//void(__thiscall* sub_23CD58)(void*, __int16) = (void(__thiscall*)(void*, __int16))((char*)H2BaseAddr + 0x23CD58);
	char(__thiscall*sub_20EB2B)(void*, int*) = (char(__thiscall*)(void*, int*))((char*)H2BaseAddr + 0x20EB2B);
	void(__thiscall*sub_23B5C1)(void*) = (void(__thiscall*)(void*))((char*)H2BaseAddr + 0x23B5C1);
	int(__cdecl*sub_4BD54)(int) = (int(__cdecl*)(int))((char*)H2BaseAddr + 0x4BD54);
	char(__cdecl*sub_4C6E0)(__int16) = (char(__cdecl*)(__int16))((char*)H2BaseAddr + 0x4C6E0);

	DWORD& dword_3D2D78 = *(DWORD*)((char*)H2BaseAddr + 0x3D2D78);
	DWORD& dword_3D2A78 = *(DWORD*)((char*)H2BaseAddr + 0x3D2A78);
	DWORD& dword_3D2B38 = *(DWORD*)((char*)H2BaseAddr + 0x3D2B38);
	DWORD& dword_3D2CB8 = *(DWORD*)((char*)H2BaseAddr + 0x3D2CB8);
	DWORD& dword_3D2E38 = *(DWORD*)((char*)H2BaseAddr + 0x3D2E38);
	BYTE* byte_3D2F30 = (BYTE*)((char*)H2BaseAddr + 0x3D2F30);

	bool v4; // zf
	unsigned int v6; // eax
	bool v7; // cf
	int v8; // eax
	char result; // al
	WORD* v10; // eax
	WORD* v11; // eax
	char v12; // bl
	DWORD v13; // eax

	signed __int16 v20;

	v4 = a2[0] == 1;
	if (v4 || a2[0] == 2 || a2[0] == 3 || a2[0] == 4)
	{
		v12 = sub_23AF4E((signed __int16*)thisptr, a2[0]);
		switch (v12)
		{
		case 41:
			if (*((BYTE*)thisptr + 3186) & 1)
				goto LABEL_47;
			v13 = dword_3D2D78;
			goto LABEL_48;
		case 42:
			if (*((BYTE*)thisptr + 3186) & 2)
				goto LABEL_47;
			v13 = dword_3D2A78;
			goto LABEL_48;
		case 43:
			if (*((BYTE*)thisptr + 3186) & 4)
				goto LABEL_47;
			v13 = dword_3D2B38;
			goto LABEL_48;
		case 44:
			v13 = dword_3D2CB8;
			if (*((BYTE*)thisptr + 3186) & 8)
				LABEL_47:
			v13 = dword_3D2E38;
		LABEL_48:
			sub_23BBBE(thisptr, (int)v13, 47);
			break;
		default:
			break;
		}
		int VKbMenuType = *((DWORD*)thisptr + 663);
		if (byte_3D2F30[8 * VKbMenuType] & 0xA && (v12 == 43 || v12 == 44))
			v12 = sub_23AF4E((signed __int16 *)thisptr, *a2);
		(*(void(__thiscall **)(void *, BYTE*))(*(DWORD*)thisptr + 36))(thisptr, (BYTE*)thisptr + 256 * v12 + 3192);
		result = 1;
	}
	else
	{
		if (a2[0] == 5)
		{
			switch (a2[2])
			{
			case 1:
			case 13:
				sub_23B080(thisptr);
				break;
			case 6:
			LABEL_9:
				v6 = *((DWORD*)thisptr + 794);
				v7 = v6 < *((DWORD*)thisptr + 793);
				v4 = v6 == *((DWORD*)thisptr + 793);
				*((BYTE*)thisptr + 3188) = 0;
				if (v7 || v4)
					goto LABEL_12;
				v8 = v6 - 2;
				goto LABEL_11;
			case 7:
			LABEL_15:
				v10 = (WORD*)*((DWORD*)thisptr + 794);
				*((BYTE*)thisptr + 3188) = 0;
				if (!*v10)
				{
				LABEL_12:
					sub_21DD04(6);
					return 1;
				}
				v8 = (int)(v10 + 1);
			LABEL_11:
				*((DWORD*)thisptr + 794) = v8;
				goto LABEL_12;
			case 2:
			LABEL_33:
				sub_23B9DE((int)thisptr);
				return 1;
			case 14:
			case 15:
				sub_23BADA(thisptr);
				return 1;
			case 5:
				sub_23C4C9((int)thisptr, 32);
				return 1;
			case 12:
			LABEL_23:
				sub_23CD58_CM(thisptr, 40);
				return 1;
			}
			return sub_20EB2B(thisptr, a2);
		}
		if (a2[0] != 7)
			return sub_20EB2B(thisptr, a2);
		switch (a2[2])
		{
		case 8:
			goto LABEL_33;
		case 13:
			goto LABEL_23;
		case 27:
			sub_23B080(thisptr);
			a2[0] = 5;
			a2[2] = 1;
			return sub_20EB2B(thisptr, a2);
		case 37:
			goto LABEL_9;
		case 38:
		case 40://Up & Down Arrows
			return 1;
		case 39:
			goto LABEL_15;
		case 46:
			sub_23B5C1(thisptr);
			v11 = (WORD*)*((DWORD*)thisptr + 794);
			if (!*v11)
				return sub_20EB2B(thisptr, a2);
			*((BYTE*)thisptr + 3188) = 0;
			if (*v11)
				*((DWORD*)thisptr + 794) = (DWORD)(v11 + 1);
			sub_21DD04(6);
			goto LABEL_33;
		default:
			v20 = *(signed __int16*)&a2[3];
			if (v20 == -1 || sub_4BD54(v20) || sub_4C6E0(v20))
				return sub_20EB2B(thisptr, a2);
			sub_23C4C9((int)thisptr, v20);
			result = 1;
			break;
		}
	}
	return result;
}

static __declspec(naked) void sub_23D060_CM_nak_VKeyTest() {//__thiscall
	__asm {
		mov eax, [esp + 4h]

		push ebp
		push edi
		push esi
		push ecx
		push ebx

		push eax
		push ecx
		call sub_23D060_CM//__stdcall

		pop ebx
		pop ecx
		pop esi
		pop edi
		pop ebp

		retn 4
	}
}

//typedef char(__stdcall *tsub_23CF88)(int, int*);
//tsub_23CF88 psub_23CF88;
static char __stdcall sub_23CF88_CM(int thisptr, int* a2) //__thiscall
{
	//return psub_23CF88(thisptr, a2);

	//char(__thiscall* sub_23CF88)(int, int*) = (char(__thiscall*)(int, int*))((char*)H2BaseAddr + 0x23CF88);
	//return sub_23CF88(thisptr, a2);

	int(__thiscall* sub_211BA1)(int) = (int(__thiscall*)(int))((char*)H2BaseAddr + 0x211BA1);
	int(*sub_209A8C)() = (int(*)())((char*)H2BaseAddr + 0x209A8C);
	void(__cdecl* sub_43233)(signed int) = (void(__cdecl*)(signed int))((char*)H2BaseAddr + 0x43233);
	char(__thiscall* sub_212152)(int, int) = (char(__thiscall*)(int, int))((char*)H2BaseAddr + 0x212152);
	int(__thiscall* sub_20E807)(int) = (int(__thiscall*)(int))((char*)H2BaseAddr + 0x20E807);
	char(__thiscall* sub_212660)(int, int, char) = (char(__thiscall*)(int, int, char))((char*)H2BaseAddr + 0x212660);
	//void(__thiscall* sub_23CD58)(void*, __int16) = (void(__thiscall*)(void*, __int16))((char*)H2BaseAddr + 0x23CD58);
	char(__thiscall* sub_2118F0)(int, int) = (char(__thiscall*)(int, int))((char*)H2BaseAddr + 0x2118F0);

	int v2; // esi
	char v3; // bl
	DWORD* v4; // ebp
	int v5; // ecx
	int VKbMenuType; // eax
	//int v7; // eax
	DWORD* v8; // eax
	char v10; // [esp+13h] [ebp-1h]

	bool v20 = false;

	v2 = thisptr;
	v10 = 0;
	v3 = 0;
	v4 = (DWORD*)sub_211BA1(thisptr);
	if (*a2 == 5)
	{
		v5 = a2[2];
		if (v5)
		{
			if (v5 == 1 || v5 == 13)
				v3 = 1;//close menu button pressed (cancel)
		}
		else
		{
			v10 = 1;
		}
	}
	if (*a2 == 7 && a2[2] == 27 || v3)//close menu esc key
	{
		VKbMenuType = v4[663];
		/*if (VKbMenuType == 7 || VKbMenuType == 5)
		{
			v7 = sub_209A8C();
			if (v7 != -1)
				sub_43233(v7);
		}*/
		v20 = true;
	}
	if (*a2 == 6)//virtual key pressed
	{
		if (!sub_212152(v2, 0))
		{
			if (sub_20E807((int)v4))
			{
				v8 = (DWORD*)sub_20E807((int)v4);
				sub_212660((int)v8, 0, 0);
			}
			sub_212660(v2, 0, 1);
		}
		//goto LABEL_20;
	}
	char result;
	if (v10 || *a2 == 6)
	{
		//LABEL_20:
		sub_23CD58_CM((void*)v4, *(WORD*)(v2 + 0xF8));//handles type input.
		result = 1;
	}
	else {
		result = sub_2118F0(thisptr, (int)a2);//only handles all kinds of menu exiting.
	}
	if (v20) {
		if (VKbMenuType >= 32 && VKbMenuType < 64 && VKbMenuType & 0b10000) {
			wchar_t* returnString = (wchar_t*)*(DWORD*)((BYTE*)v4 + 0xC64);
			DWORD returnStrBuflen = wcslen(returnString) + 1;
			char* end = (char*)malloc(sizeof(char) * returnStrBuflen);
			wcstombs2(end, returnString, returnStrBuflen);
			strcpy_s((char*)returnString, returnStrBuflen, end);
			free(end);
		}
	}
	return result;
}

static __declspec(naked) void sub_23CF88_CM_nak_VKeyTest() {//__thiscall
	__asm {
		mov eax, [esp + 4h]

		push ebp
		push edi
		push esi
		push ecx
		push ebx

		push eax
		push ecx
		call sub_23CF88_CM//__stdcall

		pop ebx
		pop ecx
		pop esi
		pop edi
		pop ebp

		retn 4
	}
}

static void CMSetupVFTablesVKb(DWORD** menu_vftable_2, DWORD** menu_vftable_3, DWORD sub_23bf3e_CMLTD_nak) {

	//clone a brightness menu_vftable_2
	*menu_vftable_2 = (DWORD*)malloc(0xA8);
	memcpy(*menu_vftable_2, (BYTE*)H2BaseAddr + 0x3D302C, 0xA8);//Virtual Keyboard

	//CALL for Title & Description
	*(DWORD*)((DWORD)*menu_vftable_2 + 0x58) = (DWORD)sub_23bf3e_CMLTD_nak;

	//CALL for real keyboard handler end/Done.
	*(DWORD*)((DWORD)*menu_vftable_2 + 0x30) = (DWORD)sub_23D060_CM_nak_VKeyTest;

	//clone a brightness menu_vftable_3
	*menu_vftable_3 = (DWORD*)malloc(0x60);
	memcpy(*menu_vftable_3, (BYTE*)H2BaseAddr + 0x3D30D4, 0x60);//Virtual Keyboard

	//CALL for key handler.
	*(DWORD*)((DWORD)*menu_vftable_3 + 0x30) = (DWORD)sub_23CF88_CM_nak_VKeyTest;
}

static int __stdcall sub_23ae3c_CMLTD(void* thisptr, int label_menu_id, int label_id_title, int label_id_description) {

	int(__thiscall* sub_211973)(int, unsigned __int16) = (int(__thiscall*)(int, unsigned __int16))((char*)H2BaseAddr + 0x211973);
	void(__thiscall* sub_21bf85)(int, int label_id) = (void(__thiscall*)(int, int))((char*)H2BaseAddr + 0x21BF85);
	void(__thiscall* sub_21BA2A)(int) = (void(__thiscall*)(int))((char*)H2BaseAddr + 0x21BA2A);

	int v1 = (int)thisptr;
	int v2 = sub_211973((int)thisptr, 3u);
	//switch (*(DWORD*)(v1 + 2652))//gets the menu_type_id
	sub_21bf85_CMLTD(v1 + 128, label_id_title, label_menu_id);
	if (v2)
		sub_21bf85_CMLTD(v2, label_id_description, label_menu_id);
	int v5 = sub_211973(v1, 2u);
	sub_21BA2A(v5);//*(BYTE*)(v5 + 116) = 1;
	return (*(int(__thiscall**)(int, int))(*(DWORD*)v1 + 36))(v1, v1 + 13432);
}

static int __stdcall sub_23bf3e_CMLTD(int thisptr, int a2, int label_menu_id, int label_id_title, int label_id_description)
{
	int(__cdecl* sub_20c701)(int) = (int(__cdecl*)(int))((char*)H2BaseAddr + 0x20c701);
	void*(__cdecl* sub_287BA9)(void* a1, int a2, unsigned int a3) = (void*(__cdecl*)(void*, int, unsigned int))((char*)H2BaseAddr + 0x287BA9);
	//int(__thiscall* sub_210a44)(int, int, int*) = (int(__thiscall*)(int, int, int*))((char*)H2BaseAddr + 0x210a44);
	int(__thiscall* sub_211e23)(int) = (int(__thiscall*)(int))((char*)H2BaseAddr + 0x211e23);

	void(__thiscall* sub_23BBBE)(void*, int, int) = (void(__thiscall*)(void*, int, int))((char*)H2BaseAddr + 0x23BBBE);
	//int(__thiscall* sub_23AE3C)(void*) = (int(__thiscall*)(void*))((char*)H2BaseAddr + 0x23AE3C);

	DWORD& dword_3D2E38 = *(DWORD*)((char*)H2BaseAddr + 0x3D2E38);
	DWORD& dword_3D2A78 = *(DWORD*)((char*)H2BaseAddr + 0x3D2A78);
	DWORD& dword_3D2CB8 = *(DWORD*)((char*)H2BaseAddr + 0x3D2CB8);
	DWORD& dword_3D2B38 = *(DWORD*)((char*)H2BaseAddr + 0x3D2B38);
	BYTE* byte_3D2F30 = (BYTE*)((char*)H2BaseAddr + 0x3D2F30);

	int* v2; // esi
	int v3; // edi
	unsigned int v4; // eax
	int v5; // ecx
	int result; // eax
	int v7[80];
	int v14[47]; // [esp+74h] [ebp-BCh]

	v2 = (int*)thisptr;
	v3 = sub_20c701(((int*)thisptr)[28]);
	if (v3 != -1)
	{
		v7[0] = 0;
		v7[1] = 1;
		v7[2] = 47;
		v7[3] = (int)v14;
		v7[4] = 0;
		v7[5] = 0;
		sub_287BA9(&v7[6], 0, 80);
		v4 = 0;
		v5 = (int)(v2 + 798);
		do
		{
			v14[v4++] = v5;
			v5 += 256;
		} while (v4 < 0x2F);
		sub_210a44_CMLTD((int)v2, v3, v7, label_menu_id, label_id_title, label_id_description);
	}
	sub_211e23((int)v2);
	sub_23BBBE(v2, (int)&dword_3D2E38, 47);
	sub_23BBBE(v2, (int)&dword_3D2A78, 47);
	sub_23BBBE(v2, (int)&dword_3D2CB8, 47);
	sub_23BBBE(v2, (int)&dword_3D2B38, 47);
	result = sub_23ae3c_CMLTD(v2, label_menu_id, label_id_title, label_id_description);
	int VKbMenuType = v2[663];
	//"SYMBOLS" AND "ACCENTS" buttons are greyed out by default
	if (!(VKbMenuType >= 32 && VKbMenuType < 64)) {
		if (byte_3D2F30[8 * VKbMenuType] & 0xA)
		{
			*((BYTE*)v2 + 14566) = 0;//disables "SYMBOLS" button from being highlightable
			*((BYTE*)v2 + 14310) = 0;//disables "ACCENTS" button from being highlightable
		}
		else
		{
			*((BYTE*)v2 + 15078) = 0;//enable "SYMBOLS" button style
			*((BYTE*)v2 + 14822) = 0;//enable "ACCENTS" button style
		}
	}
	else {
		if (VKbMenuType & 0b01)
			*((BYTE*)v2 + 14822) = 0;//enable "ACCENTS" button style
		else
			*((BYTE*)v2 + 14310) = 0;//disables "ACCENTS" button from being highlightable
		if (VKbMenuType & 0b10)
			*((BYTE*)v2 + 15078) = 0;//enable "SYMBOLS" button style
		else
			*((BYTE*)v2 + 14566) = 0;//disables "SYMBOLS" button from being highlightable

	}
	return result;
}

static __declspec(naked) void sub_23bf3e_CMLTD_nak_VKeyTest() {//__thiscall
	__asm {
		mov eax, [esp + 4h]

		push ebp
		push edi
		push esi
		push ecx
		push ebx

		push 0xFFFFFFF1//label_id_description
		push 0xFFFFFFF0//label_id_title
		push CMLabelMenuId_VKeyTest
		push eax
		push ecx
		call sub_23bf3e_CMLTD//__stdcall

		pop ebx
		pop ecx
		pop esi
		pop edi
		pop ebp

		retn 4
	}
}

static DWORD* menu_vftable_2_VKeyTest = 0;
static DWORD* menu_vftable_3_VKeyTest = 0;

void CMSetupVFTablesVKb_VKeyTest()
{
	H2BaseAddr = GetOffsetAddress(CLIENT_11122, 0);
	CMSetupVFTablesVKb(&menu_vftable_2_VKeyTest, &menu_vftable_3_VKeyTest, (DWORD)sub_23bf3e_CMLTD_nak_VKeyTest);
}

static void* __stdcall sub_23BC45_CM(void* thisptr)//__thiscall
{
	void*(__thiscall* sub_23F756)(void*, __int16, __int16) = (void*(__thiscall*)(void*, __int16, __int16))((char*)H2BaseAddr + 0x23F756);

	void* v1 = thisptr;
	sub_23F756(thisptr, -1, 0);
	//*v1 = &c_virtual_keyboard_button::`vftable';
	//*(DWORD*)v1 = (DWORD)(char*)H2BaseAddr + 0x3D30D4;
	*(DWORD*)v1 = (DWORD)menu_vftable_3_VKeyTest;
	return v1;
}

static void __stdcall sub_28870B_CM(int a1, int a2, int a3, void*(__stdcall* a4)(int), int a5)
{
	for (int i = 0; i < a3; ++i)
	{
		a4(a1);
		a1 += a2;
	}
}

static void* __stdcall sub_23BDF6_CM(void* thisptr, int a2, int a3, int a4) { //__thiscall

	void*(__thiscall* sub_2106A2)(void*, int, int, int, __int16) = (void*(__thiscall*)(void*, int, int, int, __int16))((char*)H2BaseAddr + 0x2106A2);
	void*(__cdecl* sub_287BA9)(void*, int, unsigned int) = (void*(__cdecl*)(void*, int, unsigned int))((char*)H2BaseAddr + 0x287BA9);
	//void(__stdcall* sub_28870B)(int, int, int, void(__thiscall*)(DWORD), int) = (void(__stdcall*)(int, int, int, void(__thiscall*)(DWORD), int))((char*)H2BaseAddr + 0x28870B);
	char(__cdecl* sub_2067FC)(char) = (char(__cdecl*)(char))((char*)H2BaseAddr + 0x2067FC);

	//void*(__thiscall* sub_23BC45)(void*) = (void*(__thiscall*)(void*))((char*)H2BaseAddr + 0x23BC45);
	int(*sub_23B7B2)() = (int(*)())((char*)H2BaseAddr + 0x23B7B2);

	void* v4; // esi@1
	int v5; // eax@1
	int v6; // ecx@2
	//void *v8; // [sp+0h] [bp-10h]@1
	//int v9; // [sp+Ch] [bp-4h]@1

	v4 = thisptr;
	//v8 = thisptr;
	sub_2106A2(thisptr, 167, a2, a3, a4);
	//v9 = 0;
	//*(DWORD*)v4 = &c_screen_virtual_keyboard::`vftable';
	//*(DWORD*)v4 = (DWORD)(char*)H2BaseAddr + 0x3D302C;
	*(DWORD*)v4 = (DWORD)menu_vftable_2_VKeyTest;
	*((DWORD*)v4 + 663) = -1;//VKbMenuType
	*((DWORD*)v4 + 664) = -1;
	sub_287BA9((char*)v4 + 2660, 0, 0x200u);
	*((DWORD*)v4 + 793) = 0;
	*((DWORD*)v4 + 794) = 0;
	*((WORD*)v4 + 1590) = 0;
	*((WORD*)v4 + 1591) = 4;
	*((WORD*)v4 + 1592) = 10;
	*((WORD*)v4 + 1593) = 0;
	*((BYTE*)v4 + 3188) = 0;
	sub_28870B_CM((int)((char*)v4 + 3192), 256, 47, (void*(__stdcall*)(int))sub_23BC45_CM, (int)sub_23B7B2);
	//LOBYTE(v9) = 1;
	*((DWORD*)v4 + 3806) = -1;
	*((BYTE*)v4 + 15228) = 1;
	*((BYTE*)v4 + 15229) = 0;
	v5 = 0;
	do
	{
		v6 = (int)((char *)v4 + 256 * (signed __int16)v5);
		*(WORD*)(v6 + 3440) = v5++;
		*(WORD*)(v6 + 3200) = a4;
	} while ((unsigned __int16)v5 < 0x2Fu);
	sub_2067FC(1);
	return v4;
}

static int __cdecl sub_23C72F_CM(int a1) {
	int(__cdecl* sub_20D2D8)(int) = (int(__cdecl*)(int))((char*)H2BaseAddr + 0x20D2D8);
	int(__cdecl* sub_20B8C3)(int, int) = (int(__cdecl*)(int, int))((char*)H2BaseAddr + 0x20B8C3);

	//void*(__thiscall* sub_23BDF6)(void*, int, int, int) = (void*(__thiscall*)(void*, int, int, int))((char*)H2BaseAddr + 0x23BDF6);

	int v2 = 0;
	int v3 = sub_20D2D8(15232);
	if (v3) {
		v2 = (int)sub_23BDF6_CM((void*)v3, *(DWORD*)(a1 + 4), *(DWORD*)(a1 + 8), *(WORD*)(a1 + 2));
	}
	*(BYTE*)(v2 + 108) = 1;
	sub_20B8C3(v2, a1);
	return v2;
}

static void PoSmbstowcs(wchar_t* destsrc, int maxCount) {
	DWORD endbuflen = sizeof(wchar_t) * maxCount;
	wchar_t* end = (wchar_t*)malloc(endbuflen);
	size_t outSize = 0;
	mbstowcs_s(&outSize, end, maxCount, (char*)destsrc, maxCount);
	memcpy_s(destsrc, endbuflen, end, endbuflen);
	destsrc[maxCount - 1] = 0;
	free(end);
}

void CustomMenuCall_VKeyboard_Inner(wchar_t* textBuffer, __int16 textBufferLen, WORD charsAllowed, int menuIdTitle, int title, int menuIdDesc, int description)
{
	char* lblTitle = H2CustomLanguageGetLabel(menuIdTitle, title);
	char* lblDesc = H2CustomLanguageGetLabel(menuIdDesc, description);
	add_cartographer_label(CMLabelMenuId_VKeyTest, 0xFFFFFFF0, lblTitle, true);
	add_cartographer_label(CMLabelMenuId_VKeyTest, 0xFFFFFFF1, lblDesc, true);

	int(__thiscall*sub_20B0BC)(void*) = (int(__thiscall*)(void*))((char*)H2BaseAddr + 0x20B0BC);
	signed int(__thiscall*sub_20C226)(void*, __int16, int, int, int) = (signed int(__thiscall*)(void*, __int16, int, int, int))((char*)H2BaseAddr + 0x20C226);
	signed int(__thiscall*sub_20C258)(void*, __int16, __int16, int, int, int) = (signed int(__thiscall*)(void*, __int16, __int16, int, int, int))((char*)H2BaseAddr + 0x20C258);
	void*(__thiscall*sub_20B11E)(void*) = (void*(__thiscall*)(void*))((char*)H2BaseAddr + 0x20B11E);

	unsigned int(__thiscall*sub_23B118)(void*, wchar_t*, int) = (unsigned int(__thiscall*)(void*, wchar_t*, int))((char*)H2BaseAddr + 0x23B118);
	int(__cdecl* sub_381FD)() = (int(__cdecl*)())((char*)H2BaseAddr + 0x381FD);
	bool(*sub_38F02)() = (bool(*)())((char*)H2BaseAddr + 0x38F02);
	int(*sub_38F74)() = (int(*)())((char*)H2BaseAddr + 0x38F74);
	void(__thiscall*sub_38F7A)(void*, int, int) = (void(__thiscall*)(void*, int, int))((char*)H2BaseAddr + 0x38F7A);

	//VirtualKeyboardTypes - original
	//0 to 4 - profile name
	//5,6 - variant name
	//7 - variant name default
	//8,9 - playlist name
	//10, 11 - film name
	//12, 15 - enter message text
	//13 - rename squad
	//16 - search for games by its description

	//new types
	//0b10000 - need it in char* (be sure to make the buffer twice as big tho!)
	//0b00 - "SYMBOLS" & "ACCENTS" disabled
	//0b01 - "ACCENTS" enabled
	//0b10 - "SYMBOLS" enabled

	int VKbMenuType = 32;//enter message text
	VKbMenuType |= charsAllowed;

	int a1 = 0;
	int a3 = 0;

	int v4; // esi@1
	char v8[0x20]; // [sp+8h] [bp-20h]@1

	sub_20B0BC(&v8);
	//sub_20C258(&v8, 0, 1 << a1, 2, 4, (int)(char*)H2BaseAddr + 0x23C72F);
	sub_20C258(&v8, 0, 1 << a1, 2, 4, (int)sub_23C72F_CM);
	v4 = (int)sub_20B11E(&v8);
	*(DWORD*)(v4 + 2652) = VKbMenuType;
	DWORD* sgdsf = (DWORD*)(v4 + 2652);
	if (VKbMenuType >= 32 && VKbMenuType < 64 && VKbMenuType & 0b10000) {
		PoSmbstowcs(textBuffer, textBufferLen);
	}
	sub_23B118((void*)v4, textBuffer, textBufferLen);
	//*(DWORD*)(v4 + 2656) = a3;
}
