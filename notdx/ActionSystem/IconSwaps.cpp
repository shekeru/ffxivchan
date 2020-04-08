#include "ActionManager.h"

Actor* Actor::TargetPtr() {
	int& target = TargetID(); if (target)
	for (int i = 0; i <= 423; i++) {
		auto ptr = xiv->CharMap[i];
		if (ptr && target == ptr->EntityId())
				return ptr;
	}; return NULL;
}; Actor* target; UCHAR lvl;

#define effect \
	xiv->LocalActor->HasAura
#define Combo \
	xiv->ComboSys

INT64 GetIcon::Detour(ActionSys* self, int action) {
	if (xiv->LocalActor) {
		lvl = xiv->LocalActor->JobLevel();
	switch (xiv->LocalActor->ClassJob()) {
		case Job::Conjurer:
		case Job::White_Mage:
			return self->WhiteMage(action);
		case Job::Black_Mage:
		case Job::Thaumaturge:
			return self->BlackMage(action);
		case Job::Paladin:
		case Job::Gladiator:
			return self->Gladiator(action);
		case Job::Warrior:
		case Job::Marauder:
			return self->Marauder(action);
		case Job::Astrologian:
			return self->Astrologian(action);
		case Job::Dark_Knight:
			return self->DarkKnight(action);
		case Job::Pugilist:
			return self->Pugilist(action);
		case Job::Dragoon:
		case Job::Lancer:
			return self->Dragoon(action);
		case Job::Bard:
		case Job::Archer:
			return self->Archer(action);
		case Job::Rogue:
			return self->Rogue(action);
		case Job::Samurai:
			return self->Samurai(action);
		case Job::Red_Mage:
			return self->RedMage(action);
		case Job::Gunbreaker:
			return self->GunBreaker(action);
		case Job::Dancer:
			return self->Dancer(action);
		}
	}; return self->GetIcon(action);
};

int ActionSys::Gladiator(int action) {
	// Pretty Simple Combo Checking
	switch (action) {
	case Action::Fast_Blade:
		if (lvl >= 4 && Combo->Is(Action::Fast_Blade))
			return GetIcon(Action::Riot_Blade);
		if (lvl >= 26 && Combo->Is(Action::Riot_Blade))
			return GetIcon(Action::Rage_of_Halon);
		break;
	case Action::Total_Eclipse:
		if (lvl >= 40 && Combo->Is(Action::Total_Eclipse))
			return GetIcon(Action::Prominence); break;
	}; return GetIcon(action);
};


int ActionSys::Marauder(int action) {
	local HUD = (WAR_HUD*)xiv->JobHud;
	// Pretty Simple Combo Checking
	switch (action) {
		case Action::Heavy_Swing:
			if (lvl >= 4 && Combo->Is(Action::Heavy_Swing))
				return GetIcon(Action::Maim);
			if (lvl >= 26 && Combo->Is(Action::Maim))
				return GetIcon(lvl >= 50 && !effect(Status::Storms_Eye, 2.5f) 
					? Action::Storms_Eye : Action::Storms_Path);
			if (lvl >= 35 && HUD->Has(50))
				return GetIcon(Action::Inner_Beast);
			return GetIcon(Action::Heavy_Swing);
		case Action::Overpower:
			if (lvl >= 40 && Combo->Is(Action::Overpower))
				return GetIcon(Action::Mythril_Tempest);
			if (lvl >= 45 && HUD->Has(50))
				return GetIcon(Action::Steel_Cyclone);
			return GetIcon(Action::Overpower);
	}; return GetIcon(action);
};

int ActionSys::DarkKnight(int action) {
	// Pretty Simple Combo Checking
	switch (action) {
	case Action::Hard_Slash:
		if (lvl >= 2 && Combo->Is(Action::Hard_Slash))
			return GetIcon(Action::Syphon_Strike);
		if (lvl >= 26 && Combo->Is(Action::Syphon_Strike))
			return GetIcon(Action::Souleater);
		break;
	}; return GetIcon(action);
};

int ActionSys::Pugilist(int action) {
	switch (action) {
	case Action::Bootshine:
		if (lvl >= 4 && effect(Status::Raptor_Form)) {
			if (lvl >= 18 && !effect(Status::Twin_Snakes, 1.0f))
				return GetIcon(Action::Twin_Snakes);
			return GetIcon(Action::True_Strike);
		} if (lvl >= 6 && effect(Status::Coeurl_Form))
			return GetIcon(Action::Snap_Punch);
		return GetIcon(Action::Bootshine);
	}; return GetIcon(action);
};

