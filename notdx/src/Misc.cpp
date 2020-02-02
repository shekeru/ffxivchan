#include "user.h"
#include "Quest.h"

static int exIM_Overlay = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration
| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing
| ImGuiWindowFlags_AlwaysAutoResize;

#include <chrono>
double check_elapsed(int times) {
	using namespace chrono; static auto before = system_clock::now();
	auto elapsed = duration<double, milli>(system_clock
		::now() - before).count() * times > 1000;
	if (elapsed) 
		before = system_clock::now();
	return elapsed;
}
#define state(type, name, value) \
	static type name = value;

void User::SpinBotting() {
	state(int, times, 4);
	state(bool, last_dir, 0);
	state(bool, enabled, 0);
	state(float, degree, 2.4);
	if (!sys.IsSpin) {
		enabled = 0; return;
	} if (sys.IsOpen) {
		ImGui::Begin("SpinBot Settings",
			&sys.IsSpin, ImGuiWindowFlags_NoCollapse);
		ImGui::Checkbox("Enable", &enabled);
		ImGui::InputFloat("magnitude", &degree, 0.1f);
		ImGui::InputInt("turn/sec", &times, 1);
		ImGui::End();
	}; if (enabled && check_elapsed(times) && xiv->LocalActor)
		xiv->LocalActor->Spin = (last_dir ^= 1) ? degree : -degree;
};

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
		if (ImGui::BeginMenu("Lua 5.1.5")) {
			ImGui::MenuItem("Output Window", "", &sys.IsRepl);
			ImGui::MenuItem("Load New Script", "");
			ImGui::Separator();
			if (ImGui::MenuItem("_boot.lua")) {
				vm.DoFile("_boot.lua");
			}; ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Alpha Features")) {
			ImGui::MenuItem("Log Window", "", &log.IsActive);
			ImGui::MenuItem("Test, Quests", "", &sys.IsQuest);
			ImGui::MenuItem("SpinBot", "", &sys.IsSpin);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void User::NameOverlay() {
	using namespace ImGui;
	SetNextWindowBgAlpha(0.0f); static ImGuiIO& io = ImGui::GetIO();
	SetNextWindowPos(ImVec2(io.DisplaySize.x, 0.f), ImGuiCond_Always, ImVec2(1.0f, 0.0f));
	if (Begin("Testing Overlay", NULL, ImGuiWindowFlags_NoMove | exIM_Overlay)) 
		Text("  Re: VerL, 20.1.28_Bones  "); ImGui::End();
};

void User::QuestPanel() {
	using namespace ImGui;
	if (!sys.IsQuest) return;
	static char idBuffer[24] = {0};
	static auto quest = game->ScanPattern(Offsets::QUEST_2, 3).Cast<Quest*>();
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