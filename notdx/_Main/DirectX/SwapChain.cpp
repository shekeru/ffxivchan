#include <Managers.h>
#include "user.h"

void MemorySystem::DetourAll(){
	HMODULE Kernel32 = GetModuleHandle("kernel32.dll");
	// Initialize Detours
	DetourRestoreAfterWith(); DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	// Attaching Detours
	using namespace Hooks; ActionSys::Attach(); ZoomSys::Attach();
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
	local eval = VMT::SwapChain->GetOriginalMethod(Present); sys.ResizeTarget();
	ImGui_ImplWin32_NewFrame(); ImGui_ImplDX11_NewFrame(); ImGui::NewFrame();
	User::GenerateFrame(); ImGui::EndFrame(); ImGui::Render();
	sys.pImmediateContext->OMSetRenderTargets(1, &sys.pTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return eval(pChain, SyncInterval, Flags);
}

void Interface::ResizeTarget() {
	static ID3D11Texture2D *pBackBuffer; if (pSwapChain && pDevice && !pBackBuffer) {
		pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pTargetView);
		pBackBuffer->Release(); printf("Resize target check\n");
	};
}