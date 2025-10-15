#ifdef BLT_USE_WSOCK

#define WIN32_LEAN_AND_MEAN 1
#include "InitState.h"
#include "util/util.h"
#include <filesystem>
#include <windows.h>
#include <tchar.h>

#include <memory>

#pragma pack(1)

#pragma region ALLFUNC(FUNC)

// Export with https://www.nirsoft.net/utils/dll_export_viewer.html into wsock.def.pre
// Then run
//   awk 'BEGIN {print "#define ALLFUNC(FUNC) \\"} { print "\tFUNC(" i++ "," $1 "," $4 ") \\" } \
//   END {print ""; print "#define ALLFUNC_COUNT " NR}' wsock.def.pre
// Or to generate the definition file, use
//   awk 'BEGIN { print "EXPORTS" } { print $1 "=_WSOCK_EXPORT_" $1 " @" $4 } ' wsock.def.pre > wsock.def

#define ALLFUNC(FUNC)                 \
	FUNC(__WSAFDIsSet)                \
	FUNC(accept)                      \
	FUNC(AcceptEx)                    \
	FUNC(bind)                        \
	FUNC(closesocket)                 \
	FUNC(connect)                     \
	FUNC(dn_expand)                   \
	FUNC(EnumProtocolsA)              \
	FUNC(EnumProtocolsW)              \
	FUNC(GetAcceptExSockaddrs)        \
	FUNC(GetAddressByNameA)           \
	FUNC(GetAddressByNameW)           \
	FUNC(gethostbyaddr)               \
	FUNC(gethostbyname)               \
	FUNC(gethostname)                 \
	FUNC(GetNameByTypeA)              \
	FUNC(GetNameByTypeW)              \
	FUNC(getnetbyname)                \
	FUNC(getpeername)                 \
	FUNC(getprotobyname)              \
	FUNC(getprotobynumber)            \
	FUNC(getservbyname)               \
	FUNC(getservbyport)               \
	FUNC(GetServiceA)                 \
	FUNC(GetServiceW)                 \
	FUNC(getsockname)                 \
	FUNC(getsockopt)                  \
	FUNC(GetTypeByNameA)              \
	FUNC(GetTypeByNameW)              \
	FUNC(htonl)                       \
	FUNC(htons)                       \
	FUNC(inet_addr)                   \
	FUNC(inet_network)                \
	FUNC(inet_ntoa)                   \
	FUNC(ioctlsocket)                 \
	FUNC(listen)                      \
	FUNC(MigrateWinsockConfiguration) \
	FUNC(NPLoadNameSpaces)            \
	FUNC(ntohl)                       \
	FUNC(ntohs)                       \
	FUNC(rcmd)                        \
	FUNC(recv)                        \
	FUNC(recvfrom)                    \
	FUNC(rexec)                       \
	FUNC(rresvport)                   \
	FUNC(s_perror)                    \
	FUNC(select)                      \
	FUNC(send)                        \
	FUNC(sendto)                      \
	FUNC(sethostname)                 \
	FUNC(SetServiceA)                 \
	FUNC(SetServiceW)                 \
	FUNC(setsockopt)                  \
	FUNC(shutdown)                    \
	FUNC(socket)                      \
	FUNC(TransmitFile)                \
	FUNC(WEP)                         \
	FUNC(WSAAsyncGetHostByAddr)       \
	FUNC(WSAAsyncGetHostByName)       \
	FUNC(WSAAsyncGetProtoByName)      \
	FUNC(WSAAsyncGetProtoByNumber)    \
	FUNC(WSAAsyncGetServByName)       \
	FUNC(WSAAsyncGetServByPort)       \
	FUNC(WSAAsyncSelect)              \
	FUNC(WSACancelAsyncRequest)       \
	FUNC(WSACancelBlockingCall)       \
	FUNC(WSACleanup)                  \
	FUNC(WSAGetLastError)             \
	FUNC(WSAIsBlocking)               \
	FUNC(WSApSetPostRoutine)          \
	FUNC(WSARecvEx)                   \
	FUNC(WSASetBlockingHook)          \
	FUNC(WSASetLastError)             \
	FUNC(WSAStartup)                  \
	FUNC(WSAUnhookBlockingHook)

