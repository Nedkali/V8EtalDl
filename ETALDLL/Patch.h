#pragma once
#include "D2Intercepts.h"
#include "D2Helpers.h"
#include "Offset.h"

PatchHook Patches[] = {
	{ Pointer::PatchCall,	Pointer::GetDllOffset("D2Client.dll", 0x2B494),	(DWORD)GetSelectedUnit_Intercept,		5 },//1.13d
	//{ Pointer::PatchJmp,	Pointer::GetDllOffset("D2Client.dll", 0x84417),	(DWORD)PlayerAssignment_Intercept,		5 },//1.13d
	{ Pointer::PatchBytes,	Pointer::GetDllOffset("D2Client.dll", 0x14630),	(DWORD)0xc3,							1 },//1.13d
	//{ Pointer::PatchCall,	Pointer::GetDllOffset("D2Client.dll", 0x1B047),	(DWORD)GameActChange_Intercept,			5 },//1.13d
	//{ Pointer::PatchJmp,	Pointer::GetDllOffset("D2Client.dll", 0x1B474),	(DWORD)GameActChange2_Intercept,		5 },//1.13d
	//{ Pointer::PatchCall,	Pointer::GetDllOffset("D2Client.dll", 0x461AD),	(DWORD)GameLeave_Intercept,				5 },//1.13d
	//{ Pointer::PatchCall,	Pointer::GetDllOffset("D2Client.dll", 0x29560),	(DWORD)GameAttack_Intercept,			5 },//1.13d
	{ Pointer::PatchCall,	Pointer::GetDllOffset("D2CMP.dll", 0x14CD5),	(DWORD)GameCrashFix_Intercept,			10 },//1.13d

	{ Pointer::PatchJmp,	Pointer::GetDllOffset("D2Client.dll", 0x1D7B4), (DWORD)GameDraw_Intercept,				6},//1.13d
	{ Pointer::PatchCall,	Pointer::GetDllOffset("D2Win.dll", 0xEC68),		(DWORD)GameDrawMenu_Intercept,			5}//1.13d
};
PatchHook RawKeyInfo[] = {
	{ Pointer::PatchJmp,	Pointer::GetDllOffset("Bnclient.dll",0x15EB3),	(DWORD)ClassicSTUB,		5},
	{ Pointer::PatchJmp,	Pointer::GetDllOffset("Bnclient.dll",0x16065),	(DWORD)KeyOwnerSTUB,	5},
	{ Pointer::PatchJmp,	Pointer::GetDllOffset("Bnclient.dll",0x161B8),	(DWORD)LodSTUB,			5}
};