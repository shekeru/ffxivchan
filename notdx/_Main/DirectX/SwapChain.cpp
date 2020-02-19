#include <ActionSystem/ActionManager.h>
#include "user.h"

void MemorySystem::DetourAll(){
	DetourRestoreAfterWith(); DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	// Attaching Detours
	ActionSys::Attach();
	using namespace Hooks;
	RaptureAttach(); NetworkAttach();
	MarketAttach(); ContextAttach();
	// Detours Post
	DetourTransactionCommit();
};

#include "examples/imgui_impl_dx11.h"
#include "examples/imgui_impl_win32.h"
HRESULT _fastcall Hooks::CreateSwapChain(IDXGIFactory* pFactory, ID3D11Device *pDevice,
	DXGI_SWAP_CHAIN_DESC *pDesc, IDXGISwapChain **ppSwapChain)
{
	static auto eval = VMT::Factory->GetOriginalMethod(CreateSwapChain);
	auto value = eval(pFactory, pDevice, pDesc, ppSwapChain);
	(sys.pDevice = pDevice)->GetImmediateContext(&sys.pImmediateContext);
	printf("dx11[cswc]: %x, ctx: %x, chain %x\n", pDevice, sys.pImmediateContext, 
		sys.pSwapChain = *ppSwapChain); sys.StartGUI(pDesc->OutputWindow);
	game->DetourAll(); VMT::SwapChain = new VMT(sys.pSwapChain);
	VMT::SwapChain->HookVM(Hooks::Present, 8);
	VMT::SwapChain->ApplyVMT(); return value;
}

HRESULT _fastcall Hooks::Present(IDXGISwapChain *pChain, UINT SyncInterval, UINT Flags)
{
	static auto eval = VMT::SwapChain->GetOriginalMethod(Present);
	ImGui_ImplWin32_NewFrame(); ImGui_ImplDX11_NewFrame(); ImGui::NewFrame();
	User::GenerateFrame(); ImGui::EndFrame(); ImGui::Render(); sys.ResizeTarget();
	sys.pImmediateContext->OMSetRenderTargets(1, &sys.pTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return eval(pChain, SyncInterval, Flags);
}