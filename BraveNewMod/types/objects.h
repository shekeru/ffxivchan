#pragma once
#include "enums.h"

class Actor;
class JobGaugeManager;

class RecastDetail {
public:
	UINT Cooldown, ActionID;
	float ElapsedTime, TotalTime;
};

class ComboDetail {
public:
	float Timer;
	UINT LastAction;
    UINT IsQueued, _v9, NextSkill;
public:
	bool Is(int action, float margin = 0.f) {
		return LastAction == action && Timer >= margin;
	};
	char Set(char success, int next) {
		if (success || !next || !NextSkill)
			return success;
		NextSkill = next;
		return TRUE;
	};
};

class ActionSys {
public:
	// Local Data
	void* _VMT1;
	float Animation;
	char _pad[84];
	ComboDetail Combo;
public:
	int Paladin(int action);
	//int Monk(int action);
	int Warrior(int action);
	int Monk(int action);
	int Dragoon(int action);
	int Bard(int action);
	int WhiteMage(int action);
	int Astrologian(int action);
	int BlackMage(int action);
	int Summoner(int action);
	//int Scholar(int action);
	int Ninja(int action);
	int Machinist(int action);
	int DarkKnight(int action);
	int Samurai(int action);
	int RedMage(int action);
	int Dancer(int action);
public:

	RecastDetail* ActionRecast(int action, int flag = 1);

	float GetRecastTime(Action::ActionType type, Action::ActionID action) {
		return ActionSys::_GetRecastTime(this, type, action);
	}

	float static _GetRecastTime(ActionSys* self, Action::ActionType type, Action::ActionID action);
};