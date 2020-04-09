#include "user.h"

PVOID o_security_check_cookie;
void _cdecl hk_security_check_cookie
(uintptr_t StackCookie){;};

PVOID oIsDebuggerPresent;
BOOL _stdcall hkIsDebuggerPresent() {
	return false;
};

MemorySystem::MemorySystem(const char* exe_name) {
	GetModuleInformation(GetCurrentProcess(), GetModuleHandle(exe_name), 
		&baseModule, sizeof(baseModule)); Utils::EnableConsole();
	DetourRestoreAfterWith(); DetourTransactionBegin(); DetourUpdateThread(GetCurrentThread());
	// Anti-Anti-Debugger
	oIsDebuggerPresent = GetProcAddress(GetModuleHandle("kernel32.dll"), "IsDebuggerPresent");
	DetourAttach(&oIsDebuggerPresent, hkIsDebuggerPresent);
	// Fuck your Stack Cookies
	o_security_check_cookie = ScanPattern("e8 ? ? ? ? 4c 8d 9c 24 80 03 00", 1).Cast<PVOID>(); 
	DetourAttach(&o_security_check_cookie, hk_security_check_cookie); 
	// Continue as Normal?
	DetourTransactionCommit(); xiv = new FFXIV();
	printf("ffxiv Executable Start -> %p\n", baseModule.lpBaseOfDll);
};

auto SignatureArray(char* ptr) {
	auto Vec = vector<short>{};
	for (char* end = ptr + strlen(ptr); ptr < end; ptr++) {
		if ('?' == *ptr) {
			Vec.push_back(-1); ptr++;
		} else Vec.push_back(strtoul
			(ptr, &ptr, 16));
	}; return Vec;
};

template<typename TYPE>
TYPE* MemorySystem::GetLocation(const char* signature, int start) {
	auto bytes = (UCHAR*) baseModule.lpBaseOfDll;
	auto Vec = SignatureArray((char*) signature);
	auto End = baseModule.SizeOfImage - Vec.size(); 
	for (auto i = 0ul; i < End; i++) {
		bool found = true; for (auto j = 0ul; j < Vec.size(); j++)
			if (bytes[i + j] != Vec[j] && Vec[j] != -1) {
				found = false; break;
		}; if (found) return (TYPE*)(bytes + i + start);
	}; return (TYPE*) printf("SCAN FAILURE: %s\n", signature);
};

void MemorySystem::StackTrace() {
	const int N = 24; PVOID Stack[N];
	RtlCaptureStackBackTrace(1, N, Stack, 0);
	for (int i = 0; i < N && Stack[i]; i++)
		printf(" [%i] frame: %p / %p \n", i + 1, Stack[i], 
			uintptr_t(Stack[i]) - uintptr_t(baseModule.lpBaseOfDll) 
		+ 0x140000000);
};