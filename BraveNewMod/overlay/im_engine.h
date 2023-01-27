#pragma once
#include "vmt.h"
#include "imgui.h"
#include "overlay.h"
// #include "Managers.h"
#include <iostream>
#include <fstream>

enum class KeyState
{
	None = 1,
	Down,
	Up,
	Pressed /*Down and then up*/
}; DEFINE_ENUM_FLAG_OPERATORS(KeyState);

class IM_Engine {
public:
	~IM_Engine();

	bool IsKeyDown(uint32_t vk);
	bool WasKeyPressed(uint32_t vk);
	KeyState GetKeyState(uint32_t vk);

	bool ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool ProcessKeybdMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool ProcessMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void RegisterHotkey(uint32_t vk, std::function<void(void)> f);
	void RemoveHotkey(uint32_t vk);
	// My Interface
	ImGuiContext* imtx;
	IM_Interface overlay;
	void StartGUI(HWND descW);

	// Needed Vars
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pImmediateContext;
	IDXGISwapChain* pSwapChain;
	// Locals Vars
	ID3D11RenderTargetView* pTargetView;
	WNDPROC prevProc; HWND hWindow;
	PVOID font_data; int font_size;

	void ResizeTarget();
private:
	KeyState Keymap[256];
	std::function<void(void)> Hotkeys[256];
}; 

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Move This Later
//std::map<std::string, PVOID> Windows;