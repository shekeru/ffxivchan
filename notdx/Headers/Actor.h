#pragma once
#include "types.h"
#include "vmt.h"

typedef struct Aura {
	USHORT Type, Stacks;
	FLOAT Timer; UINT CastId;
} Aura;

#define FieldRef(TYPE, NAME, SIG, IX) \
	TYPE& NAME() { local offset = *game.GetLocation \
	(SIG, IX); return *(TYPE*)(uintptr_t(this) + offset); }

#define FieldPtr(TYPE, NAME, SIG, IX) \
	TYPE* NAME() { local offset = *game.GetLocation \
	(SIG, IX); return (TYPE*)(uintptr_t(this) + offset); }

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
	// Pointers
	FieldPtr(Aura, AuraList, "89 81 ? ? ? ? 8b 89 ? ? 00 00 e8", 2);
	// Refrences
	FieldRef(UINT, MaxHP, "45 8b 8e ? ? 00 00 49 8d", 3);
	FieldRef(UINT, CurrentHP, "45 8b 86 ? ? 00 00 49 8b cd e8", 3);
	FieldRef(CombatFlags::Mask, CombatFlags, "f6 80 ? ? 00 00 02 74 18 ba", 2);
	FieldRef(UCHAR, ClassJob, "? ? ? ? 8b da 88 99 ? ? ? ? 48 8b f9", 0);
	FieldRef(UCHAR, JobLevel, "38 88 ? ? ? ? 73 08 32 c0", 2);
	// Target Testing
	bool HasAura(int value, float margin = 0.f, UINT castSrc = 0); Actor* TargetPtr();
	bool IsType(EntityType::Type type) {
		return type == *(EntityType::Type*)
			(uintptr_t(this) + 0x8c);
	};
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

class WAR_HUD : JobGauge {
public:
	UCHAR BeastGauge;
public:
	bool Has(int level) {
		return BeastGauge >= level;
	};
};

class WHM_HUD : JobGauge {
public:
	USHORT _UNK;
	USHORT Timer;
	UCHAR Lily;
};

class BLM_HUD : JobGauge {
public:
	USHORT _UNK;
	USHORT Timer;
	CHAR Stacks; // Pos: Fire, Neg: Ice
public:
	CHAR Fire() {
		return max(0, Stacks);
	};
	CHAR Ice() {
		return -1 * min(0, Stacks);
	};
};

class RDM_HUD: JobGauge {
public:
	UCHAR WhiteMana, BlackMana;
public:
	bool Has(int level) {
		return WhiteMana >= level && BlackMana >= level;
	};
};

class GNB_HUD : JobGauge {
public:
	USHORT Charges, 
		Style, Phase;
};

class DNC_HUD : JobGauge {
public:
	USHORT Feathers;
	UCHAR Steps[4], Idx;
public:
	int Forwards(int dance) {
		UCHAR phase = Steps[Idx]; 
		if (phase && dance & 1)
			phase += 1;
		return dance + phase;
	}
};