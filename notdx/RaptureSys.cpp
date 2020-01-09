#include "user.h"
#include <detours.h>

HANDLE* HeapHandle; 
ULONG64 repl_A;

PVOID SpawnWindow;
char __fastcall hkSpawnWindow(void* obj, char* Name, UCHAR flag, UINT ex) {
	static auto eval = (decltype(&hkSpawnWindow))SpawnWindow;
	printf("[Spawn] %p (%s), Flag: %i, Ex: %x\n", obj, Name, flag, ex);
	Windows[Name] = obj; return eval(obj, Name, flag, ex);
};

PVOID SendAction;
static ULONG64 ESC_SEQ[2] = { 3i64, 0x1ffffffff };
__int64 __fastcall hkSendAction(INT64 obj, __int64 N, ULONG64* arr, __int64 opt) {
	static auto eval = (decltype(&hkSendAction))SendAction; string window = "???";
	//if ((PVOID)obj == Windows["ContextMenu"]) {
	//	auto cock = IntPtr(obj)[0x160].Cast<ULONG64*>();
	//	printf("  %p::IsContextMenu(%llu) \n", obj, cock[1]);
	//	auto after = cock + 14;
	//	for (int i = 0; i < cock[1]; i++) {
	//		printf("%s\n", after[1 + i * 2]);
	//	};
	//}
	//for (auto it = Windows.begin(); it != Windows.end(); ++it)
	//	if (it->second == (PVOID)obj)
	//		window = std::string(it->first);
	//printf("SendAction(%s), N: %i, ", window, N);
	if ((PVOID)obj == Windows["ContextMenu"]) {
		printf("CtxAction, N: %i, ", N);
		for (int i = 0; i < N * 2; i += 2)
			printf("(%llx, %llx), ", arr[i],
				arr[i + 1]); printf("%x\n", opt);
		// Only If Selected
		if (N == 5 && arr[0] == 3i64 && arr[2] == 3i64 && arr[3] == repl_A) {
			printf("SELECTED!!\n"); return eval(obj, 1, ESC_SEQ, 1);
		}
	};  return eval(obj, N, arr, opt);
}

PVOID SetsCtxReal; const char* cunts = "[[REPL: Action]]";
__int64 __fastcall hkSetsCtxReal(PVOID a1, int a2, UINT64* a3) {
	static auto eval = (decltype(&hkSetsCtxReal))SetsCtxReal;
	if ((PVOID) a1 == Windows["ContextMenu"]) {
		printf("SETS_REAL: %p, %i, %p\n", a1, a2, a3);
		printf("Row Length: %llu\n", repl_A = a3[1]);
		if (0x26 == a3[14]) {
			// Init Memory
			auto ptr = (UINT64*) HeapAlloc(*HeapHandle,
				HEAP_ZERO_MEMORY, 16 * (a2 + 1));
			printf("CASE_26: HEAP(%llx), ptr: %p\n", 
				*HeapHandle, ptr); memcpy(ptr, a3, a2 * 16);
			// Add Custom Row in 0x26
			ptr[2 * a2 + 1] = (UINT64) cunts; 
			ptr[2 * a2] = 38i64; ptr[1] += 1; 
			return eval(a1, a2 + 1, ptr);
		} else if(0x6 == a3[14]) {
			auto last = a3[1] + 7;
			auto ptr = (UINT64*)HeapAlloc(*HeapHandle,
				HEAP_ZERO_MEMORY, 16 * (a2 + 2));
			printf("CASE_06: HEAP(%llx), ptr: %p\n", 
				*HeapHandle, ptr); memcpy(ptr, a3, last * 16);
			// Add Custom Row in 0x6
			ptr[2 * last + 1] = (UINT64)cunts;
			ptr[2 * last] = 6i64; ptr[1] += 1;
			memcpy(2 * (last + 1) + ptr, a3 + last, a3[1]);
			return eval(a1, a2 + 1, ptr);
		}
	}; return eval(a1, a2, a3);
}

void Hooks::RaptureAttach() {
	HeapHandle = game->ScanPattern(Offsets::HEAP_HANDLE, 3).Cast<HANDLE*>();
	SetsCtxReal = game->ScanPattern(Offsets::SETS_REAL, 1).Cast<PVOID>();
	SendAction = game->ScanPattern(Offsets::SENDACTION, 1).Cast<PVOID>();
	SpawnWindow = game->ScanPattern(Offsets::SPAWNUI, 4).Cast<PVOID>();
	// Attachments
	DetourAttach(&SetsCtxReal, hkSetsCtxReal);
	DetourAttach(&SpawnWindow, hkSpawnWindow);
	DetourAttach(&SendAction, hkSendAction);
}