#ifdef BLT_USE_IPHLPAPI

#define WIN32_LEAN_AND_MEAN 1
#include "InitState.h"
#include "util/util.h"
#include <filesystem>
#include <windows.h>

#include <memory>

#pragma pack(1)

#pragma region ALLFUNC(FUNC)

// Have to do some jank because @ symbol.
#define ALLFUNC(FUNC, AT)                              \
	FUNC(AddIPAddress)                                 \
	FUNC(AllocateAndGetInterfaceInfoFromStack)         \
	FUNC(AllocateAndGetIpAddrTableFromStack)           \
	FUNC(CancelIPChangeNotify)                         \
	FUNC(CancelMibChangeNotify2)                       \
	FUNC(CloseCompartment)                             \
	FUNC(CloseGetIPPhysicalInterfaceForDestination)    \
	FUNC(ConvertCompartmentGuidToId)                   \
	FUNC(ConvertCompartmentIdToGuid)                   \
	FUNC(ConvertGuidToStringA)                         \
	FUNC(ConvertGuidToStringW)                         \
	FUNC(ConvertInterfaceAliasToLuid)                  \
	FUNC(ConvertInterfaceGuidToLuid)                   \
	FUNC(ConvertInterfaceIndexToLuid)                  \
	FUNC(ConvertInterfaceLuidToAlias)                  \
	FUNC(ConvertInterfaceLuidToGuid)                   \
	FUNC(ConvertInterfaceLuidToIndex)                  \
	FUNC(ConvertInterfaceLuidToNameA)                  \
	FUNC(ConvertInterfaceLuidToNameW)                  \
	FUNC(ConvertInterfaceNameToLuidA)                  \
	FUNC(ConvertInterfaceNameToLuidW)                  \
	FUNC(ConvertInterfacePhysicalAddressToLuid)        \
	FUNC(ConvertIpv4MaskToLength)                      \
	FUNC(ConvertLengthToIpv4Mask)                      \
	FUNC(ConvertRemoteInterfaceAliasToLuid)            \
	FUNC(ConvertRemoteInterfaceGuidToLuid)             \
	FUNC(ConvertRemoteInterfaceIndexToLuid)            \
	FUNC(ConvertRemoteInterfaceLuidToAlias)            \
	FUNC(ConvertRemoteInterfaceLuidToGuid)             \
	FUNC(ConvertRemoteInterfaceLuidToIndex)            \
	FUNC(ConvertStringToGuidA)                         \
	FUNC(ConvertStringToGuidW)                         \
	FUNC(ConvertStringToInterfacePhysicalAddress)      \
	FUNC(CreateAnycastIpAddressEntry)                  \
	FUNC(CreateIpForwardEntry)                         \
	FUNC(CreateIpForwardEntry2)                        \
	FUNC(CreateIpNetEntry)                             \
	FUNC(CreateIpNetEntry2)                            \
	FUNC(CreatePersistentTcpPortReservation)           \
	FUNC(CreatePersistentUdpPortReservation)           \
	FUNC(CreateProxyArpEntry)                          \
	FUNC(CreateSortedAddressPairs)                     \
	FUNC(CreateUnicastIpAddressEntry)                  \
	FUNC(DeleteAnycastIpAddressEntry)                  \
	FUNC(DeleteIPAddress)                              \
	FUNC(DeleteIpForwardEntry)                         \
	FUNC(DeleteIpForwardEntry2)                        \
	FUNC(DeleteIpNetEntry)                             \
	FUNC(DeleteIpNetEntry2)                            \
	FUNC(DeletePersistentTcpPortReservation)           \
	FUNC(DeletePersistentUdpPortReservation)           \
	FUNC(DeleteProxyArpEntry)                          \
	FUNC(DeleteUnicastIpAddressEntry)                  \
	FUNC(DisableMediaSense)                            \
	FUNC(EnableRouter)                                 \
	FUNC(FlushIpNetTable)                              \
	FUNC(FlushIpNetTable2)                             \
	FUNC(FlushIpPathTable)                             \
	FUNC(FreeMibTable)                                 \
	FUNC(GetAdapterIndex)                              \
	FUNC(GetAdapterOrderMap)                           \
	FUNC(GetAdaptersAddresses)                         \
	FUNC(GetAdaptersInfo)                              \
	FUNC(GetAnycastIpAddressEntry)                     \
	FUNC(GetAnycastIpAddressTable)                     \
	FUNC(GetBestInterface)                             \
	FUNC(GetBestInterfaceEx)                           \
	FUNC(GetBestRoute)                                 \
	FUNC(GetBestRoute2)                                \
	FUNC(GetCurrentThreadCompartmentId)                \
	FUNC(GetExtendedTcpTable)                          \
	FUNC(GetExtendedUdpTable)                          \
	FUNC(GetFriendlyIfIndex)                           \
	FUNC(GetIcmpStatistics)                            \
	FUNC(GetIcmpStatisticsEx)                          \
	FUNC(GetIfEntry)                                   \
	FUNC(GetIfEntry2)                                  \
	FUNC(GetIfStackTable)                              \
	FUNC(GetIfTable)                                   \
	FUNC(GetIfTable2)                                  \
	FUNC(GetIfTable2Ex)                                \
	FUNC(GetInterfaceInfo)                             \
	FUNC(GetInvertedIfStackTable)                      \
	FUNC(GetIpAddrTable)                               \
	FUNC(GetIpErrorString)                             \
	FUNC(GetIpForwardEntry2)                           \
	FUNC(GetIpForwardTable)                            \
	FUNC(GetIpForwardTable2)                           \
	FUNC(GetIpInterfaceEntry)                          \
	FUNC(GetIpInterfaceTable)                          \
	FUNC(GetIpNetEntry2)                               \
	FUNC(GetIpNetTable)                                \
	FUNC(GetIpNetTable2)                               \
	FUNC(GetIpNetworkConnectionBandwidthEstimates)     \
	FUNC(GetIpPathEntry)                               \
	FUNC(GetIpPathTable)                               \
	FUNC(GetIpStatistics)                              \
	FUNC(GetIpStatisticsEx)                            \
	FUNC(GetMulticastIpAddressEntry)                   \
	FUNC(GetMulticastIpAddressTable)                   \
	FUNC(GetNetworkInformation)                        \
	FUNC(GetNetworkParams)                             \
	FUNC(GetNumberOfInterfaces)                        \
	FUNC(GetOwnerModuleFromPidAndInfo)                 \
	FUNC(GetOwnerModuleFromTcp6Entry)                  \
	FUNC(GetOwnerModuleFromTcpEntry)                   \
	FUNC(GetOwnerModuleFromUdp6Entry)                  \
	FUNC(GetOwnerModuleFromUdpEntry)                   \
	FUNC(GetPerAdapterInfo)                            \
	FUNC(GetPerTcp6ConnectionEStats)                   \
	FUNC(GetPerTcp6ConnectionStats)                    \
	FUNC(GetPerTcpConnectionEStats)                    \
	FUNC(GetPerTcpConnectionStats)                     \
	FUNC(GetRTTAndHopCount)                            \
	FUNC(GetSessionCompartmentId)                      \
	FUNC(GetTcp6Table)                                 \
	FUNC(GetTcp6Table2)                                \
	FUNC(GetTcpStatistics)                             \
	FUNC(GetTcpStatisticsEx)                           \
	FUNC(GetTcpTable)                                  \
	FUNC(GetTcpTable2)                                 \
	FUNC(GetTeredoPort)                                \
	FUNC(GetUdp6Table)                                 \
	FUNC(GetUdpStatistics)                             \
	FUNC(GetUdpStatisticsEx)                           \
	FUNC(GetUdpTable)                                  \
	FUNC(GetUniDirectionalAdapterInfo)                 \
	FUNC(GetUnicastIpAddressEntry)                     \
	FUNC(GetUnicastIpAddressTable)                     \
	FUNC(Icmp6CreateFile)                              \
	FUNC(Icmp6ParseReplies)                            \
	FUNC(Icmp6SendEcho2)                               \
	FUNC(IcmpCloseHandle)                              \
	FUNC(IcmpCreateFile)                               \
	FUNC(IcmpParseReplies)                             \
	FUNC(IcmpSendEcho)                                 \
	FUNC(IcmpSendEcho2)                                \
	FUNC(IcmpSendEcho2Ex)                              \
	FUNC(InitializeIpForwardEntry)                     \
	FUNC(InitializeIpInterfaceEntry)                   \
	FUNC(InitializeUnicastIpAddressEntry)              \
	FUNC(InternalCleanupPersistentStore)               \
	FUNC(InternalCreateAnycastIpAddressEntry)          \
	FUNC(InternalCreateIpForwardEntry)                 \
	FUNC(InternalCreateIpForwardEntry2)                \
	FUNC(InternalCreateIpNetEntry)                     \
	FUNC(InternalCreateIpNetEntry2)                    \
	FUNC(InternalCreateUnicastIpAddressEntry)          \
	FUNC(InternalDeleteAnycastIpAddressEntry)          \
	FUNC(InternalDeleteIpForwardEntry)                 \
	FUNC(InternalDeleteIpForwardEntry2)                \
	FUNC(InternalDeleteIpNetEntry)                     \
	FUNC(InternalDeleteIpNetEntry2)                    \
	FUNC(InternalDeleteUnicastIpAddressEntry)          \
	FUNC(InternalFindInterfaceByAddress)               \
	FUNC(InternalGetAnycastIpAddressEntry)             \
	FUNC(InternalGetAnycastIpAddressTable)             \
	FUNC(InternalGetForwardIpTable2)                   \
	FUNC(InternalGetIPPhysicalInterfaceForDestination) \
	FUNC(InternalGetIfEntry2)                          \
	FUNC(InternalGetIfTable)                           \
	FUNC(InternalGetIfTable2)                          \
	FUNC(InternalGetIpAddrTable)                       \
	FUNC(InternalGetIpForwardEntry2)                   \
	FUNC(InternalGetIpForwardTable)                    \
	FUNC(InternalGetIpInterfaceEntry)                  \
	FUNC(InternalGetIpInterfaceTable)                  \
	FUNC(InternalGetIpNetEntry2)                       \
	FUNC(InternalGetIpNetTable)                        \
	FUNC(InternalGetIpNetTable2)                       \
	FUNC(InternalGetMulticastIpAddressEntry)           \
	FUNC(InternalGetMulticastIpAddressTable)           \
	FUNC(InternalGetRtcSlotInformation)                \
	FUNC(InternalGetTcp6Table2)                        \
	FUNC(InternalGetTcp6TableWithOwnerModule)          \
	FUNC(InternalGetTcp6TableWithOwnerPid)             \
	FUNC(InternalGetTcpTable)                          \
	FUNC(InternalGetTcpTable2)                         \
	FUNC(InternalGetTcpTableEx)                        \
	FUNC(InternalGetTcpTableWithOwnerModule)           \
	FUNC(InternalGetTcpTableWithOwnerPid)              \
	FUNC(InternalGetTunnelPhysicalAdapter)             \
	FUNC(InternalGetUdp6TableWithOwnerModule)          \
	FUNC(InternalGetUdp6TableWithOwnerPid)             \
	FUNC(InternalGetUdpTable)                          \
	FUNC(InternalGetUdpTableEx)                        \
	FUNC(InternalGetUdpTableWithOwnerModule)           \
	FUNC(InternalGetUdpTableWithOwnerPid)              \
	FUNC(InternalGetUnicastIpAddressEntry)             \
	FUNC(InternalGetUnicastIpAddressTable)             \
	FUNC(InternalIcmpCreateFileEx)                     \
	FUNC(InternalSetIfEntry)                           \
	FUNC(InternalSetIpForwardEntry)                    \
	FUNC(InternalSetIpForwardEntry2)                   \
	FUNC(InternalSetIpInterfaceEntry)                  \
	FUNC(InternalSetIpNetEntry)                        \
	FUNC(InternalSetIpNetEntry2)                       \
	FUNC(InternalSetIpStats)                           \
	FUNC(InternalSetTcpEntry)                          \
	FUNC(InternalSetTeredoPort)                        \
	FUNC(InternalSetUnicastIpAddressEntry)             \
	FUNC(IpReleaseAddress)                             \
	FUNC(IpRenewAddress)                               \
	FUNC(LookupPersistentTcpPortReservation)           \
	FUNC(LookupPersistentUdpPortReservation)           \
	FUNC(NTPTimeToNTFileTime)                          \
	FUNC(NTTimeToNTPTime)                              \
	FUNC(NhGetGuidFromInterfaceName)                   \
	FUNC(NhGetInterfaceDescriptionFromGuid)            \
	FUNC(NhGetInterfaceNameFromDeviceGuid)             \
	FUNC(NhGetInterfaceNameFromGuid)                   \
	FUNC(NhpAllocateAndGetInterfaceInfoFromStack)      \
	FUNC(NotifyAddrChange)                             \
	FUNC(NotifyCompartmentChange)                      \
	FUNC(NotifyIpInterfaceChange)                      \
	FUNC(NotifyRouteChange)                            \
	FUNC(NotifyRouteChange2)                           \
	FUNC(NotifyStableUnicastIpAddressTable)            \
	FUNC(NotifyTeredoPortChange)                       \
	FUNC(NotifyUnicastIpAddressChange)                 \
	FUNC(OpenCompartment)                              \
	FUNC(ParseNetworkString)                           \
	FUNC(ResolveIpNetEntry2)                           \
	FUNC(ResolveNeighbor)                              \
	FUNC(RestoreMediaSense)                            \
	FUNC(SendARP)                                      \
	FUNC(SetAdapterIpAddress)                          \
	FUNC(SetCurrentThreadCompartmentId)                \
	FUNC(SetIfEntry)                                   \
	FUNC(SetIpForwardEntry)                            \
	FUNC(SetIpForwardEntry2)                           \
	FUNC(SetIpInterfaceEntry)                          \
	FUNC(SetIpNetEntry)                                \
	FUNC(SetIpNetEntry2)                               \
	FUNC(SetIpStatistics)                              \
	FUNC(SetIpStatisticsEx)                            \
	FUNC(SetIpTTL)                                     \
	FUNC(SetNetworkInformation)                        \
	FUNC(SetPerTcp6ConnectionEStats)                   \
	FUNC(SetPerTcp6ConnectionStats)                    \
	FUNC(SetPerTcpConnectionEStats)                    \
	FUNC(SetPerTcpConnectionStats)                     \
	FUNC(SetSessionCompartmentId)                      \
	FUNC(SetTcpEntry)                                  \
	FUNC(SetUnicastIpAddressEntry)                     \
	FUNC(UnenableRouter)                               \
	FUNC(_PfAddFiltersToInterface, ##AT, 24)           \
	FUNC(_PfAddGlobalFilterToInterface, ##AT, 8)       \
	FUNC(_PfBindInterfaceToIPAddress, ##AT, 12)        \
	FUNC(_PfBindInterfaceToIndex, ##AT, 16)            \
	FUNC(_PfCreateInterface, ##AT, 24)                 \
	FUNC(_PfDeleteInterface, ##AT, 4)                  \
	FUNC(_PfDeleteLog, ##AT, 0)                        \
	FUNC(_PfGetInterfaceStatistics, ##AT, 16)          \
	FUNC(_PfMakeLog, ##AT, 4)                          \
	FUNC(_PfRebindFilters, ##AT, 8)                    \
	FUNC(_PfRemoveFilterHandles, ##AT, 12)             \
	FUNC(_PfRemoveFiltersFromInterface, ##AT, 20)      \
	FUNC(_PfRemoveGlobalFilterFromInterface, ##AT, 8)  \
	FUNC(_PfSetLogBuffer, ##AT, 28)                    \
	FUNC(_PfTestPacket, ##AT, 20)                      \
	FUNC(_PfUnBindInterface, ##AT, 4)                  \
	FUNC(do_echo_rep)                                  \
	FUNC(do_echo_req)                                  \
	FUNC(if_indextoname)                               \
	FUNC(if_nametoindex)                               \
	FUNC(register_icmp)

#pragma endregion

struct farproc_dll
{
#define STRUCT(name, symbol, symbol_num) FARPROC o##name;
	ALLFUNC(STRUCT, @)
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
		char bufd[200];
		GetSystemDirectory(bufd, 200);
		strcat_s(bufd, "\\IPHLPAPI.dll");

		HMODULE hL = LoadLibrary(bufd);
		if (!hL)
			return false;

		State.reset(new DllState());
		State->hLThis = hInst;
		State->hL = hL;

		// Load the addresses for all the functions
#define REGISTER(name, symbol, symbol_num) farproc.o##name = GetProcAddress(hL, #name #symbol #symbol_num);
		ALLFUNC(REGISTER, @);
#undef REGISTER

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

#define DEF_STUB(name, symbol, symbol_num) \
	void _IPHP_EXPORT_##name()             \
	{                                      \
		PA = farproc.o##name;              \
		jumpToPA();                        \
	};

	ALLFUNC(DEF_STUB, @)
#undef DEF_STUB
}

#endif