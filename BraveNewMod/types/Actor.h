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

class GameObject {
	static enum ObjectKind : byte;
public:
	virtual void _destroy();
	virtual UINT GetEntityID();
public:
	GetField(UINT, EntityID, 0x74); // EntityId
	GetField(USHORT, ObjectIndex, 0x88);
	GetField(ObjectKind, ObjectType, 0x8C);
	GetField(BYTE, ObjectSubtype, 0x8D);
public:
	static enum ObjectKind : byte
	{
		None = 0,
		Player = 1,
		BattleNpc = 2,
		EventNpc = 3,
		Treasure = 4,
		Aetheryte = 5,
		GatheringPoint = 6,
		EventObj = 7,
		Mount = 8,
		Companion = 9,
		Retainer = 10,
		AreaObject = 11,
		HousingEventObject = 12,
		Cutscene = 13,
		CardStand = 14,
		Ornament = 15
	};
};

class Character : public GameObject {
public:
	GetField(UINT, CurrentHP, 0x1c4);
	GetField(Job::JobID, JobID, 0x1e0);
	GetField(BYTE, Level, 0x1e1);
};

class Companion : public Character {
public:

};

class Actor : public Character {
public:
	Character Char;
public:

public:
	UINT GetTargetID();

	//int& TargetID() {
	//	return *(int*)(uintptr_t(this) + 496);
	//}
	//int& EntityID() {
	//	return *(int*)(uintptr_t(this) + 116);
	//}
	// Status Manager: 0x1B60
	StatusManager* StatusManagerPtr() {
		return (StatusManager*)(size_t(this) + 0x1B60);
	}
//	[FieldOffset(0x1CD0)] public Character.CastInfo SpellCastInfo;
// 
	// Pointers
	//FieldPtr(Aura, AuraList, "89 81 ? ? ? ? 8b 89 ? ? 00 00 e8", 2);
	// Refrences
	// FieldRef(UINT, EntityID, "48 8b 0d ? ? ? ? 8b 51 ? 48 8b c8 e8", 9, UCHAR);
	// FieldRef(UINT, MaxHP, "45 8b 8e ? ? 00 00 49 8d", 3);
	// FieldRef(CombatFlags::Mask, CombatFlags, "f6 80 ? ? 00 00 02 74 18 ba", 2);
	// FieldRef(UCHAR, ClassJob, "? ? ? ? 8b da 88 99 ? ? ? ? 48 8b f9", 0);
	// FieldRef(UCHAR, JobLevel, "38 88 ? ? ? ? 73 08 32 c0", 2);
	// Target Testing
	bool HasStatus(int value, float margin = 0.4f, UINT castSrc = 0); 
	//Actor* TargetPtr();
	// bool IsMask(CombatFlags::Mask mask) {
		//return CombatFlags() & mask;
	//};
	bool IsType(EntityType::Type type) {
		return type == *(EntityType::Type*)
			(uintptr_t(this) + 0x8c);
	};
};

class CharacterManager {
public:
	Character* CharacterMap[100];
	Actor*     BattleChar;
	::Companion* CompanionPet;
	UINT       CompanionClassSize, 
		       UpdateIndex;
};

class GameObjectManager {
	UINT _UPDATE_IDX, _IS_ACTIVE;
	ULONG _UNK_1, _UNK_2;
	GameObject* ObjectList[200];
};

class TargetSystem {
public:
	GetField(GameObject*, Target, 0x80);
	GetField(GameObject*, SoftTarget, 0x88);
public:
	GameObject* GetCurrentTargetPtr();
};