#include "Accessors.h"

// Accessors
void GetName(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	char* value = fpGetPlayerUnit()->pPlayerData->szName;
	return info.GetReturnValue().Set(String::New(value));
}
void SetName(Local<String> property, Local<Value> value,
	const PropertyCallbackInfo<void>& info)
{
	char* res = fpGetPlayerUnit()->pPlayerData->szName;
	return info.GetReturnValue().Set(String::New(res));
}
void GetAreaId(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	DWORD value = fpGetPlayerUnit()->pAct->pRoom1->pRoom2->pLevel->dwLevelNo;
	return info.GetReturnValue().Set(Integer::New(value));
}
void SetAreaId(Local<String> property, Local<Value> value,
	const PropertyCallbackInfo<void>& info)
{
	DWORD area = fpGetPlayerUnit()->pAct->pRoom1->pRoom2->pLevel->dwLevelNo;
	return info.GetReturnValue().Set(Integer::New(area));
}
void GetX(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	DWORD value = fpGetUnitX(fpGetPlayerUnit());
	return info.GetReturnValue().Set(Integer::New(value));
}
void SetX(Local<String> property, Local<Value> value,
	const PropertyCallbackInfo<void>& info)
{
	DWORD x = fpGetUnitX(fpGetPlayerUnit());
	return info.GetReturnValue().Set(Integer::New(x));
}
void GetY(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	DWORD value = fpGetUnitY(fpGetPlayerUnit());
	return info.GetReturnValue().Set(Integer::New(value));
}
void SetY(Local<String> property, Local<Value> value,
	const PropertyCallbackInfo<void>& info)
{
	DWORD y = fpGetUnitY(fpGetPlayerUnit());
	return info.GetReturnValue().Set(Integer::New(y));
}