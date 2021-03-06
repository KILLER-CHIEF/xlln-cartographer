#include "../dllMain.h"
#include "Utils.h"

int FindLineStart(FILE* fp, int lineStrLen)
{
	int fp_offset_orig = ftell(fp);
	for (int i = lineStrLen; i < 255; i++) {
		if (fp_offset_orig - i < 0) {
			fseek(fp, fp_offset_orig, SEEK_SET);
			return 0;
		}
		fseek(fp, fp_offset_orig - i, SEEK_SET);
		int c = 0;
		if ((c = fgetc(fp)) == '\r' || c == '\n') {
			fseek(fp, fp_offset_orig, SEEK_SET);
			return fp_offset_orig - i + 1;
		}
	}
	fseek(fp, fp_offset_orig, SEEK_SET);
	return fp_offset_orig - lineStrLen;
}

bool GetFileLine(FILE* fp, char* &fileLine)
{
	bool moretogo = true;
	int fileLineLen = 256;
	fileLine = (char*)malloc(fileLineLen);
	int fileLineCursor = 0;
	int c;
	while (c = fgetc(fp)) {
		if (c == EOF) {
			moretogo = false;
			break;
		}
		else if (c == '\r') {

		}
		else if (c == '\n') {
			break;
		}
		else {
			fileLine[fileLineCursor++] = c;
		}
		if (fileLineCursor >= fileLineLen - 2) {
			fileLineLen += 256;
			fileLine = (char*)realloc(fileLine, fileLineLen);
		}
	}
	fileLine[fileLineCursor++] = 0;
	if (strlen(fileLine) == 0) {
		free(fileLine);
		fileLine = 0;
	}
	return moretogo || fileLine != 0;
}

bool GetNTStringLine(char* text, int lineNum, char* &line)
{
	int inc_line_num = 0;
	char* line_begin = text;

	while (inc_line_num++ < lineNum) {
		line_begin = strchr(line_begin, '\n');
		if (line_begin++ == 0) {
			return false;
		}
	}

	int line_len = 0;
	char* line_next = strchr(line_begin, '\n');
	if (line_next == 0) {
		line_len = strlen(line_begin);
	}
	else {
		if (*(line_next - 1) == '\r') {
			line_next--;
		}
		line_len = line_next - line_begin;
	}

	if (line_len <= 0) {
		line = 0;
		return true;
	}

	line = (char*)malloc(sizeof(char) * (line_len + 1));

	memcpy(line, line_begin, line_len);
	line[line_len] = 0;

	return true;
}

//if missing versioning parameters, major will always be on leftmost side.
char CmpVersions(char* version_base, char* version_alt)
{
	if (strcmp(version_base, version_alt) == 0)
		return 0b00000;//same

	int versions[2][4];
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < 4; i++) {
			versions[j][i] = 0;
		}
	}

	if (sscanf_s(version_base, "%d.%d.%d.%d", &versions[0][0], &versions[0][1], &versions[0][2], &versions[0][3]) <= 0) {
		return 0b10000;//invalid
	}

	if (sscanf_s(version_alt, "%d.%d.%d.%d", &versions[1][0], &versions[1][1], &versions[1][2], &versions[1][3]) <= 0) {
		return 0b10000;//invalid
	}

	for (int i = 0; i < 4; i++) {
		if (versions[0][i] == versions[1][i]) {
			continue;
		}
		else if (versions[0][i] > versions[1][i]) {//alt is old
			return 0b10000 | (0b1000 >> i);
		}
		else {//alt is new
			return 0b00000 | (0b1000 >> i);
		}
	}

	return 0b00000;//same
	//return 0b01000;//new major
	//return 0b00100;//new minor
	//return 0b00010;//new revision
	//return 0b00001;//new build
	//return 0b11000;//old major
	//return 0b10100;//old minor
	//return 0b10010;//old revision
	//return 0b10001;//old build
}

