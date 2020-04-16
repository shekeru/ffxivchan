#include "user.h"
#include "Quest.h"

void User2::QuestPanel() {
	using namespace ImGui;
	if (!sys.IsQuest) return;
	static char idBuffer[24] = {0};
	static auto quest = game.ScanPattern(Offsets::QUEST_2, 3).Cast<Quest*>();
	Begin("Tradecraft Info", &sys.IsQuest, ImGuiWindowFlags_NoCollapse);
	// Header
	Columns(4, "tradecrafts"); // 4-ways, with border
	Text("Slot"); NextColumn();
	Text("Id"); NextColumn();
	Text("Quest Name"); NextColumn();
	Text("Item Name"); NextColumn();
	Separator();
	// Body
	for (int i = 29; i < 45; i++) {
		if (quest[i].Id.value && quest[i].c_ptr) {
			sprintf(idBuffer, "%i", quest[i].Id.value);
			auto value = xiv->tradecraft.find(idBuffer);
			if (xiv->tradecraft.end() == value)
				continue;
			string item = (*value)["itemName"].get<string>();
			// Print Table
			Text(to_string(i).c_str()); NextColumn();
			Text(idBuffer); NextColumn();
			Text(quest[i].c_ptr); NextColumn();
			Text(item.c_str()); NextColumn();
		}
	}; Columns(1); End();
};

static char text[1024 * 16]{};
void User2::MusicPanel() {
	if (!sys.IsMusic) return; 
	static ImGuiStyle& style = ImGui::GetStyle();
	ImGui::SetNextWindowSize(ImVec2(640, 440), ImGuiCond_Appearing);
	ImGui::Begin("Music Panel", &sys.IsMusic, ImGuiWindowFlags_NoCollapse);
	// Output Section
	ImGui::InputTextMultiline("##music", text, IM_ARRAYSIZE(text));
	ImGui::SetItemDefaultFocus();
	if (ImGui::Button("Play Test")) {
		auto ptr = text;
		while (isprint(*ptr)) {
			if (isupper(*ptr)) {
				CallWindowProc(sys.prevProc, sys.hWindow, WM_KEYDOWN, *ptr, 0);
				CallWindowProc(sys.prevProc, sys.hWindow, WM_KEYUP, *ptr, 0);
			}
		}
	}
	ImGui::End();
};