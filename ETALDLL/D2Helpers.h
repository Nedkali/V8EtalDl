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

enum DistanceType
{
	Euclidean, Chebyshev, Manhattan
};


// Some buttons
#define BUTTON_SP findControl(CONTROL_BUTTON, 5106, -1, 264,324,272,35)
DWORD WINAPI MainThread(VOID*);
void LoadMPQ(const char* mpq);

bool SendDataCopy(char* app, int  code, char* data);
void __fastcall Say(const char* szMessage);

POINT	CalculateTextLen(const char* szwText, int Font);
CellFile* LoadCellFile(char* lpszPath, DWORD bMPQ = 3);
void myDrawAutomapCell(CellFile *cellfile, int xpos, int ypos, BYTE col);
void	myDrawText(const char* szwText, int x, int y, int color, int font);
POINT GetScreenSize();
int D2GetScreenSizeX();
int D2GetScreenSizeY();
void GameDraw(void);
void GameDrawMenu(void); 
bool IsScrollingText();

void	WorldToScreen(POINT* pPos);
void	ScreenToWorld(POINT *ptPos);
POINT	ScreenToAutomap(int x, int y);
void	AutomapToScreen(POINT* pPos);
HANDLE OpenFileRead(char *filename);
CellFile *LoadBmpCellFile(BYTE *buf1, int width, int height);
CellFile *LoadBmpCellFile(char *filename);
DWORD ReadFile(HANDLE hFile, void *buf, DWORD len);
void *memcpy2(void *dest, const void *src, size_t count);
BYTE *AllocReadFile(char *filename);
CellFile *myInitCellFile(CellFile *cf);

void  __fastcall D2CLIENT_MercItemAction_ASM(DWORD bPacketType, DWORD dwSlot);
void  __fastcall D2CLIENT_ClickItemRight_ASM(DWORD x, DWORD y, DWORD Location, DWORD pItem, DWORD pItemPath);
void  __fastcall D2CLIENT_ClickBelt(DWORD x, DWORD y, Inventory* pInventoryData);
void  __fastcall D2CLIENT_ClickBeltRight_ASM(DWORD pInventory, DWORD pPlayer, DWORD HoldShift, DWORD dwPotPos);

//AutomapLayer* InitAutomapLayer(DWORD levelno);
//DWORD __fastcall D2CLIENT_InitAutomapLayer_STUB(DWORD nLayerNo);

DWORD	GetPlayerArea(void);
Level*	GetLevel(DWORD dwLevelNo);
void	D2CLIENT_Interact(UnitAny* pUnit, DWORD dwMoveType);
BYTE	CalcPercent(DWORD dwVal, DWORD dwMaxVal, BYTE iMin = NULL);
DWORD	GetTileLevelNo(Room2* lpRoom2, DWORD dwTileNo);
UnitAny* GetMercUnit(UnitAny* pUnit);
bool	ClickNPCMenu(DWORD NPCClassId, DWORD MenuId);
int		GetItemLocation(UnitAny *pItem);

char*	GetSkillByID(WORD id);
WORD	GetSkillByName(char* szSkillName);

const char* GetUnitName(UnitAny* pUnit, char* szBuf, size_t bufSize);
void GetItemCode(UnitAny* pUnit, char* szBuf);

UnitAny* D2CLIENT_FindUnit(DWORD dwId, DWORD dwType);


static inline void AddRoomData(Room2* room) { fpAddRoomData(room->pLevel->pMisc->pAct, room->pLevel->dwLevelNo, room->dwPosX, room->dwPosY, room->pRoom1); }
static inline void RemoveRoomData(Room2* room) { fpRemoveRoomData(room->pLevel->pMisc->pAct, room->pLevel->dwLevelNo, room->dwPosX, room->dwPosY, room->pRoom1); }
static inline char* __stdcall GetLevelName(const Level* level) { return D2COMMON_GetLevelText(level->dwLevelNo)->szName; }
static inline char* __stdcall GetLevelIdName(DWORD level) { return D2COMMON_GetLevelText(level)->szName; }

void  __fastcall D2CLIENT_Interact_ASM(DWORD Struct);
DWORD __fastcall D2CLIENT_GetUnitName_STUB(DWORD UnitAny);
DWORD __fastcall D2CLIENT_GetUIVar_STUB(DWORD varno);
void __stdcall D2CLIENT_TakeWaypoint(DWORD dwWaypointId, DWORD dwArea);
void SendGold(int nGold, int nMode);
void __fastcall UseStatPoint(WORD stat, int count = 1);
void __fastcall UseSkillPoint(WORD skill, int count = 1);
DWORD __fastcall D2CLIENT_SendGamePacket_ASM(DWORD dwLen, BYTE* bPacket); 
void  __fastcall D2CLIENT_SetSelectedUnit_STUB(DWORD UnitAny);
void __fastcall D2GFX_DrawRectFrame_STUB(RECT* rect);
double GetDistance(long x1, long y1, long x2, long y2, DistanceType type = Euclidean);
struct SData
{
	char  str[255];

};

