#pragma once
#include "imgui.h"
#include "vmt.h"

class Logging {
public:
	deque<char*> Items;
	void Render() {
		using namespace ImGui;
		static bool scroll = false;
		// Output Section
		const float footer_height_to_reserve = ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
		BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false); // Leave room for 1 separator + 1 InputText
		PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
		for (int i = 0; i < Items.size(); i++)
			TextWrapped(Items[i]); 
		if (scroll)
			ImGui::SetScrollHereY(1.0f); 
		scroll = 0; PopStyleVar(); EndChild();
	}; void Log(const char* fmt...) {
		va_list args; va_start(args, fmt);
		auto buffer = new char[256];
		vsprintf(buffer, fmt, args);
		// Fuck Non-Prints
		char* check = buffer;
		while (isprint(*check)) 
			check++; *check = 0;
		Items.push_back(buffer);
		if (Items.size() > 256)
			Items.pop_front();
	}; 
}; 

class LogWindow {
public:
	void Render() {
		if (!IsActive) return;
		using namespace ImGui;
		static ImGuiStyle& style = ImGui::GetStyle();
		SetNextWindowBgAlpha(0.85f);
		SetNextWindowSize(ImVec2(640, 440), ImGuiCond_Appearing);
		Begin("Log Window", &IsActive, ImGuiWindowFlags_NoCollapse);
		// left panel
		static Logging* selected = windows.begin()->second;
		BeginChild("left pane", ImVec2(145, 0), true);
		for (auto it = windows.begin(); it != windows.end(); ++it) {
			if (Selectable(it->first, selected == it->second))
				selected = it->second;
		}; EndChild(); SameLine(); if(selected) 
			selected->Render(); End();
	}; map<const char*, Logging*> windows = {
		{ "SpawnWindow", new Logging() },
		{ "SendAction", new Logging() },
		{ "GetIcon", new Logging() },
	}; bool IsActive;
};
// Macros
#define show(EXP) \
	User::log.windows[#EXP]->Log