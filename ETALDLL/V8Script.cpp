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
	Handle<Value> name = String::New(SCRIPT_PATH);
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
	const char* exception_string = ToCString(exception);
	v8::Handle<v8::Message> message = try_catch->Message();
	if (message.IsEmpty())
	{
			MessageBox(NULL, exception_string, "V8 Error", NULL);
	}
	else
	{
		// Print (filename):(line number): (message).
		v8::String::Utf8Value filename(message->GetScriptResourceName());
		const char* filename_string = ToCString(filename);
		MessageBox(NULL, filename_string, "V8 Exception", NULL);
		////int linenum = message->GetLineNumber();
		////char astmp[512];
		////int snewn = sprintf_s(astmp, "%s%s%s%i", "File: ", filename_string, "   Line: ", linenum);
		////char* tmp = new char[snewn + 1];
		////tmp[snewn] = '\0';
		////SendCopyData(11, tmp);

		////MessageBox(NULL, tmp, "V8 Exception", NULL);
	}
	v8::String::Utf8Value stack_trace(try_catch->StackTrace());
	if (stack_trace.length() > 0)
	{
		const char* stack_trace_string = ToCString(stack_trace);
		MessageBox(NULL, stack_trace_string, "V8 Stack Trace", NULL);

	}
}