#pragma endregion

// For some reason PhysX explodes if you store these in an array on 64-bit! I have no idea why, but it's fine if we use
// a struct, so...
struct farproc_dll
{
#define STRUCT(name) FARPROC o##name;
	ALLFUNC(STRUCT)
#undef STRUCT
} farproc;

namespace raidhook
{
	namespace
	{
		struct DllState
		{
			HINSTANCE hLThis = nullptr;
			HMODULE hL = nullptr;
		};

		struct DllStateDestroyer
		{
			void operator()(DllState* state)
			{
				DestroyStates();

				if (state && state->hL)
				{
					FreeLibrary(state->hL);
				}
			}
		};

		std::unique_ptr<DllState, DllStateDestroyer> State;
	} // namespace
} // namespace raidhook

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	using namespace raidhook;

	static HINSTANCE hLDebugger = 0;

	if (reason == DLL_PROCESS_ATTACH)
	{
		// only load SBLT for raid
		// accomodate SBLT_DLL_UPDATER and possibily other executables users placed next to the games executable
		TCHAR raid_exe[] = _T("raid_win64_release");
		TCHAR processPath[MAX_PATH + 1];
		GetModuleFileName(NULL, processPath, MAX_PATH + 1); // Get the path
		TCHAR filename[MAX_PATH + 1];
		_splitpath_s( // Find the filename part of the path
			processPath, // Input
			NULL, 0, // Don't care about the drive letter
			NULL, 0, // Don't care about the directory
			filename, MAX_PATH, // Grab the filename
			NULL, 0 // Extension is always .exe
		);
		if (_tcscmp(filename, raid_exe) != 0)
		{
			return true;
		}
		
		DisableThreadLibraryCalls(hInst);
		char bufd[200];
		GetSystemDirectory(bufd, 200);
		strcat_s(bufd, "\\WSOCK32.dll");

		HMODULE hL = LoadLibrary(bufd);
		if (!hL)
			return false;

		State.reset(new DllState());
		State->hLThis = hInst;
		State->hL = hL;

		// Load the addresses for all the functions
#define REGISTER(name) farproc.o##name = GetProcAddress(hL, #name);
		ALLFUNC(REGISTER);
#undef REGISTER

		// Make sure the other DLL (IPHLPAPI.dll) doesn't exist
		if (Util::GetFileType("IPHLPAPI.dll") == Util::FileType_File)
		{
			MessageBoxA(NULL,
			            "You have both SuperBLT DLLs installed - IPHLPAPI.dll and \nWSOCK32.dll. "
			            "Please delete one (preferrably, delete IPHLPAPI.dll). Currently, the WSOCK32.dll loader will\n"
			            "be deactivated until IPHLPAPI.dll is removed.",
			            "Both SuperBLT DLLs installed!", MB_OK);
			return 1;
		}

		// load DieselLuaDebugger even before us, if installed. but only if their own loader isnt installed
		if (std::filesystem::exists("DieselLuaDebugger.dll") && !std::filesystem::exists("VERSION.dll"))
			hLDebugger = LoadLibrary("DieselLuaDebugger.dll");

		InitiateStates();
	}
	if (reason == DLL_PROCESS_DETACH)
	{
		State.reset();

		if (hLDebugger != 0)
			FreeLibrary(hLDebugger);
	}

	return 1;
}

extern "C"
{
	FARPROC PA = 0;

	int jumpToPA();

#define DEF_STUB(name)          \
	void _WSOCK_EXPORT_##name() \
	{                           \
		PA = farproc.o##name;   \
		jumpToPA();             \
	};

	ALLFUNC(DEF_STUB)
#undef DEF_STUB
}

#endif