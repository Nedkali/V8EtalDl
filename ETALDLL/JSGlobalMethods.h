#pragma once
#include "JSGlobals.h"

//Me Methods
void meSetSkill(const v8::FunctionCallbackInfo<v8::Value>& args);
void meCancel(const v8::FunctionCallbackInfo<v8::Value>& args);
void meRepair(const v8::FunctionCallbackInfo<v8::Value>& args);
void meGetState(const v8::FunctionCallbackInfo<v8::Value>& args);
void meGetQuest(const v8::FunctionCallbackInfo<v8::Value>& args);
void meSwapWeapons(const v8::FunctionCallbackInfo<v8::Value>& args);