#ifndef __DBase_H__
#define __DBase_H__

#pragma once
#include <windows.h>
#include <vector>
#include "MenuControls.h"


struct Profile {
	char Account[24];
	char AccPass[12];
	char MpqFile[32];
	char KeyOwner[32];
	char Classic[32];
	char Lod[32];
	char GameName[8];
	char GamePass[6];
	char Charloc;
	char Realm;
	char RandomGameName[8];
	char RandomGamePass[6];
	char Difficulty;
	char ScriptFile[32];
};

struct Variables
{
	char	szScriptPath[MAX_PATH];
	char	szMpqfile[30];
	char	szClassic[30];
	char	szLod[30];
	char	szKeyOwner[30];
	bool	bzladder;
	unsigned int iCharPos;
	bool	bzUseRawKeys;
	HMODULE hModule;
};

extern Profile  Prof;
extern Variables Vars;

#endif