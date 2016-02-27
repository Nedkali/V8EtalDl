#include "Accessors.h"
#include "Constants.h"
#include "D2Helpers.h"

//Unit Global Accessors
#pragma region Unit
//Need to wrap these as a single accesible property (Unit)
void GetName(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	char* value = fpGetPlayerUnit()->pPlayerData->szName;
	return info.GetReturnValue().Set(String::New(value));
}

void GetAreaId(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	DWORD value = fpGetPlayerUnit()->pAct->pRoom1->pRoom2->pLevel->dwLevelNo;
	return info.GetReturnValue().Set(Integer::New(value));
}

void GetX(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	DWORD value = fpGetUnitX(fpGetPlayerUnit());
	return info.GetReturnValue().Set(Integer::New(value));
}

void GetY(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	DWORD value = fpGetUnitY(fpGetPlayerUnit());
	return info.GetReturnValue().Set(Integer::New(value));
}

void GetHP(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	DWORD value = (D2COMMON_GetUnitStat(fpGetPlayerUnit(), 6, 0) >> 8);
	return info.GetReturnValue().Set(Integer::New(value));
}

void GetMP(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	DWORD value = (D2COMMON_GetUnitStat(fpGetPlayerUnit(), 8, 0) >> 8);
	return info.GetReturnValue().Set(Integer::New(value));
}

void GetHPMax(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	DWORD value = (D2COMMON_GetUnitStat(fpGetPlayerUnit(), 7, 0) >> 8);
	return info.GetReturnValue().Set(Integer::New(value));
}

void GetMPMax(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	DWORD value = (D2COMMON_GetUnitStat(fpGetPlayerUnit(), 9, 0) >> 8);
	return info.GetReturnValue().Set(Integer::New(value));
}

void GetAct(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	DWORD value = (fpGetPlayerUnit()->dwAct + 1);
	return info.GetReturnValue().Set(Integer::New(value));
}

void GetClassid(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	DWORD res = (fpGetPlayerUnit()->dwTxtFileNo);
	return info.GetReturnValue().Set(Integer::New(res));
}

void GetXsize(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
}

void GetYsize(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
}

void GetCode(Local<String> property, const PropertyCallbackInfo<Value>& info)
{

}

void GetGID(Local<String> property, const PropertyCallbackInfo<Value>& info)
{

}

void GetItemtype(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	Room1* pRoom = NULL;
	if (fpGetPlayerUnit()->dwType == UNIT_OBJECT && fpGetPlayerUnit()->pObjectData)
	{
		pRoom = D2COMMON_GetRoomFromUnit(fpGetPlayerUnit());
		if (pRoom && D2COMMON_GetLevelNoFromRoom(pRoom))
			return info.GetReturnValue().Set(Integer::New(fpGetPlayerUnit()->pObjectData->Type & 255));
		else
			return info.GetReturnValue().Set(Integer::New(fpGetPlayerUnit()->pObjectData->Type));
	}
}

void GetMode(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	DWORD res = (fpGetPlayerUnit()->dwMode);
	return info.GetReturnValue().Set(Integer::New(res));
}

void GetQuality(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	DWORD res = NULL;
	if (fpGetPlayerUnit()->dwType == UNIT_ITEM && fpGetPlayerUnit()->pItemData)
		res = (fpGetPlayerUnit()->pItemData->dwQuality);
	return info.GetReturnValue().Set(Integer::New(res));
}

void GetShrinetype(Local<String> property, const PropertyCallbackInfo<Value>& info)
{

}

void GetSpectype(Local<String> property, const PropertyCallbackInfo<Value>& info)
{

}

void GetSubareaid(Local<String> property, const PropertyCallbackInfo<Value>& info)
{

}

void GetType(Local<String> property, const PropertyCallbackInfo<Value>& info)
{

}

void GetItemclass(Local<String> property, const PropertyCallbackInfo<Value>& info)
{

}

void GetItemdesc(Local<String> property, const PropertyCallbackInfo<Value>& info)
{

}

void GetItemflag(Local<String> property, const PropertyCallbackInfo<Value>& info)
{

}

void GetItemlevel(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	if (fpGetPlayerUnit()->dwType != UNIT_ITEM)
		return info.GetReturnValue().Set(Integer::New(0));
	if (fpGetPlayerUnit()->pItemData)
		return info.GetReturnValue().Set(Integer::New(fpGetPlayerUnit()->pItemData->dwItemLevel));

}

void GetItemloc(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	BYTE res = NULL;
	if (fpGetPlayerUnit()->dwType == UNIT_ITEM && fpGetPlayerUnit()->pItemData)
		res = (fpGetPlayerUnit()->pItemData->GameLocation);
	return info.GetReturnValue().Set(Integer::New(res));

}

void GetItemprefix(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	char* res = NULL;
	if (fpGetPlayerUnit()->dwType == UNIT_ITEM && fpGetPlayerUnit()->pItemData)
		if (D2COMMON_GetItemMagicalMods(fpGetPlayerUnit()->pItemData->wMagicPrefix[0]))
			res = D2COMMON_GetItemMagicalMods(fpGetPlayerUnit()->pItemData->wMagicPrefix[0]);
	return info.GetReturnValue().Set(String::New(res));
}

void GetItemsuffix(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	char* res = NULL;
	if (fpGetPlayerUnit()->dwType == UNIT_ITEM && fpGetPlayerUnit()->pItemData)
		if (D2COMMON_GetItemMagicalMods(fpGetPlayerUnit()->pItemData->wMagicSuffix[0]))
			res = D2COMMON_GetItemMagicalMods(fpGetPlayerUnit()->pItemData->wMagicSuffix[0]);
	return info.GetReturnValue().Set(String::New(res));
}

void GetItemXsize(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	if (fpGetPlayerUnit()->dwType == UNIT_ITEM && fpGetPlayerUnit()->pItemData) {
		if (!D2COMMON_GetItemText(fpGetPlayerUnit()->dwTxtFileNo))
			return info.GetReturnValue().Set(Integer::New(0));
		return info.GetReturnValue().Set(Integer::New(D2COMMON_GetItemText(fpGetPlayerUnit()->dwTxtFileNo)->xSize));
	}
}

void GetItemYsize(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	if (fpGetPlayerUnit()->dwType == UNIT_ITEM && fpGetPlayerUnit()->pItemData) {
		if (!D2COMMON_GetItemText(fpGetPlayerUnit()->dwTxtFileNo))
			return info.GetReturnValue().Set(Integer::New(0));
		return info.GetReturnValue().Set(Integer::New(D2COMMON_GetItemText(fpGetPlayerUnit()->dwTxtFileNo)->ySize));
	}
}

#pragma endregion
