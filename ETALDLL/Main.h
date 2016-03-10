#ifndef __DBase_H__
#define __DBase_H__

#pragma once
#include <windows.h>
#include <vector>
#include <map>
#include "MenuControls.h"
#include "GameDraw.h"

#define D2ETAL_VERSION "v0.01"

#define PRIVATE_ITEM		3

using namespace std;

struct Private { DWORD dwPrivateType; };

struct Profile {
	char Account[24];
	char AccPass[12];
	//char MpqFile[32];
	char KeyOwner[32];
	char Classic[32];
	char Lod[32];
	char GameName[8];
	char GamePass[6];
	int Charloc;
	int Realm;
	char RandomGameName[8];
	char RandomGamePass[6];
	int Difficulty;
	char ScriptFile[32];
};

struct Variables
{
	std::map<unsigned __int32, CellFile*> mCachedCellFiles;
	char	szScriptPath[MAX_PATH];
	char	szMpqfile[30];
	char	szClassic[30];
	char	szLod[30];
	char	szKeyOwner[30];
	bool	bzladder;
	char	szScriptFile[32];
	unsigned int iCharPos;
	DWORD	meMaxGameTime;
	int		nRunWalk;
	int		nWeaponsTab;
	int		MeChickenHP;
	int		MeChickenMP;
	bool	revealautomap;
	bool	bzUseRawKeys;
	HMODULE hModule;
	
	BOOL	bActive;
	WNDPROC oldWNDPROC;
	HHOOK hMouseHook;
	HHOOK hKeybHook;

	BOOL	bClickAction;
	DWORD dwSelectedUnitId;
	DWORD dwSelectedUnitType;
	POINT pMouseCoords;


	//for access to objects in d2
	Room2* pRoom2;
	UnitAny* thisUnit;
	int		nthisPresetX;
	int		nthisPresetY;
};

extern Profile  Prof;
extern Variables Vars;

#endif