#pragma once
#include "JsFunctions.h"

void GetName(Local<String> property, const PropertyCallbackInfo<Value>& info);
void SetName(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info);

void GetAreaId(Local<String> property, const PropertyCallbackInfo<Value>& info);
void SetAreaId(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info);

void GetX(Local<String> property, const PropertyCallbackInfo<Value>& info);
void SetX(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info);

void GetY(Local<String> property, const PropertyCallbackInfo<Value>& info);
void SetY(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info);