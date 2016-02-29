//////////////////////////////////////////////////////////////////////
// JS Functions
//////////////////////////////////////////////////////////////////////
#include "JSFunctions.h"
#include "MenuControls.h"
#include "V8Script.h"
#include "D2Helpers.h"
#include "Helpers.h"
#include <shlwapi.h>
#include "D2Funcs.h"
#include <time.h>
#include <algorithm>
#include <assert.h>
#include "Accessors.h"

void StringReplace(char* str, const char find, const char replace, size_t buflen)
{
	for (size_t i = 0; i < buflen; i++)
	{
		if (str[i] == find)
			str[i] = replace;
	}
}

v8::Handle<v8::Context> CreateContext(v8::Isolate* isolate)
{
	Handle<ObjectTemplate>global = ObjectTemplate::New();
	//JS_FLINK(CCheckCollision, "CheckCollision");	// required
	JS_FLINK(CClick, "Click");
	JS_FLINK(CClickControl, "ClickControl");
	JS_FLINK(CClickMap, "ClickMap");				// required
	JS_FLINK(CClientState, "ClientState");
	JS_FLINK(CCloseD2, "CloseD2");					// required
	JS_FLINK(CDelay, "Delay");						// required
	JS_FLINK(CExitGame, "ExitGame");				// required
	//JS_FLINK(CFileOpen, "FileOpen");				// required
	JS_FLINK(CGetArea, "GetArea");					// required
	JS_FLINK(CGetBaseStat, "GetBaseStat");			// required
	JS_FLINK(CGetControl, "GetControl");			// required
	//JS_FLINK(CGetCursorType, "GetCursorType");	// required
	JS_FLINK(CGetDistance, "GetDistance");			// required
	JS_FLINK(CGetLocaleString, "GetLocaleString");	// required
	JS_FLINK(CGetLocation, "GetLocation");
	JS_FLINK(CGetPath, "GetPath");					// required
	//JS_FLINK(CGetPlayerFlag, "GetPlayerFlag");	// required
	JS_FLINK(CGetPlayerUnit, "GetPlayerUnit");		// required
	JS_FLINK(CGetPresetUnits, "GetPresetUnits");	// required
	//JS_FLINK(CGetRoom, "GetRoom");				// required
	//JS_FLINK(CGetScript, "GetScript");			// required
	JS_FLINK(CGetText, "GetText");
	JS_FLINK(CGetTickCount, "GetTickCount");		// required
	JS_FLINK(CGetUIState, "GetUIState");			// required
	JS_FLINK(CGetUnit, "GetUnit");					// required
	//JS_FLINK(CGetWaypoint, "GetWaypoint");		// required
	JS_FLINK(CGold, "Gold");						// required
	JS_FLINK(CInclude, "Include");					// required
	JS_FLINK(CLoad, "Load");						// required
	JS_FLINK(CMove, "Move");
	JS_FLINK(CPrint, "Print");						// required
	JS_FLINK(CRandom, "Random");					// required
	//JS_FLINK(CRegisterEvent, "RegisterEvent");	// required
	//JS_FLINK(CRunGC, "RunGC");					// required
	JS_FLINK(CSay, "Say");							// required
	JS_FLINK(CScreenSize, "ScreenSize");
	JS_FLINK(CSelectChar, "SelectChar");
	JS_FLINK(CSelectRealm, "SelectRealm");
	JS_FLINK(CSendCopyData, "SendCopyData");		// required
	JS_FLINK(CSetSkill, "SetSkill");				// Needs to be set for me.Global
	//JS_FLINK(CSetStatusText, "SetStatusText");	// required
	JS_FLINK(CSetText, "SetText");
	JS_FLINK(CSetTitle, "SetTitle");				// Added
	JS_FLINK(CSetUIState, "SetUIState");			// required
	JS_FLINK(CSubmitItem, "SubmitItem");			// required
	JS_FLINK(CTransmute, "Transmute");				// required
	//JS_FLINK(CUnregisterEvent, "UnregisterEvent");// required
	JS_FLINK(CMe, "GetMe");							// Testing for me.global

	//Functions Added not in D2NT
	JS_FLINK(CUseSkillPoint, "UseSkillPoint");		// Added
	JS_FLINK(CUseStatPoint, "UseStatPoint");		// Added
	
	// For Testing ONLY
	JS_FLINK(CGetLevel, "GetLevel");				// Will be removed
	JS_FLINK(CTESTWAYPOINT, "WayPoint");			// Will be removed

	return Context::New(isolate, NULL, global);
}


