#pragma once
#include "Managers.h"

class ActionSys;
// Method: IsIconReplaceable
INSTALL(IsIconReplaceable, char, int)
	Location = game->GetLocation("81 f9 2e 01 00 00 7f 39 81 f9");
$ IsIconReplaceable;
// Method: GetIcon
INSTALL(GetIcon, INT64, ActionSys*, int)
	Location = game->ScanPattern
		("E8 ? ? ? ? F6 DB 8B C8", 1)
	.Cast<PVOID>();
$ GetIcon;
// Method: RequestAction
INSTALL(RequestAction, char, ActionSys*, UINT, UINT, INT64, INT, UINT, INT)
	Location = game->GetLocation("40 53 55 57 41 54 41 57 48 83 ec 60");
$ RequestAction;
// Method: AllowRequestsGCD
INSTALL(AllowRequestsGCD, char, ActionSys*, UINT, UINT)
	Location = game->ScanPattern
		("48 8b cf e8 ? ? ? ? 84 c0 74 31", 4)
	.Cast<PVOID>();
$ AllowRequestsGCD;
// Main Class
class ActionSys {
// Local Data
	void* _VMT1;
	float Animation;
	char _pad[84];
	ComboArea Local;
public:
	int Gladiator(int);
	int Marauder(int);
	int Lancer(int);
	int Archer(int);
	int Rogue(int);
	int Samurai(int);
	int RedMage(int);
// Methods
	INT64 GetIcon(int action) {
		RESTORE(::GetIcon, action);
	}; 
// Detouring
	static void Attach() {
		printf("Attaching to ActionSys!!\n");
		::RequestAction.Attach();
		::AllowRequestsGCD.Attach();
		::IsIconReplaceable.Attach();
		::GetIcon.Attach();
	};
};