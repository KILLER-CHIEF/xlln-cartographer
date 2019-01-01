#pragma once

BOOL InitCMFunctions();

void __stdcall sub_21bf85_CMLTD(int thisptr, int label_id, int label_menu_id);
int __stdcall sub_2111ab_CMLTD(int thisptr, int a2, int label_menu_id, int label_id_title, int label_id_description);
int __stdcall sub_20F790_CM(int thisptr, __int16 selected_button_id);
void* __stdcall sub_248beb_deconstructor(LPVOID lpMem, char a2);
char __stdcall sub_210a44_CMLTD(int thisptr, int a2, int* a3, int label_menu_id, int label_id_title, int label_id_description);
int __cdecl CustomMenu_CallHead(int a1, DWORD* menu_vftable_1, DWORD* menu_vftable_2, DWORD menu_button_handler, int number_of_buttons, int menu_wgit_type);
void CallWgit(int WgitScreenfunctionPtr);
void CallWgit(int WgitScreenfunctionPtr, int open_method);
void CallWgit(int WgitScreenfunctionPtr, int open_method, int menu_wgit_type);
void CMSetupVFTables(DWORD** menu_vftable_1, DWORD** menu_vftable_2, DWORD CM_LabelButtons, DWORD sub_2111ab_CMLTD_nak, DWORD CM_FuncPtrHelper, DWORD CM_ButtonPreselection, bool isInternalMenuWindow, DWORD sub_248beb_nak_deconstructor);
