#include "user.h"

#include "examples/imgui_impl_win32.h"
#include "examples/imgui_impl_dx11.h"
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
	static auto eval = VMT::SwapChain->GetOriginalMethod(Present); using namespace User;
	ImGui_ImplWin32_NewFrame(); ImGui_ImplDX11_NewFrame(); ImGui::NewFrame();
	if (sys.IsOpen) {
		if(sys.IsDemo) 
			ImGui::ShowDemoWindow(&sys.IsDemo);
		MainMenuBar(); LuaConsole();
	} else NameOverlay();
	ImGui::EndFrame(); ImGui::Render(); sys.ResizeTarget();
	sys.pImmediateContext->OMSetRenderTargets(1, &sys.pTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return eval(pChain, SyncInterval, Flags);
}