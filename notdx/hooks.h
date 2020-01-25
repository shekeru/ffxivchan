#pragma once
#include "user.h"

class IconSys {
public:
	int RedMage(int action);
public:
	INT64 GetIcon(int action) {
		static auto original = decltype(&raw_call)(_Ptr);
		return original(this, action);
	}; 	inline static PVOID _Ptr;
	static void AttachSystem() {
		_Ptr = game->ScanPattern(Offsets::GET_ICON, 1).Cast<PVOID>();
		DetourAttach(&IconSys::_Ptr, IconSys::raw_call);
	}; static INT64 _fastcall raw_call(IconSys* self, int action);
};