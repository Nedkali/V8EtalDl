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
	return Context::New(isolate, NULL, global);
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
	


}

JS_FUNC(CSelectChar)
{
	int x = 175; int y = 125;
	int charpos = Prof.Charloc;
	if (args.Length() > 0)
	{
		charpos = args[0]->Uint32Value();
	}
	////char numberstring[(((sizeof charpos) * CHAR_BIT) + 2) / 3 + 2];
	////sprintf(numberstring, "%d", charpos);

	////MessageBox(NULL, numberstring, "Debug", NULL);
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
	int Lvl = args[0]->Uint32Value();
	D2Funcs::GetLevel(D2Funcs::GetPlayerUnit()->pAct, Lvl);
}
JS_FUNC(CRandom)
{
	HandleScope handle_scope(args.GetIsolate());
	INT32 range_min = args[0]->Uint32Value();
	INT32 range_max = args[1]->Uint32Value();

	srand((unsigned int)time(NULL));
	INT32 u = rand() / (RAND_MAX + 1) * (range_max - range_min) + range_min;
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
	DWORD area = D2Funcs::GetArea()->dwLevelNo;
	UnitAny* unit = D2Funcs::GetPlayerUnit();
	char Area[128];
	sprintf_s(Area, "%u", unit->pAct->dwAct);
	D2Funcs::Print(Area);

	char Area1[128];
	sprintf_s(Area1, "%u", area);
	D2Funcs::Print(Area1);
}

JS_FUNC(CGetPlayerUnit)
{
	D2Funcs::GetPlayerUnit()->pInfo;
}

JS_FUNC(CExitGame)
{
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
		D2Funcs::MoveTo(static_cast<WORD>(x), static_cast<WORD>(y));
		args.GetReturnValue().Set(Boolean::New(true));
	}

	args.GetReturnValue().Set(Boolean::New(false));
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
		return ;

}

JS_FUNC(CCloseD2)
{

	
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
		int random = rand() % x+b;
		Sleep(random);
	}
	return;
}

JS_FUNC(CLoad) {

	HandleScope handle_scope(args.GetIsolate());
	String::Utf8Value str(args[0]);
	char* cstr = (char*)ToCString(str);
	Isolate* isolate = Isolate::GetCurrent();
	StringReplace(cstr, '/', '\\', strlen(cstr));
	RunScript(isolate, Vars.szScriptPath, cstr);
	Handle<Context> context = CreateContext(args.GetIsolate());
	V8::Initialize();
	TryCatch try_catch;
	if (context.IsEmpty())
	{
		//ReportException(isolate, &try_catch);
		MessageBox(NULL, "Error Creating Context", "Debug", NULL);
	}
	else
	{
		context->Enter();
	}

	RunScript(isolate, Vars.szScriptPath, Prof.ScriptFile);
	Handle<v8::Object> global = context->Global();
	Handle<v8::Value> value = global->Get(String::New("NTMain"));
	if (value->IsFunction()) {
		Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(value);
		func->Call(global, 0, NULL);
	}


	context->Exit();
	V8::Dispose();
}
JS_FUNC(CInclude) {

	HandleScope handle_scope(args.GetIsolate());
	String::Utf8Value str(args[0]);
	char* cstr = (char*)ToCString(str);
	Isolate* isolate = Isolate::GetCurrent();
	StringReplace(cstr , '/', '\\', strlen(cstr));
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