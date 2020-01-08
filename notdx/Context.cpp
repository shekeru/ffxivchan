#include "user.h"

PVOID CTX_0_fn;
__int64 __fastcall CTX_0(__int64 a1, char a2) {
	static auto eval = (decltype(&CTX_0))CTX_0_fn;
	printf("\t::CTX_0::\n");
	return eval(a1, a2);
}

PVOID CTX_5_fn; const char* fuck = "Fuck";
__int64 __fastcall CTX_5(__int64 a1, unsigned __int8 a2, unsigned int a3) {
	static auto eval = (decltype(&CTX_5))CTX_5_fn;
	auto value = eval(a1, a2, a3);;
	if ((PVOID)a1 == Windows["ContextMenu"]) {
		auto cock = IntPtr(a1)[0x160].Cast<ULONG64*>();
			printf("  %p::IsContextMenu(%llu) \n", a1, cock[1]);
			auto after = cock + 14;
			for (int i = 0; i < cock[1] - 1; i++) {
				printf("%llu: %s\n", 
					after[i * 2], after[1 + i * 2]);
			}; int i = cock[1]; after[i * 2] = 38L;
			after[1 + i * 2] = (ULONG64)fuck; cock[1]++;
		printf("CTX_5: %p, %i, %i\n", a1, a2, a3);
	}; return value;
}

PVOID CTX_17_fn;
__int64 __fastcall CTX_17(__int64 a1) {
	static auto eval = (decltype(&CTX_17))CTX_17_fn;
	printf("\t::CTX_17::\n");
	return eval(a1);
}

PVOID CTX_1_fn;
__int64 __fastcall CTX_1(__int64 a1, unsigned __int16 a2, unsigned int a3, __int64 a4, unsigned int *a5) {
	static auto eval = (decltype(&CTX_1))CTX_1_fn;
	if ((PVOID) a1 == Windows["ContextMenu"])
		printf("CTX_1: %i, %i, %i, %p\n", a2, a3, a4, a5);
	return eval(a1, a2, a3, a4, a5);
}

void Hooks::ContextAttach() {
	auto Context = game->ScanPattern(Offsets::CONTEXT, 3);
	CTX_0_fn = Context[0x8 * 0].Cast<PVOID>();
	CTX_17_fn = Context[0x8 * 17].Cast<PVOID>();
	CTX_5_fn = Context[0x8 * 5].Cast<PVOID>();
	CTX_1_fn = Context[0x8 * 1].Cast<PVOID>();
	//DetourAttach(&CTX_1_fn, CTX_1);
	DetourAttach(&CTX_5_fn, CTX_5);
	DetourAttach(&CTX_17_fn, CTX_17);
	DetourAttach(&CTX_0_fn, CTX_0);
}