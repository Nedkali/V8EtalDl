#include "Accessors.h"
#include "Constants.h"
#include "D2Helpers.h"

//Unit Accessors
#pragma region Unit
//Need to wrap these as a single accesible property (Unit)

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
	char res[256] = "";
	if (pUnit->dwType == UNIT_ITEM && pUnit->pItemData)
		if (fpGetItemMagicalMods(pUnit->pItemData->wMagicPrefix[0]))
			strcpy_s(res, 256, fpGetItemMagicalMods(pUnit->pItemData->wMagicPrefix[0]));
	return res;
}

char* Itemsuffix(UnitAny* pUnit)
{
	char res[256] = "";
	if (pUnit->dwType == UNIT_ITEM && pUnit->pItemData)
		if (fpGetItemMagicalMods(pUnit->pItemData->wMagicSuffix[0]))
			strcpy_s(res, 256, fpGetItemMagicalMods(pUnit->pItemData->wMagicSuffix[0]));
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

//Me Accessors
#pragma region Me

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
	return info.GetReturnValue().Set(Integer::New(*vpAlwaysRun));
	//return info.GetReturnValue().Set(Integer::New(val));
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
	return info.GetReturnValue().Set(Integer::New(val));
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

void GetMode(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	DWORD res = (fpGetPlayerUnit()->dwMode);
	return info.GetReturnValue().Set(Integer::New(res));
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

#pragma endregion

