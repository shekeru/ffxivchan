#include "Actor.h"
#include "context.h"

UINT Actor::GetTargetID() {
	static UINT(*_GetTargetID)(Actor*) = FnPattern<UINT(*)(Actor*)>
		("E8 ? ? ? ? 49 3B C7 0F 84", 1).Resolve(Context::game);
	return _GetTargetID(this);
};

GameObject* TargetSystem::GetCurrentTargetPtr() {
	static GameObject* (*_GetCurrentTargetPtr)(TargetSystem*) = FnPattern<GameObject* (*)(TargetSystem*)>
		("E8 ? ? ? ? 48 3B C6 0F 94 C0", 1).Resolve(Context::game);
	return _GetCurrentTargetPtr(this);
}