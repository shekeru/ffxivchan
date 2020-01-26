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
	local Combo = xiv->ComboTimer;
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
	local Combo = xiv->ComboTimer;
	local HUD = (RDM_HUD*) xiv->JobHud;
	local &level = xiv->LocalActor->Level();
	using namespace Status; using namespace Action;
	// Instant-Cast/Non-Instant Switches
	if (effect(Dualcast) || effect(Swiftcast)) switch (action) {
	// If proc, swap skill type
		case Verthunder:
			if (effect(VerfireReady)) 
				return effect(VerfireReady)
					? GetIcon(Jolt) : GetIcon(Veraero);
			return GetIcon(Verthunder);
		case Veraero:
			if (effect(VerstoneReady))
				return effect(VerfireReady) 
					? GetIcon(Jolt) : GetIcon(Verthunder);
			return GetIcon(Veraero);
	} switch (action) { 
	// Check for Proc Skills
		case Verthunder:
			return effect(VerfireReady) ? 
				GetIcon(Verfire): GetIcon(Jolt);
		case Veraero:
			return effect(VerstoneReady) ?
				GetIcon(Verstone): GetIcon(Jolt); 
	// Flip AoE, based on mana needs
		case Verthunder_II: 
			return (level >= 22 && HUD->WhiteMana <= HUD->BlackMana) 
				? GetIcon(Veraero_II) : GetIcon(Verthunder_II);
	// Melee Section
		case Riposte:
			if (level >= 35 && Combo->Is(Riposte))
				return GetIcon(Zwerchhau);
			if (level >= 50 && Combo->Is(Zwerchhau))
				return GetIcon(Redoublement);
	}; return GetIcon(action);
};