JS_FUNC(CSelectRealm)
{
	if (Prof.Realm == 0)//Single Player
	{
		Control*  pControl = MENU::findControl(CONTROL_BUTTON, "SINGLE PLAYER", -1, 264, 324, 272, 35);
		if (pControl)
		{
			MENU::clickControl(pControl);
			return;
		}
	}
	Control* pControl = MENU::findControl(CONTROL_BUTTON, (char *)NULL, -1, 264, 391, 272, 25);
	if (pControl && pControl->wText2) {
		char* cstr = "";
		char* szLine = wchart_to_char(pControl->wText2);

		if (Prof.Realm == 1) { cstr = "GATEWAY: U.S. WEST"; }
		if (Prof.Realm == 2) { cstr = "GATEWAY: U.S. EAST"; }
		if (Prof.Realm == 3) { cstr = "GATEWAY: ASIA"; }
		if (Prof.Realm == 4) { cstr = "GATEWAY: EUROPE"; }

		if (strcmp(cstr, szLine) != 0) {
			MENU::clickControl(pControl);

			Sleep(2000);
			Input::SendMouseClick(295, 320 + ((Prof.Realm * 24) + 12), 0);//select correct realm
			//OK Button
			Control* pControl2 = MENU::findControl(CONTROL_BUTTON, (char *)NULL, -1, 281, 538, 96, 32);
			if (pControl2) {
				MENU::clickControl(pControl2);
				Sleep(2000);
			}

		}
	}
		Control* pControl2 = MENU::findControl(CONTROL_BUTTON, "BATTLE.NET", -1, 264, 366, 272, 35);
		if (pControl2)
			MENU::clickControl(pControl2);
		Sleep(2000);
		return;


}

JS_FUNC(CClickMap)
{
	HandleScope handle_scope(args.GetIsolate());

	uint16_t nClickType = NULL, nShift = NULL, nX = NULL, nY = NULL;
	if (args.Length() < 3)
		args.GetReturnValue().Set(Boolean::New(false));
	
	if (args[0]->IsNumber())
		nClickType = args[0]->Uint32Value();
	if (args[1]->IsNumber() || args[1]->IsBoolean())
		nShift = args[1]->Uint32Value();
	if (args[2]->IsNumber())
		nX = args[2]->Uint32Value();
	if (args[3]->IsNumber())
		nY = args[3]->Uint32Value();

	if (args.Length() == 3 && args[0]->IsNumber() && (args[1]->IsNumber() || args[1]->IsBoolean()) && args[2]->IsObject())
	{
		//myUnit* mypUnit = (myUnit*)args[2]->ToObject();
		myUnit* mypUnit = NULL; //until above line is corrected

		if (!mypUnit || (mypUnit->_dwPrivateType & PRIVATE_UNIT) != PRIVATE_UNIT)
			args.GetReturnValue().Set(Boolean::New(true));

		UnitAny* pUnit = D2CLIENT_FindUnit(mypUnit->dwUnitId, mypUnit->dwType);

		if (!pUnit)
			args.GetReturnValue().Set(Boolean::New(true));


		args.GetReturnValue().Set(Boolean::New(ClickMap(nClickType, nX, nY, nShift, pUnit)));
	}
	else if (args.Length() > 3 && args[0]->IsNumber() && (args[1]->IsNumber() || args[1]->IsBoolean()) && args[2]->IsNumber() && args[3]->IsNumber())
	{
		args.GetReturnValue().Set(Boolean::New(ClickMap(nClickType, nX, nY, nShift, NULL)));
	}

	args.GetReturnValue().Set(Boolean::New(true));
}

