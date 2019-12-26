#pragma once
#include "system.h"

#define OFFSET(TYPE, NAME, OFFSET) TYPE& NAME() \
{return *(TYPE*)(uintptr_t(this) + OFFSET); }

//class UnitArray {
//	UnitArray operator[](int index) const {
//		return this[9200 * index;
//	};
//};

class CUnit
{
#define szUnit 9200
public:
	OFFSET(UINT, ID, 116);
	char* Name() {
		return (char*)(this) + 48;
	}
	operator bool() const
	{
		return (uintptr_t) this;
	}
};