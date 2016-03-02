#include "Accessors.h"
#include "Constants.h"
#include "D2Helpers.h"

//Unit Global Accessors
#pragma region Unit
//Need to wrap these as a single accesible property (Unit)


void GetRunWalk(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	INT32 val = Vars.nRunWalk;
	/*int32_t rw = *vpAlwaysRun;
	info.GetReturnValue().Set(Integer::New(rw));*/
	return info.GetReturnValue().Set(Integer::New(val));
}
void SetRunWalk(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	INT32 val = value->Uint32Value();
	Vars.nRunWalk = val;
	int32_t rw = *vpAlwaysRun;
	info.GetReturnValue().Set(Integer::New(rw));
	return info.GetReturnValue().Set(Integer::New(val));
}

void GetAutoRevealMap(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	/*bool val = Vars.revealautomap;
	if (val == false)
		val = true;
	else if (val == true)
		val = false;
	return info.GetReturnValue().Set(Boolean::New(val));*/

}
void SetAutoRevealMap(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	/*UnitAny* unit =fpGetPlayerUnit();

	if (!unit) {
		return info.GetReturnValue().Set(Boolean::New(true));
	}

	Level* level = unit->pPath->pRoom1->pRoom2->pLevel;

	if (!level) {
		return info.GetReturnValue().Set(Boolean::New(true));
	}

	BOOL bDrawPresets = Vars.revealautomap;
	
	for (Room2* room = level->pRoom2First; room; room = room->pRoom2Next) {
		RevealRoom(room, bDrawPresets);
	}
	return info.GetReturnValue().Set(Integer::New(bDrawPresets));*/
}

void GetWeaponsTab(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	INT32 val = Vars.nWeaponsTab;
	return info.GetReturnValue().Set(Integer::New(val));
}
void SetWeaponsTab(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	INT32 val = value->Uint32Value();
	Vars.nWeaponsTab = val;

	int32_t nParameter = NULL;

	if (val > 0)
	{
		nParameter = val;
	}

	if (nParameter == NULL)
	{
		// don't perform a weapon switch if current gametype is classic
		BnetData* pData = (*vpBnData);
		if (pData)
		{
			if (!(pData->nCharFlags & PLAYER_TYPE_EXPAC))
				return info.GetReturnValue().Set(Integer::New(0));
		}
		else
			return info.GetReturnValue().Set(Integer::New(0));

		BYTE aPacket[1];
		aPacket[0] = 0x60;
		fpSendPacket(1, 1, aPacket);
	}
	else
		return info.GetReturnValue().Set(Integer::New((*vpbWeapSwitch)));
}

void GetChickenHP(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	INT32 val = Vars.MeChickenHP;
	return info.GetReturnValue().Set(Integer::New(val));
}
void SetChickenHP(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	INT32 val = value->Uint32Value();
	Vars.MeChickenHP = val;
	return info.GetReturnValue().Set(Integer::New(val));
}

void GetChickenMP(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	INT32 val = Vars.MeChickenMP;
	return info.GetReturnValue().Set(Integer::New(val));
}
void SetChickenMP(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	INT32 val = value->Uint32Value();
	Vars.MeChickenMP = val;
	return info.GetReturnValue().Set(Integer::New(val));
}

void GetMaxGameTime(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	INT32 val = Vars.meMaxGameTime;
	return info.GetReturnValue().Set(Integer::New(val));

}
void SetMaxGameTime(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	INT32 val = value->Uint32Value();
	Vars.meMaxGameTime = val;
	return info.GetReturnValue().Set(Integer::New(val));
}

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
		if (fpGetItemMagicalMods(fpGetPlayerUnit()->pItemData->wMagicPrefix[0]))
			res = fpGetItemMagicalMods(fpGetPlayerUnit()->pItemData->wMagicPrefix[0]);
	return info.GetReturnValue().Set(String::New(res));
}

void GetItemsuffix(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	char* res = NULL;
	if (fpGetPlayerUnit()->dwType == UNIT_ITEM && fpGetPlayerUnit()->pItemData)
		if (fpGetItemMagicalMods(fpGetPlayerUnit()->pItemData->wMagicSuffix[0]))
			res = fpGetItemMagicalMods(fpGetPlayerUnit()->pItemData->wMagicSuffix[0]);
	return info.GetReturnValue().Set(String::New(res));
}

void GetItemXsize(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	if (fpGetPlayerUnit()->dwType == UNIT_ITEM && fpGetPlayerUnit()->pItemData) {
		if (!fpGetItemText(fpGetPlayerUnit()->dwTxtFileNo))
			return info.GetReturnValue().Set(Integer::New(0));
		return info.GetReturnValue().Set(Integer::New(fpGetItemText(fpGetPlayerUnit()->dwTxtFileNo)->xSize));
	}
}

