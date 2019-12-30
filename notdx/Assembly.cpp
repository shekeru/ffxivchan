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
void __fastcall N_47(__int64 mbw, __int64 ntp, __int64 idk) {
	static auto eval = (decltype(&N_47))N_47_fn;
	auto n2 = IntPtr(ntp)[272].Cast<__int64>();
	if (n2) {
		auto n3 = IntPtr(n2)[32].Cast<__int64>();
		auto n_h = IntPtr(n3)[2004].Cast<int>();
		printf("call (N-47), ntp-2/3: %p/%p, n-h: %i\n", n2, n3, n_h);

	}; return eval(mbw, ntp, idk);
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