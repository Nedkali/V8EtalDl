//////////////////////////////////////////////////////////////////////
// JS Functions
//////////////////////////////////////////////////////////////////////
#include <algorithm>
#include <assert.h>
#include <shlwapi.h>
#include "JSMethods.h"
#include "JSGlobals.h"
#include "MenuControls.h"
#include "V8Script.h"
#include "D2Helpers.h"
#include "Helpers.h"
#include "D2Funcs.h"
#include "Accessors.h"
#include "Map\MapHeader.h"


#pragma warning ( push )
#pragma warning ( disable: 4244 )

bool loaded = false;
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
	JS_FLINK(CGetRoom, "GetRoom");					// required
	//JS_FLINK(CGetScript, "GetScript");			// required
	JS_FLINK(CGetText, "GetText");
	JS_FLINK(CGetTickCount, "GetTickCount");		// required
	JS_FLINK(CGetUIState, "GetUIState");			// required
	JS_FLINK(CGetUnit, "GetUnit");					// required
	JS_FLINK(CGetWayPoint, "GetWaypoint");			// required
	JS_FLINK(CGold, "Gold");						// required
	JS_FLINK(CInclude, "Include");					// required
	JS_FLINK(CLoad, "Load");						// required
	JS_FLINK(CMove, "Move");
	JS_FLINK(CPrint, "Print");						// required
	JS_FLINK(CRandom, "Random");					// required
	//JS_FLINK(CRegisterEvent, "RegisterEvent");	// required
	JS_FLINK(CRunGC, "RunGC");						// required
	JS_FLINK(CSay, "Say");							// required
	JS_FLINK(CScreenSize, "ScreenSize");
	JS_FLINK(CSelectChar, "SelectChar");
	JS_FLINK(CSelectRealm, "SelectRealm");
	JS_FLINK(CSendCopyData, "SendCopyData");		// required
	//JS_FLINK(CSetStatusText, "SetStatusText");	// required
	JS_FLINK(CSetText, "SetText");
	JS_FLINK(CSetTitle, "SetTitle");				// Added
	JS_FLINK(CSetUIState, "SetUIState");			// required
	JS_FLINK(CSubmitItem, "SubmitItem");			// required
	JS_FLINK(CTransmute, "Transmute");				// required
	//JS_FLINK(CUnregisterEvent, "UnregisterEvent");// required

	JS_FLINK(CMe, "InitMeOnce");					// required for me global

	//Functions Added not in D2NT
	JS_FLINK(CUseSkillPoint, "UseSkillPoint");		// Added
	JS_FLINK(CUseStatPoint, "UseStatPoint");		// Added
	JS_FLINK(CGetScriptName, "GetScriptName");		// Added
	JS_FLINK(CGetRealmIndex, "GetRealmIndex");		// Added
	JS_FLINK(CSetDifficulty, "SetDifficulty");

	// For Testing ONLY
	JS_FLINK(CGetLevel, "GetLevel");				// Will be removed

	return Context::New(isolate, NULL, global);
}

JS_FUNC(CSetDifficulty)//SinglePlayer ? modify for online selecting also
{
	//need some checks here so we can utilise for online selecting
	HandleScope handle_scope(args.GetIsolate());
	int dif = Prof.Difficulty;

	if (args.Length() == 1 && args[0]->IsNumber())
	{
		dif = args[0]->Uint32Value();
	}

	Control* pControl;

	if (Prof.Realm == 0)
	{
		switch (dif)
		{
		case 0:
			pControl = MENU::findControl(CONTROL_BUTTON, "NORMAL", -1, 264, 297, 272, 35);
			break;
		case 1:
			pControl = MENU::findControl(CONTROL_BUTTON, "NIGHTMARE", -1, 264, 340, 272, 35);
			break;
		case 2:
			pControl = MENU::findControl(CONTROL_BUTTON, "HELL", -1, 264, 383, 272, 35);
			break;
		default://else find highest available
			pControl = MENU::findControl(CONTROL_BUTTON, "HELL", -1, 264, 383, 272, 35);
			if (!pControl)
			{
				pControl = MENU::findControl(CONTROL_BUTTON, "NIGHTMARE", -1, 264, 340, 272, 35);
				if (!pControl)
				{
					pControl = MENU::findControl(CONTROL_BUTTON, "NORMAL", -1, 264, 297, 272, 35);
				}
			}
			break;
		}
		if (pControl)
		{
			MENU::clickControl(pControl);
			return args.GetReturnValue().Set(Boolean::New(true));
		}
		return args.GetReturnValue().Set(Boolean::New(false));
	}

}

