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
#define BasicCombo(L, N) \
	if (lvl >= L && Combo->Is(Action::##N))
#define SetAction(Name) \
	return GetIcon(Action::##Name);

INT64 GetIcon::Detour(ActionSys* self, int action) {
	if (xiv->LocalActor) {
		lvl = xiv->LocalActor->JobLevel();
		target = xiv->LocalActor->TargetPtr();
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
		case Job::Summoner:
			return self->Summoner(action);
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
		case Action::Lance_Charge:
			BasicCombo(50, Disembowel)
				SetAction(Chaos_Thrust); 
			break;
		case Action::True_Thrust:
			BasicCombo(4, True_Thrust) {
				if (lvl >= 18 && !effect(Status::Disembowel, 7.5f))
					SetAction(Disembowel); SetAction(Vorpal_Thrust);
			}; BasicCombo(26, Vorpal_Thrust) {
				if (lvl >= 6 && CanCast(Action::Life_Surge))
					SetAction(Life_Surge); SetAction(Full_Thrust);
			}; break;
	};  return GetIcon(action);
};

int ActionSys::Astrologian(int action) {
	local HUD = (AST_HUD*)xiv->JobHud;
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
	case Action::Draw:
		if (lvl >= 40 && HUD->Card)
			return GetIcon(Action::Redraw); break;
	case Action::Play:
		if (lvl >= 50 && HUD->InSet())
			SetAction(MinorArcana); break;
	}; return GetIcon(action);
};

int ActionSys::WhiteMage(int action) {
	local HUD = (WHM_HUD*) xiv->JobHud;
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
		if (lvl >= 35 && !target->HasAura(Status::Regen))
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
	switch (action) {
	}; return GetIcon(action);
};


int ActionSys::RedMage(int action) {
	local HUD = (RDM_HUD*)xiv->JobHud; namespace S = Status;
	// Instant-Cast/Non-Instant Switches, Harder
	if (effect(S::Dualcast) || effect(S::Swiftcast)) 
	switch (action) {
	case Action::Scatter:
		SetAction(Scatter);
	case Action::Jolt:
	// White Mana Need (Veraero)
		if (lvl >= 10 && !effect(S::VerstoneReady)) {
			if (effect(S::VerfireReady) || 
				HUD->WhiteMana <= HUD->BlackMana)
				SetAction(Veraero);
		};
	// Black Mana Need (Verthunder)
		if (lvl >= 4 && !effect(S::VerfireReady))
			SetAction(Verthunder);
	// Otherwise, Nothing
		SetAction(Jolt);
	} switch (action) {
	// Check for Proc Skills on Jolt, Easy
	case Action::Jolt:
		if (effect(S::VerstoneReady))
			SetAction(Verstone);
		if (effect(S::VerfireReady))
			SetAction(Verfire);
		SetAction(Jolt);
		// Flip AoE, based on mana needs
	case Action::Scatter:
		if (lvl >= 22 && HUD->WhiteMana <= HUD->BlackMana)
			SetAction(Veraero_II); SetAction(Verthunder_II);
		// Melee Section
	case Action::Riposte:
		BasicCombo(35, Riposte)
			SetAction(Zwerchhau);
		BasicCombo(50, Zwerchhau)
			SetAction(Redoublement);
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
	auto dzPtr = ActionRecast(Action::Danger_Zone);
	local HUD = (GNB_HUD*)xiv->JobHud;
	switch (action) {
	case Action::Keen_Edge:
		// Weaving?
		if (lvl >= 18 && dzPtr && !dzPtr->Cooldown)
			return GetIcon(Action::Danger_Zone);
		// Drop Maxed Charges
		if (HUD->Charges >= 2)
			return GetIcon(Action::Burst_Strike);
		// Continue Combo
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
	}; return GetIcon(action);
};

int ActionSys::Summoner(int action) {
	switch (action) {
	case Action::Ruin:
		if (target && target->IsType(EntityType::Monster)) {
			if (lvl >= 2 && !(target->HasAura(Status::Bio) || target->HasAura(Status::Bio_II)))
				return GetIcon(Action::Bio);
			if (lvl >= 6 && !(target->HasAura(Status::Miasma)))
				return GetIcon(Action::Miasma);
		}; break;
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