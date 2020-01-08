#include "user.h"
#include <detours.h>

PVOID SpawnWindow; std::map<std::string, PVOID> Windows; VMT* CtxMenu;
char __fastcall hkSpawnWindow(void* obj, char* Name, UCHAR flag, UINT ex) {
	static auto eval = (decltype(&hkSpawnWindow))SpawnWindow;
	printf("[Spawn] %p (%s), Flag: %i, Ex: %x\n", obj, Name, flag, ex);
	Windows[Name] = obj; return eval(obj, Name, flag, ex);
};


PVOID SendAction; const char* g_next = "FUCK WHY NOT";
__int64 __fastcall hkSendAction(INT64 obj, __int64 N, ULONG64* arr, __int64 opt) {
	static auto eval = (decltype(&hkSendAction))SendAction; std::string window = "???";
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
	//for (int i = 0; i < N*2; i+=2)
	//	printf("(%llu, %llu), ", arr[i], 
	//		arr[i+1]); printf("%x\n", opt); 
	return eval(obj, N, arr, opt);
}

void Hooks::RaptureAttach() {
	SendAction = game->ScanPattern(Offsets::SENDACTION, 1).Cast<PVOID>();
	SpawnWindow = game->ScanPattern(Offsets::SPAWNUI, 4).Cast<PVOID>();
	// Attaching
	DetourAttach(&SpawnWindow, hkSpawnWindow);
	DetourAttach(&SendAction, hkSendAction);
}