JS_FUNC(CGetRoom)
{
	Isolate* isolate = args.GetIsolate();
	HandleScope handle_scope(isolate);

	if (args.Length() == 1 && args[0]->IsNumber())
	{
		uint32_t levelId;
		levelId = args[0]->Uint32Value();

		if (levelId != 0) // 1 Parameter, AreaId
		{
			Level* pLevel = GetLevel(levelId);

			if (!pLevel || !pLevel->pRoom2First)
				return args.GetReturnValue().Set(Boolean::New(false));

			Room2 *pRoom2 = (Room2*)pLevel->pRoom2First;

			Vars.pRoom2 = pRoom2;
			ObjectTemplate* node = *ObjectTemplate::New();
			node->Set(String::NewFromUtf8(isolate, "areaid"), Integer::New(pRoom2->pLevel->dwLevelNo));
			node->Set(String::NewFromUtf8(isolate, "correcttomb"), Integer::New(pRoom2->pLevel && pRoom2->pLevel->pMisc && pRoom2->pLevel->pMisc->dwStaffTombLevel ? pRoom2->pLevel->pMisc->dwStaffTombLevel : -1));
			node->Set(String::NewFromUtf8(isolate, "x"), Integer::New(pRoom2->dwPosX));
			node->Set(String::NewFromUtf8(isolate, "xsize"), Integer::New(pRoom2->dwSizeX * 5));
			node->Set(String::NewFromUtf8(isolate, "y"), Integer::New(pRoom2->dwPosY));
			node->Set(String::NewFromUtf8(isolate, "ysize"), Integer::New(pRoom2->dwSizeY * 5));
			node->Set(String::NewFromUtf8(isolate, "number"), Integer::New(pRoom2->dwPresetType != 2 ? -1 : pRoom2->pType2Info->dwRoomNumber));
			//methods
			node->Set(String::NewFromUtf8(isolate, "GetFirst"), FunctionTemplate::New(roomGetFirst, args.This()));
			node->Set(String::NewFromUtf8(isolate, "GetNearby"), FunctionTemplate::New(roomGetNearby, args.This()));
			node->Set(String::NewFromUtf8(isolate, "GetNext"), FunctionTemplate::New(roomGetNext, args.This()));
			node->Set(String::NewFromUtf8(isolate, "GetPresetUnits"), FunctionTemplate::New(roomGetPresetUnits, args.This()));
			node->Set(String::NewFromUtf8(isolate, "UnitInRoom"), FunctionTemplate::New(roomUnitInRoom, args.This()));
			args.GetReturnValue().Set(node->NewInstance());
		}
		else if (levelId == 0)
		{
			Room1* pRoom1 = D2COMMON_GetRoomFromUnit(fpGetPlayerUnit());

			if (!pRoom1 || !pRoom1->pRoom2)
				return args.GetReturnValue().Set(Boolean::New(false));

			ObjectTemplate* node = *ObjectTemplate::New();
			node->Set(String::NewFromUtf8(isolate, "areaid"), Integer::New(pRoom1->pRoom2->pLevel->dwLevelNo));
			node->Set(String::NewFromUtf8(isolate, "correcttomb"), Integer::New(pRoom1->pRoom2->pLevel && pRoom1->pRoom2->pLevel->pMisc && pRoom1->pRoom2->pLevel->pMisc->dwStaffTombLevel ? pRoom1->pRoom2->pLevel->pMisc->dwStaffTombLevel : -1));
			node->Set(String::NewFromUtf8(isolate, "x"), Integer::New(pRoom1->pRoom2->dwPosX));
			node->Set(String::NewFromUtf8(isolate, "xsize"), Integer::New(pRoom1->pRoom2->dwSizeX * 5));
			node->Set(String::NewFromUtf8(isolate, "y"), Integer::New(pRoom1->pRoom2->dwPosY));
			node->Set(String::NewFromUtf8(isolate, "ysize"), Integer::New(pRoom1->pRoom2->dwSizeY * 5));
			node->Set(String::NewFromUtf8(isolate, "number"), Integer::New(pRoom1->pRoom2->dwPresetType != 2 ? -1 : pRoom1->pRoom2->pType2Info->dwRoomNumber));
			//methods
			node->Set(String::NewFromUtf8(isolate, "GetFirst"), FunctionTemplate::New(roomGetFirst, args.This()));
			node->Set(String::NewFromUtf8(isolate, "GetNearby"), FunctionTemplate::New(roomGetNearby, args.This()));
			node->Set(String::NewFromUtf8(isolate, "GetNext"), FunctionTemplate::New(roomGetNext, args.This()));
			node->Set(String::NewFromUtf8(isolate, "GetPresetUnits"), FunctionTemplate::New(roomGetPresetUnits, args.This()));
			node->Set(String::NewFromUtf8(isolate, "UnitInRoom"), FunctionTemplate::New(roomUnitInRoom, args.This()));
			Vars.pRoom2 = pRoom1->pRoom2;
			args.GetReturnValue().Set(node->NewInstance());
		}
	}
	else if (args.Length() == 3 || args.Length() == 2) // area ,x and y
	{
		Level* pLevel = NULL;

		uint32_t levelId;
		levelId = args[0]->Uint32Value();


		if (args.Length() == 3)
			pLevel = GetLevel(levelId);
		else if (fpGetPlayerUnit() && fpGetPlayerUnit()->pPath && fpGetPlayerUnit()->pPath->pRoom1 && fpGetPlayerUnit()->pPath->pRoom1->pRoom2)
			pLevel = fpGetPlayerUnit()->pPath->pRoom1->pRoom2->pLevel;

		if (!pLevel || !pLevel->pRoom2First)
			return args.GetReturnValue().Set(Boolean::New(false));

		uint32_t nX = NULL;
		uint32_t nY = NULL;
		if (args.Length() == 2)
		{
			nX = args[0]->Uint32Value();
			nY = args[1]->Uint32Value();
		}
		else if (args.Length() == 3)
		{
			nX = args[1]->Uint32Value();
			nY = args[2]->Uint32Value();
		}
		if (!nX || !nY)
			return args.GetReturnValue().Set(Boolean::New(false));

		// Scan for the room with the matching x,y coordinates.
		for (Room2* pRoom = pLevel->pRoom2First; pRoom; pRoom = pRoom->pRoom2Next)
		{
			bool bAdded = FALSE;
			if (!pRoom->pRoom1)
			{
				fpAddRoomData(fpGetPlayerUnit()->pAct, pLevel->dwLevelNo, pRoom->dwPosX, pRoom->dwPosY, fpGetPlayerUnit()->pPath->pRoom1);
				bAdded = TRUE;
			}

			CollMap* map = pRoom->pRoom1->Coll;
			if (nX >= map->dwPosGameX && nY >= map->dwPosGameY &&
				nX < (map->dwPosGameX + map->dwSizeGameX) && nY < (map->dwPosGameY + map->dwSizeGameY))
			{
				if (bAdded)
					fpRemoveRoomData(fpGetPlayerUnit()->pAct, pLevel->dwLevelNo, pRoom->dwPosX, pRoom->dwPosY, fpGetPlayerUnit()->pPath->pRoom1);

				Vars.pRoom2 = pRoom;
				ObjectTemplate* node = *ObjectTemplate::New();
				node->Set(String::NewFromUtf8(isolate, "areaid"), Integer::New(pRoom->pLevel->dwLevelNo));
				node->Set(String::NewFromUtf8(isolate, "correcttomb"), Integer::New(pRoom->pLevel && pRoom->pLevel->pMisc && pRoom->pLevel->pMisc->dwStaffTombLevel ? pRoom->pLevel->pMisc->dwStaffTombLevel : -1));
				node->Set(String::NewFromUtf8(isolate, "x"), Integer::New(pRoom->dwPosX));
				node->Set(String::NewFromUtf8(isolate, "xsize"), Integer::New(pRoom->dwSizeX * 5));
				node->Set(String::NewFromUtf8(isolate, "y"), Integer::New(pRoom->dwPosY));
				node->Set(String::NewFromUtf8(isolate, "ysize"), Integer::New(pRoom->dwSizeY * 5));
				node->Set(String::NewFromUtf8(isolate, "number"), Integer::New(pRoom->dwPresetType != 2 ? -1 : pRoom->pType2Info->dwRoomNumber));
				//methods
				node->Set(String::NewFromUtf8(isolate, "GetFirst"), FunctionTemplate::New(roomGetFirst, args.This()));
				node->Set(String::NewFromUtf8(isolate, "GetNearby"), FunctionTemplate::New(roomGetNearby, args.This()));
				node->Set(String::NewFromUtf8(isolate, "GetNext"), FunctionTemplate::New(roomGetNext, args.This()));
				node->Set(String::NewFromUtf8(isolate, "GetPresetUnits"), FunctionTemplate::New(roomGetPresetUnits, args.This()));
				node->Set(String::NewFromUtf8(isolate, "UnitInRoom"), FunctionTemplate::New(roomUnitInRoom, args.This()));
				args.GetReturnValue().Set(node->NewInstance());
			}

			if (bAdded)
				fpRemoveRoomData(fpGetPlayerUnit()->pAct, pLevel->dwLevelNo, pRoom->dwPosX, pRoom->dwPosY, fpGetPlayerUnit()->pPath->pRoom1);
		}
		
		Room2 *pRoom2 = (Room2*)pLevel->pRoom2First;
		Vars.pRoom2 = pRoom2;
		ObjectTemplate* node = *ObjectTemplate::New();
		node->Set(String::NewFromUtf8(isolate, "areaid"), Integer::New(pRoom2->pLevel->dwLevelNo));
		node->Set(String::NewFromUtf8(isolate, "correcttomb"), Integer::New(pRoom2->pLevel && pRoom2->pLevel->pMisc && pRoom2->pLevel->pMisc->dwStaffTombLevel ? pRoom2->pLevel->pMisc->dwStaffTombLevel : -1));
		node->Set(String::NewFromUtf8(isolate, "x"), Integer::New(pRoom2->dwPosX));
		node->Set(String::NewFromUtf8(isolate, "xsize"), Integer::New(pRoom2->dwSizeX * 5));
		node->Set(String::NewFromUtf8(isolate, "y"), Integer::New(pRoom2->dwPosY));
		node->Set(String::NewFromUtf8(isolate, "ysize"), Integer::New(pRoom2->dwSizeY * 5));
		node->Set(String::NewFromUtf8(isolate, "number"), Integer::New(pRoom2->dwPresetType != 2 ? -1 : pRoom2->pType2Info->dwRoomNumber));
		//methods
		node->Set(String::NewFromUtf8(isolate, "GetFirst"), FunctionTemplate::New(roomGetFirst, args.This()));
		node->Set(String::NewFromUtf8(isolate, "GetNearby"), FunctionTemplate::New(roomGetNearby, args.This()));
		node->Set(String::NewFromUtf8(isolate, "GetNext"), FunctionTemplate::New(roomGetNext, args.This()));
		node->Set(String::NewFromUtf8(isolate, "GetPresetUnits"), FunctionTemplate::New(roomGetPresetUnits, args.This()));
		node->Set(String::NewFromUtf8(isolate, "UnitInRoom"), FunctionTemplate::New(roomUnitInRoom, args.This()));
		args.GetReturnValue().Set(node->NewInstance());
	}
	else {
		Level* pLevel = GetLevel(fpGetPlayerUnit()->pPath->pRoom1->pRoom2->pLevel->dwLevelNo);

		Room2 *pRoom2 = (Room2*)pLevel->pRoom2First;

		if (!pRoom2)
			return args.GetReturnValue().Set(Boolean::New(false));

		Vars.pRoom2 = pRoom2;
		ObjectTemplate* node = *ObjectTemplate::New();
		node->Set(String::NewFromUtf8(isolate, "areaid"), Integer::New(pRoom2->pLevel->dwLevelNo));
		node->Set(String::NewFromUtf8(isolate, "correcttomb"), Integer::New(pRoom2->pLevel && pRoom2->pLevel->pMisc && pRoom2->pLevel->pMisc->dwStaffTombLevel ? pRoom2->pLevel->pMisc->dwStaffTombLevel : -1));
		node->Set(String::NewFromUtf8(isolate, "x"), Integer::New(pRoom2->dwPosX));
		node->Set(String::NewFromUtf8(isolate, "xsize"), Integer::New(pRoom2->dwSizeX * 5));
		node->Set(String::NewFromUtf8(isolate, "y"), Integer::New(pRoom2->dwPosY));
		node->Set(String::NewFromUtf8(isolate, "ysize"), Integer::New(pRoom2->dwSizeY * 5));
		node->Set(String::NewFromUtf8(isolate, "number"), Integer::New(pRoom2->dwPresetType != 2 ? -1 : pRoom2->pType2Info->dwRoomNumber));
		//methods
		node->Set(String::NewFromUtf8(isolate, "GetFirst"), FunctionTemplate::New(roomGetFirst, args.This()));
		node->Set(String::NewFromUtf8(isolate, "GetNearby"), FunctionTemplate::New(roomGetNearby, args.This()));
		node->Set(String::NewFromUtf8(isolate, "GetNext"), FunctionTemplate::New(roomGetNext, args.This()));
		node->Set(String::NewFromUtf8(isolate, "GetPresetUnits"), FunctionTemplate::New(roomGetPresetUnits, args.This()));
		node->Set(String::NewFromUtf8(isolate, "UnitInRoom"), FunctionTemplate::New(roomUnitInRoom, args.This()));

		args.GetReturnValue().Set(node->NewInstance());
	}

}