JS_FUNC(CGetControl)// unsure if this will work - testing
{
	//MENU::locateControl();
	//HandleScope handle_scope(args.GetIsolate());
	//if (args.Length() == 5)
	//{
	//	Handle<Array> array = Array::New(5);
	//	INT32 type = args[0]->Uint32Value();
	//	INT32 locx = args[1]->Uint32Value();
	//	INT32 locy = args[2]->Uint32Value();
	//	INT32 sizex = args[3]->Uint32Value();
	//	INT32 aizey = args[4]->Uint32Value();

	//	for (Control* pControl = *vpFirstControl; pControl; pControl = pControl->pNext)
	//	{
	//		if (pControl && static_cast<int>(pControl->dwType) == type)
	//		{
	//			int x = pControl->dwPosX;
	//			int y = pControl->dwPosY;
	//			int bwidth = pControl->dwSizeX;
	//			int bheight = pControl->dwSizeY;
	//			array->Set(0, Integer::New(type));
	//			array->Set(1, Integer::New(x));
	//			array->Set(2, Integer::New(y));
	//			array->Set(3, Integer::New(bwidth));
	//			array->Set(4, Integer::New(bheight));
	//			args.GetReturnValue().Set(array);
	//			return;

	//		}
	//	}

	//}

	//args.GetReturnValue().Set(NULL);
	//return;

}


JS_FUNC(CGetDistance)
{

}

JS_FUNC(CGetLocaleString)
{
	HandleScope handle_scope(args.GetIsolate());
	if (args.Length() < 1)
		args.GetReturnValue().Set(Boolean::New(true));

	uint32_t localeId = args[0]->Uint32Value();
	wchar_t* wString = fpGetLocaleText(localeId);
	char* szTmp = UnicodeToAnsi(wString);
	args.GetReturnValue().Set(String::New(szTmp));

}
JS_FUNC(CSetUIState)
{
	HandleScope handle_scope(args.GetIsolate());
	DWORD state = args[0]->Uint32Value();
	bool set = args[1]->BooleanValue();
	//this looks odd yes, but I assure you, this is how it needs to be
	if (set == false)
		D2CLIENT_SetUIState(state, TRUE);
	else if(set == true) {
		D2CLIENT_SetUIState(state, FALSE);
		if (!D2CLIENT_GetUIState(UI_GAME))
			fpCloseInteract();
	}
}
JS_FUNC(CGetUIState)
{
	HandleScope handle_scope(args.GetIsolate());
	DWORD state = args[0]->Uint32Value();
	args.GetReturnValue().Set(Boolean::New(D2CLIENT_GetUIState(state)));
}
JS_FUNC(CUseStatPoint)
{
	HandleScope handle_scope(args.GetIsolate());
	uint32_t stat = 0;
	uint32_t count = 1;
	if (args.Length() != 2)
	{
		args.GetReturnValue().Set(Boolean::New(false));
	}
	else
	{
		stat = args[0]->Uint32Value();
		count = args[1]->Uint32Value();
		UseStatPoint(stat, count);
		D2Funcs::Print("ÿc5Stat Points Added"); //this line will be removed
		args.GetReturnValue().Set(Boolean::New(true));
	}
}

JS_FUNC(CUseSkillPoint)
{
	HandleScope handle_scope(args.GetIsolate());
	uint32_t skill = 0;
	uint32_t count = 1;
	if (args.Length() != 2)
	{
		args.GetReturnValue().Set(Boolean::New(false));
	}
	else
	{
		skill = args[0]->Uint32Value();
		count = args[1]->Uint32Value();
		UseSkillPoint(skill, count);
		D2Funcs::Print("ÿc5Skill Points Added"); //this line will be removed
		args.GetReturnValue().Set(Boolean::New(true));
	}
}

JS_FUNC(CGetBaseStat)
{
	
}

JS_FUNC(CGetPath)
{

}

