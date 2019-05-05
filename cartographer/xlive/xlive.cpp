#include <Winsock2.h>
#include "../dllMain.h"
#include "xlive.h"
#include "../Utils/UtilHook.h"
#include "../H2MOD/TitlePatches.h"
#include "../H2MOD/Menus/CustomMenu.h"
#include "../H2MOD/Menus/Accounts/AccountManagement.h"
#include <thread>
#include <condition_variable>
#include <atomic>

HWND H2hWnd = NULL;

// #41140
tXLLNLogin XLLNLogin;

// #41141
tXLLNLogout XLLNLogout;

// #41142
tXLLNModifyProperty XLLNModifyProperty;

// #41143
tXLLNDebugLog XLLNDebugLog;

// #1
tXLLNOnlineSetMode XLLNOnlineSetMode = 0;
BOOL OnlineModeFunctional = FALSE;

// #5019
typedef HRESULT(WINAPI *tXLivePBufferSetByte)(void *xebBuffer, DWORD dwOffset, BYTE ucValue);
static tXLivePBufferSetByte XLivePBufferSetByte;
static HRESULT WINAPI XLivePBufferSetByteHook(void *xebBuffer, DWORD dwOffset, BYTE ucValue)
{
	ucValue = 0;
	HRESULT result = XLivePBufferSetByte(xebBuffer, dwOffset, ucValue);
	return result;
}
static DWORD XLivePBufferSetByteHookHelper = (DWORD)XLivePBufferSetByteHook;

// #5001
typedef HRESULT(WINAPI *tXLiveInput)(XLIVE_INPUT_INFO *pPii);
static tXLiveInput XLiveInput;
static HRESULT WINAPI XLiveInputHook(XLIVE_INPUT_INFO *pPii);
static DWORD XLiveInputHookHelper = (DWORD)XLiveInputHook;
static HRESULT WINAPI XLiveInputHook(XLIVE_INPUT_INFO *pPii)
{
	// Undo hook.
	WritePointer(GetOffsetAddress(0x0000E892 + 2, 0x0000e892 + 2, 0x0000E892 + 2), &XLiveInput);
	XLiveInputHookHelper = 0;

	H2hWnd = pPii->hWnd;
	TPSetWindowResizable(pPii->hWnd);

	HRESULT result = XLiveInput(pPii);
	return result;
}

// #5260
typedef DWORD(WINAPI *tXShowSigninUI)(DWORD cPanes, DWORD dwFlags);
static tXShowSigninUI XShowSigninUI;
static DWORD WINAPI XShowSigninUIHook(DWORD cPanes, DWORD dwFlags)
{
	DWORD result;
	if (Title_Version == CLIENT_11122) {
		CustomMenuCall_AccountManagement(1);
		result = ERROR_SUCCESS;
	}
	else {
		//result = XShowSigninUI(cPanes, dwFlags);
		result = XLLNLogin(0, TRUE, 0, 0);
	}
	return result;
}
static DWORD XShowSigninUIHookHelper = (DWORD)XShowSigninUIHook;

// #5215
tXShowGuideUI XShowGuideUI = NULL;
static DWORD WINAPI XShowGuideUIHook(DWORD dwUserIndex)
{
	DWORD result;
	if (CMGuideMenu()) {
		result = ERROR_SUCCESS;
	}
	else {
		result = XShowGuideUI(dwUserIndex);
		//DWORD result = XLLNLogout(0);
	}
	return result;
}
static DWORD XShowGuideUIHookHelper = (DWORD)XShowGuideUIHook;

/*// #20
typedef INT(WINAPI *tXSocketRecvFrom)(SOCKET s, char *buf, int len, int flags, sockaddr *from, int *fromlen);
tXSocketRecvFrom XSocketRecvFrom = NULL;
static INT WINAPI XSocketRecvFromHook(SOCKET s, char *buf, int len, int flags, sockaddr *from, int *fromlen)
{
	INT result = XSocketRecvFrom(s, buf, len, flags, from, fromlen);
	
	return result;
}
static DWORD XSocketRecvFromHookHelper = (DWORD)XSocketRecvFromHook;*/

static SOCKET pcarto_socket_udp_rand = INVALID_SOCKET;

static CRITICAL_SECTION pcarto_lock_customsocketlisten_thread;
static std::thread pcarto_thread_customsocketlisten;
static BOOL pcarto_customsocketlisten_running = FALSE;
static std::condition_variable pcarto_cond_customsocketlisten;
static std::atomic<bool> pcarto_customsocketlisten_exit = TRUE;
static std::atomic<bool> pcarto_customsocketlisten_break_sleep = FALSE;