void ReadIniFile(void* fileConfig, bool configIsFILE, const char* header, char* headerVersion, int(interpretSettingFunc)(char* fileLine, char* version, int lineNumber))
{
	bool foundFirstHeader = false;
	char version[30] = "0";
	bool keepReading = true;
	int lineNumber = 0;
	char* fileLine;
	while (keepReading && ((configIsFILE && GetFileLine((FILE*)fileConfig, fileLine)) || (!configIsFILE && GetNTStringLine((char*)fileConfig, lineNumber, fileLine)))) {
		lineNumber++;
		if (fileLine) {
			if (fileLine[0] == header[0] && sscanf_s(fileLine, header, &version, 30)) {
				foundFirstHeader = true;
				char debugTextBuffer[50];
				snprintf(debugTextBuffer, 50, "Found header on line %d asserting version: %s", lineNumber, version);
				//addDebugText(debugTextBuffer);
				if (CmpVersions(headerVersion, version) == 0) {//does not send this line to interpreter.
					free(fileLine);
					continue;
				}
				else {
					//addDebugText("Incorrect Version! Continue searching!");
				}
			}
			int rtnCode = interpretSettingFunc(fileLine, foundFirstHeader ? version : 0, lineNumber);
			if (!(rtnCode & 0b10)) {
				free(fileLine);
				if (rtnCode & 0b1)
					keepReading = false;
			}
		}
	}
}

void GetVKeyCodeString(int vkey, char* rtnString, int strLen)
{
	snprintf(rtnString, 5, "0x%x", vkey);
	char key_name[20];
	memset(key_name, 0, sizeof(key_name));
	if (vkey >= 0x70 && vkey <= 0x87) {
		int func_num = vkey - 0x70 + 1;
		snprintf(key_name, 20, "VK_F%d", func_num);
	}
	else if (vkey == 0x24) {
		snprintf(key_name, 20, "VK_Home");
	}
	if (strlen(key_name) > 0) {
		snprintf(rtnString + strlen(rtnString), strLen - 5, " - %s", key_name);
	}
}

void PadCStringWithChar(char* strToPad, int toFullLength, char c)
{
	for (int i = strlen(strToPad); i < toFullLength - 1; i++) {
		memset(strToPad + i, c, sizeof(char));
	}
	memset(strToPad + toFullLength - 1, 0, sizeof(char));
}

int GetWidePathFromFullWideFilename(wchar_t* filepath, wchar_t* rtnpath)
{
	wchar_t* offset = wcsrchr(filepath, L'\\');
	wchar_t* off2 = wcsrchr(filepath, L'/');
	offset = offset == NULL ? off2 : ((off2 != NULL && offset < off2) ? off2 : offset);
	if (offset == NULL) {
		return -1;
	}
	swprintf(rtnpath, offset - filepath + 2, filepath);
	return 0;
}

LONG GetDWORDRegKey(HKEY hKey, wchar_t* strValueName, DWORD* nValue)
{
	DWORD dwBufferSize(sizeof(DWORD));
	DWORD nResult(0);
	LONG nError = ::RegQueryValueExW(hKey,
		strValueName,
		0,
		NULL,
		reinterpret_cast<LPBYTE>(&nResult),
		&dwBufferSize);
	if (ERROR_SUCCESS == nError)
	{
		*nValue = nResult;
	}
	return nError;
}

char* custom_label_literal(char* label_escaped)
{
	int label_escaped_length = strlen(label_escaped);
	int length_shift = 0;
	char* label_literal = (char*)malloc(label_escaped_length + 1);

	for (int i = 0; i < label_escaped_length; i++) {
		if (label_escaped[i] == '\\' && label_escaped[i + 1] == '\\') {
			label_literal[i + length_shift] = '\\';
			length_shift--;
			i++;
		}
		else if (label_escaped[i] == '\\' && label_escaped[i + 1] == 'n') {
			label_literal[i + length_shift] = '\n';
			length_shift--;
			i++;
		}
		else if (label_escaped[i] == '\\' && label_escaped[i + 1] == 'r') {
			label_literal[i + length_shift] = '\r';
			length_shift--;
			i++;
		}
		else {
			label_literal[i + length_shift] = label_escaped[i];
		}
	}

	label_literal[label_escaped_length + length_shift] = 0;
	return label_literal;
}