JS_FUNC(CGetPath)
{

	Isolate* isolate = args.GetIsolate();
	HandleScope handle_scope(isolate);
	if (args.Length() < 6)
		return args.GetReturnValue().Set(Boolean::New(false));

	UINT lvl = 0, x = 0, y = 0, dx = 0, dy = 0, reductionType = 0, radius = 20;

	lvl = args[0]->Uint32Value();
	x = args[1]->Uint32Value();
	y = args[2]->Uint32Value();
	dx = args[3]->Uint32Value();
	dy = args[4]->Uint32Value();
	reductionType = args[5]->BooleanValue();

	Level* level = GetLevel(lvl);
	if (!level)
		return args.GetReturnValue().Set(Boolean::New(false));

	ActMap* map = ActMap::GetMap(level);

	Point start(x, y), end(dx, dy);
	PathReducer* reducer = NULL;

	switch (reductionType)
	{
	case 0: reducer = new WalkPathReducer(map, DiagonalShortcut, radius); break;
	case 1: reducer = new TeleportPathReducer(map, DiagonalShortcut, radius); break;
	case 2: reducer = new NoPathReducer(map); break;
	default:
		return args.GetReturnValue().Set(Boolean::New(false));
	}

	PointList list;

	AStarPath<> path(map, reducer);

	path.GetPath(start, end, list, true);

	int count = list.size();

	Local<Array> nodes = Array::New(list.size());
	for (int i = 0; i < count; i++)
	{
		Local<Array> node = Array::New();
		node->Set(0, Integer::New(list[i].first));
		node->Set(1, Integer::New(list[i].second));
		nodes->Set(i, node);
	}
	args.GetReturnValue().Set(nodes);
	delete reducer;
	map->CleanUp();
}

JS_FUNC(CGetRealmIndex)
{
	int realm = 0;
	Control* pControl = MENU::findControl(CONTROL_BUTTON, (char *)NULL, -1, 264, 391, 272, 25);
	if (pControl && pControl->wText2) {
		char* cstr = "";
		char* szLine = wchart_to_char(pControl->wText2);

		if (strcmp("GATEWAY: U.S. WEST", szLine) == 0) { realm = 1; }
		if (strcmp("GATEWAY: U.S. EAST", szLine) == 0) { realm = 2; }
		if (strcmp("GATEWAY: ASIA", szLine) == 0) { realm = 3; }
		if (strcmp("GATEWAY: EUROPE", szLine) == 0) { realm = 4; }

	}

	args.GetReturnValue().Set(v8::Int32::New(realm));

}

