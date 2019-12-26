#include "sdk.h"
#include "gui.h"

VOID WINAPI ModuleEntry(HMODULE hInstance) {
	SDK::Memory = new MemorySystem("ffxiv_dx11.exe");
		Sleep(9750); SDK::RegisterLua(); 
	sys.sdkDispatch = (WNDPROC)SetWindowLongPtr
		(sys.hWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);
	SDK::LuaExec("print(\"Loaded Lua State...\")\n");
	// main loop
	while (!sys.Stop) {
		//auto chars = (CUnit**) SDK::Memory->ScanPattern("48 8b 42 08 48 c1 e8 03 3d a7 01 00 00 77 24 8b c0 48 8d 0d");
		//auto party = SDK::Memory->ScanPattern("48 8D 7C 24 20 66 66 0F 1F 84 00 00 00 00 00 48 8B 17 48 8D 0D");
		//printf("chars: %p\n", chars[0]);
		//if (chars[0]) {
		//	printf("name: %s\n", chars[0]->Name());
		//}
		//	printf("ID3D11DeviceContext: %x\n", *dRealCtx);
		Sleep(1000);
	}; sys.~Interface();
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
	VMT::Factory->HookVM(Hooks::CreateSwapChain, 
		IDXGIFactory__CreateSwapChain); VMT::Factory->ApplyVMT(); 
	return value;
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