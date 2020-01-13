#include "user.h"
#include "../Actor.h"

static int exIM_Overlay = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration
| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing
| ImGuiWindowFlags_AlwaysAutoResize;

float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}; bool spinToWin = 0;

void User::MainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("eval(xiv)")) {
			ImGui::MenuItem("version 0.2", "", false, false);
			ImGui::MenuItem("ImGui Demo", "", &sys.IsDemo);
			ImGui::MenuItem("SpinToWin", "", &spinToWin);
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
		}; ImGui::EndMainMenuBar();
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
	if (spinToWin) {
		static Actor*& LocalActor = *game->ScanPattern
		(Offsets::LOCAL_ACTOR, 3).Cast<Actor**>();
		LocalActor->spin = RandomFloat(-2.45, 2.45);
	};
};