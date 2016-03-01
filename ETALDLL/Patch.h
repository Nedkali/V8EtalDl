#pragma once
#include "D2Intercepts.h"
#include "D2Helpers.h"
#include "Offset.h"

PatchHook Patches[] = {
	{ Pointer::PatchJmp,	Pointer::GetDllOffset("D2Client.dll", 0x1D7B4), (DWORD)GameDraw_Intercept, 6},//1.13d
};
PatchHook RawKeyInfo[] = {
	{ Pointer::PatchJmp,	Pointer::GetDllOffset("Bnclient.dll",0x15EB3),	(DWORD)ClassicSTUB,		5},
	{ Pointer::PatchJmp,	Pointer::GetDllOffset("Bnclient.dll",0x16065),	(DWORD)KeyOwnerSTUB,		5},
	{ Pointer::PatchJmp,	Pointer::GetDllOffset("Bnclient.dll",0x161B8),	(DWORD)LodSTUB,			5}
};