JS_FUNC(CGetPresetUnits)
{
	Isolate* isolate = args.GetIsolate();
	HandleScope handle_scope(isolate);	

	Local<Object> node = Object::New();

	uint32_t levelId = NULL;
	uint32_t nClassId = NULL;
	uint32_t nType = NULL;

	if(args.Length() == 0)
		args.GetReturnValue().Set(Boolean::New(false));

	if (args.Length() >= 1)
		levelId = args[0]->Uint32Value();

	if(levelId == 0)
		args.GetReturnValue().Set(Boolean::New(false));

	Level* pLevel = GetLevel(levelId);

	if (!pLevel)
		args.GetReturnValue().Set(Boolean::New(false));

	if (args.Length() >= 2)
		nType = args[1]->Uint32Value();

	if (args.Length() >= 3)
		nClassId = args[2]->Uint32Value();
	
	bool bAddedRoom = FALSE;
	DWORD dwArrayCount = NULL;

	for (Room2 *pRoom = pLevel->pRoom2First; pRoom; pRoom = pRoom->pRoom2Next)
	{
		bAddedRoom = FALSE;

		if (!pRoom->pPreset)
		{
			fpAddRoomData(fpGetPlayerUnit()->pAct, pLevel->dwLevelNo, pRoom->dwPosX, pRoom->dwPosY, fpGetPlayerUnit()->pPath->pRoom1);
			bAddedRoom = TRUE;
		}

		for (PresetUnit* pUnit = pRoom->pPreset; pUnit; pUnit = pUnit->pPresetNext)
		{
			// Does it fit?
			if ((nType == NULL || pUnit->dwType == nType) && (nClassId == NULL || pUnit->dwTxtFileNo == nClassId))
			{
				myPresetUnit* presetUnit = new myPresetUnit;
				
				presetUnit->dwPosX = pUnit->dwPosX;
				presetUnit->dwPosY = pUnit->dwPosY;
				presetUnit->dwRoomX = pRoom->dwPosX;
				presetUnit->dwRoomY = pRoom->dwPosY;
				presetUnit->dwType = pUnit->dwType;
				presetUnit->dwId = pUnit->dwTxtFileNo;
				presetUnit->dwLevel = levelId;

				node->Set(String::NewFromUtf8(isolate, "areaid"), Integer::New(presetUnit->dwLevel));
				node->Set(String::NewFromUtf8(isolate, "type"), Integer::New(presetUnit->dwType));
				node->Set(String::NewFromUtf8(isolate, "id"), Integer::New(presetUnit->dwId));
				node->Set(String::NewFromUtf8(isolate, "x"), Integer::New(presetUnit->dwPosX));
				node->Set(String::NewFromUtf8(isolate, "y"), Integer::New(presetUnit->dwPosY));
				node->Set(String::NewFromUtf8(isolate, "roomx"), Integer::New(presetUnit->dwRoomX));
				node->Set(String::NewFromUtf8(isolate, "roomy"), Integer::New(presetUnit->dwRoomY));
				//subareaid crashes game atm
				//node->Set(String::NewFromUtf8(isolate, "subareaid"), Integer::New(fpGetPlayerUnit()->pPath->pRoom1->pRoom2->pLevel->pNextLevel->dwLevelNo));

				dwArrayCount++;
			}
		}

		if (bAddedRoom)
		{
			fpRemoveRoomData(fpGetPlayerUnit()->pAct, pLevel->dwLevelNo, pRoom->dwPosX, pRoom->dwPosY, fpGetPlayerUnit()->pPath->pRoom1);
			bAddedRoom = FALSE;
		}
	}
	args.GetReturnValue().Set(node);
}

JS_FUNC(CGetUnit)
{
	Local<Context> context = Context::GetCurrent();
	Isolate* isolate = args.GetIsolate();
	HandleScope scope(isolate);

	if (args.Length() < 1)
		args.GetReturnValue().Set(Boolean::New(true));

	int nType = -1;
	uint32_t nClassId = (uint32_t)-1;
	uint32_t nMode = (uint32_t)-1;
	uint32_t nUnitId = (uint32_t)-1;
	char szName[128] = "";
	if (args.Length() > 0 && args[0]->IsNumber())
		nType = args[0]->Uint32Value();

	if (args.Length() > 1 && args[1]->IsString()) {
		String::Utf8Value str(args[1]);
		char* cstr = (char*)ToCString(str);
		strcpy_s(szName, sizeof(szName), cstr);
	}

	if (args.Length() > 1 && args[1]->IsNumber())
		nClassId = args[1]->Uint32Value();

	if (args.Length() > 2 && args[2]->IsNumber())
		nMode = args[2]->Uint32Value();

	if (args.Length() > 3 && args[3]->IsNumber())
		nUnitId = args[3]->Uint32Value();

	UnitAny* pUnit = GetUnit(szName, nClassId, nType, nMode, nUnitId);

	if (nType == 100)
		pUnit = fpGetCursorItem();
	else if (nType == 101)
	{
		pUnit = fpGetSelectedUnit();
		if (!pUnit)
			pUnit = (*vpSelectedInvItem);
	}
	else
		pUnit = GetUnit(szName, nClassId, nType, nMode, nUnitId);

	if (!pUnit)
		args.GetReturnValue().Set(Boolean::New(true));

	myUnit* pmyUnit = new myUnit; // leaked?

	if (!pmyUnit)
		args.GetReturnValue().Set(Boolean::New(true));

	if (pUnit && pmyUnit)
	{
		pmyUnit->_dwPrivateType = PRIVATE_UNIT;
		pmyUnit->dwClassId = nClassId;
		pmyUnit->dwMode = nMode;
		pmyUnit->dwType = pUnit->dwType;
		pmyUnit->dwUnitId = pUnit->dwUnitId;
		strcpy_s(pmyUnit->szName, sizeof(pmyUnit->szName), szName);
	}

	Local<Object> node = Object::New();
	node->Set(String::NewFromUtf8(isolate, "name"), String::New(pUnit->pPlayerData->szName));
	node->Set(String::NewFromUtf8(isolate, "classid"), Integer::New(pmyUnit->dwClassId));
	node->Set(String::NewFromUtf8(isolate, "mode"), Integer::New(pmyUnit->dwMode));
	node->Set(String::NewFromUtf8(isolate, "unitid"), Integer::New(pmyUnit->dwUnitId));
	node->Set(String::NewFromUtf8(isolate, "type"), Integer::New(pmyUnit->dwType));
	node->Set(String::NewFromUtf8(isolate, "x"), Integer::New(fpGetUnitX(pUnit))); //just for testing
	node->Set(String::NewFromUtf8(isolate, "y"), Integer::New(fpGetUnitY(pUnit))); //just for testing
	args.GetReturnValue().Set(node);
}

