#include "ZoomManager.h"
#include "user.h"

ZoomSys* Camera;
void UpdateTick::Detour(ZoomSys* self) {
	(Camera = self)->UpdateTick();
}

void User2::ZoomPanel() {
	using namespace ImGui; if (!sys.IsZoom) return; 
	Begin("Zoom Panel", &sys.IsZoom, ImGuiWindowFlags_NoCollapse);
	Text("Current Object: %p", Camera); if (Camera) {
		SliderFloat3("Zoom Fields", &Camera->Zoom.Current, 0.95, 24.5);
		SliderFloat3("Fov Fields", &Camera->Fov.Current, 0.12, 1.25);
	}; ImGui::End();
}