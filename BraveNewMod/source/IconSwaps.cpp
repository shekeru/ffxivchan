#include "Actor.h"
#include "JobGauge.h"
#include "hooks.h"
#include "enums.h"

using namespace IconSwaps;
using namespace Action;
using namespace Job;

JobGauge* SharedGaugePtr;
JobGaugeManager* JobGaugeMgrPtr;
Job::JobID* CurrentJob;

GameObject* target;
GameObject::ObjectKind target_type;

UINT playerId;
BYTE lvl;

#define SwitchTo(Job) \
	action = self->Job(action); break;

#define UsingHUD(which) \
	which* HUD = reinterpret_cast<which*>(SharedGaugePtr);

#define effect \
	Globals::LocalActor->HasStatus

#define MACRO_REPOSE_RESCUE \
	case Repose: \
		if (target_type != GameObject::BattleNpc) \
			return Rescue; \
	break; \


UINT64 GetIcon_Test(ActionSys * self, UINT action) {

};

$set_original(RequestAction);
char RequestAction::Function(ActionSys* self, UINT actionType, UINT actionID,
	INT64 ptr4, INT a5, UINT a6, INT a7) {
	get_original;

	auto value = original(self, actionType, actionID, ptr4, a5, a6, a7);
	//printf("acq: %p, a5: %i, id: %i, a7: %i\n", self, a5, action, a7);
	//if (AllowRequestsGCD::Function(self, actionType, actionID))
		return self->Combo.Set(value, actionID);

	//return value;
};

$set_original(GetRecastIndex);
$set_original(GetRecastStruct);

RecastDetail* ActionSys::ActionRecast(int action, int flag){
	static auto _GetRecastIndex = decltype(&GetRecastIndex::Function)(GetRecastIndex::GetLocation());
	static auto _GetRecastStruct = decltype(&GetRecastStruct::Function)(GetRecastStruct::GetLocation());

	return _GetRecastStruct(this, _GetRecastIndex(this, flag, action));
};

$set_original(AllowRequestsGCD);
char AllowRequestsGCD::Function(ActionSys* self, UINT flag, UINT action) {
	get_original;

	auto result = original(self, flag, action);
	RecastDetail* recast = self->ActionRecast(action, flag);

	//printf("recast: %p\n", recast);

	if (!result && recast) {
		return (recast->TotalTime - recast->ElapsedTime) < 1.0f;
	}

	return result;
}

$set_original(GetIcon);
UINT64 GetIcon::Function(ActionSys* self, UINT action) {
	get_original; 
	
	if (JobGaugeMgrPtr == nullptr) {
		JobGaugeMgrPtr = Offsets::JobGaugePtr.Resolve(Context::Current().game);
		SharedGaugePtr = &JobGaugeMgrPtr->StaticGauge;
		CurrentJob = JobGaugeMgrPtr->GetJobPtr();
	}

	//if (rand() % 5000 == 0) {
	//	printf("ActionSys: %llx, Gauge: %llx, Job: %i\n", self, gauge, gauge->GetJobID());
	//}

	if (Globals::LocalActor) {
		lvl = Globals::LocalActor->Level();
		playerId = Globals::LocalActor->EntityID();
	}

	if (Globals::TargetSys) {
		target = Globals::TargetSys->GetCurrentTargetPtr();
		target_type = target ? target->ObjectType() : GameObject::None;
	}
	
	switch (*CurrentJob) {
		//case Gladiator:
		case Paladin:
			SwitchTo(Paladin);
		case Monk:
			SwitchTo(Monk);
		//case Lancer:
		case Dragoon:
			SwitchTo(Dragoon);
		case Bard:
			SwitchTo(Bard);
		//case Marauder:
		case Warrior:
			SwitchTo(Warrior);
		//case Conjurer:
		case White_Mage:
			SwitchTo(WhiteMage);
		case Astrologian:
			SwitchTo(Astrologian);
		case Black_Mage:
			SwitchTo(BlackMage);
		//case Arcanist:
		case Summoner:
			SwitchTo(Summoner);
		//case Rogue:
		case Ninja:
			SwitchTo(Ninja);
		case Machinist:
			SwitchTo(Machinist);
		case Dark_Knight:
			SwitchTo(DarkKnight);
		case Samurai:
			SwitchTo(Samurai);
		case Red_Mage:
			SwitchTo(RedMage);
		case Dancer:
			SwitchTo(Dancer);
	}; 
	
	return original(self, action);
};

