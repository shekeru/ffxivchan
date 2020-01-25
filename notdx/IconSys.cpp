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
	return GetIcon(action);
};

int IconSys::RedMage(int action) {
	local Combo = xiv->ComboTimer;
	local HUD = (RDM_HUD*) xiv->JobHud;
	local &level = xiv->LocalActor->Level();
	// Instant-Cast/Non-Instant Switches
	if (effect(Dualcast) || effect(Swiftcast)) switch (action) {
	// If proc, swap skill type
		case Action::Verthunder:
			if (effect(Status::VerfireReady)) 
				return GetIcon(Veraero);
			return GetIcon(Verthunder);
		case Action::Veraero:
			if (effect(Status::VerstoneReady))
				return GetIcon(Verthunder); 
			return GetIcon(Veraero);
	} switch (action) { 
	// Check for Proc Skills
		case Action::Verthunder:
			return effect(VerfireReady) ? 
				GetIcon(Verfire): GetIcon(Jolt);
		case Action::Veraero:
			return effect(VerstoneReady) ?
				GetIcon(Verstone): GetIcon(Jolt); 
	// Flip AoE, based on mana needs
		case Action::Verthunder_II: 
			return (level >= 22 && HUD->WhiteMana <= HUD->BlackMana) 
				? GetIcon(Veraero_II) : GetIcon(Verthunder_II);
	// Melee Section
		case Action::Riposte:
			if (level >= 35 && Combo->Is(Riposte))
				return GetIcon(Zwerchhau);
			if (level >= 50 && Combo->Is(Zwerchhau))
				return GetIcon(Redoublement);
	}; return GetIcon(action);
};