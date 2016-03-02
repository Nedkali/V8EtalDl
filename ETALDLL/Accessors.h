#pragma once
#include "JSGlobals.h"

//Unit Global Accessors
#pragma region Unit

void meSetSkill(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info);

char* Itemflag(UnitAny* pUnit);
int Itemclass(UnitAny* pUnit);
char* Itemprefix(UnitAny* pUnit);
char* Itemsuffix(UnitAny* pUnit);
int ItemXsize(UnitAny* pUnit);
int ItemYsize(UnitAny* pUnit);
int Itemtype(UnitAny* pUnit);
char* Itemdesc(UnitAny* pUnit);
void ReadProcessBYTES(HANDLE hProcess, DWORD lpAddress, void* buf, int len);


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

void GetName(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetAreaId(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetX(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetY(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetHP(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetMP(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetHPMax(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetMPMax(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetAct(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetClassid(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetXsize(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetYsize(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetCode(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetGID(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetItemtype(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetMode(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetQuality(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetShrinetype(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetSpectype(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetSubareaid(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetType(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetItemclass(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetItemdesc(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetItemflag(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetItemlevel(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetItemloc(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetItemprefix(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetItemsuffix(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetItemXsize(Local<String> property, const PropertyCallbackInfo<Value>& info);

void GetItemYsize(Local<String> property, const PropertyCallbackInfo<Value>& info);



// add and figure these out later
//[+] Unit.GetItemCost()
//[+] Unit.GetItems()
//[+] Unit.GetMerc()
//[+] Unit.GetNext()
//[+] Unit.GetOptimalAttackPos()
//[+] Unit.GetParent()
//[+] Unit.GetRoom()
//[+] Unit.GetSkill()
//[+] Unit.GetStat()
//[+] Unit.GetState()
//[+] Unit.IsAttackable()

#pragma endregion

//Me Accessors
#pragma region Me

#pragma endregion

//PlayerUnit Accessors
#pragma region PlayerUnit

#pragma endregion

//Script Accessors
#pragma region Script

#pragma endregion