char* custom_label_escape(char* label_literal)
{
	int label_literal_length = strlen(label_literal);
	int length_shift = 0;
	int label_escaped_length = label_literal_length + length_shift + 2;
	char* label_escaped = (char*)malloc(label_escaped_length);

	for (int i = 0; i < label_literal_length; i++) {
		if (label_escaped_length < i + length_shift + 2) {
			label_escaped = (char*)realloc(label_escaped, label_literal_length + length_shift + 2);
		}
		if (label_literal[i] == '\\') {
			label_escaped[i + length_shift] = '\\';
			length_shift++;
			label_escaped[i + length_shift] = '\\';
		}
		else if (label_literal[i] == '\n') {
			label_escaped[i + length_shift] = '\\';
			length_shift++;
			label_escaped[i + length_shift] = 'n';
		}
		else if (label_literal[i] == '\r') {
			label_escaped[i + length_shift] = '\\';
			length_shift++;
			label_escaped[i + length_shift] = 'r';
		}
		else {
			label_escaped[i + length_shift] = label_literal[i];
		}
	}

	label_escaped[label_literal_length + length_shift] = 0;
	return label_escaped;
}

bool FloatIsNaN(float &vagueFloat)
{
	DWORD* vague = (DWORD*)&vagueFloat;
	if ((*vague >= 0x7F800000 && *vague <= 0x7FFFFFFF) || (*vague >= 0xFF800000 && *vague <= 0xFFFFFFFF)) {
		return true;
	}
	return false;
}

int HostnameToIp(char* hostname, char* ip, DWORD buflen)
{
	struct hostent *he;
	struct in_addr **addr_list;
	int i;
	//WSADATA wsaData;
	//WSAStartup(MAKEWORD(2, 2), &wsaData);
	if ((he = gethostbyname(hostname)) == NULL) {
		return WSAGetLastError();
	}

	addr_list = (struct in_addr **) he->h_addr_list;

	for (i = 0; addr_list[i] != NULL; i++)
	{
		//Return the first one;
		strncpy_s(ip, buflen, inet_ntoa(*addr_list[i]), 100);
		return 0;
	}

	return 1;
}

