#pragma once
#include "Managers.h"
// Forwards
class Recast {
public:
	int Cooldown, ActionId;
	float Elapsed, Recast;
}; class ActionSys;
// Method: IsIconReplaceable
INSTALL(IsIconReplaceable, char, int)
	Location = game.GetLocation
		("81 f9 2e 01 00 00 7f 39 81 f9");
$ IsIconReplaceable;
// Method: GetIcon
INSTALL(GetIcon, INT64, ActionSys*, int)
	Location = game.ScanPattern
		("e8 ? ? ? ? 44 8b f0 8b d8", 1)
	.Cast<PVOID>();
$ GetIcon;
// Method: RequestAction
INSTALL(RequestAction, char, ActionSys*, UINT, UINT, INT64, INT, UINT, INT)
	Location = game.GetLocation
		("40 53 55 57 41 54 41 57 48 83 ec 60");
$ RequestAction;
// Method: AllowRequestsGCD
INSTALL(AllowRequestsGCD, char, ActionSys*, UINT, UINT)
	Location = game.ScanPattern
		("48 8b cf e8 ? ? ? ? 84 c0 74 31", 4)
	.Cast<PVOID>();
$ AllowRequestsGCD;
// Method: GetRecastIndex
INSTALL(GetRecastIndex, INT64, ActionSys*, UINT, UINT)
	Location = game.ScanPattern
		("0f 29 7c 24 20 e8 ? ? ? ? 8b d0", 6)
	.Cast<PVOID>();
$ GetRecastIndex;
// Method: GetRecastStruct
INSTALL(GetRecastStruct, Recast*, ActionSys*, UINT)
	Location = game.GetLocation
		("40 53 48 83 ec 20 48 63 da 85 d2 78");
$ GetRecastStruct;
// Main Class
class ActionSys {
// Local Data
	void* _VMT1;
	float Animation;
	char _pad[84];
	ComboArea Local;
public:
	int Marauder(int);
	int Gladiator(int);
	int Astrologian(int);
	int DarkKnight(int);
	int WhiteMage(int);
	int BlackMage(int);
	int Pugilist(int);
	int Dragoon(int);
	int Archer(int);
	int Rogue(int);
	int Samurai(int);
	int RedMage(int);
	int GunBreaker(int);
	int Dancer(int);
	int Summoner(int);
	int Scholar(int);
	int Machinist(int);
// Methods
	INT64 GetIcon(int action) {
		RESTORE(::GetIcon, action);
	}; 
	Recast* ActionRecast(int action, int flag = 1) {
		return GetRecastStruct.Call(this, GetRecastIndex
			.Call(this, flag, action));
	};
	bool CanCast(int action) {
		auto ptr = ActionRecast(action);
		return ptr && !ptr->Cooldown;
	}; // Detouring
	static void Attach() {
		printf("Attaching to ActionSys!!\n");
		::RequestAction.Attach();
		::AllowRequestsGCD.Attach();
		::IsIconReplaceable.Attach();
		::GetIcon.Attach();
	};
};