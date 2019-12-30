#include "vmt.h"
#include <detours.h>
#include "Market.h"
#include "Network.h"

uintptr_t N_41_fn;
void __fastcall N_41(__int64 a1) {
	static auto eval = (decltype(&N_41)) N_41_fn;
	printf("call (N-41): %p\n", a1);
	return eval(a1);
}

uintptr_t N_47_fn;
void __fastcall N_47(MarketWindow* mbw, __int64 ntp, __int64 stp) {
	static auto eval = (decltype(&N_47))N_47_fn; eval(mbw, ntp, stp);
	auto iTables = IntPtr(ntp)[272][32].Cast<__int64>();
	auto iData = (IntResults*)(iTables + 2004);
	auto aTables = IntPtr(stp)[248][32].Cast<__int64>();
	auto aData = (StrResults*)(aTables + 1608);
	if (!iData->Count) 
		return;
	printf("call (N-47): %p %p %p\n", mbw, ntp, stp);
	printf("Found %i Listings, 0x160_mby: %p...\n", 
		iData->Count, mbw->Properties);
	//for (int j = 0; j < iData->Count; j++)
	//	printf(" [+] Price: %i, Retainer: %s\n",
	//		iData->Items[j].Price, aData->Items[j].Retainer);
}

PVOID ProcessZonePacket;
UINT64 __fastcall hkPzPacket(INT64 a1, UINT a2, INT64 a3) {
	static auto eval = (decltype(&hkPzPacket))ProcessZonePacket;
	MarketOfferings* lists = (MarketOfferings*)(a3 + 16);
	MarketHistory* hist = (MarketHistory*)(a3 + 16);
	TaxRates* tax = (TaxRates*)(a3 + 16);
	switch (*(UINT16*)(a3 + 2)) {
		//case 0x25E:
		//	printf("MarketTaxRates: %i\n", a2);
		//	break;
		//case 0x328:
		//	printf("MarketBoardItemRequestStart: %i\n", lists->RequestId);
		//	break;
		//case 0x15F:
		//	printf("MarketBoardOfferings: %i\n", lists->RequestId);
		//  printf("FUCK: %i, %i\n", lists->IndexEnd, lists->IndexStart);
		//	break;
		case 0x113:
			printf("MarketBoardHistory, ItemId: %i\n", 
				hist->catalogId); break;
	}; return eval(a1, a2, a3);
}

PVOID SpawnWindow; std::map<std::string, PVOID> Windows;
char __fastcall hkSpawnWindow(void* obj, char* Name, UCHAR flag, UINT ex) {
	static auto eval = (decltype(&hkSpawnWindow))SpawnWindow;
	printf("Spawning (%s) at %p, Flag: %i, Ex: %x\n", Name, obj, flag, ex);
	Windows[Name] = obj; return eval(obj, Name, flag, ex);
};

void MemorySystem::DetourAll() {
	SpawnWindow = game->ScanPattern(Offsets::SPAWNUI, 4).Cast<PVOID>();
	ProcessZonePacket = (PVOID)game->GetLocation(Offsets::NETWORK);
	auto Market = game->ScanPattern(Offsets::MARKET, 3);
	N_47_fn = Market[0x8 * 47].Cast<uintptr_t>();
	N_41_fn = Market[0x8 * 41].Cast<uintptr_t>();
	// Detours Init
	DetourRestoreAfterWith(); DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	// Attach Detours
	DetourAttach(&SpawnWindow, hkSpawnWindow);
	DetourAttach(&ProcessZonePacket, hkPzPacket);
	//DetourAttach(&(PVOID&)N_41_fn, N_41);
	DetourAttach(&(PVOID&)N_47_fn, N_47);
	// Detours Post
	DetourTransactionCommit();
}