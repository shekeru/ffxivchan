#pragma once
#include "Managers.h"

class R_Float {
public:
	float Current, Min, Max;
};

class ZoomSys;
// Method: UpdateTick
INSTALL(UpdateTick, void, ZoomSys*)
Location = game->GetLocation("40 55 53 48 8d 6c 24 b1 48 81 ec c8 00 00 00 48 8b d9");
$ UpdateTick;

class ZoomSys {
public:
	virtual void _unk();
public:
	float _pad[0x43];
	R_Float Zoom, Fov; // start: 0x114, end: 0x12C
	float _pad1[0x14], rZoom; // end: 0x180
	float _pad2[0x22], zLim0, zLim1; // start: 0x208
public:
	// Methods
	void UpdateTick() {
		RESTORE(::UpdateTick);
	};
	// Detouring
	static void Attach() {
		printf("Installing Zoom Hack!!\n");
		::UpdateTick.Attach();
	};
};