JS_FUNC(CMe)
{
	Local<Context> context = Context::GetCurrent();
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);
	Local<Object> node = Object::New();
	node->SetAccessor(String::New("act"), GetAct);
	node->SetAccessor(String::New("name"), GetName);
	node->SetAccessor(String::New("charname"), GetName);
	node->SetAccessor(String::New("areaid"), GetAreaId);
	node->SetAccessor(String::New("x"), GetX);
	node->SetAccessor(String::New("y"), GetY);
	node->SetAccessor(String::New("hp"), GetHP);
	node->SetAccessor(String::New("mp"), GetMP);
	node->SetAccessor(String::New("hpmax"), GetHPMax);
	node->SetAccessor(String::New("mpmax"), GetMPMax);
	node->SetAccessor(String::New("classid"), GetClassid);
	node->SetAccessor(String::New("mode"), GetMode);
	args.GetReturnValue().Set(node);
}

JS_FUNC(CGold)
{
	HandleScope handle_scope(args.GetIsolate());

	int nGold = NULL;
	int nMode = 1;

	if (args.Length() > 0 && args[0]->IsNumber())
		nGold = args[0]->Uint32Value();

	if (args.Length() > 1 && args[1]->IsNumber())
		nMode = args[1]->Uint32Value();

	SendGold(nGold, nMode);

	args.GetReturnValue().Set(Boolean::New(true));
}

JS_FUNC(CSubmitItem)
{
	HandleScope handle_scope(args.GetIsolate());

	if (UnitAny* pUnit = fpGetCursorItem())
	{
		if (fpGetPlayerUnit()->dwAct == 1)
		{
			if (GetPlayerArea() == fpGetPlayerUnit()->pAct->pMisc->dwStaffTombLevel)
			{
				*vpCursorItemMode = 3;
				BYTE aPacket[17] = { NULL };
				aPacket[0] = 0x44;
				*(DWORD*)&aPacket[1] = fpGetPlayerUnit()->dwUnitId;
				*(DWORD*)&aPacket[5] = *vpOrificeId;
				*(DWORD*)&aPacket[9] = pUnit->dwUnitId;
				*(DWORD*)&aPacket[13] = 3;
				fpSendPacket(17, 1, aPacket);
				args.GetReturnValue().Set(Boolean::New(true));
			}
			else
				args.GetReturnValue().Set(Boolean::New(false));
		}
		else if (fpGetPlayerUnit()->dwAct == 0 || fpGetPlayerUnit()->dwAct == 4) // dwAct is 0-4, not 1-5
		{
			if (*vpRecentInteractId && fpIsTownByLevelNo(GetPlayerArea()))
			{
				fpSubmitItem(pUnit->dwUnitId);
				args.GetReturnValue().Set(Boolean::New(true));
			}
			else
				args.GetReturnValue().Set(Boolean::New(false));
		}
		else
			args.GetReturnValue().Set(Boolean::New(false));
	}
	else
		args.GetReturnValue().Set(Boolean::New(false));

	args.GetReturnValue().Set(Boolean::New(true));
}

