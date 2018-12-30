#pragma once

void WriteBytes(DWORD destAddress, LPVOID bytesToWrite, int numBytes);
void WritePointer(DWORD offset, void *ptr);
