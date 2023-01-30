#pragma once
#include "Status.h"
#include "objects.h"
#include "enums.h"
#include "vmt.h"

#define FieldRef(TYPE, NAME, SIG, IX, T) \
	TYPE& NAME() { local offset = *game.GetLocation<T> \
	(SIG, IX); return *(TYPE*)(uintptr_t(this) + offset); }

#define FieldPtr(TYPE, NAME, SIG, IX, T) \
	TYPE* NAME() { local offset = *game.GetLocation<T> \
	(SIG, IX); return (TYPE*)(uintptr_t(this) + offset); }

#define GetField(TYPE, NAME, OFFSET) \
	TYPE NAME(){return *(TYPE*)(size_t(this) + OFFSET);}

class StatusManager {
public:
	Actor* OwnerPtr;
	Status AuraList[30];
};

class Actor {
public:
	void* _vtable;
public:
	GetField(UINT, CurrentHP, 0x1c4);
	GetField(BYTE, GetLevel, 0x1e1);

	int& TargetID() {
		return *(int*)(uintptr_t(this) + 496);
	}
	int& EntityID() {
		return *(int*)(uintptr_t(this) + 116);
	}
	// Status Manager: 0x1B60
	StatusManager* StatusManagerPtr() {
		return (StatusManager*)(size_t(this) + 0x1B60);
	}
	// Pointers
	//FieldPtr(Aura, AuraList, "89 81 ? ? ? ? 8b 89 ? ? 00 00 e8", 2);
	// Refrences
	// FieldRef(UINT, EntityID, "48 8b 0d ? ? ? ? 8b 51 ? 48 8b c8 e8", 9, UCHAR);
	// FieldRef(UINT, MaxHP, "45 8b 8e ? ? 00 00 49 8d", 3);
	// FieldRef(CombatFlags::Mask, CombatFlags, "f6 80 ? ? 00 00 02 74 18 ba", 2);
	// FieldRef(UCHAR, ClassJob, "? ? ? ? 8b da 88 99 ? ? ? ? 48 8b f9", 0);
	// FieldRef(UCHAR, JobLevel, "38 88 ? ? ? ? 73 08 32 c0", 2);
	// Target Testing
	//bool HasAura(int value, float margin = 0.f, UINT castSrc = 0); Actor* TargetPtr();
	// bool IsMask(CombatFlags::Mask mask) {
		//return CombatFlags() & mask;
	//};
	bool IsType(EntityType::Type type) {
		return type == *(EntityType::Type*)
			(uintptr_t(this) + 0x8c);
	};
};