#include "../dllMain.h"
#include "UtilHook.h"


void WriteBytes(DWORD destAddress, LPVOID bytesToWrite, int numBytes)
{
	DWORD OldProtection;
	DWORD temp;

	VirtualProtect((LPVOID)destAddress, numBytes, PAGE_EXECUTE_READWRITE, &OldProtection);
	memcpy((LPVOID)destAddress, bytesToWrite, numBytes);
	VirtualProtect((LPVOID)destAddress, numBytes, OldProtection, &temp); //quick fix for exception that happens here
}

void WritePointer(DWORD offset, void *ptr)
{
	BYTE* pbyte = (BYTE*)&ptr;
	BYTE assmNewFuncRel[0x4] = { pbyte[0], pbyte[1], pbyte[2], pbyte[3] };
	WriteBytes(offset, assmNewFuncRel, 0x4);
}