static VOID CustomSocketListen()
{
	std::mutex mutexPause;
	char buff[1024];
	sockaddr SenderAddr;
	int SenderAddrSize = sizeof(SenderAddr);

	char packetHeadToClient[4] = { XLLN_CUSTOM_PACKET_SENTINEL, XLLNCustomPacketType::CUSTOM_OTHER, 'P', 'C' };

	while (1) {
		//EnterCriticalSection(&pcarto_lock_customsocketlisten_thread);

		int result = 0;
		do {
			result = recvfrom(pcarto_socket_udp_rand, buff, sizeof(buff), 0, &SenderAddr, &SenderAddrSize);
			if (result > 0) {
				if (result > sizeof(packetHeadToClient) && *(unsigned int*)buff == *(unsigned int*)&packetHeadToClient) {
					switch (buff[sizeof(packetHeadToClient)]) {
						case PCartoCustomPacketType::UNKNOWN: {
							XLLNDebugLog(0, "PCartoOnline: Received PCarto 00 response.");
							break;
						}
						default: {
							XLLNDebugLog(0, "PCartoOnline: Received UNKNOWN PCarto packet.");
							continue;
						}
					}
				}
				else {
					XLLNDebugLog(0, "PCartoOnline: Received UNKNOWN Custom UDP 0 packet.");
				}
			}
		} while (result > 0);
		
		//LeaveCriticalSection(&pcarto_lock_customsocketlisten_thread);

		std::unique_lock<std::mutex> lock(mutexPause);
		pcarto_cond_customsocketlisten.wait_for(lock, std::chrono::seconds(1), []() { return pcarto_customsocketlisten_exit == TRUE || pcarto_customsocketlisten_break_sleep == TRUE; });
		if (pcarto_customsocketlisten_exit) {
			break;
		}
		pcarto_customsocketlisten_break_sleep = FALSE;
	}

	shutdown(pcarto_socket_udp_rand, SD_SEND);

	unsigned int retryNumber = 0;
	const unsigned int MAX_RETRY_NUMBER = 2 * 1000 / 10; // 10ms sleep from below. Wait no longer than 2 seconds.

	while (true) {
		int result = recv(pcarto_socket_udp_rand, buff, sizeof(buff), 0);

		if (result == 0) {
			break;
			//return 0; // Client gracefully closed connection.
		}
		else if (result > 0) {
			break;
			//return -1; // Received unexpected data instead of socket closure
		}
		else
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK) {
				if (retryNumber++ == MAX_RETRY_NUMBER) {
					break;
					//return -1; // Client didn't close socket within specified time
				}

				Sleep(10); // wait 10ms
			}
			else {
				break;
				//return -1; // Unexpected error occured
			}
		}
	}

	closesocket(pcarto_socket_udp_rand);
	pcarto_socket_udp_rand = INVALID_SOCKET;
}

static VOID CustomSocketListenStart()
{
	EnterCriticalSection(&pcarto_lock_customsocketlisten_thread);

	if (pcarto_customsocketlisten_running) {
		pcarto_customsocketlisten_break_sleep = TRUE;
		pcarto_cond_customsocketlisten.notify_all();
	}
	else {
		pcarto_customsocketlisten_running = TRUE;
		pcarto_customsocketlisten_exit = FALSE;
		pcarto_thread_customsocketlisten = std::thread(CustomSocketListen);
	}

	LeaveCriticalSection(&pcarto_lock_customsocketlisten_thread);
}
static VOID CustomSocketListenStop()
{
	EnterCriticalSection(&pcarto_lock_customsocketlisten_thread);

	if (pcarto_customsocketlisten_running) {
		pcarto_customsocketlisten_running = FALSE;
		LeaveCriticalSection(&pcarto_lock_customsocketlisten_thread);
		if (pcarto_customsocketlisten_exit == FALSE) {
			pcarto_customsocketlisten_exit = TRUE;
			pcarto_cond_customsocketlisten.notify_all();
			pcarto_thread_customsocketlisten.join();
		}
	}
	else {
		LeaveCriticalSection(&pcarto_lock_customsocketlisten_thread);
	}
}


static DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	srand((unsigned int)time(NULL));

	CustomSocketListenStart();

	return TRUE;
}

