//////////////////////////////////////////////////////////////////////
// OOG.h
//////////////////////////////////////////////////////////////////////
#pragma once

#include <Windows.h>
#include <string>
#include <vector>

#include "D2Pointers.h"
#include "Common.h"
#include "Constants.h"
#include "Input.h"
#include "Main.h"
#include "D2Intercepts.h"


// Some buttons
#define BUTTON_SP findControl(CONTROL_BUTTON, 5106, -1, 264,324,272,35)
DWORD WINAPI MainThread(VOID*);
void LoadMPQ(const char* mpq);
void  Logincontrol();
void SendDataCopy(char* app, int  code, char* data);
void __fastcall Say(const char* szMessage);


DWORD	GetPlayerArea(void);
Level*	GetLevel(DWORD dwLevelNo);
void	D2CLIENT_Interact(UnitAny* pUnit, DWORD dwMoveType);
BYTE	CalcPercent(DWORD dwVal, DWORD dwMaxVal, BYTE iMin = NULL);
DWORD	GetTileLevelNo(Room2* lpRoom2, DWORD dwTileNo);
UnitAny* GetMercUnit(UnitAny* pUnit);

BOOL	ClickNPCMenu(DWORD NPCClassId, DWORD MenuId);
int		GetItemLocation(UnitAny *pItem);

char*	GetSkillByID(WORD id);
WORD	GetSkillByName(char* szSkillName);

const char* GetUnitName(UnitAny* pUnit, char* szBuf, size_t bufSize);
void GetItemCode(UnitAny* pUnit, char* szBuf);

UnitAny* D2CLIENT_FindUnit(DWORD dwId, DWORD dwType);


static inline void AddRoomData(Room2* room) { D2COMMON_AddRoomData(room->pLevel->pMisc->pAct, room->pLevel->dwLevelNo, room->dwPosX, room->dwPosY, room->pRoom1); }
static inline void RemoveRoomData(Room2* room) { D2COMMON_RemoveRoomData(room->pLevel->pMisc->pAct, room->pLevel->dwLevelNo, room->dwPosX, room->dwPosY, room->pRoom1); }
static inline char* __stdcall GetLevelName(const Level* level) { return D2COMMON_GetLevelText(level->dwLevelNo)->szName; }
static inline char* __stdcall GetLevelIdName(DWORD level) { return D2COMMON_GetLevelText(level)->szName; }

void  __fastcall D2CLIENT_Interact_ASM(DWORD Struct);
DWORD __fastcall D2CLIENT_GetUnitName_STUB(DWORD UnitAny);
DWORD __fastcall D2CLIENT_GetUIVar_STUB(DWORD varno);
void __stdcall D2CLIENT_TakeWaypoint(DWORD dwWaypointId, DWORD dwArea);

struct SData
{
	char  str[255];

};

