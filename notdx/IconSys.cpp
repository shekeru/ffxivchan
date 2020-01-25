#include "hooks.h"
#include <detours.h>

INT64 _fastcall IconSys::raw_call(IconSys* self, int action) {
	if (xiv->LocalActor) switch (xiv->LocalActor->JobId()) {
		case Job::Red_Mage:
			return self->RedMage(action);
	default:
		break;
	}; return self->GetIcon(action);
};

int IconSys::RedMage(int action) {
	static auto HUD = (RDM_HUD*) xiv->JobHud;
	switch (action) {
		case Action::Verthunder:
			if (xiv->LocalActor->HasAura(Status::VerfireReady))
				return Action::Verfire;
			goto jolt_case;
		case Action::Veraero:
			if (xiv->LocalActor->HasAura(Status::VerstoneReady))
				return Action::Verstone;
			goto jolt_case;
		case Action::Verthunder_II:
			if (xiv->LocalActor->Level() >= 22 && 
					HUD->WhiteMana <= HUD->BlackMana)
				return Action::Veraero_II;
		default:
			break;
		jolt_case:
			if (!xiv->LocalActor->HasAura(Status::Dualcast) &&
				!xiv->LocalActor->HasAura(Status::Swiftcast))
				return Action::Jolt;
		}; return GetIcon(action);
};