#pragma once

class Window {
public:
	void* vfuncs;
	char Name[32];
	// 0x18a & 4 -> show bit
	// 0x180 -> windowStatus
	// 0x1cc -> windowId
};