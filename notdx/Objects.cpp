#include "sdk.h"
#include "gui.h"

static int exIM_Overlay = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration
| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing
| ImGuiWindowFlags_AlwaysAutoResize;

void User::NameOverlay() {
	const float DISTANCE = 0.0f;
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 window_pos = ImVec2(io.DisplaySize.x - DISTANCE, DISTANCE);
	ImVec2 window_pos_pivot = ImVec2(1.0f, 0.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowBgAlpha(0.0f); // Transparent background
	if (ImGui::Begin("Testing Overlay", NULL, ImGuiWindowFlags_NoMove
		| exIM_Overlay)) ImGui::Text(" [Read, Eval, Print, Loop] "); ImGui::End();
};

void Strtrim(char* str) { 
	char* str_end = str + strlen(str); 
	while (str_end > str && str_end[-1] == ' ') 
		str_end--; *str_end = 0; 
}; char InputBuf[256];

void User::LuaConsole() {
	ImGui::SetNextWindowBgAlpha(0.95f);
	ImGui::SetNextWindowSize(ImVec2(640, 440));
	ImGui::Begin("Lua Console", NULL, ImGuiWindowFlags_NoCollapse);
	// Output
	const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar); // Leave room for 1 separator + 1 InputText
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
	for (int i = 0; i < sys.Items.size(); i++)
	{
		auto item = sys.Items[i];
		switch (item.std) {
		case 1:
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));
			break;
		case 2:
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
			break;
		}; ImGui::TextUnformatted(item.str.c_str()); 
			if (item.std) ImGui::PopStyleColor();
	} ImGui::PopStyleVar(); ImGui::EndChild(); ImGui::Separator();
	// Input
	bool reclaim_focus = false; ImGui::PushItemWidth(-1.0f);
	if (ImGui::InputText("", InputBuf, IM_ARRAYSIZE(InputBuf), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		Strtrim(InputBuf);
		if (*InputBuf) {
			sys.Console(1, "> "+string(InputBuf));
			SDK::LuaExec(InputBuf);
		}; strcpy(InputBuf, "");
	}; ImGui::PopItemWidth();
	// Auto-focus on window apparition
	ImGui::SetItemDefaultFocus();
	if (reclaim_focus)
		ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
	ImGui::End();
};