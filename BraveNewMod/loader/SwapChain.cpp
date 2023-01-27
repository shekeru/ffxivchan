#include "context.h"
#include "hooks.h"
#include "vmt.h"

using namespace vMethods;

HRESULT __fastcall DXGI_Factory::CreateSwapChain::Function(
	IDXGIFactory* pFactory, ID3D11Device* pDevice,
	DXGI_SWAP_CHAIN_DESC* pDesc, IDXGISwapChain** ppSwapChain
) {
	get_context; get_virtual($vmt.vDXGI_Factory);
	local& _vmt = $vmt.vSwapChain;
	
	auto value = original(pFactory, pDevice, pDesc, ppSwapChain);
	printf("CreateSwapChain() -> swc: %llx, d3d: %llx \n",
		ctx.gui.pSwapChain = *ppSwapChain, ctx.gui.pDevice = pDevice);
	ctx.gui.StartGUI(pDesc->OutputWindow);

	if (_vmt != nullptr)
		delete _vmt;

	_vmt = new VMT(ctx.gui.pSwapChain);
	_vmt->ReplaceMethod<SwapChain::Present>();
	_vmt->ApplyVMT();

	return value;
};

#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

HRESULT __fastcall SwapChain::Present::Function(
	IDXGISwapChain* pChain, UINT SyncInterval, UINT Flags
) {
	get_context; get_virtual($vmt.vSwapChain);
	static byte nil = printf("Present called!\n");

	ctx.gui.ResizeTarget();
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX11_NewFrame(); 
	ImGui::NewFrame();

	ctx.gui.overlay.Render();

	ImGui::EndFrame(); 
	ImGui::Render();

	ctx.gui.pImmediateContext->OMSetRenderTargets(1, &ctx.gui.pTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return original(pChain, SyncInterval, Flags);
};

void IM_Engine::ResizeTarget() {
	static ID3D11Texture2D* pBackBuffer;
	if (pSwapChain && pDevice && !pBackBuffer) {
		pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pTargetView);
		pBackBuffer->Release(); printf("Resize target check\n");
	};
}