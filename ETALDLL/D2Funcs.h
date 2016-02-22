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
		//Globals
		/*[+] CheckCollision()
		[+] ClickMap()
		[+] CloseD2()
		[+] Delay()
		[+] ExitGame()
		[+] FileOpen()
		[+] GetArea()
		[+] GetBaseStat()
		[+] GetControl()
		[+] GetCursorType()
		[+] GetDistance()
		[+]GetLocaleString()
		[+] GetPath()
		[+] GetPlayerFlag()
		[+] GetPlayerUnit()
		[+] GetPresetUnits()
		[+] GetRoom()
		[+] GetScript()
		[+] GetTickCount()
		[+] GetUIState()
		[+] GetUnit()
		[+] GetWaypoint()
		[+] Gold()
		[+] Include()
		[+] Load()
		[+] Print()
		[+] RegisterEvent()
		[+] Random()
		[+] RunGC()
		[+] Say()
		[+] SendCopyData()
		[+] SetStatusText()
		[+] SetUIState()
		[+] SubmitItem()
		[+] Transmute()
		[+] UnregisterEvent()*/

	static bool Interact(WORD dwUnitType, WORD dwUnitID);
	static bool MoveTo(WORD x, WORD y);
	static bool TeleTo(WORD x, WORD y, bool Left);
	static bool SetSkill(WORD wSkillId, bool Left);
	static bool GetSkill(WORD wSkillId);
	static bool WaypointTo(WORD wpid, BYTE(dest));
	static void Print(const char * szFormat, ...);
	static UnitAny GetPlayerUnit();
	static bool ExitGame();
};

#endif