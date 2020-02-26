#include "user.h"

HANDLE* HeapHandle; 
ULONG64 opt_A;

PVOID SendAction;

class Spec {
	// 3 = Int?
	// 6 = char*?
public:
	ULONG64 Type, Value;
};

INT64 ToDoList(PVOID obj, INT64 len, Spec* arr, INT64 flags) {
	local Send = decltype(&ToDoList)(SendAction);
	if (len == 4 && int(arr[0].Value) == 6) { // Title Interactions
		if (int(arr[2].Value) == 1) { // Left Click: 0, Right Click: 1
			printf("TodoList[%p], right click: %i, 4: %i \n",
				obj, int(arr[1].Value), int(arr[3].Value));
		}
	}; return Send(obj, len, arr, flags);
}

ULONG64 GATHERING[2] = { 0i64, -1 };
ULONG64 ESC_SEQ[2] = { 3i64, 0x1ffffffff };
VOID WINAPI GatherCallback(ULONG64 nil);
INT64 hkSendAction(PVOID obj, __int64 N, ULONG64* arr, __int64 opt) {
	local eval = decltype(&hkSendAction)(SendAction);
	UINT* value = (UINT*)(UINT64(Windows["Gathering"]) + 0x180);
	for (auto it = Windows.begin(); it != Windows.end(); ++it)
		if (it->second == obj) {
			show(SendAction)("ptr: %p, SendAction(%s), N: %i, ", obj, it->first, N);
			for (int i = 0; i < N * 2; i += 2)
				show(SendAction)("(%x, %llx), ", arr[i] & 0xFFFFFFFF,
					arr[i + 1]); show(SendAction)("%x\n", opt);
		}
	// Conditions
	//if (obj == Windows["ToDoList"])
	//	return ToDoList(obj, N, (Spec*) arr, opt);
	if (obj == Windows["Gathering"] && int
		(arr[1]) == 0x81 && !GATHERING[0]) {
		GATHERING[1] = arr[3] & 0xFFFFFFFF;
	} else if (obj == Windows["ContextMenu"]) {
		// Only If Selected
		if (N == 5 //len: 5 
			&& arr[0] == 3i64 && arr[2] == 3i64
			&& arr[3] == opt_A) { // my option selected
			if (GATHERING[1] != -1 && opt_A == 1) {
				if (*value == 0x23340303) {
					GATHERING[0] = 3i64; CreateThread(0, 0,
						(LPTHREAD_START_ROUTINE) GatherCallback,
						NULL, 0, 0); return eval(obj, 1, ESC_SEQ, 1);
				}; GATHERING[0] = NULL;
			}; 
		};
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
		if (N != 8 || GATHERING[1] == -1) {
			goto skip_insert;
		};  printf("SETS_REAL: %p, %i, %p\n", ctx, N, arr);
		printf("Row Length: %llu\n", opt_A = (int) arr[1]);
		if (N - opt_A == 7) { // no bools
			// Init Memory
			auto ptr = (UINT64*) HeapAlloc(*HeapHandle,
				HEAP_ZERO_MEMORY, 16 * (N + 1));
			printf("CASE_26: HEAP(%llx), ptr: %p\n", 
				*HeapHandle, ptr); memcpy(ptr, arr, N * 16);
			// Add Custom Row in 0x26
			ptr[2 * N + 1] = (UINT64) dString;
			if(N == 8 && GATHERING[1] != -1)
				ptr[2 * N + 1] = (UINT64) AutoGather;
			ptr[2 * N] = arr[14]; ptr[1] += 1;
			return eval(ctx, N + 1, ptr);
		} else { // with bools
			auto idx = 2 * (opt_A + 7);
			auto ptr = (UINT64*)HeapAlloc(*HeapHandle,
				HEAP_ZERO_MEMORY, 16 * (N + 2));
			printf("CASE_06: HEAP(%llx), ptr: %p\n", 
				*HeapHandle, ptr); memcpy(ptr, arr, idx * 8);
			// Add Custom Row in 0x6
			ptr[idx + 1] = (UINT64) dString;
			ptr[idx] = arr[14]; ptr[1] += 1;
			memcpy(2 + idx + ptr, arr + idx, 16 * opt_A);
			ptr[2 + idx + 2 * opt_A] = 3i64;
			return eval(ctx, N + 2, ptr);
		}
	}; 
skip_insert:
	return eval(ctx, N, arr);
}

PVOID SpawnWindow; 
ULONG64 CONFIRM_A [2] = { 3i64, 7i64 };
char hkSpawnWindow(PVOID obj, char* Name, UCHAR flag, UINT ex) {
	local spawnW = decltype(&hkSpawnWindow)(SpawnWindow); Windows[Name] = obj;
	show(SpawnWindow)("[%p] %s, flag: %i, ex: %x\n", obj, Name, flag, ex);
	local sendA = decltype(&hkSendAction)(SendAction);
	// Intercept
	if (Windows["ContentsFinderConfirm"] == obj)
		sendA(obj, 1, CONFIRM_A, 1);
	if (Windows["SalvageResult"] == obj)
		sendA(obj, 1, ESC_SEQ, 0);
	return spawnW(obj, Name, flag, ex);
};

PVOID NumToScreen;
void hk_NumToScreen(__int64 ptr, __int64 idx, int val) {
	local eval = decltype(&hk_NumToScreen)(NumToScreen);
	if (val == 47) {
		const int N = 16; PVOID Stack[N];
		RtlCaptureStackBackTrace(0, N, Stack, 0);
		printf("ptr: %p, idx: %i, v: %i \n", ptr, idx, val);
		for (int i = 0; i < N; i++)
			printf(" [+] frame: %p\n", Stack[i]);
	}; eval(ptr, idx, val);
};

// group -> -1: none, 0: gen, 1: action, 2: item
struct HotbarSlot {
	int subtype, resource, group, ID, 
		tracking, can_use, _6, _7;
	int percent, _9, display, quantity, 
		_12, _13, _14, _15;
};

PVOID RSU_1;
INT64 hk_RSU_1(INT64 a1, INT64 a2, INT64 a3, INT64 a4, INT64 a5, UINT out, UINT a7) {
	local eval = decltype(&hk_RSU_1)(RSU_1);
	if (out == 191) {
		printf("ptr: %p, %p, %p, \n [+] %p, %p, %i, %i\n",
			a1, a2, a3, a4, a5, out, a7);
	}; return eval(a1, a2, a3, a4, a5, out, a7);
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
	// New
	RSU_1 = game->GetLocation("48 89 4c 24 08 55 57 41 56 41 57");
	//DetourAttach(&RSU_1, hk_RSU_1);
	//NumToScreen = game->GetLocation("3b 51 08 7d 15 48 8b 41 20 48 63 d2 44 39");
	//DetourAttach(&NumToScreen, hk_NumToScreen);
}