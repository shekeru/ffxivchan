#include "context.h"

static int exIM_Overlay = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration
| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing
| ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar;

void NameOverlay() {
	using namespace ImGui; static ImGuiIO& io = GetIO();
	PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(6.f, 1.f));
	SetNextWindowPos(ImVec2(io.DisplaySize.x, 0.f),
		ImGuiCond_Always, ImVec2(1.0f, 0.0f)); SetNextWindowBgAlpha(0.0f);
	Begin("test overlay", NULL, ImGuiWindowFlags_NoMove | exIM_Overlay);
	Text("Re; VerL, Patch 6.31, Jan 26"); End(); PopStyleVar();
};

void IM_Interface::MainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Re; VerL")) {
			ImGui::MenuItem("ImGui Demo", "", &ShowDemo);
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
		//if (ImGui::BeginMenu("Lua 5.1.5")) {
		//	ImGui::MenuItem("Output Window", "", &sys.IsRepl);
		//	ImGui::MenuItem("Load New Script", "");
		//	ImGui::Separator();
		//	if (ImGui::MenuItem("_boot.lua")) {
		//		vm.DoFile("_boot.lua");
		//	}; ImGui::EndMenu();
		//}
		if (ImGui::BeginMenu("Stable Features")) {
			// ImGui::MenuItem("Log Window", "", &User.log.IsOpen);
			// ImGui::MenuItem("Test, Quests", "", &sys.IsQuest);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Alpha Features")) {
			// ImGui::MenuItem("Zoom Settings", "", &sys.IsZoom);
			// ImGui::MenuItem("Music Panel", "", &sys.IsMusic);
			// ImGui::MenuItem("SpinBot", "", &sys.IsSpin);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
};

void IM_Interface::Render() {
	get_context;
	static byte nil = printf("IM_Interface::Render() called!\n");

	if (IsOpen) {
		//log.Render(); 
		//if (ShowDemo)
			//ImGui::ShowDemoWindow(&ShowDemo);
		MainMenuBar(); 
		//User2::LuaConsole(); 
		//User2::QuestPanel(); 
		//User2::ZoomPanel();
	} else NameOverlay();
};