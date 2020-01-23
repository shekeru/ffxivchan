#include "user.h"
#include "../Actor.h"
#include "..\res\resource.h"

VOID WINAPI ModuleEntry(HMODULE hInstance) {
	game = new MemorySystem("ffxiv_dx11.exe"); xiv = new FFXIV();
	Sleep(9750); vm.Connect(); sys.prevProc = (WNDPROC)
		SetWindowLongPtr (sys.hWindow, GWLP_WNDPROC, (LONG_PTR) WndProc);
	// Resources
	const HANDLE hbicon = ::LoadImage(
		hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON,
		::GetSystemMetrics(SM_CXICON),
		::GetSystemMetrics(SM_CYICON),
		0);
	if (hbicon)
		::SendMessage(sys.hWindow, WM_SETICON, ICON_BIG, (LPARAM) hbicon);
	const HANDLE hsicon = ::LoadImage(
		hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON,
		::GetSystemMetrics(SM_CXSMICON),
		::GetSystemMetrics(SM_CYSMICON),
		0);
	if (hsicon)
		::SendMessage(sys.hWindow, WM_SETICON, ICON_SMALL, (LPARAM) hsicon);
	// Misc Testing
	ifstream ifs("../game/lua/data/tradecraft.json"); 
		ifs >> xiv->tradecraft; ifs.close();
	cout << xiv->tradecraft;
	//auto chat_pb = game->ScanPattern(Offsets::CHAT, 3);
	//auto chat = chat_pb[0x2C10][0x16D8 + 0x48].Cast<ChatLog>();
	//printf("chat_pb, chats: %p, %p\n", chat_pb, chat);
	while (!sys.Stop) {
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