void GetItemYsize(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	if (fpGetPlayerUnit()->dwType == UNIT_ITEM && fpGetPlayerUnit()->pItemData) {
		if (!fpGetItemText(fpGetPlayerUnit()->dwTxtFileNo))
			return info.GetReturnValue().Set(Integer::New(0));
		return info.GetReturnValue().Set(Integer::New(fpGetItemText(fpGetPlayerUnit()->dwTxtFileNo)->ySize));
	}
}

char* Itemflag(UnitAny* pUnit)
{
	char* tmp;
	if (!(pUnit->dwType == UNIT_ITEM) && pUnit->pItemData)
		return tmp = "unknown";

	ItemText* pTxt;
	pTxt = fpGetItemText(pUnit->dwTxtFileNo);
	if (!pTxt)
		return tmp = "unknown";

	char szCode[4];
	memcpy(szCode, pTxt->szCode, 3);
	szCode[3] = 0x00;
	tmp = szCode;
	return tmp;
}

int Itemclass(UnitAny* pUnit)
{
	return 0;
}

char* Itemprefix(UnitAny* pUnit)
{
	char* res = NULL;
	if (pUnit->dwType == UNIT_ITEM && pUnit->pItemData)
		if (fpGetItemMagicalMods(pUnit->pItemData->wMagicPrefix[0]))
			res = fpGetItemMagicalMods(pUnit->pItemData->wMagicPrefix[0]);
	return res;
}

char* Itemsuffix(UnitAny* pUnit)
{
	char* res = NULL;
	if (pUnit->dwType == UNIT_ITEM && pUnit->pItemData)
		if (fpGetItemMagicalMods(pUnit->pItemData->wMagicSuffix[0]))
			res = fpGetItemMagicalMods(pUnit->pItemData->wMagicSuffix[0]);
	return res;
}

int ItemXsize(UnitAny* pUnit)
{
	if (pUnit->dwType == UNIT_ITEM && pUnit->pItemData) {
		if (!fpGetItemText(pUnit->dwTxtFileNo))
			return 0;
		return (fpGetItemText(pUnit->dwTxtFileNo)->xSize);
	}
	return 0;
}

int ItemYsize(UnitAny* pUnit)
{
	if (pUnit->dwType == UNIT_ITEM && pUnit->pItemData) {
		if (!fpGetItemText(pUnit->dwTxtFileNo))
			return 0;
		return (fpGetItemText(pUnit->dwTxtFileNo)->ySize);
	}
	return 0;
}

int Itemtype(UnitAny* pUnit)
{
	Room1* pRoom = NULL;
	if (pUnit->dwType == UNIT_OBJECT && pUnit->pObjectData)
	{
		pRoom = D2COMMON_GetRoomFromUnit(pUnit);
		if (pRoom && D2COMMON_GetLevelNoFromRoom(pRoom))
			return (pUnit->pObjectData->Type & 255);
		else
			return pUnit->pObjectData->Type;
	}
	return 0;
}

void ReadProcessBYTES(HANDLE hProcess, DWORD lpAddress, void* buf, int len)
{
	DWORD oldprot, dummy = 0;
	VirtualProtectEx(hProcess, (void*)lpAddress, len, PAGE_READWRITE, &oldprot);
	ReadProcessMemory(hProcess, (void*)lpAddress, buf, len, 0);
	VirtualProtectEx(hProcess, (void*)lpAddress, len, oldprot, &dummy);
}

DWORD check_1 = Pointer::GetDllOffset("D2Client.dll", 0x11CB1C);
DWORD check_2 = Pointer::GetDllOffset("D2Client.dll", 0x11CB28);
DWORD read_1 = Pointer::GetDllOffset("D2Win.DLL", 0xC9E68);
char* Itemdesc(UnitAny* pUnit)
{
	char* res = NULL;
	if (pUnit->dwType != UNIT_ITEM)
		return res;


	wchar_t wBuffer[2048] = L"";
	wchar_t bBuffer[1] = { 1 };
	if (pUnit->pItemData && pUnit->pItemData->pOwnerInventory && pUnit->pItemData->pOwnerInventory->pOwner)
	{
		::WriteProcessMemory(GetCurrentProcess(), (void*)check_1, bBuffer, 1, NULL);
		::WriteProcessMemory(GetCurrentProcess(), (void*)check_2, &pUnit, 4, NULL);

		fpLoadItemDesc(pUnit->pItemData->pOwnerInventory->pOwner, 0);
		ReadProcessBYTES(GetCurrentProcess(), read_1, wBuffer, 2047);
	}

	char *tmp = UnicodeToAnsi(wBuffer);
	if (tmp)
	{
		return tmp;
		delete[] tmp;
		tmp = NULL;
	}
	return res;
}

#pragma endregion
