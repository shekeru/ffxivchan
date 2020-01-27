#include "ActionManager.h"

char RequestAction::Detour(ActionSys* self, UINT a2, UINT action,
	INT64 ptr4, INT a5, UINT a6, INT a7)
{
	auto value = ::RequestAction.Call(self, a2, action, ptr4, a5, a6, a7);
	//printf("acq: %p, a5: %i, id: %i, a7: %i\n", self, a5, action, a7);
	return xiv->ComboSys->Set(value, action);
}

char AllowRequestsGCD::Detour(ActionSys* self, UINT flag, UINT action) {
	::AllowRequestsGCD.Call(self, flag, action); 
	/* if (xiv) {
		printf("LocalActor: %p\n", xiv->LocalActor);
		printf("ComboSys: %p\n", xiv->ComboSys);
		printf("JobGauge: %p\n", xiv->JobHud);
	}; */ return 1;
}