#pragma once
#include "JSGlobals.h"

//Unit Accessors
#pragma region Unit

char* Itemflag(UnitAny* pUnit);
int Itemclass(UnitAny* pUnit);
char* Itemprefix(UnitAny* pUnit);
char* Itemsuffix(UnitAny* pUnit);
int ItemXsize(UnitAny* pUnit);
int ItemYsize(UnitAny* pUnit);
int Itemtype(UnitAny* pUnit);
char* Itemdesc(UnitAny* pUnit);
void ReadProcessBYTES(HANDLE hProcess, DWORD lpAddress, void* buf, int len);

#pragma endregion

//Me Accessors
#pragma region Me

void GetRunWalk(Local<String> property, const PropertyCallbackInfo<Value>& info);
void SetRunWalk(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info);

void GetWeaponsTab(Local<String> property, const PropertyCallbackInfo<Value>& info);
void SetWeaponsTab(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info);

void GetChickenHP(Local<String> property, const PropertyCallbackInfo<Value>& info);
void SetChickenHP(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info);

void GetChickenMP(Local<String> property, const PropertyCallbackInfo<Value>& info);
void SetChickenMP(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info);

void GetAutoRevealMap(Local<String> property, const PropertyCallbackInfo<Value>& info);
void SetAutoRevealMap(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info);

void GetMaxGameTime(Local<String> property, const PropertyCallbackInfo<Value>& info);
void SetMaxGameTime(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info);

void GetMode(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetClassid(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetName(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetAreaId(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetX(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetY(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetHP(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetMP(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetHPMax(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetMPMax(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetAct(Local<String> property, const PropertyCallbackInfo<Value>& info);
#pragma endregion

//PlayerUnit Accessors
#pragma region PlayerUnit

#pragma endregion

//Script Accessors
#pragma region Script

#pragma endregion