#define BasicCombo(L, N, RS) \
	if (lvl >= L && Combo.Is(Action::##N) RS)

inline int ActionSys::Paladin(int action) {
	switch (action) {

	case Fast_Blade:
		if (lvl >= 4 && Combo.Is(Fast_Blade))
			return Riot_Blade;
		if (lvl >= 26 && Combo.Is(Riot_Blade))
			return Rage_of_Halon;
		break;

	case Total_Eclipse:
		if (lvl >= 40 && Combo.Is(Total_Eclipse))
			return Prominence;
		break;		

	};

	return action;
};

inline int ActionSys::Monk(int action) {

	switch (action) {

	case True_Strike:
		if (lvl >= 18 && effect(Status::Twin_Snakes, 2.5f))
			return Action::Twin_Snakes;
		break;

	};

	return action;
};

inline int ActionSys::Dragoon(int action) {

	switch (action) {
	// Single-Target
	case True_Thrust:
		BasicCombo(26, Vorpal_Thrust)
			return Full_Thrust;
		BasicCombo(50, Disembowel)
			return Chaos_Thrust;
	break;
	// AOE
	case Doom_Spike:
		BasicCombo(62, Doom_Spike)
			return Sonic_Thrust;
	break;

	};
	return action;
};

inline int ActionSys::Bard(int action) {

	switch (action) {
		// Brain Dead Rotation
	case Action::Heavy_Shot:
		if (lvl >= 2 && effect(Status::StraightShotReady))
			return Straight_Shot;
		break;

	};

	return action;
};

inline int ActionSys::Warrior(int action) {
	UsingHUD(WarriorGauge);

	switch (action) {
	// Single
	case Action::Heavy_Swing:
		BasicCombo(4, Heavy_Swing)
			return Maim;
		BasicCombo(26, Maim)
			return Storms_Path;
	break;
	// Multi
	case Action::Overpower:
		BasicCombo(40, Overpower)
			return Mythril_Tempest;
	break;
	// F1

	//case Inner_Beast:
	//	if (lvl >= 50 && HUD->BeastGauge < 50)
	//		return Infuriate;
	//break;

	};

	return action;
};

inline int ActionSys::WhiteMage(int action) {
	UsingHUD(WhiteMageGauge);
	auto active = (Actor*)target;

	switch (action) {
	MACRO_REPOSE_RESCUE;
	// Regen + Cure
	case Action::Cure:
		if (!active || (active->ObjectType() == GameObject::BattleNpc && active->IsMask(StatusFlags::Aggressive)))
			active = Globals::LocalActor;
		if (lvl >= 35 && !active->HasStatus(Status::Regen))
			return Regen; 
	break;
	// DoT Switch
	case Action::Stone:
		if (lvl >= 4 && target && target->ObjectType() == GameObject::BattleNpc) {
			if (active->HasStatus(Status::AeroII) || active->HasStatus(Status::Aero))
				return Stone;
			else
				return Aero;
		};
	break;
	// Cure II
	case Action::CureII:
		if (lvl >= 52 && HUD->Lily && !effect(Status::Freecure))
			return Afflatus_Solace; 
	break;
	// Medica
	case Action::Medica:
		if (lvl >= 50 && !effect(Status::MedicaII))
			return MedicaII;
	break;

	};

	return action;
};


inline int ActionSys::Astrologian(int action) {
	UsingHUD(AstrologianGauge);
	auto active = (Actor*)target;

	switch (action) {
	MACRO_REPOSE_RESCUE;
	// Regen + Cure
	case Action::Benefic:
		if (!active || active->ObjectType() == GameObject::BattleNpc)
			active = Globals::LocalActor;
		if (lvl >= 34 && !active->HasStatus(Status::Aspected_Benefic))
			return Aspected_Benefic;
	break;
	// DoT Switch
	case Action::Malefic:
		if (lvl >= 4 && target && target->ObjectType() == GameObject::BattleNpc) {
			if (active->HasStatus(Status::CombustII) || active->HasStatus(Status::Combust))
				return Malefic;
			else
				return Combust;
		};
	break;
	// Medica
	case Action::Helios:
		if (lvl >= 42 && !effect(Status::Aspected_Helios))
			return Aspected_Helios;
	break;
	// Cards
	case Action::Draw:
		if (HUD->Card)
			return Play;
	break;

	};
	return action;
};


inline int ActionSys::BlackMage(int action) {
	UsingHUD(BlackMageGauge);

	switch (action) {
	case Action::Blizzard:
		if (lvl >= 4 && HUD->Fire())
			return Transpose;
	break;

	case Action::Fire:
		if (lvl >= 4 && HUD->Ice())
			return Transpose;
		//if (lvl >= 34 && effect(Status::Firestarter))
		//	return Fire_III; 
	break;

	case Action::Freeze:
		if (lvl >= 50 && HUD->ElementStance >= 0)
			return Flare;
	break;
	};

	return action;

};

inline int ActionSys::Summoner(int action) {
	UsingHUD(SummonerGauge);

	switch (action) {
	// Single-Target
	case Fester:
		if (!(HUD->Flags & AetherFlags::Aetherflow))
			return Energy_Drain;
		break;
	case Painflare:
		if (!(HUD->Flags & AetherFlags::Aetherflow))
			return Energy_Syphon;
		break;
	};

	return action;

};

inline int ActionSys::Ninja(int action) {
	//static Actor*& LocalActor = Offsets::LocalActorPtr.Resolve(Context::Current().game);
	switch (action) {
	// Single-Target
	case Spinning_Edge:
		BasicCombo(4, Spinning_Edge)
			return Gust_Slash;
		BasicCombo(26, Gust_Slash)
			return Aeolian_Edge;
		break;
	// Multi-Target
	case Death_Blossom:
		BasicCombo(52, Death_Blossom)
			return Hakke_Mujinsatsu;
		break;
	};

	return action;
};


inline int ActionSys::Machinist(int action) {
	switch (action) {
	// Single-Target
	case Split_Shot:
		//if (Combo.Is(Split_Shot))
		//	return Slug_Shot;
		//if (Combo.Is(Slug_Shot))
		//	return Clean_Shot;
		break;
	// Multi-Target
	};

	return action;
};

inline int ActionSys::DarkKnight(int action) {
	switch (action) {
		// Single-Target
	case Hard_Slash:
		BasicCombo(4, Hard_Slash)
			return Syphon_Strike;
		BasicCombo(26, Syphon_Strike)
			return Souleater;
		break;
		// Multi-Target
	case Unleash:
		BasicCombo(40, Unleash)
			return Stalwart_Soul;
		break;
	};

	return action;

};

inline int ActionSys::Samurai(int action) {

	switch (action) {
	case Hakaze:
		BasicCombo(30, Jinpu)
			return Gekko;
		BasicCombo(40, Shifu)
			return Kasha;
		break;

	//case Fuga:
	//	BasicCombo(35, Fuga)
	//		return Mangetsu;
	//break;


	};
	return action;
};

inline int ActionSys::RedMage(int action) {
	UsingHUD(RedMageGauge);

	switch (action) {
	// Role Actions
	//case Action::Addle:
	//	if(target_type != GameObject::BattleNpc)
	//		return Vercure;
	//break;
	case Action::Sleep:
		if (target_type != GameObject::BattleNpc)
			return Verraise;
	break;
	// Meele-Combo
	case Action::Riposte:
		BasicCombo(35, Riposte)
			return Zwerchhau;
		BasicCombo(50, Zwerchhau)
			return Redoublement;
	break;
	// Slow Cast
	case Verthunder:
		if (lvl >= 10 && HUD->BlackMana > HUD->WhiteMana)
			return (lvl >= 68 && HUD->ManaStacks == 3 && lvl < 70) ? Verthunder : Veraero;
	break;
	// Quick-Cast
	case Jolt:
		if (lvl >= 26 && effect(Status::VerfireReady, 0.5f))
			return Verfire;
		if (lvl >= 30 && effect(Status::VerstoneReady, 0.5f))
			return Verstone;
	break;
	// Multi
	case Verthunder_II:
		if (lvl >= 22 && HUD->BlackMana > HUD->WhiteMana)
			return (lvl >= 68 && HUD->ManaStacks == 3 && lvl < 70) ? Verthunder_II : Veraero_II;
	break;
	};

	return action;
};


inline int ActionSys::Dancer(int action) {
	UsingHUD(DancerGauge);

	switch (action) {
	// Single Target
	case Cascade:
		if (lvl >= 20 && effect(Status::SilkenSymmetry))
			return Reverse_Cascade;
		if (lvl >= 40 && effect(Status::SilkenFlow))
			return Fountainfall;
		BasicCombo(2, Cascade)
			return Fountain;
		break;
	// AoE Rotation
	case Windmill:
		if (lvl >= 35 && effect(Status::SilkenSymmetry))
			return Rising_Windmill;
		if (lvl >= 45 && effect(Status::SilkenFlow))
			return Bloodshower;
		BasicCombo(25, Windmill)
			return Bladeshower;
		break;
	// Multi-Step Dance
	case Standard_Step:
	case Technical_Step:
		return HUD->NextStep(action);
	};

	return action;
};

$set_original(IsIconReplaceable);
char IsIconReplaceable::Function(UINT action) {
	return 1;
};

float ActionSys::_GetRecastTime(ActionSys* self, ActionType type, ActionID action) {
	local original = decltype(&ActionSys::_GetRecastTime)
		(Offsets::GetRecastTime.Resolve(Context::Current().game));
	return (self, type, action);
};

bool Actor::HasStatus(int value, float margin, UINT castSrc) {
	if (this == nullptr)
		return false;

	auto Effects = StatusManagerPtr()->AuraList; 

	if (!castSrc)
		castSrc = playerId;

	for (BYTE i = 0; i < 30; i++)
		if (Effects[i].Type == value && Effects[i].CastId == castSrc)
			return Effects[i].Timer >= margin || Effects[i].Timer < NULL;

	return false;
};