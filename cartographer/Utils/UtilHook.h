#pragma once

void WriteBytes(DWORD destAddress, LPVOID bytesToWrite, int numBytes);
template <typename value_type>
inline void WriteValue(DWORD offset, value_type data)
{
	WriteBytes(offset, &data, sizeof(data));
}
void WritePointer(DWORD offset, void *ptr);
void *DetourFunc(BYTE *src, const BYTE *dst, const int len);
void RetourFunc(BYTE *src, BYTE *restore, const int len);
void *DetourClassFunc(BYTE *src, const BYTE *dst, const int len);
void RetourClassFunc(BYTE *src, BYTE *restore, const int len);
void PatchCall(DWORD call_addr, DWORD new_function_ptr);
inline void PatchCall(DWORD call_addr, void *new_function_ptr)
{
	PatchCall(call_addr, reinterpret_cast<DWORD>(new_function_ptr));
}
inline void PatchCall(void *call_addr, void *new_function_ptr)
{
	PatchCall(reinterpret_cast<DWORD>(call_addr), reinterpret_cast<DWORD>(new_function_ptr));
}
void PatchWinAPICall(DWORD call_addr, DWORD new_function_ptr);
inline void PatchWinAPICall(DWORD call_addr, void *new_function_ptr)
{
	PatchWinAPICall(call_addr, reinterpret_cast<DWORD>(new_function_ptr));
}
inline void PatchWinAPICall(void *call_addr, void *new_function_ptr)
{
	PatchWinAPICall(reinterpret_cast<DWORD>(call_addr), reinterpret_cast<DWORD>(new_function_ptr));
}
VOID Codecave(DWORD destAddress, VOID(*func)(VOID), BYTE nopCount);
template<int len>
inline void NopFill(DWORD address)
{
	BYTE bytesArray[len];
	memset(bytesArray, 0x90, len);
	WriteBytes(address, bytesArray, len);
}
