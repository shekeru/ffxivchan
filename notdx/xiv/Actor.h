#pragma once
#include "types.h"

typedef struct Aura {
	USHORT Type;
	USHORT Stacks;
	FLOAT TimeLeft;
	UINT CastId;
} Aura;

enum ActorOffsets {
	Job = 6388,
	Level = 6390,
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
			(uintptr_t(this) + Job);
		return *ID;
	};
	inline UCHAR Level() {
		static UCHAR* N = (UCHAR*)
			(uintptr_t(this) + N);
		return *N;
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