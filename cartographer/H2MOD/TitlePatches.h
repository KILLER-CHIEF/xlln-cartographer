#pragma once

BOOL InitTitlePatches();
BOOL UninitTitlePatches();

char* __stdcall H2GetLabel(int a1, int label_id, int a3, int a4);

VOID TPSetWindowResizable(HWND hWindow);
