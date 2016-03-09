#define WIN32_LEAN_AND_MEAN
#include "D2Helpers.h"
#include "Offset.h"
#include "v8.h"
#include "V8Script.h"
#include "JSGlobals.h"

using namespace v8;

Profile  Prof;
Variables Vars;
void StartUpV8();
bool SendDataCopy(char* app, int  code, char* data)// make this bool later on
{
	HWND EtalhWnd = FindWindow(NULL, app);
	if (!EtalhWnd)
	{
		//MessageBox(NULL, "Window not found", "File Path", NULL);
		return false;
	}
	DWORD thwnd = GetCurrentProcessId();

	char* lpszString = data;
	COPYDATASTRUCT cds;
	cds.dwData = code;
	cds.cbData = sizeof(TCHAR) * (strlen(lpszString) + 1);
	cds.lpData = lpszString;
	bool b = SendMessage(EtalhWnd, WM_COPYDATA, thwnd, (LPARAM)&cds) == 0;
	//SendMessage(EtalhWnd, WM_COPYDATA, thwnd, (LPARAM)&cds);

	return b;
}

DWORD WINAPI MainThread(VOID* param)
{
	Input* input = new Input;
	input->Initialize();
	
	bool starter = true;
	bool ingame = false;
	bool startup = false;

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
	else if (Vars.bzUseRawKeys == false && strlen(Prof.KeyOwner) > 5)
	{
		if (Pointer::LoadCDKeyMPQ(Prof.KeyOwner))
		{
			SendDataCopy("Etal Manager", 11, "Loaded Key MPQ");			
		}
		else {
			SendDataCopy("Etal Manager", 11, "Error Loading Key MPQ");
			SendDataCopy("Etal Manager", 11, Vars.szMpqfile);
		}		
	}

	#pragma endregion
	
	if (strlen(Prof.ScriptFile) > 5)
	{

		while (1)
		{
			switch (MENU::ClientState())
			{
			case ClientStateInGame :
				break;
			case ClientStateMenu :
			case ClientStateNull:
			case ClientStateBusy :
				if (!startup) {
					StartUpV8();
					startup = true;
				}
				break;
			}
			Sleep(100);
		}
	}
	FreeLibraryAndExitThread((HMODULE)param, 0);

	return true;
}

void StartUpV8()
{
	// **V8
	V8::Initialize();
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope handle_scope(isolate);
	Handle<Context> context = CreateContext(isolate);
	TryCatch try_catch;

	if (context.IsEmpty())
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

}
