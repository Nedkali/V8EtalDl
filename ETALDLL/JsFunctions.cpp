//////////////////////////////////////////////////////////////////////
// JS Functions
//////////////////////////////////////////////////////////////////////
#include "JSFunctions.h"
#include "V8Script.h"
#include "D2Helpers.h"
#include <shlwapi.h>
#include "D2Funcs.h"


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
	JS_FLINK(CInclude, "Include");
	JS_FLINK(CSetTitle, "SetTitle");
	JS_FLINK(CSendCopyData, "SendCopyData");
	JS_FLINK(CClick, "Click");
	JS_FLINK(CGetTickCount, "GetTickCount");
	JS_FLINK(CDelay, "Delay");
	JS_FLINK(CCloseD2, "CloseD2");
	JS_FLINK(CScreenSize, "ScreenSize");
	JS_FLINK(CPrint, "Print");
	return Context::New(isolate, NULL, global);
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
	int x = args[0]->Uint32Value();
	Sleep(x);
	return;
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
	//std::replace(cstr, cstr + strlen(cstr), '%', (char)(unsigned char)0xFE);
	D2Funcs::Print(cstr);
}