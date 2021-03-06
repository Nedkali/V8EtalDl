//////////////////////////////////////////////////////////////////////
// JS Defines
//////////////////////////////////////////////////////////////////////

#include "v8.h"

#define JS_FLINK(CFUNC, JFUNC)	global->Set(v8::String::New(JFUNC), v8::FunctionTemplate::New(CFUNC))	// **Function linker
#define JS_FUNC(FNAME) void FNAME(const v8::FunctionCallbackInfo<v8::Value>& args)	// **Function define

#define JS_METHOD(MethodName)void MethodName(const v8::FunctionCallbackInfo<v8::Value>& args) // **Method define

#define JSBool v8::Handle<v8::Boolean>
#define JSInt v8::Handle<v8::Integer>
#define JSString v8::Handle<v8::String>
#define JSArray(ARRAYNAME, ARRAYLENGTH) v8::Handle<v8::Array> ARRAYNAME = v8::Array::New(ARRAYLENGTH)
