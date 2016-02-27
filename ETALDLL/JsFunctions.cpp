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
	JS_FLINK(CLoad, "Load");
	JS_FLINK(CInclude, "Include");
	JS_FLINK(CSetTitle, "SetTitle");
	JS_FLINK(CSendCopyData, "SendCopyData");
	JS_FLINK(CClick, "Click");
	JS_FLINK(CGetTickCount, "GetTickCount");
	JS_FLINK(CDelay, "Delay");
	JS_FLINK(CCloseD2, "CloseD2");
	JS_FLINK(CScreenSize, "ScreenSize");
	JS_FLINK(CPrint, "Print");
	JS_FLINK(CMove, "Move");
	JS_FLINK(CSetSkill, "SetSkill");
	JS_FLINK(CExitGame, "ExitGame");
	JS_FLINK(CGetPlayerUnit, "GetPlayerUnit");
	JS_FLINK(CGetArea, "GetArea");
	JS_FLINK(CSay, "Say");
	JS_FLINK(CRandom, "Random");
	JS_FLINK(CGetLevel, "GetLevel");
	JS_FLINK(CClickControl, "ClickControl");
	JS_FLINK(CGetLocation, "GetLocation");
	JS_FLINK(CSelectChar, "SelectChar");
	JS_FLINK(CGetText, "GetText");
	JS_FLINK(CSetText, "SetText");
	JS_FLINK(CClientState, "ClientState");
	JS_FLINK(CSubmitItem, "SubmitItem");
	JS_FLINK(CTransmute, "Transmute");
	JS_FLINK(CTESTWAYPOINT, "WayPoint");
	return Context::New(isolate, NULL, global);
}

JS_FUNC(CSubmitItem)
{
	//Isolate* isolate = Isolate::GetCurrent();
	//HandleScope scope(isolate);

	//if (UnitAny* pUnit = D2CLIENT_GetCursorItem())
	//{
	//	if (fpGetPlayerUnit()->dwAct == 1)
	//	{
	//		if (GetPlayerArea() == fpGetPlayerUnit()->pAct->pMisc->dwStaffTombLevel)
	//		{
	//			*p_D2CLIENT_CursorItemMode = 3;
	//			BYTE aPacket[17] = { NULL };
	//			aPacket[0] = 0x44;
	//			*(DWORD*)&aPacket[1] = fpGetPlayerUnit()->dwUnitId;
	//			*(DWORD*)&aPacket[5] = *p_D2CLIENT_OrificeId;
	//			*(DWORD*)&aPacket[9] = pUnit->dwUnitId;
	//			*(DWORD*)&aPacket[13] = 3;
	//			fpSendPacket(17, 1, aPacket);
	//			args.GetReturnValue().Set(Boolean::New(true));
	//		}
	//		else
	//			args.GetReturnValue().Set(Boolean::New(false));
	//	}
	//	else if (fpGetPlayerUnit()->dwAct == 0 || fpGetPlayerUnit()->dwAct == 4) // dwAct is 0-4, not 1-5
	//	{
	//		if (*p_D2CLIENT_RecentInteractId && D2COMMON_IsTownByLevelNo(GetPlayerArea()))
	//		{
	//			D2CLIENT_SubmitItem(pUnit->dwUnitId);
	//			args.GetReturnValue().Set(Boolean::New(true));
	//		}
	//		else
	//			args.GetReturnValue().Set(Boolean::New(false));
	//	}
	//	else
	//		args.GetReturnValue().Set(Boolean::New(false));
	//}
	//else
	//	args.GetReturnValue().Set(Boolean::New(false));

	//args.GetReturnValue().Set(Boolean::New(true));
}

JS_FUNC(CTransmute)
{
	/*Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);

	bool cubeOn = !!D2CLIENT_GetUIState(UI_CUBE);
	if (!cubeOn)
		D2CLIENT_SetUIState(UI_CUBE, TRUE);

	D2CLIENT_Transmute();

	if (!cubeOn)
		D2CLIENT_SetUIState(UI_CUBE, FALSE);

	args.GetReturnValue().Set(Boolean::New(true));*/
}

JS_FUNC(CClientState)
{
	args.GetReturnValue().Set(v8::Int32::New(MENU::ClientState()));

}

JS_FUNC(CSetText)
{

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

		String::Utf8Value str2(args[1]);
		char* cstr2 = (char*)ToCString(str2);

		Control* pControl = MENU::findControl(Type, cstr, Disabled, PosX, PosY, SizeX, SizeY);

		if (true)
		{
			MENU::SetControlText(pControl, cstr);
		}


	}
}

JS_FUNC(CGetText)
{
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);
	Local<v8::Array> arr = v8::Local<v8::Array>::Cast(MENU::GetText(Prof.Charloc, Prof.Realm));
	args.GetReturnValue().Set(arr);

}

JS_FUNC(CSelectChar)
{
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
	int id = MENU::GetLocationID();
	args.GetReturnValue().Set(id);
	return;
}

JS_FUNC(CClickControl)
{
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

	//Just testing for now
	INT32 u = D2Funcs::Random(range_min, range_max);

	//for some reason this isn't working correctly

	//srand((unsigned int)time(NULL));
	//INT32 u = rand() / (RAND_MAX + 1) * (range_max - range_min) + range_min;

	args.GetReturnValue().Set(u);

}

JS_FUNC(CSay)
{
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
	UnitAny* pUnit = D2CLIENT_FindUnit(fpGetPlayerUnit()->dwUnitId, fpGetPlayerUnit()->dwType);
	//D2Funcs::Interact();
	D2CLIENT_TakeWaypoint(119, wp);
	if (!D2CLIENT_GetUIState(UI_GAME))
		fpCloseInteract();

}
JS_FUNC(CGetPlayerUnit)
{
	Isolate* isolate = args.GetIsolate();
	HandleScope handle_scope(isolate);
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
		D2Funcs::ExitGame();
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

	if (args.Length() == 2)
	{
		int x = args[0]->Uint32Value();
		int y = args[1]->Uint32Value();
		D2Funcs::MoveTo(x, y);
		args.GetReturnValue().Set(Boolean::New(true));
	}
	else {
		UnitAny* me = D2Funcs::GetPlayerUnit();
		D2Funcs::MoveTo(fpGetUnitX(me) + 4, fpGetUnitY(me) - 2);
		args.GetReturnValue().Set(Boolean::New(true));
	}
	return;
}
JS_FUNC(CScreenSize)
{

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
	if (ClientGameState() != ClientStateMenu)
		D2Funcs::ExitGame();
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
	args.GetReturnValue().Set(v8::Int32::New(GetTickCount()));
}


JS_FUNC(CPrint)
{
	HandleScope handle_scope(args.GetIsolate());
	String::Utf8Value str(args[0]);
	char* cstr = (char*)ToCString(str);
	D2Funcs::Print("%s", cstr);;
}
