#include "main.h"

char RequestAction::Detour(Rapture* self, UINT a2, UINT action,
	INT64 ptr4, INT a5, UINT a6, INT a7)
{
	auto value = ::RequestAction.Call(self, a2, action, ptr4, a5, a6, a7);
	printf("acq: %p, a5: %i, id: %i, a7: %i\n", self, a5, action, a7);
	return xiv->ComboSys->Set(value, action);
}

char AllowRequestsGCD::Detour(Rapture* self, UINT flag, UINT action) {
	::AllowRequestsGCD.Call(self, flag, action); return 1;
}