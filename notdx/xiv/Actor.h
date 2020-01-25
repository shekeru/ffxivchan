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
	inline UCHAR JobId() {
		static UCHAR* ID = (UCHAR*)
			(uintptr_t(this) + ClassJob);
		return *ID;
	};
	inline UCHAR Level() {
		static UCHAR* value = (UCHAR*)
			(uintptr_t(this) + JobLevel);
		return *value;
	};
	inline bool HasAura(int value) {
		static Aura* Effects = (Aura*)
			(uintptr_t(this) + AuraStatus);
		for (int i = 0; i < 30; i++)
			if (Effects[i].Type == value)
				return true;
		return false;
	};
};

typedef struct Combo {
	int Timer, LastMove;
} Combo;

class JobGauge {
	void* vfunc_table_0;
	void* vfunc_talle_1;
};

class RDM_HUD: JobGauge {
public:
	UCHAR WhiteMana, BlackMana;
};