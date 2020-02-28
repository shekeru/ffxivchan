#include "user.h"

PVOID SendAction;
HANDLE* HeapHandle; 
ULONG64 opt_A;

typedef PVOID CtxFn; int CtxSize;
PVOID CtxVectorInit; CtxFn* CtxCallback;
CtxFn* hkCtxVectorInit(CtxFn*& ref, INT64 len) {
	ORIGINAL(hkCtxVectorInit, CtxVectorInit); return
		CtxCallback = original(ref, CtxSize = len);
}

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

PVOID CtxAssign;
DWORD* hkCtxAssign
(__int64 a1, __int64 a2, __int64 a3, __int64 a4, __int64 a5)
{
	ORIGINAL(hkCtxAssign, CtxAssign);
	auto v = original(a1, a2, a3, a4, a5);
	printf("%p, %p, %p, %p, %p \n\n", a1, a2, a3, a4, a5);
	return v;
};


PVOID UiCounter; PVOID UiTable[256];
__int64 hkUiCounter(__int64 table, __int64 unk, void* winPtr) {
	ORIGINAL(hkUiCounter, UiCounter);
	auto value = original(table, unk, winPtr);
	if (value > 1) {
		printf("t: %p, %p <%p> -> %x\n", table, unk, winPtr);
		UiTable[IntPtr(table)[0x808].Cast<DWORD>()] = winPtr;
	}; return value;
}

// 0x41 ->  6, NULL -> 38
PVOID CTX_UP_5; // 7: right click
const char* AutoGather = "Action: Gather All";
__int64 hkCTX_UP_5(void* ptr, UINT signal, UINT last, Spec* data,
	void* ctxList, INT64 zone, UINT16 uID, int flag) {
	ORIGINAL(hkCTX_UP_5, CTX_UP_5); if (signal == 7) {
		const char* inject = NULL; auto parent = (char*)UiTable[uID];
		if (parent == Windows["Gathering"]) 
			inject = AutoGather;
		printf("potential window: %p, %s \n", parent, 
			parent ? parent + 8 : 0);
		if (parent && inject) {
			ULONG64& length = data[0].Value; Spec* Body = data + 7;
			if (length < last - 7) {
				memmove(Body + length + 1, Body + length,
					sizeof(Body) * length);
				Body[2 * (length + 1)] = Spec
					{ 3, 1 }; last += 1;
			}; Body[length] = Spec{ Body[0].Type, (ULONG64)
				inject }; length += 1; last += 1;
		}
	}; return original(ptr, signal, last, data, ctxList, zone, uID, flag);
};

void Hooks::RaptureAttach() {
	HeapHandle = game->ScanPattern(Offsets::HEAP_HANDLE, 3).Cast<HANDLE*>();
	SendAction = game->ScanPattern(Offsets::SENDACTION, 1).Cast<PVOID>();
	SpawnWindow = game->ScanPattern(Offsets::SPAWNUI, 4).Cast<PVOID>();
	// Attachments
	DetourAttach(&SpawnWindow, hkSpawnWindow);
	DetourAttach(&SendAction, hkSendAction);
	// New
	//RSU_1 = game->GetLocation("48 89 4c 24 08 55 57 41 56 41 57");
	CtxVectorInit = game->ScanPattern
		("e8 ? ? ? ? 48 8b 43 08 48 2b 03 48 c1 f8 03", 1)
	.Cast<PVOID>(); DetourAttach(&CtxVectorInit, hkCtxVectorInit);
	// fuck
	CTX_UP_5 = game->GetLocation("4c 89 4c 24 20 44 89 44 24 18 53 55 56 57 48 81");
	DetourAttach(&CTX_UP_5, hkCTX_UP_5);
	UiCounter = game->ScanPattern("e8 ? ? ? ? 48 8b cb e8 ? ? ? ? e9 e4 00", 1)
		.Cast<PVOID>(); 	DetourAttach(&UiCounter, hkUiCounter);
	//DetourAttach(&RSU_1, hk_RSU_1);
	//NumToScreen = game->GetLocation("3b 51 08 7d 15 48 8b 41 20 48 63 d2 44 39");
	//DetourAttach(&NumToScreen, hk_NumToScreen);
}