JS_FUNC(CGetScriptName)
{
	HandleScope handle_scope(args.GetIsolate());
	v8::Local<v8::String> str = v8::StackTrace::CurrentStackTrace(1,
		v8::StackTrace::kScriptName)->GetFrame(0)->GetScriptName();

	args.GetReturnValue().Set(str);

}

JS_FUNC(CRunGC)
{
}

JS_FUNC(CGetWayPoint)
{
	HandleScope handle_scope(args.GetIsolate());
	if (args.Length() < 1 || !args[0]->IsNumber())
	{
		args.GetReturnValue().Set(Boolean::New(false));
	}
	
	int nWaypointId = args[0]->Uint32Value();

	if (nWaypointId > 40)
		nWaypointId = NULL;

	args.GetReturnValue().Set(Boolean::New(!!fpCheckWaypoint((*vpWaypointTable), nWaypointId)));
}

JS_FUNC(CSelectRealm)
{
	HandleScope handle_scope(args.GetIsolate());

	if (MENU::GetLocationID() == 27)
	{
		Sleep(50);
		Input::SendMouseClick(295, 320 + ((Prof.Realm * 24) + 12), 0);//select correct realm
																		  //OK Button
		Control* pControl2 = MENU::findControl(CONTROL_BUTTON, (char *)NULL, -1, 281, 538, 96, 32);
		if (pControl2) {
			MENU::clickControl(pControl2);
			Sleep(800);
		}

	}
	//Control* pControl2 = MENU::findControl(CONTROL_BUTTON, "BATTLE.NET", -1, 264, 366, 272, 35);
	//if (pControl2)
	//	MENU::clickControl(pControl2);
	//Sleep(2000);
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
		//UnitAny* pUnit = (UnitAny*)*args[2]->ToObject(); // Vars.thisUnit;

		UnitAny* pUnit = Vars.thisUnit;


		//D2Funcs::Print("%i", pUnit->wX); //unit x coord
		//D2Funcs::Print("%i", pUnit->wY); //unit y coord
		//D2Funcs::Print("%i", pUnit->dwTxtFileNo); //unit classid
		//D2Funcs::Print("%s", pUnit->pPlayerData->szName); //unit name

		if (pUnit)
			return args.GetReturnValue().Set(Boolean::New(ClickMap(nClickType, nX, nY, nShift, pUnit)));
	}
	else if (args.Length() > 3 && args[0]->IsNumber() && (args[1]->IsNumber() || args[1]->IsBoolean()) && args[2]->IsNumber() && args[3]->IsNumber())
	{
		return args.GetReturnValue().Set(Boolean::New(ClickMap(nClickType, nX, nY, nShift, NULL)));
	}
}

JS_FUNC(CGetControl)// unsure if this will work - testing
{
	MENU::locateControl();
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
	HandleScope handle_scope(args.GetIsolate());
	int32_t nX1 = NULL;
	int32_t nX2 = NULL;
	int32_t nY1 = NULL;
	int32_t nY2 = NULL;
	
	//Needs work
	//2 args : first arg has to be me global atm
	if (args.Length() == 2)
	{
		if (args[0]->IsObject() && args[1]->IsObject())
		{
			UnitAny* me = fpGetPlayerUnit();
			UnitAny* pUnit = Vars.thisUnit;

			if (pUnit)
			{
				nX1 = fpGetUnitX(me);
				nY1 = fpGetUnitY(me);
				nX2 = fpGetUnitX(pUnit);
				nY2 = fpGetUnitY(pUnit);
			}
		}
	}
	if (args.Length() == 3)
	{
		if (args[0]->IsNumber() && args[1]->IsNumber() && args[2]->IsObject())
		{
			UnitAny* pUnit = Vars.thisUnit;
			if (pUnit)
			{
				nX1 = args[0]->Uint32Value();
				nY1 = args[1]->Uint32Value();
				nX2 = fpGetUnitX(pUnit);
				nY2 = fpGetUnitY(pUnit);
			}
		}
	}
	if (args.Length() == 4)
	{
		if (args[0]->IsNumber() && args[1]->IsNumber() && args[2]->IsNumber() && args[3]->IsNumber())
		{
			nX1 = args[0]->Uint32Value();
			nY1 = args[1]->Uint32Value();
			nX2 = args[2]->Uint32Value();
			nY2 = args[3]->Uint32Value();
		}
	}

	double jsdist = (double)abs(GetDistance(nX1, nY1, nX2, nY2));
	
	args.GetReturnValue().Set(Integer::New(jsdist));

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
	if(!D2CLIENT_GetUIState(state))
		args.GetReturnValue().Set(Boolean::New(false));
	else if (D2CLIENT_GetUIState(state))
		args.GetReturnValue().Set(Boolean::New(true));
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
		//TODO: Add check for current available stat points
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
		//TODO: Add check for current available skill points
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

JS_FUNC(CGetPresetUnits)
{
	Isolate* isolate = args.GetIsolate();
	HandleScope handle_scope(isolate);

	uint32_t nMode = (uint32_t)-1;
	uint32_t nUnitId = (uint32_t)-1;
	uint32_t levelId = NULL;
	uint32_t nClassId = NULL;
	uint32_t nType = NULL;
	char szName[128] = "";
	char tmp[256] = "";

	if (args.Length() == 0)
		args.GetReturnValue().Set(Boolean::New(false));

	if (args.Length() >= 1)
		levelId = args[0]->Uint32Value();

	if (levelId == 0)
		args.GetReturnValue().Set(Boolean::New(false));

	Level* pLevel = GetLevel(levelId);

	if (!pLevel)
		args.GetReturnValue().Set(Boolean::New(false));

	if (args.Length() >= 2)
		nType = args[1]->Uint32Value();

	if (args.Length() >= 3)
		nClassId = args[2]->Uint32Value();

	bool bAddedRoom = FALSE;
	int dwArrayCount = 0;

	UnitAny* gpUnit = GetUnit(szName, nClassId, nType, nMode, nUnitId);

	Array* nodes = *Array::New();
	Vars.thisUnit = gpUnit;
	myUnit* pmyUnit = (myUnit*)gpUnit;

	int32_t nX1 = NULL;
	int32_t nY1 = NULL;
	int32_t nX2 = NULL;
	int32_t nY2 = NULL;
	double jsdist = -1;

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

				ObjectTemplate* node = *ObjectTemplate::New();
				presetUnit->dwPosX = pUnit->dwPosX;
				presetUnit->dwPosY = pUnit->dwPosY;
				presetUnit->dwRoomX = pRoom->dwPosX;
				presetUnit->dwRoomY = pRoom->dwPosY;
				presetUnit->dwType = pUnit->dwType;
				presetUnit->dwId = pUnit->dwTxtFileNo;
				presetUnit->dwLevel = fpGetPlayerUnit()->pPath->pRoom1->pRoom2->pLevel->dwLevelNo;
				Vars.nthisPresetX = (presetUnit->dwPosX) + (presetUnit->dwRoomX * 5);
				Vars.nthisPresetY = (presetUnit->dwPosY) + (presetUnit->dwRoomY * 5);

				gpUnit = GetUnit(szName, pUnit->dwTxtFileNo, pUnit->dwType, nMode, nUnitId);
				GetUnitName(gpUnit, tmp, 256);
				if (gpUnit) {
					Vars.thisUnit = gpUnit;
					nX1 = fpGetUnitX(fpGetPlayerUnit());
					nY1 = fpGetUnitY(fpGetPlayerUnit());
					nX2 = fpGetUnitX(gpUnit);
					nY2 = fpGetUnitY(gpUnit);
					jsdist = (double)abs(GetDistance(nX1, nY1, nX2, nY2));
				}
				node->Set(String::NewFromUtf8(isolate, "name"), String::New(tmp));
				node->Set(String::NewFromUtf8(isolate, "classid"), Integer::New(presetUnit->dwId));
				node->Set(String::NewFromUtf8(isolate, "mode"), Integer::New(nMode));
				//node->Set(String::NewFromUtf8(isolate, "unitid"), Integer::New(gpUnit->dwUnitId));
				node->Set(String::NewFromUtf8(isolate, "x"), Integer::New(presetUnit->dwPosX));
				node->Set(String::NewFromUtf8(isolate, "y"), Integer::New(presetUnit->dwPosY));
				node->Set(String::NewFromUtf8(isolate, "roomx"), Integer::New(presetUnit->dwRoomX));
				node->Set(String::NewFromUtf8(isolate, "roomy"), Integer::New(presetUnit->dwRoomY));
				node->Set(String::NewFromUtf8(isolate, "areaid"), Integer::New(presetUnit->dwLevel));
				node->Set(String::NewFromUtf8(isolate, "type"), Integer::New(presetUnit->dwType));
				node->Set(String::NewFromUtf8(isolate, "id"), Integer::New(presetUnit->dwId));
				node->Set(String::NewFromUtf8(isolate, "newx"), Integer::New(Vars.nthisPresetX));
				node->Set(String::NewFromUtf8(isolate, "newy"), Integer::New(Vars.nthisPresetY));
				node->Set(String::NewFromUtf8(isolate, "distance"), Integer::New(jsdist));
				//subareaid crashes game atm
				node->Set(String::NewFromUtf8(isolate, "subareaid"), Integer::New(pLevel->dwLevelNo+1));
				nodes->Set(dwArrayCount, node->NewInstance());
				dwArrayCount++;
			}
		}

		if (bAddedRoom)
		{
			fpRemoveRoomData(fpGetPlayerUnit()->pAct, pLevel->dwLevelNo, pRoom->dwPosX, pRoom->dwPosY, fpGetPlayerUnit()->pPath->pRoom1);
			bAddedRoom = FALSE;
		}
	}
	args.GetReturnValue().Set(nodes->ToObject());
}

