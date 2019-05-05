#pragma once

BOOL InitCMFunctions();

void __stdcall sub_21bf85_CMLTD(int thisptr, int label_id, int label_menu_id);
int __stdcall sub_2111ab_CMLTD(int thisptr, int a2, int label_menu_id, int label_id_title, int label_id_description);
int __stdcall sub_20F790_CM(int thisptr, __int16 selected_button_id);
void* __fastcall sub_248beb_deconstructor(LPVOID lpMem, DWORD _EDX, char a2);
char __stdcall sub_210a44_CMLTD(int thisptr, int a2, int* a3, int label_menu_id, int label_id_title, int label_id_description);
int __stdcall CM_CloseMenu(void *thisptr);
int __stdcall CM_PressButtonAnimation(void *thisptr);
void __stdcall CM_CloseMenuStack();
int __stdcall sub_250DD8_ClickHandler2(void *thisptr, bool closeMenu);
int __cdecl CustomMenu_CallHead(int a1, DWORD* menu_vftable_1, DWORD* menu_vftable_2, DWORD menu_button_handler, int number_of_buttons, int menu_wgit_type);
VOID CallWidget(void *wgitFnPtr);
VOID CallWidget(void *wgitFnPtr, bool history);
void CMSetupVFTables(DWORD** menu_vftable_1, DWORD** menu_vftable_2, DWORD CM_LabelButtons, DWORD sub_2111ab_CMLTD_nak, DWORD CM_FuncPtrHelper, DWORD CM_ButtonPreselection, bool isInternalMenuWindow, DWORD sub_248beb_nak_deconstructor);
