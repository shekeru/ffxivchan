#include "user.h"

HRESULT _fastcall Hooks::CreateSwapChain(IDXGIFactory* pFactory, ID3D11Device *pDevice,
	DXGI_SWAP_CHAIN_DESC *pDesc, IDXGISwapChain **ppSwapChain)
{
	static auto eval = VMT::Factory->GetOriginalMethod(CreateSwapChain);
	auto value = eval(pFactory, pDevice, pDesc, ppSwapChain);
	(user.pDevice = pDevice)->GetImmediateContext(&user.pImmediateContext);
	printf("dx11[cswc]: %x, ctx: %x, chain %x\n", pDevice, user.pImmediateContext, 
		user.pSwapChain = *ppSwapChain); user.StartGUI(pDesc->OutputWindow);
	VMT::SwapChain = new VMT(user.pSwapChain);
	VMT::SwapChain->HookVM(Hooks::Present, 8);
	VMT::SwapChain->ApplyVMT();
	return value;
}

HRESULT _fastcall Hooks::Present(IDXGISwapChain *pChain, UINT SyncInterval, UINT Flags)
{
	static auto eval = VMT::SwapChain->GetOriginalMethod(Present); using namespace User;
	ImGui_ImplWin32_NewFrame(); ImGui_ImplDX11_NewFrame(); ImGui::NewFrame();
	if (user.IsOpen) {
		if(user.IsDemo) 
			ImGui::ShowDemoWindow(&user.IsDemo);
		MainMenuBar(); LuaConsole();
	} else NameOverlay();
	ImGui::EndFrame(); ImGui::Render(); user.ResizeTarget();
	user.pImmediateContext->OMSetRenderTargets(1, &user.pTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return eval(pChain, SyncInterval, Flags);
}