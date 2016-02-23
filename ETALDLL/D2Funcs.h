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

	// Set in JSFunctions
	static bool CloseD2();
	static bool Delay();
	static bool Include();
	static bool GetTickCount();
	static bool Load();
	static bool RunGC();
	static bool SendCopyData();

	// Globals -
	static bool CheckCollision();
	static bool ClickMap();
	static bool FileOpen();
	static bool GetBaseStat();
	static bool GetControl();
	static bool GetCursorType();
	static bool GetDistance();
	static bool GetLocaleString();
	static bool GetPath();
	static bool GetPlayerFlag();
	static bool GetPresetUnits();
	static bool GetRoom();
	static bool GetScript();
	static bool GetUIState();
	static bool GetUnit();
	static bool GetWaypoint();
	static bool Gold();
	static bool RegisterEvent();
	static bool SetStatusText();
	static bool SetUIState();
	static bool SubmitItem();
	static bool Transmute();
	static bool UnregisterEvent();

	static Level* GetLevel(Act* pAct, int level);
	static int Random(WORD a, WORD b);
	static void Print(const char * szText, ...);
	static UnitAny* GetPlayerUnit();
	static Level* GetArea();
	static bool ExitGame();
	//static void Say(const char *szMessage);
	static bool Interact(WORD dwUnitType, WORD dwUnitID);
	static bool MoveTo(WORD x, WORD y);						// this will be removed
	static bool TeleTo(WORD x, WORD y, bool Left);			// this will be removed
	static bool WaypointTo(WORD wpid, BYTE(dest));

	static bool SetSkill(WORD wSkillId, bool Left);			// this needs to be set for me eg me.SetSkill()
	static bool GetSkill(WORD wSkillId);					// this is for SetSkill

};

struct GetPlayerUnit
{
	DWORD Name;
	DWORD UnitId;
	DWORD AreaId;
	DWORD Type;
};

struct myArea {
	DWORD AreaId;
	DWORD Exits;
	//JSObject* ExitArray;
};
#endif