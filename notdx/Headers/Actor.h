#pragma once
#include "types.h"
#include "vmt.h"

typedef struct Aura {
	USHORT Type, Stacks;
	FLOAT Timer; UINT CastId;
} Aura;

#define Attribute(TYPE, NAME, SIG, IX) \
	TYPE& NAME() { local offset = *game->GetLocation \
	(SIG, IX); return *(TYPE*)(uintptr_t(this) + offset); }

class Actor {
public:
	void* _vtable;
	char pad[152];
	float X, Z, Y;
	int nil;
	float Spin;
public:
	int& TargetID() {
		return *(int*)(uintptr_t(this) + 496);
	}
	int& EntityId() {
		return *(int*)(uintptr_t(this) + 116);
	}
	Attribute(UCHAR, ClassJob, "? ? ? ? 8b da 88 99 ? ? ? ? 48 8b f9", 0);
	Attribute(UCHAR, JobLevel, "38 88 ? ? ? ? 73 08 32 c0", 2);
	//Attribute(Aura*, AuraList, "89 81 ? ? ? ? 8b 89 ? ? 00 00 e8", 2);
	bool HasAura(int value, float margin = 0.f) {
		local offset = *game->GetLocation
			("89 81 ? ? ? ? 8b 89 ? ? 00 00 e8", 2);
		auto Effects = (Aura*)(uintptr_t(this) + offset);
		for (int i = 0; i < 30; i++)
			if (Effects[i].Type == value)
				return Effects[i].Timer >= margin
					|| Effects[i].Timer < NULL;
		return false;
	};
	// Target Testing
	Actor* TargetPtr();
};

class ComboArea {
	float Timer; int 
		ActiveId, IsQueued, 
		_v9, NextSkill;
public:
	bool Is(int action, float margin = 0.f) {
		return ActiveId == action && Timer >= margin;
	}; 
	char Set(char success, int next) {
		if (success || !next || !NextSkill)
			return success; NextSkill = next; 
		return TRUE;
	};
};

class JobGauge {
	void* vfunc_table_0;
	void* vfunc_talle_1;
};

class RDM_HUD: JobGauge {
public:
	UCHAR WhiteMana, BlackMana;
public:
	bool Has(int level) {
		return WhiteMana >= level && BlackMana >= level;
	};
};