static BOOL InitOther()
{
	InitializeCriticalSection(&pcarto_lock_customsocketlisten_thread);

	WSADATA wsaData;
	DWORD result = WSAStartup(2, &wsaData);
	//BOOL success = result == 0;

	if ((pcarto_socket_udp_rand = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
		XLLNDebugLog(0, "PCartoOnline: ERROR: Could not create UDP 0 socket.");
	}

	char broadcast = '1';
	if (setsockopt(pcarto_socket_udp_rand, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0) {
		XLLNDebugLog(0, "PCartoOnline: ERROR: Unable to setsockopt SO_BROADCAST for UDP 0 socket.");
	}

	u_long iMode = 1;
	if (ioctlsocket(pcarto_socket_udp_rand, FIONBIO, &iMode) != NO_ERROR) {
		XLLNDebugLog(0, "PCartoOnline: ERROR: Unable to ioctlsocket FIONBIO 1 for UDP 0 socket.");
	}

	CreateThread(0, NULL, ThreadProc, (LPVOID)hModModule, NULL, NULL);

	return TRUE;
}

static BOOL UninitOther()
{
	CustomSocketListenStop();

	WSACleanup();

	DeleteCriticalSection(&pcarto_lock_customsocketlisten_thread);
	return TRUE;
}

static BOOL WINAPI PostInitXLLN()
{
	// Get XLLN-Online exports.
	HMODULE hDllXllnOnline = GetModuleHandle(L"xlln-online.dll");
	if (hDllXllnOnline) {
		XLLNOnlineSetMode = (tXLLNOnlineSetMode)GetProcAddress(hDllXllnOnline, (PCSTR)1);
		if (!XLLNOnlineSetMode) {
			XLLNDebugLog(0, "PCarto: NOTICE xlln-online.dll XLLNOnlineSetMode NOT FOUND. Playing in offline mode.");
		}
		OnlineModeFunctional = XLLNOnlineSetMode != NULL;
	}
	else {
		XLLNDebugLog(0, "PCarto: NOTICE xlln-online.dll NOT FOUND. Playing in offline mode.");
	}

	return TRUE;
}

namespace XLLNModifyPropertyType {
	enum Type : DWORD {
		UNKNOWN = 0,
		FPS_LIMIT,
		CUSTOM_LOCAL_USER_hIPv4,
		LiveOverLan_BROADCAST_HANDLER,
		POST_INIT_FUNC,
	};
}

BOOL InitXLive()
{
	// Get XLLN exports.
	HMODULE hDllXlive = GetModuleHandle(L"xlive.dll");
	if (!hDllXlive)
		return FALSE;
	XLLNLogin = (tXLLNLogin)GetProcAddress(hDllXlive, (PCSTR)41140);
	if (!XLLNLogin)
		return FALSE;
	XLLNLogout = (tXLLNLogout)GetProcAddress(hDllXlive, (PCSTR)41141);
	if (!XLLNLogout)
		return FALSE;
	XLLNModifyProperty = (tXLLNModifyProperty)GetProcAddress(hDllXlive, (PCSTR)41142);
	if (!XLLNModifyProperty)
		return FALSE;
	XLLNDebugLog = (tXLLNDebugLog)GetProcAddress(hDllXlive, (PCSTR)41143);
	if (!XLLNDebugLog)
		return FALSE;

	XLLNModifyProperty(XLLNModifyPropertyType::POST_INIT_FUNC, (DWORD*)PostInitXLLN, NULL);

	if (Title_Version == CLIENT_11081 || Title_Version == CLIENT_11091 || Title_Version == CLIENT_11122) {
		// Hook imports.
		XLivePBufferSetByte = (tXLivePBufferSetByte)**(DWORD**)(GetOffsetAddress(0x0000E880 + 2, 0x0000E880 + 2, 0x0000E880 + 2));
		WritePointer(GetOffsetAddress(0x0000E880 + 2, 0x0000E880 + 2, 0x0000E880 + 2), &XLivePBufferSetByteHookHelper);

		XLiveInput = (tXLiveInput)**(DWORD**)(GetOffsetAddress(0x0000E892 + 2, 0x0000e892 + 2, 0x0000E892 + 2));
		WritePointer(GetOffsetAddress(0x0000E892 + 2, 0x0000e892 + 2, 0x0000E892 + 2), &XLiveInputHookHelper);

		XShowSigninUI = (tXShowSigninUI)**(DWORD**)(GetOffsetAddress(0x002a7acd + 2, 0x002a861d + 2, 0x002a8769 + 2));
		WritePointer(GetOffsetAddress(0x002a7acd + 2, 0x002a861d + 2, 0x002a8769 + 2), &XShowSigninUIHookHelper);

		XShowGuideUI = (tXShowGuideUI)**(DWORD**)(GetOffsetAddress(0x0000e8a4 + 2, 0x0000e8a4 + 2, 0x0000e8a4 + 2));
		WritePointer(GetOffsetAddress(0x0000e8a4 + 2, 0x0000e8a4 + 2, 0x0000e8a4 + 2), &XShowGuideUIHookHelper);

	}

	//InitOther();

	return TRUE;
}

BOOL UninitXLive()
{
	//UninitOther();

	if (Title_Version == CLIENT_11081 || Title_Version == CLIENT_11091 || Title_Version == CLIENT_11122) {
		// Undo hooks.
		WritePointer(GetOffsetAddress(0x0000E880 + 2, 0x0000E880 + 2, 0x0000E880 + 2), &XLivePBufferSetByte);
		if (XLiveInputHookHelper)
			WritePointer(GetOffsetAddress(0x0000E892 + 2, 0x0000e892 + 2, 0x0000E892 + 2), &XLiveInput);
		WritePointer(GetOffsetAddress(0x002a7acd + 2, 0x002a861d + 2, 0x002a8769 + 2), &XShowSigninUI);
		WritePointer(GetOffsetAddress(0x0000e8a4 + 2, 0x0000e8a4 + 2, 0x0000e8a4 + 2), &XShowGuideUI);
	}
	return TRUE;
}