int ActionSys::Dragoon(int action) {
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

int ActionSys::Astrologian(int action) {
	target = xiv->LocalActor->TargetPtr();
	switch (action) {
	case Action::Benefic:
		if (!target || target->IsType(EntityType::Monster))
			target = xiv->LocalActor;
		if (lvl >= 34 && effect(Status::Dinural_Sect)
			&& !target->HasAura(Status::Aspected_Benefic))
			return GetIcon(Action::Aspected_Benefic); break;
	case Action::Malefic:
		if (lvl >= 4 && target && target->IsType(EntityType::Monster)) {
			if (target->HasAura(Status::Combust) || target->HasAura
				(Status::CombustII)); else return GetIcon(Action::Combust);
		}; return GetIcon(Action::Malefic);
	case Action::Helios:
		if (lvl >= 42 && !effect(Status::Aspected_Helios))
			return GetIcon(Action::Aspected_Helios); break;
	case Action::Esuna:
		if (lvl >= 12 && target && !target->CurrentHP())
			if (!target->IsType(EntityType::Monster))
				return GetIcon(Action::Ascend); break;
	}; return GetIcon(action);
};

int ActionSys::WhiteMage(int action) {
	local HUD = (WHM_HUD*) xiv->JobHud;
	target = xiv->LocalActor->TargetPtr();
	switch (action) {
	case Action::Stone:
		if (lvl >= 4 && target && target->IsType(EntityType::Monster)) {
			if (target->HasAura(Status::Aero)
				|| target->HasAura(Status::AeroII)); 
			else return GetIcon(Action::Aero);
		}; return GetIcon(Action::Stone);
	case Action::Cure:
		if (!target || target->IsType(EntityType::Monster))
			target = xiv->LocalActor;
		if (lvl >= 25 && !target->HasAura(Status::Regen))
				return GetIcon(Action::Regen);
		return GetIcon(Action::Cure);
	case Action::CureII:
		if (lvl >= 52 && HUD->Lily && !effect(Status::Freecure))
			return GetIcon(Action::Afflatus_Solace);
		return GetIcon(Action::CureII);
	case Action::Medica:
		if (lvl >= 50 && !effect(Status::MedicaII))
			return GetIcon(Action::MedicaII);
		return GetIcon(Action::Medica);
	case Action::Esuna:
		if (lvl >= 12 && target && !target->CurrentHP())
			if(!target->IsType(EntityType::Monster))
				return GetIcon(Action::Raise); break;
	}; return GetIcon(action);
};

int ActionSys::BlackMage(int action) {
	local HUD = (BLM_HUD*)xiv->JobHud;
	target = xiv->LocalActor->TargetPtr();
	switch (action) {
	case Action::Fire:
		if (lvl >= 34 && !HUD->Fire())
			return Action::Fire_III;
	case Action::Blizzard:
		if (lvl >= 40 && !HUD->Ice())
			return Action::Blizzard_III;
	}; return GetIcon(action);
}

int ActionSys::Archer(int action) {
	target = xiv->LocalActor->TargetPtr();
	switch (action) {
	case Action::Heavy_Shot:
		// Brain Dead Rotation
		if (lvl >= 2 && effect(Status::StraightShotReady))
			return GetIcon(Action::Straight_Shot);
		if (lvl >= 6 && target) {
			if (!target->HasAura(Status::VenomousBite))
				return GetIcon(Action::Venomous_Bite);
			if (lvl >= 30 && !target->HasAura(Status::Windbite))
				return GetIcon(Action::Windbite);
		}; break;
	}; return GetIcon(action);
};

int ActionSys::Rogue(int action) {
	switch (action) {
	case Action::Spinning_Edge:
		if (lvl >= 4 && Combo->Is(Action::Spinning_Edge))
			return GetIcon(Action::Gust_Slash);
		if (lvl >= 26 && Combo->Is(Action::Gust_Slash))
			return GetIcon(Action::Aeolian_Edge);
		return GetIcon(Action::Spinning_Edge);
	}; return GetIcon(action);
};

int ActionSys::Samurai(int action) {
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
	local HUD = (RDM_HUD*)xiv->JobHud;
	using namespace Status; using namespace Action;
	// Instant-Cast/Non-Instant Switches, Harder
	if (effect(Dualcast) || effect(Swiftcast)) switch (action) {
	case Scatter:
		return GetIcon(Scatter);
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
	case Scatter:
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

int ActionSys::Dancer(int action) {
	local HUD = (DNC_HUD*) xiv->JobHud;
	switch(action) {
	// Single Target
	case Action::Cascade:
		if (effect(Status::FlourishingCascade))
			return Action::Reverse_Cascade;
		if (lvl >= 2 && Combo->Is(Action::Cascade))
			return Action::Fountain;
		if (effect(Status::FlourishingFountain))
			return Action::Fountainfall;
		return Action::Cascade;
	// AoE Rotation
	case Action::Windmill:
		if (effect(Status::FlourishingWindmill))
			return Action::Rising_Windmill;
		if (lvl >= 25 && Combo->Is(Action::Windmill))
			return Action::Bladeshower;
		if (effect(Status::FlourishingShower))
			return Action::Bloodshower;
		return Action::Windmill;
	// Multi-Step Dance
	case Action::Standard_Step:
	case Action::Technical_Step:
		return GetIcon(HUD->Forwards(action));
	}; return GetIcon(action);
};

int ActionSys::GunBreaker(int action) {
	local HUD = (GNB_HUD*)xiv->JobHud;
	switch (action) {
	case Action::Keen_Edge:
		if (lvl >= 4 && Combo->Is(Action::Keen_Edge))
			return GetIcon(Action::Brutal_Shell);
		if (lvl >= 26 && Combo->Is(Action::Brutal_Shell))
			return GetIcon(Action::Solid_Barrel);

		break;
	case Action::Demon_Slice:
		if (lvl >= 40 && Combo->Is(Action::Demon_Slice))
			return GetIcon(Action::Demon_Slaughter); break;
	case Action::Gnashing_Fang:
		switch (HUD->Phase) {
			case 1:
				return GetIcon(Action::Savage_Claw);
			case 2:
				return GetIcon(Action::Wicked_Talon);
		}; break;
	case Action::Danger_Zone:
		if (HUD->Charges >= 2)
			return GetIcon(Action::Burst_Strike);
		break;
	}; return GetIcon(action);
};

char IsIconReplaceable::Detour(int action) {
	return 1;
};

bool Actor::HasAura(int value, float margin, UINT castSrc) {
	auto Effects = AuraList(); if (!castSrc)
		castSrc = xiv->LocalActor->EntityId();
	// might be 60??
	for (int i = 0; i < 30; i++)
		if (Effects[i].Type == value && Effects[i].CastId == castSrc)
			return Effects[i].Timer >= margin || Effects[i].Timer <= NULL;
	return false;
};