JS_FUNC(CGetUnit)
{
	Isolate* isolate = args.GetIsolate();
	HandleScope scope(isolate);

	if (args.Length() < 1)
		args.GetReturnValue().Set(Boolean::New(true));

	int nType = -1;
	uint32_t nClassId = (uint32_t)-1;
	uint32_t nMode = (uint32_t)-1;
	uint32_t nUnitId = (uint32_t)-1;
	char szName[128] = "";
	char tmp[256] = "";

	ObjectTemplate* node = *ObjectTemplate::New();
	Array* pReturnArray = *Array::New();

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

	Vars.thisUnit = pUnit;

	if (!pUnit)
		args.GetReturnValue().Set(Boolean::New(true));

	myUnit* pmyUnit = new myUnit; // leaked?

	if (!pmyUnit)
		args.GetReturnValue().Set(Boolean::New(true));

	myPresetUnit* presetUnit = new myPresetUnit;

	Level* pLevel = GetLevel(fpGetPlayerUnit()->pPath->pRoom1->pRoom2->pLevel->dwLevelNo);
	int dwArrayCount = 0;

	if (pUnit && pmyUnit)
	{
		GetUnitName(pUnit, tmp, 256);
		pmyUnit->_dwPrivateType = PRIVATE_UNIT;
		pmyUnit->dwClassId = nClassId;
		pmyUnit->dwMode = nMode;
		pmyUnit->dwType = pUnit->dwType;
		pmyUnit->dwUnitId = pUnit->dwUnitId;

		//this needs a ton of work
		switch (nType)
		{
		case UNIT_PLAYER:
			node->Set(String::NewFromUtf8(isolate, "act"), Integer::New(pUnit->dwAct));
			node->Set(String::NewFromUtf8(isolate, "name"), String::New(tmp));
			node->Set(String::NewFromUtf8(isolate, "classid"), Integer::New(pUnit->dwTxtFileNo));
			node->Set(String::NewFromUtf8(isolate, "mode"), Integer::New(pmyUnit->dwMode));
			node->Set(String::NewFromUtf8(isolate, "type"), Integer::New(pmyUnit->dwType));
			node->Set(String::NewFromUtf8(isolate, "x"), Integer::New(fpGetUnitX(pUnit)));
			node->Set(String::NewFromUtf8(isolate, "y"), Integer::New(fpGetUnitY(pUnit)));
			node->Set(String::NewFromUtf8(isolate, "areaid"), Integer::New(pUnit->pAct->pRoom1->pRoom2->pLevel->dwLevelNo));
			node->Set(String::NewFromUtf8(isolate, "hp"), Integer::New((D2COMMON_GetUnitStat(pUnit, 6, 0) >> 8)));
			node->Set(String::NewFromUtf8(isolate, "mp"), Integer::New((D2COMMON_GetUnitStat(pUnit, 8, 0) >> 8)));
			node->Set(String::NewFromUtf8(isolate, "hpmax"), Integer::New((D2COMMON_GetUnitStat(pUnit, 7, 0) >> 8)));
			node->Set(String::NewFromUtf8(isolate, "mpmax"), Integer::New((D2COMMON_GetUnitStat(pUnit, 9, 0) >> 8)));
			node->Set(String::NewFromUtf8(isolate, "level"), Integer::New((D2COMMON_GetUnitStat(pUnit, 12, 0) >> 8)));
			//methods
			node->Set(String::NewFromUtf8(isolate, "GetStat"), FunctionTemplate::New(unitGetStat, args.This()));
			node->Set(String::NewFromUtf8(isolate, "GetState"), FunctionTemplate::New(unitGetState, args.This()));
			node->Set(String::NewFromUtf8(isolate, "GetItems"), FunctionTemplate::New(unitGetItems, args.This()));
			break;
		case PRIVATE_UNIT:
			node->Set(String::NewFromUtf8(isolate, "name"), String::New(tmp));
			node->Set(String::NewFromUtf8(isolate, "classid"), Integer::New(pmyUnit->dwClassId));
			node->Set(String::NewFromUtf8(isolate, "mode"), Integer::New(pmyUnit->dwMode));
			node->Set(String::NewFromUtf8(isolate, "unitid"), Integer::New(pmyUnit->dwUnitId));
			node->Set(String::NewFromUtf8(isolate, "type"), Integer::New(pmyUnit->dwType));
			node->Set(String::NewFromUtf8(isolate, "x"), Integer::New(fpGetUnitX(pUnit)));
			node->Set(String::NewFromUtf8(isolate, "y"), Integer::New(fpGetUnitY(pUnit)));
			//methods
			node->Set(String::NewFromUtf8(isolate, "GetStat"), FunctionTemplate::New(unitGetStat, args.This()));
			node->Set(String::NewFromUtf8(isolate, "GetState"), FunctionTemplate::New(unitGetState, args.This()));
			node->Set(String::NewFromUtf8(isolate, "GetItems"), FunctionTemplate::New(unitGetItems, args.This()));
			break;
		case UNIT_OBJECT:

			for (Room2 *pRoom = pLevel->pRoom2First; pRoom; pRoom = pRoom->pRoom2Next)
			{

				for (PresetUnit* prUnit = pRoom->pPreset; prUnit; prUnit = prUnit->pPresetNext, dwArrayCount++)
				{
					// Does it fit?
					if ((nType == NULL || prUnit->dwType == nType) && (nClassId == NULL || prUnit->dwTxtFileNo == nClassId))
					{

						//Local<Object> nodes = Object::New();
						presetUnit->dwPosX = prUnit->dwPosX;
						presetUnit->dwPosY = prUnit->dwPosY;
						presetUnit->dwRoomX = pRoom->dwPosX;
						presetUnit->dwRoomY = pRoom->dwPosY;
						presetUnit->dwType = prUnit->dwType;
						presetUnit->dwId = prUnit->dwTxtFileNo;
						presetUnit->dwLevel = fpGetPlayerUnit()->pPath->pRoom1->pRoom2->pLevel->dwLevelNo;
						Vars.nthisPresetX = (presetUnit->dwPosX) + (presetUnit->dwRoomX * 5);
						Vars.nthisPresetY = (presetUnit->dwPosY) + (presetUnit->dwRoomY * 5);

						node->Set(String::NewFromUtf8(isolate, "name"), String::New(tmp));
						node->Set(String::NewFromUtf8(isolate, "classid"), Integer::New(presetUnit->dwId));
						node->Set(String::NewFromUtf8(isolate, "mode"), Integer::New(pmyUnit->dwMode));
						node->Set(String::NewFromUtf8(isolate, "unitid"), Integer::New(pmyUnit->dwUnitId));
						node->Set(String::NewFromUtf8(isolate, "x"), Integer::New(presetUnit->dwPosX));
						node->Set(String::NewFromUtf8(isolate, "y"), Integer::New(presetUnit->dwPosY));
						node->Set(String::NewFromUtf8(isolate, "roomx"), Integer::New(presetUnit->dwRoomX));
						node->Set(String::NewFromUtf8(isolate, "roomy"), Integer::New(presetUnit->dwRoomY));
						node->Set(String::NewFromUtf8(isolate, "areaid"), Integer::New(presetUnit->dwLevel));
						node->Set(String::NewFromUtf8(isolate, "type"), Integer::New(presetUnit->dwType));
						node->Set(String::NewFromUtf8(isolate, "id"), Integer::New(presetUnit->dwId));
						node->Set(String::NewFromUtf8(isolate, "newx"), Integer::New(Vars.nthisPresetX));
						node->Set(String::NewFromUtf8(isolate, "newy"), Integer::New(Vars.nthisPresetY));
						node->Set(String::NewFromUtf8(isolate, "subareaid"), Integer::New(pLevel->pNextLevel->dwLevelNo));
						//pReturnArray->Set(dwArrayCount, nodes);
					}
				}
			}
			//return args.GetReturnValue().Set(pReturnArray);
			break;
		case UNIT_MISSILE:
			node->Set(String::NewFromUtf8(isolate, "name"), String::New(tmp));
			node->Set(String::NewFromUtf8(isolate, "classid"), Integer::New(pmyUnit->dwClassId));
			node->Set(String::NewFromUtf8(isolate, "mode"), Integer::New(pmyUnit->dwMode));
			node->Set(String::NewFromUtf8(isolate, "unitid"), Integer::New(pmyUnit->dwUnitId));
			node->Set(String::NewFromUtf8(isolate, "type"), Integer::New(pmyUnit->dwType));
			break;
		case UNIT_ITEM:
			/*if (pUnit->dwType == UNIT_ITEM && pUnit->pItemData) {
				if (!D2COMMON_GetItemText(pUnit->dwTxtFileNo))
					break;
				type = (D2COMMON_GetItemText(pUnit->dwTxtFileNo)->nType);
			}*/
			
			node->Set(String::NewFromUtf8(isolate, "name"), String::New(tmp));
			node->Set(String::NewFromUtf8(isolate, "quality"), Integer::New(pUnit->pItemData->dwQuality));
			node->Set(String::NewFromUtf8(isolate, "itemloc"), Integer::New(pUnit->pItemData->GameLocation));
			node->Set(String::NewFromUtf8(isolate, "itemlevel"), Integer::New(pUnit->pItemData->dwItemLevel));
			//node->Set(String::NewFromUtf8(isolate, "itemclass"), Integer::New(Itemclass(pUnit)));
			node->Set(String::NewFromUtf8(isolate, "itemdesc"), String::New(Itemdesc(pUnit)));
			//node->Set(String::NewFromUtf8(isolate, "itemflag"), String::New(Itemflag(pUnit)));
			//node->Set(String::NewFromUtf8(isolate, "ip"), String::New(Itemprefix(pItem)));
			//node->Set(String::NewFromUtf8(isolate, "itemsuffix"), String::New(Itemsuffix(pUnit)));
			//node->Set(String::NewFromUtf8(isolate, "itemtype"), Integer::New(Itemtype(pUnit)));
			node->Set(String::NewFromUtf8(isolate, "classid"), Integer::New(pUnit->dwTxtFileNo));
			node->Set(String::NewFromUtf8(isolate, "mode"), Integer::New(pUnit->dwMode));
			node->Set(String::NewFromUtf8(isolate, "unitid"), Integer::New(pUnit->dwUnitId));
			node->Set(String::NewFromUtf8(isolate, "type"), Integer::New(pUnit->dwType));
			node->Set(String::NewFromUtf8(isolate, "xsize"), Integer::New(ItemXsize(pUnit)));
			node->Set(String::NewFromUtf8(isolate, "ysize"), Integer::New(ItemYsize(pUnit)));
			node->Set(String::NewFromUtf8(isolate, "GetStat"), FunctionTemplate::New(unitGetStat, args.This()));
			break;
		case UNIT_TILE:
			for (Room2 *pRoom = pLevel->pRoom2First; pRoom; pRoom = pRoom->pRoom2Next)
			{

				for (PresetUnit* prUnit = pRoom->pPreset; prUnit; prUnit = prUnit->pPresetNext, dwArrayCount++)
				{
					// Does it fit?
					if ((nType == NULL || prUnit->dwType == nType) && (nClassId == NULL || prUnit->dwTxtFileNo == nClassId))
					{
						//Local<Object> nodes = Object::New();
						presetUnit->dwPosX = prUnit->dwPosX;
						presetUnit->dwPosY = prUnit->dwPosY;
						presetUnit->dwRoomX = pRoom->dwPosX;
						presetUnit->dwRoomY = pRoom->dwPosY;
						presetUnit->dwType = prUnit->dwType;
						presetUnit->dwId = prUnit->dwTxtFileNo;
						presetUnit->dwLevel = fpGetPlayerUnit()->pPath->pRoom1->pRoom2->pLevel->dwLevelNo;
						Vars.nthisPresetX = (presetUnit->dwPosX) + (presetUnit->dwRoomX * 5);
						Vars.nthisPresetY = (presetUnit->dwPosY) + (presetUnit->dwRoomY * 5);

						node->Set(String::NewFromUtf8(isolate, "name"), String::New(tmp));
						node->Set(String::NewFromUtf8(isolate, "classid"), Integer::New(presetUnit->dwId));
						node->Set(String::NewFromUtf8(isolate, "mode"), Integer::New(pmyUnit->dwMode));
						node->Set(String::NewFromUtf8(isolate, "unitid"), Integer::New(pmyUnit->dwUnitId));
						node->Set(String::NewFromUtf8(isolate, "x"), Integer::New(presetUnit->dwPosX));
						node->Set(String::NewFromUtf8(isolate, "y"), Integer::New(presetUnit->dwPosY));
						node->Set(String::NewFromUtf8(isolate, "roomx"), Integer::New(presetUnit->dwRoomX));
						node->Set(String::NewFromUtf8(isolate, "roomy"), Integer::New(presetUnit->dwRoomY));
						node->Set(String::NewFromUtf8(isolate, "areaid"), Integer::New(presetUnit->dwLevel));
						node->Set(String::NewFromUtf8(isolate, "type"), Integer::New(presetUnit->dwType));
						node->Set(String::NewFromUtf8(isolate, "id"), Integer::New(presetUnit->dwId));
						node->Set(String::NewFromUtf8(isolate, "newx"), Integer::New(Vars.nthisPresetX));
						node->Set(String::NewFromUtf8(isolate, "newy"), Integer::New(Vars.nthisPresetY));
						node->Set(String::NewFromUtf8(isolate, "subareaid"), Integer::New(pLevel->pNextLevel->dwLevelNo));
						//pReturnArray->Set(dwArrayCount, nodes);
					}
				}
			}
			//return args.GetReturnValue().Set(pReturnArray);
			break;
		default :
			node->Set(String::NewFromUtf8(isolate, "name"), String::New(tmp));
			node->Set(String::NewFromUtf8(isolate, "classid"), Integer::New(pmyUnit->dwClassId));
			node->Set(String::NewFromUtf8(isolate, "mode"), Integer::New(pmyUnit->dwMode));
			node->Set(String::NewFromUtf8(isolate, "unitid"), Integer::New(pmyUnit->dwUnitId));
			node->Set(String::NewFromUtf8(isolate, "type"), Integer::New(pmyUnit->dwType));
			break;
		}
	}
	//global for all unittypes
	node->Set(String::New("GetNext"), FunctionTemplate::New(unitGetNext,args.This()));

	delete[] pmyUnit, presetUnit;
	args.GetReturnValue().Set(node->NewInstance());
}

