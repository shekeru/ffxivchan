#include "user.h"
#include <detours.h>

HANDLE* HeapHandle; 
ULONG64 repl_A;

PVOID SpawnWindow;
char __fastcall hkSpawnWindow(PVOID obj, char* Name, UCHAR flag, UINT ex) {
	static auto eval = (decltype(&hkSpawnWindow))SpawnWindow;
	show(SpawnWindow)("at: %p, %s, Flag: %i, Ex: %x\n", obj, Name, flag, ex);
	Windows[Name] = obj; return eval(obj, Name, flag, ex);
};

PVOID SendAction;
static ULONG64 ESC_SEQ[2] = { 3i64, 0x1ffffffff };
static ULONG64 GATHERING[2]{ 0i64, -1 };
VOID WINAPI GatherCallback(ULONG64 nil);
__int64 __fastcall hkSendAction(PVOID obj, __int64 N, ULONG64* arr, __int64 opt) {
	static auto eval = decltype(&hkSendAction)(SendAction); string window = "???";
	//if ((PVOID)obj == Windows["ContextMenu"]) {
	//	auto cock = IntPtr(obj)[0x160].Cast<ULONG64*>();
	//	printf("  %p::IsContextMenu(%llu) \n", obj, cock[1]);
	//	auto after = cock + 14;
	//	for (int i = 0; i < cock[1]; i++) {
	//		printf("%s\n", after[1 + i * 2]);
	//	};
	//}
	//for (auto it = Windows.begin(); it != Windows.end(); ++it)
	//	if (it->second == obj)
	//		window = std::string(it->first);
	//printf("SendAction(%s), N: %i, ", window, N);
	//for (int i = 0; i < N * 2; i += 2)
	//	printf("(%x, %llx), ", arr[i] & 0xFFFFFFFF,
	//		arr[i + 1]); printf("%x\n", opt);
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
	static auto eval = decltype(&hkSendAction)(SendAction);
	UINT* value = (UINT*)(UINT64(Windows["Gathering"]) + 0x180);
	printf("Gathering Location: %p\n", Windows["Gathering"]);
	while(*value == 0x23340303) {
		printf("\t Callback!! %x\n", value);
		eval(Windows["Gathering"], 1, GATHERING, 1); 
		Sleep(1250);
	}; GATHERING[1] = -1; GATHERING[0] = 0;
}

const char* AutoGather = "Action: Gather All";
PVOID SetsCtxReal; const char* dString = "      >_<  ";
__int64 __fastcall hkSetsCtxReal(PVOID ctx, int N, UINT64* arr) {
	static auto eval = decltype(&hkSetsCtxReal)(SetsCtxReal);
	if (ctx == Windows["ContextMenu"] && N && arr) {
		printf("SETS_REAL: %p, %i, %p\n", ctx, N, arr);
		printf("Row Length: %llu\n", repl_A = (int) arr[1]);
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

// Icon System Fuckfest
class IconSys {
public:
	INT64 GetIcon(int action) {
		static auto original = decltype(&raw_call)(_Ptr);
		return original(this, action);
	}; 	inline static PVOID _Ptr;
public:
	int RedMage(int action) {
		switch (action) {
		case Action::Verthunder:
			if (xiv->LocalActor->HasAura(Status::VerfireReady))
				return Action::Verfire;
			goto jolt_case;
		case Action::Veraero:
			if (xiv->LocalActor->HasAura(Status::VerstoneReady))
				return Action::Verstone;
			goto jolt_case;
		default:
			break;
		jolt_case:
			if (!xiv->LocalActor->HasAura(Status::Dualcast) &&
				!xiv->LocalActor->HasAura(Status::Swiftcast))
				return Action::Jolt;
		}; return GetIcon(action);
	}
public:
	static __int64 __fastcall raw_call(IconSys* self, int action) {
		if (xiv->LocalActor) switch (xiv->LocalActor->JobId()) {
		case Job::RedMage:
			return self->RedMage(action);
		default:
			break;
		}; return self->GetIcon(action);
	}
};

void Hooks::RaptureAttach() {
	HeapHandle = game->ScanPattern(Offsets::HEAP_HANDLE, 3).Cast<HANDLE*>();
	IconSys::_Ptr = game->ScanPattern(Offsets::GET_ICON, 1).Cast<PVOID>();
	SetsCtxReal = game->ScanPattern(Offsets::SETS_REAL, 1).Cast<PVOID>();
	SendAction = game->ScanPattern(Offsets::SENDACTION, 1).Cast<PVOID>();
	SpawnWindow = game->ScanPattern(Offsets::SPAWNUI, 4).Cast<PVOID>();
	// Attachments
	DetourAttach(&IconSys::_Ptr, IconSys::raw_call);
	DetourAttach(&SetsCtxReal, hkSetsCtxReal);
	DetourAttach(&SpawnWindow, hkSpawnWindow);
	DetourAttach(&SendAction, hkSendAction);
}