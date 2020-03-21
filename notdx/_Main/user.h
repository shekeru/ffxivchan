#pragma once
#include "logs.h"
#include "Managers.h"
// Fucking ImGUI autism
#include <functional>
#include <iostream>
#include <fstream>
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
	bool IsKeyDown(uint32_t vk);
	bool WasKeyPressed(uint32_t vk);
	KeyState GetKeyState(uint32_t vk);
	// Windows Aids
	bool ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool ProcessKeybdMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool ProcessMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	// Settings
	void RegisterHotkey(uint32_t vk, function<void(void)> f);
	void RemoveHotkey(uint32_t vk);
	// My Interface
	ImGuiContext* ctx;
	void StartGUI(HWND descW);
	// Pretty Much Global Vars
	bool IsOpen, Stop, IsDemo, IsRepl, 
		IsSpin, IsQuest, IsMusic, IsZoom;
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
	function<void(void)> Hotkeys[256]; KeyState Keymap[256];
}; inline Interface sys; LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
inline map<string, PVOID> Windows;
// User Components
namespace User {
	void MainMenuBar(), NameOverlay(), ZoomPanel(),
		LuaConsole(), SpinBotting(), QuestPanel(), 
	MusicPanel(); inline LogWindow log = LogWindow();
	inline void GenerateFrame() {
		if (sys.IsOpen) {
			log.Render(); if (sys.IsDemo)
				ImGui::ShowDemoWindow(&sys.IsDemo);
			MainMenuBar(); LuaConsole(); QuestPanel(); ZoomPanel();
		} else NameOverlay(); SpinBotting(); MusicPanel();
	};
};