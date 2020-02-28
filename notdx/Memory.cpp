#include "user.h"

PVOID o_security_check_cookie;
void _cdecl hk_security_check_cookie
(uintptr_t StackCookie){;};

MemorySystem::MemorySystem(const char* exe_name) {
	GetModuleInformation(GetCurrentProcess(),
		GetModuleHandle(exe_name), &baseModule, sizeof(baseModule));
	DetourRestoreAfterWith(); DetourTransactionBegin(); DetourUpdateThread(GetCurrentThread());
	o_security_check_cookie = ScanPattern("e8 ? ? ? ? 4c 8d 9c 24 80 03 00", 1)
		.Cast<PVOID>(); DetourAttach(&o_security_check_cookie, hk_security_check_cookie); 
	DetourTransactionCommit(); printf("ffxiv Executable Start -> %p\n", baseModule.lpBaseOfDll);
};

void MemorySystem::StackTrace() {
	const int N = 24; PVOID Stack[N];
	RtlCaptureStackBackTrace(1, N, Stack, 0);
	for (int i = 0; i < N && Stack[i]; i++)
		printf(" [%i] frame: %p / %x \n", i + 1, Stack[i],
			uintptr_t(Stack[i]) - uintptr_t(baseModule.lpBaseOfDll));
};