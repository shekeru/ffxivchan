#pragma once

typedef struct Aura {
	USHORT Type;
	USHORT Stacks;
	FLOAT TimeLeft;
	UINT CastId;
} Aura;

enum ActorOffsets {
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
	inline bool HasAura(int value) {
		static Aura* Effects = (Aura*)
			(uintptr_t(this) + AuraStatus);
		for (int i = 0; i < 30; i++)
			if (Effects[i].Type == value)
				return true;
		return false;
	};
};