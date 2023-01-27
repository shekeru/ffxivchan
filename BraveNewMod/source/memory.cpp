#include "memory.h"

MemorySystem::MemorySystem(LPCWSTR exe_name) {
	GetModuleInformation(GetCurrentProcess(),
		GetModuleHandle(exe_name), &baseModule, sizeof(baseModule));
	printf("Executable Start -> %p\n", baseModule.lpBaseOfDll);
	//DetourRestoreAfterWith(); DetourTransactionBegin(); DetourUpdateThread(GetCurrentThread());

	// DetourTransactionCommit();
};

UCHAR* MemorySystem::FindPattern(const char* signature, int start) {
	auto base = (UCHAR*)baseModule.lpBaseOfDll;
	std::vector sig = SignatureArray((char*)signature);
	auto end = baseModule.SizeOfImage - sig.size();

	for (auto i = 0ul; i < end; i++) {
		bool found = true;

		for (auto j = 0ul; j < sig.size(); j++) {
			if (base[i + j] != sig[j] && sig[j] != -1) {
				found = false; break;
			};
		};

		if (found) {
			return base + i + start;
		};

	}; printf("SCAN FAILURE: %s\n", signature);
	return nullptr;
};

std::vector<short> MemorySystem::SignatureArray(char* ptr) {
	std::vector bytes = std::vector<short>{};
	for (char* end = ptr + strlen(ptr); ptr < end; ptr++) {
		if ('?' == *ptr) {
			bytes.push_back(-1); ptr++;
		}
		else bytes.push_back(strtoul
		(ptr, &ptr, 16));
	}; return bytes;
};

void MemorySystem::StackTrace() {
	const int N = 24; PVOID Stack[N];
	RtlCaptureStackBackTrace(1, N, Stack, 0);
	for (int i = 0; i < N && Stack[i]; i++)
		printf(" [%i] frame: %p / %p \n", i + 1, Stack[i],
			uintptr_t(Stack[i]) - uintptr_t(baseModule.lpBaseOfDll)
			+ 0x140000000);
};