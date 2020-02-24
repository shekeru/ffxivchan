#include <ActionSystem/ActionManager.h>
#include "user.h"

//PVOID oDebugActiveProcess;
//BOOL _stdcall hkDebugActiveProcess(DWORD dwProcessId) {
//	ORIGINAL(hkDebugActiveProcess, oDebugActiveProcess); auto self = GetProcessId(0);
//	printf("Debugger requested by proc(%i), in %i\n", dwProcessId, self);
//	return dwProcessId == self ? true : original(dwProcessId);
//}
//
//PVOID oIsDebuggerPresent;
//BOOL _stdcall hkIsDebuggerPresent() {
//	printf("no debugger\n");
//	return false;
//};

void MemorySystem::DetourAll(){
	HMODULE Kernel32 = GetModuleHandle("kernel32.dll");
	// Initialize Detours
	DetourRestoreAfterWith(); DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	// Attaching Detours
	//oIsDebuggerPresent = GetProcAddress(Kernel32, "IsDebuggerPresent");
	//oDebugActiveProcess = GetProcAddress(Kernel32, "DebugActiveProcess");
	//printf("%p - %p\n", oIsDebuggerPresent, oDebugActiveProcess);
	//DetourAttach(&oDebugActiveProcess, hkDebugActiveProcess); 
	//DetourAttach(&oIsDebuggerPresent, hkIsDebuggerPresent); 
	// Poorly Implemented Shit
	using namespace Hooks; ActionSys::Attach();
	RaptureAttach(); NetworkAttach(); MarketAttach(); ContextAttach();
	// Detours Final
	DetourTransactionCommit();
};

#include "examples/imgui_impl_dx11.h"
#include "examples/imgui_impl_win32.h"
HRESULT _fastcall Hooks::CreateSwapChain(IDXGIFactory* pFactory, ID3D11Device *pDevice,
	DXGI_SWAP_CHAIN_DESC *pDesc, IDXGISwapChain **ppSwapChain)
{
	local eval = VMT::Factory->GetOriginalMethod(CreateSwapChain);
	auto value = eval(pFactory, pDevice, pDesc, ppSwapChain);
	(sys.pDevice = pDevice)->GetImmediateContext(&sys.pImmediateContext);
	printf("CreateSwapChain() -> swc: %p, d3d: %x \n", sys.pSwapChain 
		= *ppSwapChain, pDevice); sys.StartGUI(pDesc->OutputWindow);
	VMT::SwapChain = new VMT(sys.pSwapChain); game->DetourAll();
	VMT::SwapChain->HookVM(Hooks::Present, 8);
	VMT::SwapChain->ApplyVMT(); return value;
}

HRESULT _fastcall Hooks::Present(IDXGISwapChain *pChain, UINT SyncInterval, UINT Flags)
{
	static auto eval = VMT::SwapChain->GetOriginalMethod(Present);
	ImGui_ImplWin32_NewFrame(); ImGui_ImplDX11_NewFrame(); ImGui::NewFrame();
	User::GenerateFrame(); ImGui::EndFrame(); ImGui::Render();
	sys.pImmediateContext->OMSetRenderTargets(1, &sys.pTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return eval(pChain, SyncInterval, Flags);
}