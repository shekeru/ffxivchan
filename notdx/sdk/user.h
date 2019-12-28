#pragma once
#include "repl.h"
#include "imgui.h"
#include "examples/imgui_impl_win32.h"
#include "examples/imgui_impl_dx11.h"
// Fucking ImGUI autism
#include <functional>
enum class KeyState
{
	None = 1,
	Down,
	Up,
	Pressed /*Down and then up*/
}; DEFINE_ENUM_FLAG_OPERATORS(KeyState);
class Interface {
public:
	~Interface();
	// Input Shit
	bool IsKeyDown(std::uint32_t vk);
	bool WasKeyPressed(std::uint32_t vk);
	KeyState GetKeyState(std::uint32_t vk);
	// Windows Aids
	bool ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool ProcessKeybdMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool ProcessMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	// Settings
	void RegisterHotkey(std::uint32_t vk, std::function<void(void)> f);
	void RemoveHotkey(std::uint32_t vk);
	// My Interface
	void StartGUI(HWND descW);
	ImGuiContext* ctx;
	// Pretty Much Global Vars
	bool IsOpen, Stop, IsDemo, IsRepl;
	// Needed Vars
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pImmediateContext;
	IDXGISwapChain* pSwapChain;
	// Locals Vars
	ID3D11RenderTargetView* pTargetView;
	WNDPROC prevProc;
	HWND hWindow;
	// Fuck It
	void ResizeTarget() {
		ID3D11Texture2D *pBackBuffer;
		pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pTargetView);
		pBackBuffer->Release();
	}
private:
	std::function<void(void)> Hotkeys[256];
	KeyState Keymap[256];
}; inline Interface user; LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
// User Components
namespace User {
	void MainMenuBar();
	void NameOverlay();
	void LuaConsole();
};