JS_FUNC(CTransmute)
{
	HandleScope handle_scope(args.GetIsolate());

	bool cubeOn = !!D2CLIENT_GetUIState(UI_CUBE);
	if (!cubeOn)
		D2CLIENT_SetUIState(UI_CUBE, TRUE);

	D2CLIENT_Transmute();

	if (!cubeOn)
		D2CLIENT_SetUIState(UI_CUBE, FALSE);

	args.GetReturnValue().Set(Boolean::New(true));
}

JS_FUNC(CClientState)
{
	HandleScope handle_scope(args.GetIsolate());
	args.GetReturnValue().Set(v8::Int32::New(MENU::ClientState()));
}

JS_FUNC(CSetText)
{
	HandleScope handle_scope(args.GetIsolate());
	char* accName = Prof.Account;
	char* pass = Prof.AccPass;
	if (args.Length() == 2)
	{
		String::Utf8Value str(args[0]);
		accName = (char*)ToCString(str);
		String::Utf8Value str1(args[1]);
		pass = (char*)ToCString(str1);
	}
	//MENU::locateControl();
	Sleep(1000);
	// enter Account name
	Control* pControl = MENU::findControl(1, (char *)NULL, -1, 322, 342, 162, 19);
	if (!pControl)
		return;

	MENU::SetControlText(pControl, accName);
	Sleep(750);

	// enter pass
	pControl = MENU::findControl(1, (char *)NULL, -1, 322, 396, 162, 19);
	if (!pControl)
		return;

	MENU::SetControlText(pControl, pass);
	Sleep(1200);
	// click login
	pControl = MENU::findControl(CONTROL_BUTTON, "LOG IN", -1, 264, 484, 272, 35);
	if (pControl)
	{
		MENU::clickControl(pControl);
	}
	Sleep(500);
}

JS_FUNC(CGetText)
{
	HandleScope handle_scope(args.GetIsolate());
	Local<v8::Array> arr = v8::Local<v8::Array>::Cast(MENU::GetText(Prof.Charloc, Prof.Realm));
	args.GetReturnValue().Set(arr);

}

JS_FUNC(CSelectChar)
{
	HandleScope handle_scope(args.GetIsolate());
	int x = 175; int y = 125;
	int charpos = Prof.Charloc;
	if (args.Length() > 0)
	{
		charpos = args[0]->Uint32Value();
	}

	if (charpos == 1) { x = 445; }
	if (charpos == 3) { x = 445; }
	if (charpos == 5) { x = 445; }
	if (charpos == 7) { x = 445; }
	if (charpos == 2) { y = 215; }
	if (charpos == 4) { y = 305; }
	if (charpos == 6) { y = 395; }
	Input::SendMouseClick(x, y, 0);
	Sleep(100);
	Input::SendMouseClick(x, y, 0);
}

JS_FUNC(CGetLocation)
{
	HandleScope handle_scope(args.GetIsolate());
	int id = MENU::GetLocationID();
	args.GetReturnValue().Set(id);
	return;
}

JS_FUNC(CClickControl)
{
	HandleScope handle_scope(args.GetIsolate());
	Handle<v8::Value> obj(args[0]);

	if (obj->IsArray()) {

		Local<v8::Array> arr = v8::Local<v8::Array>::Cast(args[0]);

		int Type = (int)(arr->Get(0)->Int32Value());
		String::Utf8Value str(arr->Get(1));
		char* cstr = (char*)ToCString(str);
		int Disabled = (int)(arr->Get(2)->Int32Value());
		int PosX = (int)(arr->Get(3)->Int32Value());
		int PosY = (int)(arr->Get(4)->Int32Value());
		int SizeX = (int)(arr->Get(5)->Int32Value());
		int SizeY = (int)(arr->Get(6)->Int32Value());

		Control* pControl = MENU::findControl(Type, cstr, Disabled, PosX, PosY, SizeX, SizeY);

		if (pControl)
		{
			MENU::clickControl(pControl);
			args.GetReturnValue().Set(Boolean::New(true));
		}
	}

	args.GetReturnValue().Set(Boolean::New(false));
}

JS_FUNC(CGetLevel)
{
	//Just testing for now
	HandleScope handle_scope(args.GetIsolate());
	int Lvl = args[0]->Uint32Value();
	Level* lvl = D2Funcs::GetLevel(fpGetPlayerUnit()->pAct, Lvl);
	INT32  lvlno = lvl->dwLevelNo;
	args.GetReturnValue().Set(lvlno);
}

