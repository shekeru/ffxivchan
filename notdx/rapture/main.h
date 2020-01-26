#pragma once
#include "user.h"

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

class Rapture;
// Method: IsIconReplaceable
INSTALL(IsIconReplaceable, char, int)
	Location = (PVOID) game->GetLocation
		("81 f9 2e 01 00 00 7f 39 81 f9");
$ IsIconReplaceable;
// Method: GetIcon
INSTALL(GetIcon, INT64, Rapture*, int)
	Location = game->ScanPattern
		("E8 ? ? ? ? F6 DB 8B C8", 1).Cast<PVOID>();
$ GetIcon;
// Method: RequestAction
INSTALL(RequestAction, char, Rapture*, UINT, UINT, INT64, INT, UINT, INT)
	Location = (PVOID) game->GetLocation
		("40 53 55 57 41 54 41 57 48 83 ec 60");
$ RequestAction;
// Method: AllowRequestsGCD
INSTALL(AllowRequestsGCD, char, Rapture*, UINT, UINT)
	Location = game->ScanPattern
		("48 8b cf e8 ? ? ? ? 84 c0 74 31", 4)
	.Cast<PVOID>();
$ AllowRequestsGCD;
// Main Class
class Rapture {
public:
	int Lancer(int action);
	int RedMage(int action);
// Methods
	INT64 GetIcon(int action) {
		RESTORE(::GetIcon, action);
	}; 
// Detouring
	static void Attach() {
		printf("Starting Rapture SubSystem!!\n");
		::RequestAction.Attach();
		::AllowRequestsGCD.Attach();
		::IsIconReplaceable.Attach();
		::GetIcon.Attach();
	}
};