#include "hooks.h"
#include <detours.h>

#define effect(value) \
	xiv->LocalActor->HasAura(value)

INT64 _fastcall IconSys::hkGetIcon(IconSys* self, int action) {
	if (xiv->LocalActor) switch (xiv->LocalActor->JobId()) {
		case Job::Lancer:
			return self->Lancer(action);
		case Job::Red_Mage:
			return self->RedMage(action);
	}; return self->GetIcon(action);
};

int IconSys::Lancer(int action) {
	local Combo = xiv->ComboSys;
	local HUD = (RDM_HUD*)xiv->JobHud;
	local &level = xiv->LocalActor->Level();
	// Pretty Simple Combo Checking
	switch (action) {
		case Action::True_Thrust:
			// Brain Dead Rotation
			if (level >= 4 && Combo->Is(Action::True_Thrust)) {
				// Buffing Damage
				if (level >= 18 && !effect(Status::Disembowel, 5.f))
					return GetIcon(Action::Disembowel);
				return GetIcon(Action::Vorpal_Thrust);
			}; if (level >= 26 && Combo->Is(Action::Vorpal_Thrust))
				return GetIcon(Action::Full_Thrust);
			return GetIcon(Action::True_Thrust);
	};  return GetIcon(action);
};

int IconSys::RedMage(int action) {
	local Combo = xiv->ComboSys;
	local HUD = (RDM_HUD*)xiv->JobHud;
	local &lvl = xiv->LocalActor->Level();
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