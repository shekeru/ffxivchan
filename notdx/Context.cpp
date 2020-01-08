#include "user.h"

PVOID CTX_0_fn;
__int64 __fastcall CTX_0(__int64 a1, char a2) {
	static auto eval = (decltype(&CTX_0))CTX_0_fn;
	printf("\t::CTX_0::\n");
	return eval(a1, a2);
}

PVOID CTX_5_fn;
__int64 __fastcall CTX_5(__int64 a1, unsigned __int8 a2, unsigned int a3) {
	static auto eval = (decltype(&CTX_5))CTX_5_fn;
	//auto var = 1[*(UINT64**)(a1 + 0x160)];
	printf("CTX_5: %p, %i, %i\n", a1, a2, a3);
	return eval(a1, a2, a3);
}

PVOID CTX_17_fn;
__int64 __fastcall CTX_17(__int64 a1) {
	static auto eval = (decltype(&CTX_17))CTX_17_fn;
	printf("\t::CTX_17::\n");
	return eval(a1);
}

void Hooks::ContextAttach() {
	auto Context = game->ScanPattern(Offsets::CONTEXT, 3);
	CTX_0_fn = Context[0x8 * 0].Cast<PVOID>();
	CTX_17_fn = Context[0x8 * 17].Cast<PVOID>();
	CTX_5_fn = Context[0x8 * 5].Cast<PVOID>();
	DetourAttach(&CTX_5_fn, CTX_5);
	DetourAttach(&CTX_17_fn, CTX_17);
	DetourAttach(&CTX_0_fn, CTX_0);
}