#define WIN32_LEAN_AND_MEAN
#include "D2Helpers.h"
#include "Offset.h"
#include "v8.h"
#include "V8Script.h"
#include "JSFunctions.h"

using namespace v8;

Profile  Prof;
Variables Vars;

void SendDataCopy(char* app, int  code, char* data)// make this bool later on
{
	HWND EtalhWnd = FindWindow(NULL, app);
	if (!EtalhWnd)
	{
		//MessageBox(NULL, "Window not found", "File Path", NULL);
		return;
	}
	DWORD thwnd = GetCurrentProcessId();

	char* lpszString = data;
	COPYDATASTRUCT cds;
	cds.dwData = code;
	cds.cbData = sizeof(TCHAR) * (strlen(lpszString) + 1);
	cds.lpData = lpszString;
	//bool b = SendMessage(EtalhWnd, WM_COPYDATA, thwnd, (LPARAM)&cds) != 0;
	SendMessage(EtalhWnd, WM_COPYDATA, thwnd, (LPARAM)&cds);
	return;
}


DWORD WINAPI MainThread(VOID* param)
{
	Input* input = new Input;
	input->Initialize();

	bool starter = true;
	bool ingame = false;

	#pragma region Key_Handling

	if (Vars.bzUseRawKeys == true)
	{
		if (Pointer::ADDRawKeys(Prof.KeyOwner, Prof.Classic, Prof.Lod))
		{
			SendDataCopy("Etal Manager", 11, "Loaded Raw Keys");
			SendDataCopy("Etal Manager", 11, Vars.szKeyOwner);
			SendDataCopy("Etal Manager", 11, Vars.szClassic);
			SendDataCopy("Etal Manager", 11, Vars.szLod);
		}
		else {
			SendDataCopy("Etal Manager", 11, "Error Loading Raw Keys");
		}
	}
	else if (Vars.bzUseRawKeys == false)
	{
		if (Pointer::LoadCDKeyMPQ(Prof.MpqFile))
		{
			SendDataCopy("Etal Manager", 11, "Loaded Key MPQ");
			SendDataCopy("Etal Manager", 11, Vars.szMpqfile);
		}
		else {
			SendDataCopy("Etal Manager", 11, "Error Loading Key MPQ");
		}
		
	}


	#pragma endregion


	// **V8
	V8::Initialize();
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope handle_scope(isolate);
	Handle<Context> context = CreateContext(isolate);
	TryCatch try_catch;

	if(context.IsEmpty())
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

	FreeLibraryAndExitThread((HMODULE)param, 0);

	return true;
}

