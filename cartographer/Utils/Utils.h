#pragma once
#include <stdio.h>

int FindLineStart(FILE* fp, int lineStrLen);
bool GetFileLine(FILE* fp, char* &fileLine);
bool GetNTStringLine(char* text, int lineNum, char* &line);
char CmpVersions(char* version_base, char* version_alt);
void ReadIniFile(void* fileConfig, bool configIsFILE, const char* header, char* headerVersion, int(interpretSettingFunc)(char* fileLine, char* version, int lineNumber));
void GetVKeyCodeString(int vkey, char* rtnString, int strLen);
void PadCStringWithChar(char* strToPad, int toFullLength, char c);
int GetWidePathFromFullWideFilename(wchar_t* filepath, wchar_t* rtnpath);
LONG GetDWORDRegKey(HKEY hKey, wchar_t* strValueName, DWORD* nValue);
char* custom_label_literal(char* label_escaped);
char* custom_label_escape(char* label_literal);
bool FloatIsNaN(float &vagueFloat);
int HostnameToIp(char* hostname, char* ip, DWORD buflen);
DWORD crc32buf(char* buf, size_t len);
bool ComputeFileCrc32Hash(wchar_t* filepath, DWORD &rtncrc32);
char* encode_rfc3986(char* label_literal, int label_literal_length);
void wcstombs2(char* buffer, wchar_t* text, int buf_len);
bool StrnCaseInsensEqu(char* str1, char* str2, unsigned int chk_len);
void EnsureDirectoryExists(wchar_t* path);
int TrimRemoveConsecutiveSpaces(char* text);
