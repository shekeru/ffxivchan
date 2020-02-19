#pragma once
#include "Actor.h"
#include <detours.h>
// More Macros
#define INSTALL(NAME, type, ...) \
	class NAME { public: PVOID Location; \
		static type Detour(__VA_ARGS__);\
		decltype(&Detour) Call; \
		void Attach() { \

#define $ \
	Call = decltype(&Detour)(Location); \
	DetourAttach((void**) &Call, Detour); \
	} ;} inline

#define RESTORE(NAME, ...) \
	return NAME.Call(this, __VA_ARGS__)
// Manager Headers
#include "ActionSystem/ActionManager.h"
#include "UserSystem/UserManager.h"
// Game Pointers
class FFXIV {
public:
// Double Pointers
	Actor*& LocalActor = *game->ScanPattern
		("48 8b 05 ? ? ? ? 48 89 6c 24 60", 3)
	.Cast<Actor**>();
	UserSys*& UIManager = *game->ScanPattern
		("48 83 3D ? ? ? ? ? 8B 9E ? ? ? ?", 3, 1)
	.Cast<UserSys**>();
// Normal Pointers
	ComboArea* ComboSys = game->ScanPattern
		("48 89 2D ? ? ? ? 85 c0", 3)
	.Cast<ComboArea*>();
	JobGauge* JobHud = game->ScanPattern
		("48 8d 0d ? ? ? ? 0f 95 c2 e8", 3)
	.Cast<JobGauge*>();
// Move this
	FFXIV() {
		printf("LocalActor*: %p\n", &LocalActor);
	}; json tradecraft;
}; inline FFXIV* xiv;