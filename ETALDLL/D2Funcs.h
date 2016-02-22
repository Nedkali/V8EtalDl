#pragma once
#ifndef __D2Funcs_H__
#define __HELPERS_H__

#include "Helpers.h"
#include "D2Pointers.h"
#include "D2Intercepts.h"
#include "D2Structs.h"

class D2Funcs
{
public:
	D2Funcs();
	~D2Funcs();

	static void Print(const char * szFormat, ...);
};

#endif