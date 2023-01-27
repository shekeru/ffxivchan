#pragma once
#include "memory.h"

#define provide inline auto

namespace Offsets {

	auto security_check_cookie = FollowPattern
		("e8 ? ? ? ? 4c 8d 9c 24 80 03 00", 1);
};