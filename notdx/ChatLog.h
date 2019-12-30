#pragma once
#include "vmt.h"

class ChatLog {
public:
	intptr_t OffsetArrayStart, 
		OffsetArrayPos, OffsetArrayEnd, 
	LogStart, LogNext, LogEnd;
};