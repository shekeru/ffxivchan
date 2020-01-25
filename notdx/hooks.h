#pragma once
#include "user.h"

class IconSys {
	BINDING(INT64) hkGetIcon(IconSys* self, int action);
	BINDING(char) IsIconReplaceable(int action) {
		return 1;
	}; INSTANCE(void, _Ptr), *_Chk;
public:
	int RedMage(int action);
// Construction
	INT64 GetIcon(int action) {
		ORIGINAL(hkGetIcon, _Ptr);
		return original(this, action);
	}; 	
	static void AttachSystem() {
		_Chk = (PVOID) game->GetLocation(Offsets::CAN_SWAP);
		_Ptr = game->ScanPattern(Offsets::GET_ICON, 1).Cast<PVOID>();
		DetourAttach(&IconSys::_Ptr, IconSys::hkGetIcon);
		DetourAttach(&_Chk, IsIconReplaceable);
	}; 
};