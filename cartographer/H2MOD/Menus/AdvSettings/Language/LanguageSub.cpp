#include "../../../../dllMain.h"
#include "LanguageSub.h"
#include "../../CustomMenuFunctions.h"
#include "../../../Language/CustomLabels.h"
#include "../../../Language/CustomLanguage.h"
#include "../../Error/Error.h"
#include <map>

static int CM_Language_Main = 0;
static int Language_Sub_Count = 0;
static std::map<int, int> cm_lang_variant_map;
static std::map<int, int> cm_lang_other_lang_map;

static void CM_Language_Sub_Setup_Buttons() {
	Language_Sub_Count = 0;
	cm_lang_variant_map.clear();
	cm_lang_other_lang_map.clear();
	for (auto const &ent1 : custom_languages) {
		if (CM_Language_Main == -1) {
			if (ent1->other) {
				add_cartographer_label(CMLabelMenuId_Language, 0x40 + Language_Sub_Count, ent1->lang_name, true);
				cm_lang_variant_map[Language_Sub_Count] = ent1->lang_variant;
				cm_lang_other_lang_map[Language_Sub_Count] = ent1->lang_base;
				Language_Sub_Count++;
			}
		}
		else {
			if (ent1->lang_base == CM_Language_Main && !ent1->other) {
				if (ent1->lang_variant == 0) {
					char* base_var_proto = H2CustomLanguageGetLabel(CMLabelMenuId_Language, 0xFFFFFFF2);
					DWORD Base_Var_Name_buflen = strlen(base_var_proto) + 256;
					char* Base_Var_Name = (char*)malloc(Base_Var_Name_buflen);
					snprintf(Base_Var_Name, Base_Var_Name_buflen, base_var_proto, ent1->lang_name);
					add_cartographer_label(CMLabelMenuId_Language, 0x40, Base_Var_Name, true);
					free(Base_Var_Name);
				}
				else {
					add_cartographer_label(CMLabelMenuId_Language, 0x40 + Language_Sub_Count, ent1->lang_name, true);
				}
				cm_lang_variant_map[Language_Sub_Count] = ent1->lang_variant;
				Language_Sub_Count++;
			}
		}
	}
}

static void __fastcall c_list_widget_label_buttons(DWORD _ECX, DWORD _EDX, int a1, int a2)
{
	int(__thiscall *sub_211909)(int, int, int, int) = (int(__thiscall*)(int, int, int, int))(GetOffsetAddress(CLIENT_11122, 0x00211909));

	__int16 button_id = *(WORD*)(a1 + 112);
	int v3 = sub_211909(a1, 6, 0, 0);
	if (v3) {
		sub_21bf85_CMLTD(v3, button_id + 1, CMLabelMenuId_Language);
	}
}

static int __fastcall widget_title_description(int a1, DWORD _EDX, char a2)
{
	return sub_2111ab_CMLTD(a1, a2, CMLabelMenuId_Language, 0xFFFFFFF3, 0xFFFFFFF4);
}

static int __fastcall widget_button_handler(void *thisptr, DWORD _EDX, int a2, DWORD *a3)
{
	unsigned __int16 button_id = *a3 & 0xFFFF;
	setCustomLanguage(CM_Language_Main == -1 ? cm_lang_other_lang_map[button_id] : CM_Language_Main, cm_lang_variant_map[button_id]);
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
	return CustomMenu_CallHead(a1, menu_vftable_1, menu_vftable_2, (DWORD)widget_button_handler, Language_Sub_Count, 272);
}

static int(__cdecl *widget_function_ptr_helper())(int)
{
	return widget_call_head;
}

void CMSetupVFTables_LanguageSub()
{
	CMSetupVFTables(&menu_vftable_1, &menu_vftable_2, (DWORD)c_list_widget_label_buttons, (DWORD)widget_title_description, (DWORD)widget_function_ptr_helper, (DWORD)widget_preselected_button, true, 0);
}

bool CustomMenuCall_LanguageSub(int langMain)
{
	CM_Language_Main = langMain;
	CM_Language_Sub_Setup_Buttons();
	if (Language_Sub_Count > 0) {
		CallWidget(widget_call_head);
		return true;
	}
	else {
		CustomMenuCall_Error_Inner(CMLabelMenuId_Error, 4, 5);
	}
	return false;
}