JS_FUNC(CRandom)
{
	HandleScope handle_scope(args.GetIsolate());
	INT32 range_min = args[0]->Uint32Value();
	INT32 range_max = args[1]->Uint32Value();

	INT32 random = rand() % range_min + range_max;

	args.GetReturnValue().Set(random);

}

JS_FUNC(CSay)
{
	HandleScope handle_scope(args.GetIsolate());
	if (args.Length() == 1)
	{
		HandleScope handle_scope(args.GetIsolate());
		String::Utf8Value str(args[0]);
		char* cstr = (char*)ToCString(str);
		Say(cstr);
		args.GetReturnValue().Set(Boolean::New(true));
	}
	args.GetReturnValue().Set(Boolean::New(false));
	return;
}

JS_FUNC(CGetArea)
{
	HandleScope handle_scope(args.GetIsolate());
	//Just testing for now
	INT32 areaid = D2Funcs::GetArea()->dwLevelNo;//Player Area num
	char AreaID[128];//Player Area num
	sprintf_s(AreaID, "%u", areaid);

	D2Funcs::Print(AreaID);

	DWORD unit = fpGetPlayerUnit()->pAct->dwAct;//Player Act
	char act[128];//Player Act
	sprintf_s(act, "%u", unit);

	D2Funcs::Print(act);

	DWORD parea = GetPlayerArea();
	char areap[128];//Player Area
	sprintf_s(areap, "%u", parea);
	D2Funcs::Print(areap);

	args.GetReturnValue().Set(v8::Int32::New(areaid));
}

JS_FUNC(CTESTWAYPOINT)
{
	HandleScope handle_scope(args.GetIsolate());
	INT32 wp = args[0]->Uint32Value();
	D2CLIENT_TakeWaypoint(0, wp);
	if (!D2CLIENT_GetUIState(UI_GAME))
		fpCloseInteract();

}
JS_FUNC(CGetPlayerUnit)
{
	HandleScope handle_scope(args.GetIsolate());
	v8::Local<v8::Object> fc = Object::New();
	//still missing some
	fc->SetAccessor(String::New("act"), GetAct);
	fc->SetAccessor(String::New("name"), GetName);
	fc->SetAccessor(String::New("areaid"), GetAreaId);
	fc->SetAccessor(String::New("x"), GetX);
	fc->SetAccessor(String::New("y"), GetY);
	fc->SetAccessor(String::New("hp"), GetHP);
	fc->SetAccessor(String::New("mp"), GetMP);
	fc->SetAccessor(String::New("hpmax"), GetHPMax);
	fc->SetAccessor(String::New("mpmax"), GetMPMax);
	fc->SetAccessor(String::New("classid"), GetClassid);
	fc->SetAccessor(String::New("mode"), GetMode);
	Local<String> props = Handle<String>::Cast(fc);
	args.GetReturnValue().Set(fc);
}
JS_FUNC(CExitGame)
{
	if (ClientGameState() != ClientStateMenu)
		fpExitGame();
}

JS_FUNC(CSetSkill)
{
	HandleScope handle_scope(args.GetIsolate());
	if (args.Length() == 2)
	{
		int skillid = args[0]->Uint32Value();
		bool bLeft = args[1]->BooleanValue();
		D2Funcs::SetSkill(skillid, bLeft);
		args.GetReturnValue().Set(Boolean::New(true));
	}

	args.GetReturnValue().Set(Boolean::New(false));
	return;
}

JS_FUNC(CMove)
{
	HandleScope handle_scope(args.GetIsolate());
	UnitAny* me = fpGetPlayerUnit();

	if (args.Length() == 2)
	{
		int x = args[0]->Uint32Value();
		int y = args[1]->Uint32Value();
		for (int i = 0; i < 5 && (fpGetUnitX(me) != x) && (fpGetUnitY(me) != y); i++) {
			D2Funcs::MoveTo(x, y);
			fpCloseInteract();
			Sleep(100);
		}
		args.GetReturnValue().Set(Boolean::New(true));
	}
	else {
		D2Funcs::MoveTo(fpGetUnitX(me) + 4, fpGetUnitY(me) - 2);
		if (!D2CLIENT_GetUIState(UI_GAME))
			fpCloseInteract();
		args.GetReturnValue().Set(Boolean::New(true));
	}
	return;
}
JS_FUNC(CScreenSize)
{
	HandleScope handle_scope(args.GetIsolate());
	v8::Handle<v8::Array> result = v8::Array::New(2);
	int x = *vpScreenSizeX;
	int y = *vpScreenSizeY;

	result->Set(0, Integer::New(x));
	result->Set(1, Integer::New(y));
	args.GetReturnValue().Set(result);
	return;

}

