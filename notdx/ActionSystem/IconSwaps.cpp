#include "ActionManager.h"

Actor* Actor::TargetPtr() {
	int& target = TargetID(); if (target)
	for (int i = 0; i <= 423; i++) {
		auto ptr = xiv->CharMap[i];
		if (ptr && target == ptr->EntityId())
				return ptr;
	}; return NULL;
}

#define effect \
	xiv->LocalActor->HasAura

INT64 GetIcon::Detour(ActionSys* self, int action) {
	if (xiv->LocalActor) switch (xiv->LocalActor->ClassJob()) {
		case Job::Paladin:
		case Job::Gladiator:
			return self->Gladiator(action);
		case Job::Warrior:
		case Job::Marauder:
			return self->Marauder(action);
		case Job::Dragoon:
		case Job::Lancer:
			return self->Lancer(action);
		case Job::Archer:
			return self->Archer(action);
		case Job::Rogue:
			return self->Rogue(action);
		case Job::Samurai:
			return self->Samurai(action);
		case Job::Red_Mage:
			return self->RedMage(action);
	}; return self->GetIcon(action);
};

int ActionSys::Gladiator(int action) {
	local Combo = xiv->ComboSys;
	local &lvl = xiv->LocalActor->JobLevel();
	// Pretty Simple Combo Checking
	switch (action) {
	case Action::Fast_Blade:
		if (lvl >= 4 && Combo->Is(Action::Fast_Blade))
			return GetIcon(Action::Riot_Blade);
		if (lvl >= 26 && Combo->Is(Action::Riot_Blade))
			return GetIcon(Action::Rage_of_Halon);
		break;
	}; return GetIcon(action);
};


int ActionSys::Marauder(int action) {
	local Combo = xiv->ComboSys;
	local &lvl = xiv->LocalActor->JobLevel();
	// Pretty Simple Combo Checking
	switch (action) {
		case Action::Heavy_Swing:
			if (lvl >= 4 && Combo->Is(Action::Heavy_Swing))
				return GetIcon(Action::Maim);
			if(lvl >= 26 && Combo->Is(Action::Maim))
				return GetIcon(Action::Storms_Path);
			break;
	}; return GetIcon(action);
};

int ActionSys::Lancer(int action) {
	local Combo = xiv->ComboSys;
	local &lvl = xiv->LocalActor->JobLevel();
	// Pretty Simple Combo Checking
	switch (action) {
		// Buffing Damage
		case Action::Disembowel:
			if (lvl >= 50 && Combo->Is(Action::Disembowel))
				return GetIcon(Action::Chaos_Thrust);
			return GetIcon(Action::Disembowel);
		case Action::True_Thrust:
			// Brain Dead Rotation
			if (lvl >= 4 && Combo->Is(Action::True_Thrust)) {
				if (lvl >= 18 && !effect(Status::Disembowel, 7.5f))
					return GetIcon(Action::Disembowel);
				return GetIcon(Action::Vorpal_Thrust);
			}; if (lvl >= 26 && Combo->Is(Action::Vorpal_Thrust))
				return GetIcon(Action::Full_Thrust);
			if (lvl >= 50 && Combo->Is(Action::Disembowel))
				return GetIcon(Action::Chaos_Thrust);
			return GetIcon(Action::True_Thrust);
	};  return GetIcon(action);
};


int ActionSys::Archer(int action) {
	local &lvl = xiv->LocalActor->JobLevel();
	auto target = xiv->LocalActor->TargetPtr();
	// Pretty Simple Combo Checking
	switch (action) {
	case Action::Heavy_Shot:
		// Brain Dead Rotation
		if (lvl >= 2 && effect(Status::StraightShotReady))
			return GetIcon(Action::Straight_Shot);
		if (lvl >= 6 && target) {
			if (!target->HasAura(Status::VenomousBite))
				return GetIcon(Action::Venomous_Bite);
		}; break;
	}; return GetIcon(action);
};

int ActionSys::Rogue(int action) {
	local Combo = xiv->ComboSys;
	local &lvl = xiv->LocalActor->JobLevel();
	// Pretty Simple Combo Checking
	switch (action) {
	case Action::Spinning_Edge:
		if (lvl >= 4 && Combo->Is(Action::Spinning_Edge))
			return GetIcon(Action::Gust_Slash);
		return GetIcon(Action::Spinning_Edge);
	}; return GetIcon(action);
};

int ActionSys::Samurai(int action) {
	local Combo = xiv->ComboSys;
	//local HUD = (SAM_HUD*)xiv->JobHud;
	local &lvl = xiv->LocalActor->JobLevel();
	// Instant-Cast/Non-Instant Switches, Harder
	switch (action) {
	case Action::Gekko:
		if (lvl >= 4 && Combo->Is(Action::Hakaze))
			return GetIcon(Action::Jinpu);
		if (lvl >= 30 && Combo->Is(Action::Jinpu))
			return GetIcon(Action::Gekko);
		return GetIcon(Action::Hakaze);
	case Action::Kasha:
		if (lvl >= 18 && Combo->Is(Action::Hakaze))
			return GetIcon(Action::Shifu);
		if (lvl >= 40 && Combo->Is(Action::Shifu))
			return GetIcon(Action::Kasha);
		return GetIcon(Action::Hakaze);
	case Action::Yukikaze:
		return GetIcon(Combo->Is(Action::Hakaze) 
			? Action::Yukikaze : Action::Hakaze);
	}; return GetIcon(action);
};


int ActionSys::RedMage(int action) {
	local Combo = xiv->ComboSys;
	local HUD = (RDM_HUD*)xiv->JobHud;
	local &lvl = xiv->LocalActor->JobLevel();
	using namespace Status; using namespace Action;
	// Instant-Cast/Non-Instant Switches, Harder
	if (effect(Dualcast) || effect(Swiftcast)) switch (action) {
	case Jolt: 
	// White Mana Need (Veraero)
		if (lvl >= 10 && !effect(VerstoneReady)) {
			if (effect(VerfireReady) || 
				HUD->WhiteMana <= HUD->BlackMana)
				return GetIcon(Veraero);
		};
	// Black Mana Need (Verthunder)
		if (lvl >= 4 && !effect(VerfireReady)) {
			return GetIcon(Verthunder);
		}; 
	// Otherwise, Nothing
		return GetIcon(Jolt);
	} switch (action) {
	// Check for Proc Skills on Jolt, Easy
	case Jolt:
		if (effect(VerstoneReady))
			return GetIcon(Verstone);
		if (effect(VerfireReady))
			return GetIcon(Verfire);
		return GetIcon(Jolt);
		// Flip AoE, based on mana needs
	case Verthunder_II:
		return (lvl >= 22 && HUD->WhiteMana <= HUD->BlackMana)
			? GetIcon(Veraero_II) : GetIcon(Verthunder_II);
		// Melee Section
	case Riposte:
		if (lvl >= 35 && Combo->Is(Riposte))
			return GetIcon(Zwerchhau);
		if (lvl >= 50 && Combo->Is(Zwerchhau))
			return GetIcon(Redoublement);
	}; return GetIcon(action);
};

char IsIconReplaceable::Detour(int action) {
	return 1;
};