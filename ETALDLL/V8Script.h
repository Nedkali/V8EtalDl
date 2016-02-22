//////////////////////////////////////////////////////////////////////
// JSCommon.h
//////////////////////////////////////////////////////////////////////
#include "v8.h"
#include "JSDefines.h"


using namespace v8;

#define DBG(MSG) MessageBox(NULL, MSG, "Debug", NULL)

Handle<String>JSReadFile(const char* name);
const char* ToCString(const v8::String::Utf8Value& value);
Handle<Value> RunScript(Isolate* isolate, const char* SCRIPT_PATH, const char* SCRIPT_NAME);
void ReportException(v8::Isolate* isolate, v8::TryCatch* try_catch);