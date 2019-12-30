#include "user.h"

static int exIM_Overlay = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration
| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing
| ImGuiWindowFlags_AlwaysAutoResize;

void User::MainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("eval(xiv)")) {
			ImGui::MenuItem("version 0.2", "", false, false);
			ImGui::MenuItem("ImGui Demo", "", &sys.IsDemo);
			ImGui::Separator(); ImGui::MenuItem("Options", "");
			if (ImGui::BeginMenu("Colors"))
			{
				float sz = ImGui::GetTextLineHeight();
				for (int i = 0; i < ImGuiCol_COUNT; i++)
				{
					const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
					ImVec2 p = ImGui::GetCursorScreenPos();
					ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), 
						ImGui::GetColorU32((ImGuiCol)i)); ImGui::Dummy(ImVec2(sz, sz));
					ImGui::SameLine(); ImGui::MenuItem(name);
				};  ImGui::EndMenu();
			};  ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Lua 5.1.4")) {
			ImGui::MenuItem("Output Window", "", &sys.IsRepl);
			ImGui::MenuItem("Load New Script", "");
			ImGui::Separator();
			if (ImGui::MenuItem("_boot.lua")) {
				vm.DoFile("_boot.lua");
			}
			ImGui::EndMenu();
		};  ImGui::EndMainMenuBar();
	}
}

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

ImVector<char*>       History;
int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
int TextEditCallback(ImGuiInputTextCallbackData* data);
void User::LuaConsole() {
	if (!sys.IsRepl) return;
	static bool scroll = false;
	static ImGuiStyle& style = ImGui::GetStyle(); 
	ImGui::SetNextWindowBgAlpha(0.85f);
	ImGui::SetNextWindowSize(ImVec2(640, 440), ImGuiCond_Appearing);
	ImGui::Begin("Lua Output [_boot.lua]", &sys.IsDemo, ImGuiWindowFlags_NoCollapse);
	// Output Section
	const float footer_height_to_reserve = 1.6 * ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false); // Leave room for 1 separator + 1 InputText
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
	for (int i = 0; i < vm.Items.size(); i++)
	{
		auto item = vm.Items[i];
		switch (item.std) {
		case 1:
			ImGui::PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_CheckMark]);
			break;
		case 2:
			ImGui::PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_PlotLinesHovered]);
			break;
		}; ImGui::TextWrapped(item.str.c_str());
			if (item.std) ImGui::PopStyleColor();
	}; if (scroll) ImGui::SetScrollHereY(1.0f); scroll = 0;
	ImGui::PopStyleVar(); ImGui::EndChild(); ImGui::Separator();
	// Input
	ImGui::PushItemWidth(-1.0f);
	if (ImGui::InputText("", InputBuf, IM_ARRAYSIZE(InputBuf), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		Strtrim(InputBuf);
		if (scroll = *InputBuf) {
			vm.Output(1, ">> "+string(InputBuf));
			vm.DoString(InputBuf);
		}; strcpy(InputBuf, "");
		ImGui::SetKeyboardFocusHere(-1);
	}; ImGui::PopItemWidth();
	// Auto-focus on window apparition
	ImGui::SetItemDefaultFocus();
	ImGui::End();
};

int TextEditCallback(ImGuiInputTextCallbackData* data)
{
	switch (data->EventFlag)
	{
	case ImGuiInputTextFlags_CallbackHistory:
	{
		// Example of HISTORY
		const int prev_history_pos = HistoryPos;
		if (data->EventKey == ImGuiKey_UpArrow)
		{
			if (HistoryPos == -1)
				HistoryPos = History.Size - 1;
			else if (HistoryPos > 0)
				HistoryPos--;
		}
		else if (data->EventKey == ImGuiKey_DownArrow)
		{
			if (HistoryPos != -1)
				if (++HistoryPos >= History.Size)
					HistoryPos = -1;
		}

		// A better implementation would preserve the data on the current input line along with cursor position.
		if (prev_history_pos != HistoryPos)
		{
			const char* history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
			data->DeleteChars(0, data->BufTextLen);
			data->InsertChars(0, history_str);
		}
	}
	}; return 0;
}