void init_me()
{
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope handle_scope(isolate);	
	Local<Context> context = Context::GetCurrent();
	Persistent<Context> persistent_context(isolate, context);	
	Context::Scope context_scope(context);
	context->Enter();

	Local<String> source = String::New("var me = InitMeOnce();");
	Local<Script> script = Script::Compile(source);
	script->Run();
	context->Exit();
	return;
}

JS_FUNC(CMe)
{
	Local<Context> context = Context::GetCurrent();
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);
	GameStructInfo* pInfo = *vpGameInfo;
	BnetData* pData = *vpBnData;

	ObjectTemplate* node = *ObjectTemplate::New();

	switch(MENU::ClientState())
	{
	case ClientStateMenu :
		node->Set(String::New("account"), String::New(Prof.Account));
		node->Set(String::New("charloc"), Integer::New(Prof.Charloc));
		node->Set(String::New("diff"), Integer::New(Prof.Difficulty));
		node->Set(String::New("fps"), Integer::New(*vpFPS));
		node->Set(String::New("gatewayid"), Integer::New(Prof.Realm));
		node->Set(String::New("ingame"), Boolean::New((MENU::ClientState() == ClientStateMenu ? false : true)));
		node->Set(String::New("ping"), Integer::New(*vpPing));
		node->Set(String::New("playertype"), Boolean::New(!!(pData->nCharFlags & PLAYER_TYPE_HARDCORE)));
		node->Set(String::New("realm"), String::New(pData->szRealmName));
		node->Set(String::New("screensize"), Integer::New(fpGetScreenSize()));
		//probably need to add more here and fix any above
		break;
	case ClientStateInGame :
		node->Set(String::New("account"), String::New(Prof.Account));
		node->SetAccessor(String::New("act"), GetAct, NULL);
		node->SetAccessor(String::New("areaid"), GetAreaId, NULL);
		// node->SetAccessor(String::New("charloc"), charloc, NULL);
		node->SetAccessor(String::New("charname"), GetName, NULL);
		node->SetAccessor(String::New("chickenhp"), GetChickenHP, SetChickenHP);
		node->SetAccessor(String::New("chickenmp"), GetChickenMP, SetChickenMP);
		node->SetAccessor(String::New("classid"), GetClassid, NULL);
		node->Set(String::New("diff"), Integer::New(fpGetDifficulty()));
		node->Set(String::New("fps"), Integer::New(*vpFPS));
		node->Set(String::New("gamename"), String::New(pInfo->szGameName));
		node->Set(String::New("gamepassword"), String::New(pInfo->szGamePassword));
		node->Set(String::New("gameserverip"), String::New(pInfo->szGameServerIp));
		node->Set(String::New("gametype"), Integer::New(*vpExpCharFlag));
		// node->SetAccessor(String::New("gatewayid"), gatewayid, NULL);
		// node->SetAccessor(String::New("gid"), gid, NULL);
		node->SetAccessor(String::New("hpmax"), GetHPMax, NULL);
		node->SetAccessor(String::New("hp"), GetHP, NULL);
		node->Set(String::New("ingame"), Boolean::New((MENU::ClientState() == ClientStateMenu ? false : true)));
		node->Set(String::New("itemoncursor"), Boolean::New(!!(fpGetCursorItem())));
		node->Set(String::New("ladder"), Boolean::New((double)(pData->ladderflag) == 0 ? false : true));
		node->SetAccessor(String::New("maxgametime"), GetMaxGameTime, SetMaxGameTime);
		node->SetAccessor(String::New("mode"), GetMode, NULL);
		node->SetAccessor(String::New("mpmax"), GetMPMax, NULL);
		node->SetAccessor(String::New("mp"), GetMP, NULL);
		node->SetAccessor(String::New("name"), GetName, NULL);
		node->Set(String::New("ping"), Integer::New(*vpPing));
		node->Set(String::New("playertype"), Boolean::New(!!(pData->nCharFlags & PLAYER_TYPE_HARDCORE)));
		// node->SetAccessor(String::New("playtype"), playtype, NULL);
		// node->SetAccessor(String::New("quitonhostile"), quitonhostile, NULL);
		node->Set(String::New("realm"), String::New(pData->szRealmName));
		node->Set(String::New("realmshort"), String::New(pData->szRealmName2));
		// node->SetAccessor(String::New("revealautomap"), revealautomap, NULL);
		node->SetAccessor(String::New("runwalk"), GetRunWalk, SetRunWalk); //needs more work, set's value but doesn't actually swap atm
		// node->SetAccessor(String::New("showenemyonautomap"), showenemyonautomap, NULL);
		// node->SetAccessor(String::New("showmissileonautomap"), showmissileonautomap, NULL);
		node->Set(String::New("screensize"), Integer::New(fpGetScreenSize()));
		node->Set(String::New("type"), Integer::New(fpGetPlayerUnit()->dwType));
		node->SetAccessor(String::New("weaponstab"), GetWeaponsTab, SetWeaponsTab); //needs more work, set's value but doesn't actually swap atm
		node->SetAccessor(String::New("x"), GetX, NULL);
		node->SetAccessor(String::New("y"), GetY, NULL);
		//Methods
		node->Set(String::New("Cancel"), FunctionTemplate::New(meCancel, args.This()));
		node->Set(String::New("GetItems"), FunctionTemplate::New(meGetItems, args.This()));
		node->Set(String::New("GetStat"), FunctionTemplate::New(meGetStat, args.This()));
		node->Set(String::New("GetState"), FunctionTemplate::New(meGetState, args.This()));
		node->Set(String::New("ClickItem"), FunctionTemplate::New(meClickItem, args.This()));
		//[+] Me.ClickMercItem()
		//[+] Me.ClickParty()
		//[+] Me.GetCursorItem()
		//[+] Me.GetMercCost()
		node->Set(String::New("GetQuest"), FunctionTemplate::New(meGetQuest, args.This()));
		//[+] Me.GetSkillStatus()
		node->Set(String::New("Repair"), FunctionTemplate::New(meRepair));
		node->Set(String::New("SelectNPCMenu"), FunctionTemplate::New(meSelectNPCMenu, args.This()));
		node->Set(String::New("SetSkill"), FunctionTemplate::New(meSetSkill, args.This()));
		node->Set(String::New("SwapWeapons"), FunctionTemplate::New(meSwapWeapons, args.This()));
		node->Set(String::New("TakeWaypoint"), FunctionTemplate::New(meTakeWayPoint, args.This()));
		//[+] Me.UseBelt()
		break;
	}

	
	args.GetReturnValue().Set(node->NewInstance());
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

	fpTransmute();

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
	Local<v8::Array> arr = v8::Local<v8::Array>::Cast(MENU::GetText(Prof.Charloc));
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

	if (args.Length() < 2 || !args[0]->IsNumber() || !args[1]->IsNumber())
	{
		args.GetReturnValue().Set(Boolean::New(false));
	}

	INT32 range_min = args[0]->Uint32Value();
	INT32 range_max = args[1]->Uint32Value();
	// only seed the rng once
	static bool hasseed = false;
	if (!hasseed)
	{
		srand(GetTickCount());
		hasseed = true;
	}

	long long seed = 0;
	if (MENU::ClientState() == ClientStateInGame)
		seed = fpRand(fpGetPlayerUnit()->dwSeed);
	else
		seed = rand();

	if (range_max > range_min + 1)
	{
		INT32 random = (seed % (range_max - range_min + 1)) + range_min;
		args.GetReturnValue().Set(random);
	}
	else
		args.GetReturnValue().Set(range_max);

}

