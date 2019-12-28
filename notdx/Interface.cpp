#include "gui.h"

#include "imgui_internal.h"
void Interface::Connect(HWND descW) {
	ctx = ImGui::CreateContext();
	ImGui_ImplWin32_Init(hWindow = descW); 
	ImGui_ImplDX11_Init(pDevice, pImmediateContext);
	// Style Shit
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowBorderSize = 0.0f; style.Alpha = 0.92f;
	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(132/255.0, 209/255.0, 96/255.0, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(235 / 255.0, 64 / 255.0, 52 / 255.0, 1.0f);
	// Font Shit
	//ImGuiIO& io = ImGui::GetIO();
	//auto jap_shit = io.Fonts->AddFontFromFileTTF("meiryo_00.ttf", 16.f, NULL);
	//ImGui::SetCurrentFont(jap_shit);
	// Menu Toggle
	RegisterHotkey(VK_INSERT, []() {
		sys.IsOpen ^= 1;
	});
};

Interface::~Interface() {
	// Clean imgui
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext(ctx);
	// Clean input sys
	if (sdkDispatch)
		SetWindowLongPtr(hWindow, GWLP_WNDPROC, (LONG_PTR)sdkDispatch);
	sdkDispatch = NULL;
};

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	sys.ProcessMessage(msg, wParam, lParam);
	// Fuck my Life
	if (sys.IsOpen)
		ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
	else 
		CallWindowProc(sys.sdkDispatch, hWnd, msg, wParam, lParam);
	return true;
}

bool Interface::ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_LBUTTONDBLCLK:
	case WM_XBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
	case WM_XBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_LBUTTONUP:
	case WM_XBUTTONUP:
		return ProcessMouseMessage(uMsg, wParam, lParam);
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		return ProcessKeybdMessage(uMsg, wParam, lParam);
	default:
		return false;
	}
}

bool Interface::ProcessMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	auto key = VK_LBUTTON;
	auto state = KeyState::None;
	switch (uMsg) {
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
		state = uMsg == WM_MBUTTONUP ? KeyState::Up : KeyState::Down;
		key = VK_MBUTTON;
		break;
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
		state = uMsg == WM_RBUTTONUP ? KeyState::Up : KeyState::Down;
		key = VK_RBUTTON;
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
		state = uMsg == WM_LBUTTONUP ? KeyState::Up : KeyState::Down;
		key = VK_LBUTTON;
		break;
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_XBUTTONDBLCLK:
		state = uMsg == WM_XBUTTONUP ? KeyState::Up : KeyState::Down;
		key = (HIWORD(wParam) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2);
		break;
	default:
		return false;
	}
	if (state == KeyState::Up && Keymap[key] == KeyState::Down)
		Keymap[key] = KeyState::Pressed;
	else
		Keymap[key] = state;
	return true;
}

bool Interface::ProcessKeybdMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	auto key = wParam;
	auto state = KeyState::None;

	switch (uMsg) {
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		state = KeyState::Down;
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		state = KeyState::Up;
		break;
	default:
		return false;
	}

	if (state == KeyState::Up && Keymap[int(key)] == KeyState::Down) {
		Keymap[int(key)] = KeyState::Pressed;

		auto& hotkey_callback = Hotkeys[key];

		if (hotkey_callback)
			hotkey_callback();

	}
	else {
		Keymap[int(key)] = state;
	}

	return true;
}

KeyState Interface::GetKeyState(std::uint32_t vk)
{
	return Keymap[vk];
}

bool Interface::IsKeyDown(std::uint32_t vk)
{
	return Keymap[vk] == KeyState::Down;
}

bool Interface::WasKeyPressed(std::uint32_t vk)
{
	if (Keymap[vk] == KeyState::Pressed) {
		Keymap[vk] = KeyState::Up;
		return true;
	}
	return false;
}

void Interface::RegisterHotkey(std::uint32_t vk, std::function<void(void)> f)
{
	Hotkeys[vk] = f;
}

void Interface::RemoveHotkey(std::uint32_t vk)
{
	Hotkeys[vk] = nullptr;
}