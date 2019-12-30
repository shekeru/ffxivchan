#include "vmt.h"
#include <detours.h>
#include "Market.h"

uintptr_t N_41_fn;
void __fastcall N_41(__int64 a1) {
	static auto eval = (decltype(&N_41)) N_41_fn;
	printf("call (N-41): %p\n", a1);
	return eval(a1);
}

uintptr_t N_47_fn;
void __fastcall N_47(__int64 mbw, __int64 ntp, __int64 stp) {
	static auto eval = (decltype(&N_47))N_47_fn; eval(mbw, ntp, stp);
	auto iTables = IntPtr(ntp)[272][32].Cast<__int64>();
	auto iData = (IntResults*)(iTables + 2004);
	auto aTables = IntPtr(stp)[248][32].Cast<__int64>();
	auto aData = (StrResults*)(aTables + 1608);
	if (!iData->Count) 
		return;
	printf("call (N-47): %p %p %p\n", mbw, ntp, stp);
	printf("Found %i Listings, jStruct_0: %p...\n", iData->Count, 
		IntPtr(mbw)[640].Cast<void*>());
	for (int j = 0; j < iData->Count; j++)
		printf(" [+] Price: %i, Retainer: %s\n",
			iData->Items[j].Price, aData->Items[j].Retainer);
}

void MemorySystem::DetourAll() {
	DetourRestoreAfterWith(); DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	auto Market = game->ScanPattern(Offsets::MARKET, 3);
	N_47_fn = Market[0x8 * 47].Cast<uintptr_t>();
	N_41_fn = Market[0x8 * 41].Cast<uintptr_t>();
	DetourAttach(&(PVOID&)N_47_fn, N_47);
	//DetourAttach(&(PVOID&)N_41_fn, N_41);
	DetourTransactionCommit();
}