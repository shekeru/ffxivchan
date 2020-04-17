#pragma once
#include "types.h"
#include "vmt.h"

typedef struct Aura {
	USHORT Type, Stacks;
	FLOAT Timer; UINT CastId;
} Aura;

#define FieldRef(TYPE, NAME, SIG, IX, T) \
	TYPE& NAME() { local offset = *game.GetLocation<T> \
	(SIG, IX); return *(TYPE*)(uintptr_t(this) + offset); }

#define FieldPtr(TYPE, NAME, SIG, IX, T) \
	TYPE* NAME() { local offset = *game.GetLocation<T> \
	(SIG, IX); return (TYPE*)(uintptr_t(this) + offset); }

class Actor {
public:
	void* _vtable;
public:
	int& TargetID() {
		return *(int*)(uintptr_t(this) + 496);
	}
	//int& EntityID() {
	//	return *(int*)(uintptr_t(this) + 116);
	//}
	// Pointers
	FieldPtr(Aura, AuraList, "89 81 ? ? ? ? 8b 89 ? ? 00 00 e8", 2);
	// Refrences
	FieldRef(UINT, EntityID, "74 19 8b 50 ? 48 8d 0d", 4, UCHAR);
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

class AST_HUD : JobGauge {
public:
	int _unk; UCHAR 
		Card, Seal[3];
	bool InSet() {
		auto Option = Aspect();
		for (char i = 0; i < 3; i++) {
			if (i[Seal] == Option)
				return true;
		}; return false;
	}
	UCHAR Aspect() {
		switch (Card)
		{
		case 1:
		case 2:
			return 1;
		case 3:
		case 5:
			return 2;
		case 4:
		case 6:
			return 3;
		default:
			return -1;
		};
	}
};