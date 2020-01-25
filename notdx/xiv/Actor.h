#pragma once
#include "types.h"

typedef struct Aura {
	USHORT Type;
	USHORT Stacks;
	FLOAT TimeLeft;
	UINT CastId;
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
	bool HasAura(int value) {
		local Effects = (Aura*)
			(uintptr_t(this) + AuraStatus);
		for (int i = 0; i < 30; i++)
			if (Effects[i].Type == value)
				return true;
		return false;
	};
};

class Combo {
	float Timer; int ActiveId;
public:
	bool Is(int action, float margin = 0.f) {
		return ActiveId == action 
			&& Timer >= margin;
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