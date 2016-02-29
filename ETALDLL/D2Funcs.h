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
	static Level* GetLevel(Act* pAct, int level);
	static void Print(const char * szText, ...);
	static Level* GetArea();
	static bool Interact(DWORD dwUnitType, DWORD dwUnitID);
	static bool MoveTo(WORD x, WORD y);						// this will be removed
	static bool TeleTo(WORD x, WORD y, bool Left);			// this will be removed
	static bool WaypointTo(DWORD wpid, BYTE(dest));

	static bool SetSkill(WORD wSkillId, bool Left);			// this needs to be set for me eg me.SetSkill()
	static bool GetSkill(WORD wSkillId);					// this is for SetSkill

};
struct myArea {
	DWORD AreaId;
	DWORD Exits;
	//JSObject* ExitArray;
};
#endif