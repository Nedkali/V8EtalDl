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
		}
		else {
			SendDataCopy("Etal Manager", 11, "Error loading Raw Keys");
			SendDataCopy("Etal Manager", 11, Vars.szKeyOwner);
			SendDataCopy("Etal Manager", 11, Vars.szClassic);
			SendDataCopy("Etal Manager", 11, Vars.szLod);
		}
	}
	else if (Vars.bzUseRawKeys == false)
	{
		if (Pointer::LoadCDKeyMPQ(Prof.MpqFile))
		{
			SendDataCopy("Etal Manager", 11, "Loaded Key MPQ");			
		}
		else {
			SendDataCopy("Etal Manager", 11, "Error Loading Key MPQ");
			SendDataCopy("Etal Manager", 11, Vars.szMpqfile);
		}
		
	}
	#pragma endregion

	ClientGameState state = MENU::ClientState();
	while (state == ClientStateInGame)
	{
		Sleep(1000);
	}


	// **V8
	V8::Initialize();
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope handle_scope(isolate);
	Handle<Context> context = CreateContext(isolate);
	TryCatch try_catch;

	if(context.IsEmpty())
	{
		ReportException(isolate, &try_catch);
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

//LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
//{
//    switch (Msg)
//    {
//    case WM_COPYDATA:
//        switch (int(wParam))
//        {
//        case 555:
//            Vars.restart = true;
//                break;
//
//        }
//
//    }
//
//    return DefWindowProc(hWnd, Msg, wParam, lParam);
//
//}