#define _CRT_SECURE_NO_WARNINGS
#include "V8Script.h"
#include <Windows.h>
#include "D2Helpers.h"

using namespace v8;

const char* ToCString(const v8::String::Utf8Value& value)
{
	return *value ? *value : "<string conversion failed>";
}

Handle<Value> RunScript(Isolate* isolate, const char* SCRIPT_PATH, const char* SCRIPT_NAME)
{
	char str[MAX_PATH];
	strcpy(str, SCRIPT_PATH);
	strcat(str, SCRIPT_NAME);
	
	v8::HandleScope handle_scope(isolate);
	TryCatch try_catch;
	Handle<Value> name = String::New(SCRIPT_NAME);
	String::Utf8Value file(String::New(str));
	Handle<String> source = JSReadFile(str);
	if (source.IsEmpty())
	{
		ReportException(isolate, &try_catch);
	}
	else
	{
		Handle<Script> script = Script::Compile(source, name);

		if (script.IsEmpty())
		{
			ReportException(isolate, &try_catch);
		}
		else
		{
			Handle<Value> result = script->Run();
			if (result.IsEmpty())
			{
				ReportException(isolate, &try_catch);
			}
		}
	}
	return Handle<Value>();
}



Handle<String>JSReadFile(const char* name)
{
	FILE* file = fopen(name, "rb");
	if (file == NULL)
	{
		return Handle<String>();
	}

	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	rewind(file);

	char* chars = new char[size + 1];
	chars[size] = '\0';
	for (int i = 0; i < size;)
	{
		int read = static_cast<int>(fread(&chars[i], 1, size - i, file));
		i += read;
	}
	fclose(file);
	Handle<String> result = String::New(chars, size);
	delete[] chars;
	return result;
}

void ReportException(v8::Isolate* isolate, v8::TryCatch* try_catch)
{
	v8::HandleScope handle_scope(isolate);
	v8::String::Utf8Value exception(try_catch->Exception());
	v8::Handle<v8::Message> message = try_catch->Message();
	if (message.IsEmpty())
	{
		// V8 didn't provide any extra information about this error; just print the exception.
		char* cstr = (char*)ToCString(exception);
		SendDataCopy("Etal Manager", 11, cstr);
	}
	else
	{
		//Print (filename):(line number): (message).
		v8::String::Utf8Value filename(message->GetScriptResourceName());
		char* cstr = (char*)ToCString(filename);
		int iNumber = message->GetLineNumber();
		char buffer[20]; _itoa(iNumber, buffer, 10);
		char fn[100];
		strcpy(fn, "ÿc1ERROR   File: ");
		strcat(fn, cstr); strcat(fn, " Line: ");
		strcat(fn, buffer);
		SendDataCopy("Etal Manager", 11, fn);
		D2Funcs::Print("%s", fn);

	}
	v8::String::Utf8Value stack_trace(try_catch->StackTrace());
	if (stack_trace.length() > 0)
	{
		const char* stack_trace_string = ToCString(stack_trace);
		MessageBox(NULL, stack_trace_string, "V8 Stack Trace", NULL);

	}

	isolate->Exit();
}