JS_FUNC(CSay)
{
	HandleScope handle_scope(args.GetIsolate());
	if (args.Length() == 1)
	{
		String::Utf8Value str(args[0]);
		char* cstr = (char*)ToCString(str);
		Say(cstr);
		args.GetReturnValue().Set(Boolean::New(true));
	}
	args.GetReturnValue().Set(Boolean::New(false));
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
	//Local<String> props = Handle<String>::Cast(fc);
	args.GetReturnValue().Set(fc);
}

JS_FUNC(CExitGame)
{
	if (ClientGameState() != ClientStateMenu)
		fpExitGame();
	Sleep(2000);
	bool result = SendDataCopy("Etal Manager", 6153, "Boo");
	if (!result)
	{
		TerminateProcess(GetCurrentProcess(), 0);
	}
}

JS_FUNC(CMove)
{
	HandleScope handle_scope(args.GetIsolate());
	UnitAny* me = fpGetPlayerUnit();

	if (args.Length() == 2)
	{
		long zx = args[0]->Uint32Value();
		long zy = args[1]->Uint32Value();
		int x = args[0]->Uint32Value();
		int y = args[1]->Uint32Value();
		fpMapToAbsScreen(&zx, &zy);
		ClickMap(0, x, y, FALSE, NULL);
		Sleep(50);
		ClickMap(2, x, y, FALSE, NULL);
		args.GetReturnValue().Set(Boolean::New(true));
	}
	else {
		ClickMap(0, fpGetUnitX(me) + 4, fpGetUnitY(me) - 2, FALSE, NULL);
		Sleep(50);
		ClickMap(2, fpGetUnitX(me) + 4, fpGetUnitY(me) - 2, FALSE, NULL);
		args.GetReturnValue().Set(Boolean::New(true));
	}
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
	if (!loaded) {
		init_me();
	}
	if (args.Length() == 1)
	{
		int x = args[0]->Uint32Value();
		Sleep(x);
	}
	else if (args.Length() == 2)
	{
		int range_min = args[0]->Uint32Value();
		int range_max = args[1]->Uint32Value();
		// only seed the rng once
		static bool seeded = false;
		if (!seeded)
		{
			srand(GetTickCount());
			seeded = true;
		}

		long long seed = 0;
		if (MENU::ClientState() == ClientStateInGame)
			seed = fpRand(fpGetPlayerUnit()->dwSeed);
		else
			seed = rand();

		if (range_max > range_min + 1)
		{
			INT32 random = (seed % (range_max - range_min + 1)) + range_min; 
			Sleep(random);
		}
		else
			Sleep(range_max);
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
	if (!loaded) {
		init_me();
	}
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

#pragma warning ( pop )
