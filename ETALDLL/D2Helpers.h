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



struct SData
{
	char  str[255];

};

