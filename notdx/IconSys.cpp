#include "hooks.h"
#include <detours.h>

#define effect(value) \
	xiv->LocalActor->HasAura(value)

INT64 _fastcall IconSys::hkGetIcon(IconSys* self, int action) {
	if (xiv->LocalActor) switch (xiv->LocalActor->JobId()) {
		case Job::Red_Mage:
			return self->RedMage(action);
	default:
		break;
	}; return self->GetIcon(action);
};

int IconSys::RedMage(int action) {
	local Combo = xiv->ComboTimer;
	local HUD = (RDM_HUD*) xiv->JobHud;
	local &level = xiv->LocalActor->Level();
	switch (action) {
		case Action::Verthunder:
			if (effect(Status::VerfireReady))
				return Action::Verfire;
			goto jolt_case;
		case Action::Veraero:
			if (effect(Status::VerstoneReady))
				return Action::Verstone;
			goto jolt_case;
		case Action::Verthunder_II:
			if (level >= 22 && HUD->WhiteMana <= HUD->BlackMana)
				return Action::Veraero_II;
		case Action::Riposte:
			if (level >= 35 && Combo->Is(Riposte))
				return GetIcon(Action::Zwerchhau);
			if (level >= 50 && Combo->Is(Zwerchhau))
				return GetIcon(Action::Redoublement);
		default:
			break;
		jolt_case:
			if (!effect(Status::Dualcast) && 
				!effect(Status::Swiftcast))
					return Action::Jolt;
		}; return GetIcon(action);
};