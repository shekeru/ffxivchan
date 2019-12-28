#include "user.h"

VOID WINAPI ModuleEntry(HMODULE hInstance) {
	game = new MemorySystem("ffxiv_dx11.exe");
		Sleep(9750); vL = new LuaVM();
	user.prevProc = (WNDPROC)SetWindowLongPtr
		(user.hWindow, GWLP_WNDPROC, (LONG_PTR) WndProc);
	while (!user.Stop) {
		Sleep(1000);
	};
FreeLibrary:
	FreeLibraryAndExitThread(hInstance, 1);
}

#define EXPORT comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
EXTERN_C HRESULT WINAPI CreateDXGIFactory(REFIID riid, void** ppFactory)
{
#pragma EXPORT
	decltype(&CreateDXGIFactory) original = DynLib
		{ "C:\\Windows\\System32\\dxgi.dll" }["CreateDXGIFactory"];
	auto value = original(riid, ppFactory); VMT::Factory = new VMT(*ppFactory);
	printf("CreateDXGIFactory() -> %p, yields %x\n", original, *ppFactory);
	VMT::Factory->HookVM(Hooks::CreateSwapChain, 10); 
	VMT::Factory->ApplyVMT(); return value;
}

EXTERN_C HRESULT WINAPI CreateDXGIFactory1(REFIID riid, void** ppFactory)
{
#pragma EXPORT
	decltype(&CreateDXGIFactory1) original = DynLib
	{ "C:\\Windows\\System32\\dxgi.dll" }["CreateDXGIFactory1"];
	printf("should Never Be called {CreateDXGIFactory1}"); 
	return original(riid, ppFactory);
}

BOOL APIENTRY DllMain(
	HMODULE hInstance,
	DWORD fdwReason,
	LPVOID lpvReserved
) {
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		Utils::EnableConsole();
		DisableThreadLibraryCalls(hInstance);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)
			ModuleEntry, hInstance, 0, 0); break;
	case DLL_PROCESS_DETACH:
		if (lpvReserved == nullptr)
			Utils::DisableConsole();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	default:
		break;
	};  return TRUE;
}