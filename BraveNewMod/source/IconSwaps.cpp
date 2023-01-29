#include "JobGauge.h"
#include "hooks.h"
#include "enums.h"

using namespace IconSwaps;
using namespace Action;
using namespace Job;

Job::JobID* CurrentJob;
JobGaugeManager* JobGaugeMgrPtr;
JobGauge* SharedGaugePtr;

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

	switch (*CurrentJob) {
		case Gladiator:
		case Paladin:
			SwitchTo(Paladin);
		case Lancer:
		case Dragoon:
			SwitchTo(Dragoon);
		case Conjurer:
		case White_Mage:
			SwitchTo(WhiteMage);
		case Rogue:
		case Ninja:
			SwitchTo(Ninja);
		case Dancer:
			SwitchTo(Dancer);
	}; 
	
	return original(self, action);
};

inline int ActionSys::Paladin(int action) {
	switch (action) {

	case Fast_Blade:
		if (Combo.Is(Fast_Blade))
			return Riot_Blade;
		if (Combo.Is(Riot_Blade))
			return Rage_of_Halon;
		break;

	case Total_Eclipse:
		if (Combo.Is(Total_Eclipse))
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

#define WHM_HUD (WhiteMageGauge)(StaticGaugePtr->LocalGauge);
inline int ActionSys::WhiteMage(int action) {

	switch (action) {


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