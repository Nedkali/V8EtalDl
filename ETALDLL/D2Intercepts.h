#ifndef D2INTERCEPTS_H
#define D2INTERCEPTS_H

#include "D2Structs.h"

VOID __fastcall ClassicSTUB();
VOID __fastcall LodSTUB();
VOID __fastcall KeyOwnerSTUB();
void GameDraw_Intercept();
void GameDrawMenu_Intercept();
UnitAny* GetSelectedUnit_Intercept(void);
void GameCrashFix_Intercept();

#endif