//Copyright (C) 1986 Gary S. Brown.  You may use this program, or
// code or tables extracted from it, as desired without restriction.
typedef DWORD UNS_32_BITS;
static UNS_32_BITS crc_32_tab[] = { /* CRC polynomial 0xedb88320 */
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
	0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
	0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
	0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
	0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
	0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
	0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
	0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
	0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
	0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
	0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
	0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
	0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
	0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
	0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
	0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
	0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
	0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
	0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
	0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
	0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
	0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
	0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
	0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
	0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
	0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
	0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
	0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
	0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
	0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
	0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
	0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
	0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
	0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
	0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
	0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
	0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
	0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
	0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
	0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
	0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

#define UPDC32(octet, crc) (crc_32_tab[((crc) ^ (octet)) & 0xff] ^ ((crc) >> 8))

DWORD crc32buf(char* buf, size_t len)
{
	register DWORD oldcrc32;

	oldcrc32 = 0xFFFFFFFF;

	for (; len; --len, ++buf)
	{
		oldcrc32 = UPDC32(*buf, oldcrc32);
	}

	return ~oldcrc32;
}

bool ComputeFileCrc32Hash(wchar_t* filepath, DWORD &rtncrc32)
{
	register DWORD oldcrc32;

	oldcrc32 = 0xFFFFFFFF;

	const int BUFSIZE = 1024;
	BOOL bResult = FALSE;
	HANDLE hFile = NULL;
	BYTE rgbFile[BUFSIZE];
	DWORD cbRead = 0;

	hFile = CreateFile(filepath,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return false;
	}

	while (bResult = ReadFile(hFile, rgbFile, BUFSIZE, &cbRead, NULL))
	{
		if (cbRead == 0) {
			break;
		}

		for (unsigned int i = 0; i < cbRead; i++) {
			oldcrc32 = UPDC32(rgbFile[i], oldcrc32);
		}
	}

	if (!bResult) {
		return false;
	}

	CloseHandle(hFile);

	rtncrc32 = ~oldcrc32;

	return true;
}

static bool rfc3986_allow(char i)
{
	//isalnum(i)//PoS crashes
	if ((i >= '0' && i <= '9') ||
		(i >= 'a' && i <= 'z') ||
		(i >= 'A' && i <= 'Z') ||
		i == '~' || i == '-' || i == '.' || i == '_')
		return true;
	return false;
}

char* encode_rfc3986(char* label_literal, int label_literal_length)
{
	if (label_literal_length < 0)
		label_literal_length = strlen(label_literal);
	int escaped_buflen = (label_literal_length * 3) + 1;
	char* label_escaped = (char*)malloc(escaped_buflen * sizeof(char));
	int escaped_buff_i = 0;

	for (int i = 0; i < label_literal_length; i++) {
		unsigned char uletter = label_literal[i];
		if (!rfc3986_allow(uletter)) {
			sprintf_s(label_escaped + escaped_buff_i, 4, "%%%02X", uletter);
			escaped_buff_i += 2;
		}
		else {
			label_escaped[escaped_buff_i] = label_literal[i];
		}
		escaped_buff_i++;
	}
	label_escaped[escaped_buff_i] = 0;
	return label_escaped;
}

void wcstombs2(char* buffer, wchar_t* text, int buf_len)
{
	int loop_len = wcslen(text);
	if (loop_len > buf_len)
		loop_len = buf_len;
	int buffer_i = 0;
	for (int i = 0; i < loop_len; i++) {
		if (text[i] >= 0 && text[i] <= 0xFF) {
			buffer[buffer_i++] = (char)text[i];
		}
	}
	buffer[buffer_i] = 0;
}

bool StrnCaseInsensEqu(char* str1, char* str2, unsigned int chk_len)
{
	unsigned int chk_len2 = strlen(str1);
	if (chk_len2 < chk_len) {
		chk_len = chk_len2;
	}
	chk_len2 = strlen(str2);
	if (chk_len2 != chk_len) {
		return false;
	}
	const int case_diff = 'A' - 'a';
	for (unsigned int i = 0; i < chk_len; i++) {
		if (str1[i] != str2[i]) {
			int sa = str1[i];
			if (sa >= 'a' && sa <= 'z') {
				sa += case_diff;
			}
			else if (sa >= 'A' && sa <= 'Z') {
				sa -= case_diff;
			}
			if (sa == str2[i]) {
				continue;
			}
			return false;
		}
	}

	return true;
}

void EnsureDirectoryExists(wchar_t* path)
{
	int buflen = wcslen(path) + 1;
	wchar_t* path2 = (wchar_t*)malloc(sizeof(wchar_t) * buflen);
	memcpy(path2, path, sizeof(wchar_t) * buflen);

	for (int i = 1; i < buflen; i++) {
		if (path2[i] == L'/' || path2[i] == L'\\') {
			wchar_t temp_cut = 0;
			if (path2[i + 1] != 0) {
				temp_cut = path2[i + 1];
				path2[i + 1] = 0;
			}
			CreateDirectoryW(path2, NULL);
			if (temp_cut) {
				path2[i + 1] = temp_cut;
			}
		}
	}

	free(path2);
}

int TrimRemoveConsecutiveSpaces(char* text)
{
	int text_len = strlen(text);
	int text_pos = 0;
	for (int j = 0; j < text_len; j++) {
		if (text_pos == 0) {
			if (text[j] != ' ')
				text[text_pos++] = text[j];
			continue;
		}
		if (!(text[j] == ' ' && text[text_pos - 1] == ' '))
			text[text_pos++] = text[j];
	}
	text[text_pos] = 0;
	if (text[text_pos - 1] == ' ')
		text[--text_pos] = 0;
	return text_pos;//new length
}
