#include "user.h"
#include "../Actor.h"
#include "../Quest.h"

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
		if (ImGui::BeginMenu("Lua 5.1.4")) {
			ImGui::MenuItem("Output Window", "", &sys.IsRepl);
			ImGui::MenuItem("Load New Script", "");
			ImGui::Separator();
			if (ImGui::MenuItem("_boot.lua")) {
				vm.DoFile("_boot.lua");
			}; ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Misc")) {
			ImGui::MenuItem("SpinBot", "", &sys.IsSpin);
			if (ImGui::MenuItem("Test, Quests", "")) {
				auto location = game->ScanPattern(Offsets::QUEST_2, 3).Cast<Quest*>();
				for (int i = 29; i < 45; i++) {
					auto quest = location[i];
					if (quest.Id.value && quest.c_ptr) {
						const char* str_id = to_string(quest.Id.value).c_str();
						printf("Slot #%i, %s: %s\n", i, str_id, quest.c_ptr);
							json sel = xiv->tradecraft[str_id];
						printf("yield, Needs: %s\n", sel["itemName"]);
					}
					else
						printf("Empty Quest Slot #%i\n", i);
				}
			}; ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
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