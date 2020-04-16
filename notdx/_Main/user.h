#pragma once
#include "vmt.h"
#include "imgui.h"
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
inline class IM_Engine {
public:
	~IM_Engine();
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
} sys; LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
// Move This Later
inline map<string, PVOID> Windows;
// User Shit
class Logging {
public:
	bool scroll; deque<char*> Items;
	void Render(), Log(const char* fmt...);
};
class LogWindow {
public:
	LogWindow(); void Render(); bool IsOpen;
	map<const char*, Logging*> windows;
};
// Macros
#define show(EXP) \
	User.log.windows[#EXP]->Log
// User Components
inline class IM_Interface {
public:
	bool IsOpen, ShowDemo;
	LogWindow log; void Render(); 
} User;
namespace User2 {
	void MainMenuBar(), NameOverlay(), ZoomPanel(),
		LuaConsole(), QuestPanel(), MusicPanel(); 
};