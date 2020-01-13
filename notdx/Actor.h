#pragma once

class Actor {
public:
	char pad[152];
	float X, Z, Y;
	int nil;
	float spin;
public:
	virtual void _unkvf();
};