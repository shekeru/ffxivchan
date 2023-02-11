#pragma once
#include "enums.h"

// Reference: https://github.com/aers/FFXIVClientStructs/blob/main/FFXIVClientStructs/FFXIV/Client/Game/Gauge/JobGauges.cs
class JobGauge {
	void* vfunc_table_0;
public:
	Job::JobID GetJob() {
		return *(Job::JobID*)(size_t(this) + 0x50);
	};
};

// Better termed as the Global PTR & Gague
class JobGaugeManager {
public:
	JobGauge* _UnsafePtr;
	JobGauge  StaticGauge;
public:
	Job::JobID* GetJobPtr() {
		return (Job::JobID*)(size_t(this) + 0x58);
	};
	Job::JobID& GetJobRef() {
		return *(Job::JobID*)(size_t(this) + 0x58);
	};
};

// Tanks
class PaladinGauge : JobGauge {
public:
	BYTE OauthGauge;
public:
	bool Has(int level) {
		return OauthGauge >= level;
	};
};

class WarriorGauge : JobGauge {
public:
	BYTE BeastGauge;
public:
	bool Has(int level) {
		return BeastGauge >= level;
	};
};

class DarkKnight : JobGauge {
public:
	USHORT BloodGauge,
		   DarksideTimer,
		   DarkArtsState,
		   ShadowTimer;
};

class GunbreakerGauge : JobGauge {
public:
	BYTE Charges, _UNK;
	//USHORT MaxTimerDuration, P
	USHORT Style, Phase;
};

// Physical

class DancerGauge : JobGauge {
public:
	BYTE Feathers, Espirit;
	UCHAR Steps[4], Index;
public:
	int NextStep(int dance) {
		UCHAR phase = Steps[Index];
		if (phase && dance & 1)
			phase += 1;
		return dance + (Index < 4 ? phase : 0);
	}
};

class MCH_HUD : JobGauge {
public:
	USHORT OverHeated, _UNK;
	UCHAR HeatGauge;
};

// Healers

class WhiteMageGauge : JobGauge {
public:
	USHORT _UNK_PAD; // ??
	SHORT LilyTimer; // milliseconds
	BYTE  Lily,
		  BloodLily;
};


class AstrologianGauge : JobGauge {
public:
	SHORT Timer, _UNK_1;  // 0x8
	UCHAR _UNK_2, Card, Seals;
	//bool InSet() {
	//	auto Option = Aspect();
	//	for (char i = 0; i < 3; i++) {
	//		if (i[Seal] == Option)
	//			return true;
	//	}; return false;
	//}
	//UCHAR Aspect() {
	//	switch (Card)
	//	{
	//	case 1:
	//	case 2:
	//		return 1;
	//	case 3:
	//	case 5:
	//		return 2;
	//	case 4:
	//	case 6:
	//		return 3;
	//	default:
	//		return -1;
	//	};
	//}
};

// Casters

class RedMageGauge : JobGauge {
public:
	UCHAR WhiteMana, BlackMana, ManaStacks;
public:
	bool Has(int level) {
		return WhiteMana >= level && BlackMana >= level;
	};
};

class BlackMageGauge : JobGauge {
public:
	USHORT EnochianTimer,
		   ElementTimeRemaining;
	CHAR   ElementStance;
	UCHAR  UmbralHearts,
		   PolyglotStacks;
public:
	UCHAR Fire() {
		return max(0, ElementStance);
	};
	UCHAR Ice() {
		return -1 * min(0, ElementStance);
	};
};

namespace AetherFlags {
	enum AetherFlags : BYTE
	{
		None = 0,
		Aetherflow1 = 1 << 0,
		Aetherflow2 = 1 << 1,
		Aetherflow = Aetherflow1 | Aetherflow2,
		IfritAttuned = 1 << 2,
		TitanAttuned = 1 << 3,
		GarudaAttuned = TitanAttuned | IfritAttuned,
		PhoenixReady = 1 << 4,
		IfritReady = 1 << 5,
		TitanReady = 1 << 6,
		GarudaReady = 1 << 7
	};
};

class SummonerGauge : JobGauge {
public:
	USHORT SummonTimer, AttunementTimer;
	BYTE SummonID, SummonGlamID; 
	BYTE AttunementCount;
	AetherFlags::AetherFlags Flags;

		//Aetherflow;
};