JS_FUNC(CCloseD2)
{
	HandleScope handle_scope(args.GetIsolate());
	if (ClientGameState() != ClientStateMenu)
		fpExitGame();
	TerminateProcess(GetCurrentProcess(), 0);
}

JS_FUNC(CDelay)
{
	HandleScope handle_scope(args.GetIsolate());
	if (args.Length() == 1)
	{
		int x = args[0]->Uint32Value();
		Sleep(x);
	}
	else if (args.Length() == 2)
	{
		int x = args[0]->Uint32Value();
		int b = args[1]->Uint32Value();
		int random = rand() % x + b;
		Sleep(random);
	}
	return;
}

JS_FUNC(CLoad) {

	HandleScope handle_scope(args.GetIsolate());
	String::Utf8Value str(args[0]);
	char* cstr = (char*)ToCString(str);
	StringReplace(cstr, '/', '\\', strlen(cstr));

	Isolate* isolate = Isolate::GetCurrent();
	isolate->Exit();
	Isolate* newisolate = Isolate::New();

	Handle<Context> context = CreateContext(args.GetIsolate());
	TryCatch try_catch;
	if (context.IsEmpty())
	{
		ReportException(isolate, &try_catch);
	}
	else
	{
		context->Enter();
	}

	RunScript(newisolate, Vars.szScriptPath, cstr);

	Handle<v8::Object> global = context->Global();
	Handle<v8::Value> value = global->Get(String::New("NTMain"));
	if (value->IsFunction()) {
		Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(value);
		func->Call(global, 0, NULL);

	}
	context->Exit();
	newisolate->Dispose();
}

JS_FUNC(CInclude) {
	HandleScope handle_scope(args.GetIsolate());
	String::Utf8Value str(args[0]);
	char* cstr = (char*)ToCString(str);
	StringReplace(cstr, '/', '\\', strlen(cstr));
	Isolate* isolate = Isolate::GetCurrent();
	RunScript(isolate, Vars.szScriptPath, cstr);
}

JS_FUNC(CSetTitle)
{
	HandleScope handle_scope(args.GetIsolate());
	int a = 0;
	if (args.Length() == 1)
	{
		String::Utf8Value title(args[0]);

		a = SetWindowText(fpGetHwnd(), ToCString(title));
	}
	if (a == 0)
	{
		args.GetReturnValue().Set(Boolean::New(false));
		return;
	}
	args.GetReturnValue().Set(Boolean::New(true));
	return;
}

JS_FUNC(CSendCopyData) {

	HandleScope handle_scope(args.GetIsolate());

	if (args.Length() == 3)
	{
		String::Utf8Value destapp(args[0]);
		char* str1 = (char*)ToCString(destapp);
		int x = args[1]->Uint32Value();
		String::Utf8Value string(args[2]);
		char* str2 = (char*)ToCString(string);
		//MessageBox(NULL, str1, "Debug", NULL);
		SendDataCopy(str1, x, str2);
		args.GetReturnValue().Set(Boolean::New(true));
		return;
	}

	args.GetReturnValue().Set(Boolean::New(false));
	return;
}

JS_FUNC(CClick) {

	HandleScope handle_scope(args.GetIsolate());

	if (args.Length() == 3)
	{
		int x = args[0]->Uint32Value();
		int y = args[1]->Uint32Value();
		int clickmode = args[2]->Uint32Value();
		Input::SendMouseClick(x, y, clickmode);
		args.GetReturnValue().Set(Boolean::New(true));
	}

	args.GetReturnValue().Set(Boolean::New(false));
	return;
}

JS_FUNC(CGetTickCount)
{
	HandleScope handle_scope(args.GetIsolate());
	args.GetReturnValue().Set(v8::Int32::New(GetTickCount()));
}


JS_FUNC(CPrint)
{
	HandleScope handle_scope(args.GetIsolate());
	String::Utf8Value str(args[0]);
	char* cstr = (char*)ToCString(str);
	D2Funcs::Print("%s", cstr);;
}
