#include "user.h"

HANDLE* HeapHandle; 
ULONG64 repl_A;

PVOID SendAction;
ULONG64 GATHERING[2]{ 0i64, -1 };
ULONG64 ESC_SEQ[2] = { 3i64, 0x1ffffffff };
VOID WINAPI GatherCallback(ULONG64 nil);
INT64 hkSendAction(PVOID obj, __int64 N, ULONG64* arr, __int64 opt) {
	local eval = decltype(&hkSendAction)(SendAction);
	for (auto it = Windows.begin(); it != Windows.end(); ++it)
		if (it->second == obj) {
			show(SendAction)("SendAction(%s), N: %i, ", it->first, N);
			for (int i = 0; i < N * 2; i += 2)
				show(SendAction)("(%x, %llx), ", arr[i] & 0xFFFFFFFF,
					arr[i + 1]); show(SendAction)("%x\n", opt);
		}
	// Conditions
	if (obj == Windows["Gathering"] && int
		(arr[1]) == 0x81 && !GATHERING[0]) {
		GATHERING[1] = arr[3] & 0xFFFFFFFF;
	} else if (obj == Windows["ContextMenu"]) {
		// Only If Selected
		if (N == 5 && arr[0] == 3i64 && arr[2] == 3i64 && arr[3] == repl_A) {
			if (GATHERING[1] != -1 && (GATHERING[0] = 3i64)) {
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)
					GatherCallback, NULL, 0, 0);
			}; return eval(obj, 1, ESC_SEQ, 1);
		}
	};  return eval(obj, N, arr, opt);
}

VOID WINAPI GatherCallback(ULONG64 nil) {
	local eval = decltype(&hkSendAction)(SendAction);
	UINT* value = (UINT*)(UINT64(Windows["Gathering"]) + 0x180);
	printf("Gathering Location: %p\n", Windows["Gathering"]);
	while(*value == 0x23340303) {
		printf("\t Callback!! %x\n", value);
		eval(Windows["Gathering"], 1, GATHERING, 1); 
		Sleep(1250);
	}; GATHERING[1] = -1; GATHERING[0] = 0;
}

const char* AutoGather = "Action: Gather All";
PVOID SetsCtxReal; const char* dString = "-- Debug: Action";
__int64 hkSetsCtxReal(PVOID ctx, int N, UINT64* arr) {
	local eval = decltype(&hkSetsCtxReal)(SetsCtxReal);
	if (ctx == Windows["ContextMenu"] && N && arr) {
		printf("SETS_REAL: %p, %i, %p\n", ctx, N, arr);
		printf("Row Length: %llu\n", repl_A = (int) arr[1]);
		if (N != 9 || GATHERING[1] == -1)
			goto skip_insert;
		if (N - repl_A == 7) { // no bools
			// Init Memory
			auto ptr = (UINT64*) HeapAlloc(*HeapHandle,
				HEAP_ZERO_MEMORY, 16 * (N + 1));
			printf("CASE_26: HEAP(%llx), ptr: %p\n", 
				*HeapHandle, ptr); memcpy(ptr, arr, N * 16);
			// Add Custom Row in 0x26
			ptr[2 * N + 1] = (UINT64) dString;
			if(N == 9 && GATHERING[1] != -1)
				ptr[2 * N + 1] = (UINT64) AutoGather;
			ptr[2 * N] = arr[14]; ptr[1] += 1;
			return eval(ctx, N + 1, ptr);
		} else { // with bools
			auto idx = 2 * (repl_A + 7);
			auto ptr = (UINT64*)HeapAlloc(*HeapHandle,
				HEAP_ZERO_MEMORY, 16 * (N + 2));
			printf("CASE_06: HEAP(%llx), ptr: %p\n", 
				*HeapHandle, ptr); memcpy(ptr, arr, idx * 8);
			// Add Custom Row in 0x6
			ptr[idx + 1] = (UINT64) dString;
			ptr[idx] = arr[14]; ptr[1] += 1;
			memcpy(2 + idx + ptr, arr + idx, 16 * repl_A);
			ptr[2 + idx + 2 * repl_A] = 3i64;
			return eval(ctx, N + 2, ptr);
		}
	}; 
skip_insert:
	return eval(ctx, N, arr);
}

PVOID SpawnWindow;
char hkSpawnWindow(PVOID obj, char* Name, UCHAR flag, UINT ex) {
	local spawnW = decltype(&hkSpawnWindow)(SpawnWindow); Windows[Name] = obj;
	show(SpawnWindow)("at: %p, %s, Flag: %i, Ex: %x\n", obj, Name, flag, ex);
	local sendA = decltype(&hkSendAction)(SendAction);
	if (Windows["SalvageResult"] == obj)
		sendA(obj, 1, ESC_SEQ, 1);
	return spawnW(obj, Name, flag, ex);
};

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