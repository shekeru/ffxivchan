#pragma once
#include "types.h"
#include "base.h"

typedef struct Aura {
	USHORT Type, Stacks;
	FLOAT Timer; UINT CastId;
} Aura;

enum ActorOffsets {
	ClassJob = 6388,
	JobLevel = 6390,
	AuraStatus = 6520,
};

class Actor {
public:
	virtual void _unkvf();
public:
	char pad[152];
	float X, Z, Y;
	int nil;
	float Spin;
public:
	UCHAR& JobId() {
		local value = (UCHAR*)
			(uintptr_t(this) + ClassJob);
		return *value;
	};
	UCHAR& Level() {
		local value = (UCHAR*)
			(uintptr_t(this) + JobLevel);
		return *value;
	};
	bool HasAura(int value, float margin = 0.f) {
		local Effects = (Aura*)
			(uintptr_t(this) + AuraStatus);
		for (int i = 0; i < 30; i++)
			if (Effects[i].Type == value)
				return Effects[i].Timer >= margin;
		return false;
	};
};

class ComboArea {
	float Timer; int 
		ActiveId, IsQueued, 
		_v9, NextSkill;
public:
	bool Is(int action, float margin = 0.f) {
		return ActiveId == action && 
			abs(Timer) >= margin;
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