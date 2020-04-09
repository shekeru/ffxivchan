#include "user.h"

class Spec {
	// 3 = Int?
	// 6 = char*?
public:
	ULONG64 Type, Value;
};

PVOID SendAction;
typedef PVOID CtxFn; int CtxSize;
PVOID CtxVectorInit; CtxFn* CtxCallback;
CtxFn* hkCtxVectorInit(CtxFn*& ref, INT64 len) {
	ORIGINAL(hkCtxVectorInit, CtxVectorInit); return
		CtxCallback = original(ref, CtxSize = len);
}; const char* AutoGather = "Action: Gather All";
	Spec* CtxMenu; char* CtxCaller;

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
	show(SendAction)("ptr: %p, SendAction(%s), N: %i, ", obj, (char*) obj + 8, N);
		for (int i = 0; i < N * 2; i += 2)
			show(SendAction)("(%x, %llx), ", arr[i] & 0xFFFFFFFF,
				arr[i + 1]); show(SendAction)("%x\n", opt);
	// Conditions
	// if (obj == Windows["ToDoList"])
	//	return ToDoList(obj, N, (Spec*) arr, opt);
	if (obj == Windows["Gathering"] && int
		(arr[1]) == 0x81 && !GATHERING[0]) {
		GATHERING[1] = arr[3] & 0xFFFFFFFF;
	} else if (obj == Windows["ContextMenu"] && CtxCaller == Windows["Gathering"]) {
		if (N == 5 && arr[0] == 3i64 && arr[2] == 3i64 && GATHERING[1] != -1) {
			if (CtxMenu && CtxMenu[int(arr[3])+7].Value == (ULONG64)AutoGather) {
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
};

PVOID WindowReady; ULONG64 CONFIRM_A [2] { 3i64, 7i64 };
char hkWindowReady(PVOID obj, char* Name, UCHAR flag, UINT ex) {
	local spawnW = decltype(&hkWindowReady)(WindowReady);
	local sendA = decltype(&hkSendAction)(SendAction);
	if (!strcmp(Name, "ContentsFinderConfirm"))
		sendA(obj, 1, CONFIRM_A, 1);
	if (!strcmp(Name, "SalvageResult"))
		sendA(obj, 1, ESC_SEQ, 1);
	return spawnW(obj, Name, flag, ex);
};

// group -> -1: none, 0: gen, 1: action, 2: item
struct HotbarSlot {
	int subtype, resource, group, ID, 
		tracking, can_use, _6, _7;
	int percent, _9, display, quantity, 
		_12, _13, _14, _15;
};

PVOID SpawnWindow; PVOID UiTable[256]; PVOID SuperClass;
__int64 hkSpawnWindow(void* super, void* ptr, const char* str) {
	ORIGINAL(hkSpawnWindow, SpawnWindow); auto value = original(super, ptr, str); 
	UiTable[value] = ptr; local sendA = decltype(&hkSendAction)(SendAction);
	SuperClass = super; Windows[str] = ptr; show(SpawnWindow)
		("ptr: %p <%s>, id: %i\n", ptr, str, value); return value;
}

// 0x41 ->  6, NULL -> 38
// Various Mouse Actions here
HANDLE ProcHeap;  PVOID CtxSets_R5; // 7: right click
__int64 hkCtxSets_R5(void* rAtkM, UINT signal, UINT size, Spec* data,
	void* ctxList, INT64 zone, UINT16 uID, int flag) {
	ORIGINAL(hkCtxSets_R5, CtxSets_R5); if (signal == 7) {
		if(!CtxMenu) CtxMenu = (Spec*) HeapAlloc(ProcHeap, HEAP_ZERO_MEMORY, 592);
		const char* inject = NULL; CtxCaller = (char*) UiTable[uID];
		// Context Menu Injections
		if (CtxCaller == Windows["Gathering"])
			inject = AutoGather;
		//printf("dt: %p, window: %i -> %p, %s \n", data, 
		//	uID, CtxCaller, CtxCaller ? CtxCaller + 8 : 0);
		if (CtxCaller && inject && data) {
			Spec *Body = CtxMenu + 7; memcpy(CtxMenu, data, 16 * size); 
			if (data->Value < size - 7) {
				memmove(Body + CtxMenu->Value + 1, Body + CtxMenu->Value, 16 * CtxMenu->Value);
				Body[2 * (CtxMenu->Value + 1)] = Spec{ 3, 1 }; size += 1;
			}; Body[CtxMenu->Value] = Spec{ Body[0].Type, (ULONG64) inject }; 
			CtxMenu->Value += 1; size += 1; data = CtxMenu;
		} 
	}; return original(rAtkM, signal, size, data, ctxList, zone, uID, flag);
};

PVOID FlagSwitch;
__int64 FlagWriter(void* ptr, int state, char* skill, unsigned int action, int ex) {
	ORIGINAL(FlagWriter, FlagSwitch);
	printf("ptr: %p, %s (%i) -> %i\n", ptr, skill, action, state);
	return original(ptr, state, skill, action, ex);
}

PVOID CAST2;
void hkCAST2(void* ptr) {
	ORIGINAL(hkCAST2, CAST2);
	printf("cast2: %p\n", ptr);
	//original(ptr);
}

PVOID UpdatesBarInteger; PVOID cRelated;
void hkUpdatesBarInteger(__int64 rcx, __int64 rdx, int r8d) {
	ORIGINAL(hkUpdatesBarInteger, UpdatesBarInteger);
	if (rdx == 0x159 && r8d == 210 && false) {
		auto rax = *(__int64 *)(rcx + 0x20);
		printf("Related: %p, Real: %p\n", 
			cRelated, rax + rdx * 4);
	}; return original(rcx, rdx, r8d);
};

PVOID BarInteger_Parent;
__int64 hkBarInteger_Parent(__int64 rcx, __int64 rdx, char** c_ptr, __int64 a4, __int64 a5, unsigned int slot_no, unsigned int a7) {
	ORIGINAL(hkBarInteger_Parent, BarInteger_Parent); cRelated = c_ptr; static CONTEXT ctx; 
	auto value = original(rcx, rdx, c_ptr, a4, a5, slot_no, a7);
	if (strstr(*c_ptr, "Holmgang")) {
		RtlCaptureContext(&ctx); printf("rsp: %p, v: %i \n", 
			(DWORD*)(ctx.Rsp - 0x6c), *(DWORD*)(ctx.Rsp - 0x6c));
	}; return value;
};

PVOID CD_ByObject;
__int64 hkCD_ByObject(char** ptr, char flag) {
	ORIGINAL(hkCD_ByObject, CD_ByObject);
	auto value = original(ptr, flag);
	//if (strstr(*ptr, "Holmgang"))
		//printf("\tptr:%p, is: %i\n", ptr, value);
	return value;
}

PVOID optionB;
__int64 hkoptionB(__int64 ptr, DWORD& st_ref, int a3) {
	ORIGINAL(hkoptionB, optionB); 
	if (st_ref == 205) printf("pre!!\n");
	auto val = original(ptr, st_ref, a3);
	if (st_ref == 205 || val == 205) {
		printf("ptr: %p, a3: %i, v: %p\n", ptr, a3, val);
	}; return val;
};

PVOID GetElapsed_ptr;
float GetElapsed(ActionSys ptr, UINT8 flag, INT64 actionID) {
	ORIGINAL(GetElapsed, GetElapsed_ptr);
	auto value = GetElapsed(ptr, flag, actionID);
	if (actionID == Action::Holmgang) {
		printf("elapsed: %f\n", value);
	}; return value;
};

void Hooks::RaptureAttach() {
	printf("Process Heap: %x\n", ProcHeap = GetProcessHeap());
	// Fuck Me
	CtxSets_R5 = game->GetLocation("4c 89 4c 24 20 44 89 44 24 18 53 55 56 57 48 81");
	SpawnWindow = game->ScanPattern("e8 ? ? ? ? 85 c0 75 1d 48 8b 0b", 1).Cast<PVOID>();
	CtxVectorInit = game->ScanPattern("e8 ? ? ? ? 48 8b 43 08 48 2b 03 48 c1 f8 03", 1).Cast<PVOID>();
	WindowReady = game->ScanPattern("48 8b ce e8 ? ? ? ? 48 8b 4d 4f", 4).Cast<PVOID>();
	SendAction = game->ScanPattern("e8 ? ? ? ? 8b 44 24 20 c1 e8 05", 1).Cast<PVOID>();
	FlagSwitch = game->ScanPattern("cf 89 44 24 20 e8 ? ? ? ? b0 01", 6).Cast<PVOID>();
	CAST2 = game->GetLocation("40 53 48 83 ec 20 83 b9 30 09 00 00 00 48 8b d9 7e 57 48");
	// Attachments
	DetourAttach(&CtxSets_R5, hkCtxSets_R5);
	DetourAttach(&SpawnWindow, hkSpawnWindow);
	DetourAttach(&CtxVectorInit, hkCtxVectorInit);
	DetourAttach(&WindowReady, hkWindowReady);
	DetourAttach(&SendAction, hkSendAction);
	//DetourAttach(&FlagSwitch, FlagWriter);
	//DetourAttach(&CAST2, hkCAST2);
	//UpdatesBarInteger = game->ScanPattern("e8 ? ? ? ? 44 8b 65 97 eb 30", 1)
	//	.Cast<PVOID>(); DetourAttach(&UpdatesBarInteger, hkUpdatesBarInteger);
	//BarInteger_Parent = game->ScanPattern("e8 ? ? ? ? eb 20 e8 ? ? ? ? 8d 56 ff", 1)
	//	.Cast<PVOID>(); DetourAttach(&BarInteger_Parent, hkBarInteger_Parent);
	//CD_ByObject = game->ScanPattern("e8 ? ? ? ? 41 8d 57 05 44 8b c0", 1)
	//	.Cast<PVOID>(); DetourAttach(&CD_ByObject, hkCD_ByObject);
	//optionB = game->ScanPattern("49 8b cd e8 ? ? ? ? 48 8b 0d ? ? ? ? ba 41 01", 4)
	//	.Cast<PVOID>(); DetourAttach(&optionB, hkoptionB);
	GetElapsed_ptr = game->ScanPattern("e8 ? ? ? ? 44 8b c7 48 8d 0d ? ? ? ? 41", 1)
		.Cast<PVOID>(); DetourAttach(&GetElapsed_ptr, GetElapsed);
}