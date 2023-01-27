#pragma once
#include "memory.h"

namespace Offsets {
	inline auto security_check_cookie = FollowPattern
		("e8 ? ? ? ? 4c 8d 9c 24 80 03 00", 1);
	
	// Get Icon Found Below IsIconReplaceable
	inline auto IsIconReplaceable = FollowPattern
		("E8 ? ? ? ? 84 C0 74 4C 8B D3", 1);
	// Alt: via massive case statement
	// 8D 42 EB 3D FC 00
	inline auto GetIcon = FollowPattern
		("E8 ? ? ? ? 8B F8 3B DF", 1);
};