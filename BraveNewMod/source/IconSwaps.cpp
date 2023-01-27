#include "hooks.h"
#include "types.h"

using namespace IconSwaps;

$set_original(GetIcon);
UINT64 GetIcon::Function(ActionSys* self, UINT action) {
	get_original;

	if (rand() % 1000 == 0) {
		printf("ActionSys: %llx\n", self);
	}

	switch (action) {

	case Action::Fast_Blade:
		if (self->Combo.Is(Action::Fast_Blade))
			return original(self, Action::Riot_Blade);
		if (self->Combo.Is(Action::Riot_Blade))
			return original(self, Action::Rage_of_Halon);
		break;

	case Action::Total_Eclipse:
		if (self->Combo.Is(Action::Total_Eclipse))
			return original(self, Action::Prominence);
		break;

	}; return original(self, action);
};

$set_original(IsIconReplaceable);
char IsIconReplaceable::Function(UINT action) {
	return 1;
};