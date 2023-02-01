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
BYTE lvl;

#define SwitchTo(Job) \
	action = self->Job(action); break;

#define UsingHUD(which) \
	which* HUD = reinterpret_cast<which*>(SharedGaugePtr);

UINT64 GetIcon_Test(ActionSys * self, UINT action) {

};

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
	}

	if (Globals::TargetSys) {
		target = Globals::TargetSys->GetCurrentTargetPtr();
		target_type = target ? target->ObjectType() : GameObject::None;
	}
	
	switch (*CurrentJob) {
		case Gladiator:
		case Paladin:
			SwitchTo(Paladin);
		case Lancer:
		case Dragoon:
			SwitchTo(Dragoon);
		case Marauder:
		case Warrior:
			SwitchTo(Warrior);
		case Conjurer:
		case White_Mage:
			SwitchTo(WhiteMage);
		case Arcanist:
		case Summoner:
			SwitchTo(Summoner);
		case Rogue:
		case Ninja:
			SwitchTo(Ninja);
		case Machinist:
			SwitchTo(Machinist);
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

inline int ActionSys::Dragoon(int action) {

	switch (action) {

	case Doom_Spike:
		if (Combo.Is(Doom_Spike))
			return Sonic_Thrust;
		break;

	};

	return action;
};

inline int ActionSys::Warrior(int action) {
	UsingHUD(WarriorGauge);

	switch (action) {
	// SIngle
	case Action::Heavy_Swing:
		BasicCombo(4, Heavy_Swing)
			return Maim;
		BasicCombo(26, Maim)
			return Storms_Path;
	break;
	// Multi
	case Action::Overpower:
		BasicCombo(26, Overpower)
			return Mythril_Tempest;
	break;
	// F1

	case Inner_Beast:
		if (lvl >= 50 && HUD->BeastGauge < 50)
			return Infuriate;
	break;

	};

	return action;
};

#define WHM_HUD (WhiteMageGauge)(StaticGaugePtr->LocalGauge);
inline int ActionSys::WhiteMage(int action) {

	switch (action) {


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
		if (Combo.Is(Spinning_Edge))
			return Gust_Slash;
		if (Combo.Is(Gust_Slash))
			return Aeolian_Edge;
		break;
	// Multi-Target
	case Death_Blossom:
		if (Combo.Is(Death_Blossom))
			return Hakke_Mujinsatsu;
		break;
	};

	return action;

};


inline int ActionSys::Machinist(int action) {
	switch (action) {
	// Single-Target
	case Split_Shot:
		if (Combo.Is(Split_Shot))
			return Slug_Shot;
		if (Combo.Is(Slug_Shot))
			return Clean_Shot;
		break;
	// Multi-Target
	};

	return action;

};

inline int ActionSys::RedMage(int action) {
	UsingHUD(RedMageGauge);

	switch (action) {
	// Role Actions
	case Action::Addle:
		if(lvl >= 54 && target_type != GameObject::BattleNpc)
			return Vercure;
	break;
	case Action::Sleep:
		if (lvl >= 64 && target_type != GameObject::BattleNpc)
			return Verraise;
	break;
	// Meele-Combo
	case Action::Riposte:
		BasicCombo(35, Riposte)
			return Zwerchhau;
		BasicCombo(50, Zwerchhau)
			return Redoublement;
	break;
	case Verthunder:
		//if (lvl >= 10 && HUD->BlackMana)

	break;
	};

	return action;
};


inline int ActionSys::Dancer(int action) {
	UsingHUD(DancerGauge);

	switch (action) {
		// Single Target
	case Cascade:
		if (Combo.Is(Cascade))
			return Fountain;
		break;
		// AoE Rotation
	case Windmill:
		if (Combo.Is(Windmill))
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