#include "JSMethods.h"
#include "D2Helpers.h"
#include "Constants.h"
#include "MPQStats.h"
#include "Accessors.h"
#define ArraySize(x) (sizeof((x)) / sizeof((x)[0]))

//helpers
const char* cstring(const v8::String::Utf8Value& value)
{
	return *value ? *value : "<string conversion failed>";
}


//Me Methods
#pragma region Me


JS_METHOD(meGetItems)
{
	Isolate* isolate = args.GetIsolate();
	HandleScope scope(isolate);
	
	uint32_t nClassId = (uint32_t)-1;
	uint32_t nMode = (uint32_t)-1;
	uint32_t nUnitId = (uint32_t)-1;
	uint32_t nType = (uint32_t)4;

	char szName[128] = "";
	char tmp[256] = "";
	if (args.Length() == 0)
	{
		myUnit* lpUnit = (myUnit*)fpGetPlayerUnit();

		/*if (!lpUnit || (lpUnit->_dwPrivateType & PRIVATE_UNIT) != PRIVATE_UNIT)
			return args.GetReturnValue().Set(Boolean::New(false));*/

		UnitAny* pUnit = fpGetPlayerUnit(); // D2CLIENT_FindUnit(lpUnit->dwUnitId, lpUnit->dwType);

		/*if (!pUnit || !pUnit->pInventory || !pUnit->pInventory->pFirstItem)
			return args.GetReturnValue().Set(Boolean::New(false));*/

		Array* pReturnArray = *Array::New();

		int dwArrayCount = 0;

		for (UnitAny* pItem = pUnit->pInventory->pFirstItem; pItem; pItem = fpGetNextItemFromInventory(pItem), dwArrayCount++)
		{
			invUnit* pmyUnit = new invUnit;

			if (!pmyUnit)
				continue;

			GetUnitName(pItem, tmp, 256);
			pmyUnit->_dwPrivateType = PRIVATE_UNIT;
			pmyUnit->szName[0] = NULL;
			pmyUnit->dwMode = pItem->dwMode;
			pmyUnit->dwClassId = pItem->dwTxtFileNo;
			pmyUnit->dwUnitId = pItem->dwUnitId;
			pmyUnit->dwType = UNIT_ITEM;
			pmyUnit->dwOwnerId = pUnit->dwUnitId;
			pmyUnit->dwOwnerType = pUnit->dwType;

			Object* node = *Object::New();
			
			//node->Set(String::NewFromUtf8(isolate, "GetStat"), FunctionTemplate::New(unitGetStat));
			node->Set(String::NewFromUtf8(isolate, "name"), String::New(tmp));
			node->Set(String::NewFromUtf8(isolate, "quality"), Integer::New(pItem->pItemData->dwQuality));
			node->Set(String::NewFromUtf8(isolate, "itemloc"), Integer::New(pItem->pItemData->GameLocation));
			node->Set(String::NewFromUtf8(isolate, "itemlevel"), Integer::New(pItem->pItemData->dwItemLevel));
			//node->Set(String::NewFromUtf8(isolate, "itemclass"), Integer::New(Itemclass(pUnit)));
			node->Set(String::NewFromUtf8(isolate, "itemdesc"), String::New(Itemdesc(pItem)));
			//node->Set(String::NewFromUtf8(isolate, "itemflag"), String::New(Itemflag(pUnit)));
			//node->Set(String::NewFromUtf8(isolate, "ip"), String::New(Itemprefix(pItem)));
			//node->Set(String::NewFromUtf8(isolate, "itemsuffix"), String::New(Itemsuffix(pUnit)));
			//node->Set(String::NewFromUtf8(isolate, "itemtype"), Integer::New(Itemtype(pUnit)));
			node->Set(String::NewFromUtf8(isolate, "classid"), Integer::New(pmyUnit->dwClassId));
			node->Set(String::NewFromUtf8(isolate, "mode"), Integer::New(pmyUnit->dwMode));
			node->Set(String::NewFromUtf8(isolate, "unitid"), Integer::New(pmyUnit->dwUnitId));
			node->Set(String::NewFromUtf8(isolate, "type"), Integer::New(pmyUnit->dwType));
			node->Set(String::NewFromUtf8(isolate, "xsize"), Integer::New(ItemXsize(pItem)));
			node->Set(String::NewFromUtf8(isolate, "ysize"), Integer::New(ItemYsize(pItem)));
			Vars.thisUnit = pItem; 
			pReturnArray->Set(dwArrayCount, node->ToObject());

		}
		args.GetReturnValue().Set(pReturnArray->ToObject());
	}
	else
	{
		if (args.Length() > 0 && args[0]->IsString()) {
			String::Utf8Value str(args[0]);
			char* cstr = (char*)cstring(str);
			strcpy_s(szName, sizeof(szName), cstr);
		}
		if (args.Length() > 0 && args[0]->IsNumber())
			nClassId = args[0]->Uint32Value();

		if (args.Length() > 1 && args[1]->IsNumber())
			nMode = args[1]->Uint32Value();

		if (args.Length() > 2 && args[2]->IsNumber())
			nUnitId = args[2]->Uint32Value();

		UnitAny* pItem = GetUnit(szName, nClassId, nType, nMode, nUnitId);

		pItem = GetUnit(szName, nClassId, nType, nMode, nUnitId);

		if (!pItem)
			args.GetReturnValue().Set(Boolean::New(true));
		
		myUnit *pmyUnit = (myUnit*)pItem;

		invUnit* pmyItem = new invUnit; // leaked?

		if (!pmyItem)
			return args.GetReturnValue().Set(Boolean::New(false));

		GetUnitName(pItem, tmp, 256);
		pmyItem->_dwPrivateType = PRIVATE_ITEM;
		pmyItem->dwClassId = nClassId;
		pmyItem->dwMode = nMode;
		pmyItem->dwType = pItem->dwType;
		pmyItem->dwUnitId = pItem->dwUnitId;
		pmyItem->dwOwnerId = pmyUnit->dwUnitId;
		pmyItem->dwOwnerType = pmyUnit->dwType;
		strcpy_s(pmyItem->szName, sizeof(pmyItem->szName), szName);

		ObjectTemplate* node = *ObjectTemplate::New();

		node->Set(String::NewFromUtf8(isolate, "name"), String::New(tmp));
		node->Set(String::NewFromUtf8(isolate, "quality"), Integer::New(pItem->pItemData->dwQuality));
		node->Set(String::NewFromUtf8(isolate, "itemloc"), Integer::New(pItem->pItemData->GameLocation));
		node->Set(String::NewFromUtf8(isolate, "itemlevel"), Integer::New(pItem->pItemData->dwItemLevel));
		//node->Set(String::NewFromUtf8(isolate, "itemclass"), Integer::New(Itemclass(pUnit)));
		node->Set(String::NewFromUtf8(isolate, "itemdesc"), String::New(Itemdesc(pItem)));
		//node->Set(String::NewFromUtf8(isolate, "itemflag"), String::New(Itemflag(pUnit)));
		//node->Set(String::NewFromUtf8(isolate, "ip"), String::New(Itemprefix(pItem)));
		//node->Set(String::NewFromUtf8(isolate, "itemsuffix"), String::New(Itemsuffix(pUnit)));
		//node->Set(String::NewFromUtf8(isolate, "itemtype"), Integer::New(Itemtype(pUnit)));
		node->Set(String::NewFromUtf8(isolate, "classid"), Integer::New(pmyItem->dwClassId));
		node->Set(String::NewFromUtf8(isolate, "mode"), Integer::New(pmyItem->dwMode));
		node->Set(String::NewFromUtf8(isolate, "unitid"), Integer::New(pmyItem->dwUnitId));
		node->Set(String::NewFromUtf8(isolate, "type"), Integer::New(pmyItem->dwType));
		node->Set(String::NewFromUtf8(isolate, "xsize"), Integer::New(ItemXsize(pItem)));
		node->Set(String::NewFromUtf8(isolate, "ysize"), Integer::New(ItemYsize(pItem)));
		node->Set(String::NewFromUtf8(isolate, "GetStat"), FunctionTemplate::New(unitGetStat, args.This()));

		Vars.thisUnit = pItem;

		args.GetReturnValue().Set(node->NewInstance());
	}
}

JS_METHOD(meClickItem)
{
	HandleScope handle_scope(args.GetIsolate());
	typedef void __fastcall clickequip(UnitAny * pPlayer, Inventory * pIventory, int loc);
	
	if (*vpTransactionDialog != 0 || *vpTransactionDialogs != 0 || *vpTransactionDialogs_2 != 0)
	{
		return args.GetReturnValue().Set(Boolean::New(false));
	}

	myUnit* pmyUnit = NULL;
	UnitAny* pUnit = NULL;


	POINT Belt[] =
	{
		{ 0,0 }, // 0
		{ 1,0 }, // 1
		{ 2,0 }, // 2
		{ 3,0 }, // 3

		{ 0,1 }, // 4
		{ 1,1 }, // 5
		{ 2,1 }, // 6
		{ 3,1 }, // 7

		{ 0,2 }, // 8
		{ 1,2 }, // 9
		{ 2,2 }, // 10
		{ 3,2 }, // 11

		{ 0,3 }, // 12
		{ 1,3 }, // 13
		{ 2,3 }, // 14
		{ 3,3 }, // 15
	};

	*vpCursorHoverX = 0xFFFFFFFF;
	*vpCursorHoverY = 0xFFFFFFFF;

	DWORD BodyClickTable = Pointer::GetDllOffset("D2Client.dll", 0xEE4B8);

	if (args.Length() == 1 && args[0]->IsNumber())
	{
		int32_t nBodyLoc = args[0]->Uint32Value();

		clickequip * click = (clickequip*)*(DWORD*)(BodyClickTable + (4 * nBodyLoc));

		if (!click)
		{
			return args.GetReturnValue().Set(Boolean::New(false));
		}

		click(fpGetPlayerUnit(), fpGetPlayerUnit()->pInventory, nBodyLoc);
		return args.GetReturnValue().Set(Boolean::New(true));
	}
	else if (args.Length() == 1 && args[0]->IsObject())
	{
		pmyUnit = (myUnit*)Vars.thisUnit;

		pUnit = Vars.thisUnit;

		clickequip * click = (clickequip*)*(DWORD*)(BodyClickTable + (4 * pUnit->pItemData->BodyLocation));

		if (!click)
		{
			return args.GetReturnValue().Set(Boolean::New(false));
		}

		click(fpGetPlayerUnit(), fpGetPlayerUnit()->pInventory, pUnit->pItemData->BodyLocation);
		return args.GetReturnValue().Set(Boolean::New(true));
	}
	else if (args.Length() == 2 && args[0]->IsNumber() && args[1]->IsNumber())
	{
		int32_t nClickType = args[0]->Uint32Value();
		int32_t nBodyLoc = args[1]->Uint32Value();


		if (nClickType == NULL)
		{
			clickequip * click = (clickequip*)*(DWORD*)(BodyClickTable + (4 * nBodyLoc));

			if (!click)
			{
				return args.GetReturnValue().Set(Boolean::New(false));
			}

			click(fpGetPlayerUnit(), fpGetPlayerUnit()->pInventory, nBodyLoc);

		}
		// Click Merc Gear
		else if (nClickType == 4)
		{
			if (nBodyLoc == 1 || nBodyLoc == 3 || nBodyLoc == 4)
			{
				UnitAny* pMerc = fpGetMercUnit();

				if (pMerc)
				{
					D2CLIENT_MercItemAction(0x61, nBodyLoc);
					return args.GetReturnValue().Set(Boolean::New(true));
				}
			}
		}
		return args.GetReturnValue().Set(Boolean::New(true));
	}
	else if (args.Length() == 2 && args[0]->IsNumber() && args[1]->IsObject())
	{
		pmyUnit = (myUnit*)Vars.thisUnit;

		pUnit = Vars.thisUnit;

		int32_t nClickType = args[0]->Uint32Value();

		int InventoryLocation = GetItemLocation(pUnit);
		int ClickLocation = LOCATION_NULL;

		int x = pUnit->pItemPath->dwPosX;
		int y = pUnit->pItemPath->dwPosY;

		*vpCursorHoverX = x;
		*vpCursorHoverY = y;

		InventoryLayout* pLayout = NULL;

		if (nClickType == 4)
		{
			UnitAny* pMerc = fpGetMercUnit();

			if (pMerc)
				if (pUnit->pItemData && pUnit->pItemData->pOwner)
					if (pUnit->pItemData->pOwner->dwUnitId == pMerc->dwUnitId)
					{
						D2CLIENT_MercItemAction(0x61, pUnit->pItemData->BodyLocation);
						return args.GetReturnValue().Set(Boolean::New(true));
					}
			return args.GetReturnValue().Set(Boolean::New(false));
		}
		else if (InventoryLocation == LOCATION_INVENTORY || InventoryLocation == LOCATION_STASH || InventoryLocation == LOCATION_CUBE)
		{
			switch (InventoryLocation)
			{
			case LOCATION_INVENTORY:
				pLayout = (InventoryLayout*)vpInventoryLayout;
				ClickLocation = CLICKTARGET_INVENTORY;
				break;
			case LOCATION_STASH:
				pLayout = (InventoryLayout*)vpStashLayout;
				ClickLocation = CLICKTARGET_STASH;
				break;
			case LOCATION_CUBE:
				pLayout = (InventoryLayout*)vpCubeLayout;
				ClickLocation = CLICKTARGET_CUBE;
				break;
			}

			x = pLayout->Left + x * pLayout->SlotPixelWidth + 10;
			y = pLayout->Top + y * pLayout->SlotPixelHeight + 10;

			if (nClickType == NULL)
				fpLeftClickItem(fpGetPlayerUnit(), fpGetPlayerUnit()->pInventory, x, y, nClickType, pLayout, ClickLocation);
			else
				D2CLIENT_RightClickItem(x, y, ClickLocation, fpGetPlayerUnit(), fpGetPlayerUnit()->pInventory);

		}
		else if (InventoryLocation == LOCATION_BELT)
		{
			int i = x;

			if (i < 0 || i > 0x0F)
			{
				return args.GetReturnValue().Set(Boolean::New(false));
			}

			if (fpGetScreenSize() == 2)
			{
				x = 440 + (Belt[i].x * 29);
				y = 580 - (Belt[i].y * 29);
			}
			else {
				x = 360 + (Belt[i].x * 29);
				y = 460 - (Belt[i].y * 29);
			}
			if (nClickType == NULL)
				D2CLIENT_ClickBelt(x, y, fpGetPlayerUnit()->pInventory);
			else
				D2CLIENT_ClickBeltRight(fpGetPlayerUnit()->pInventory, fpGetPlayerUnit(), nClickType == 1 ? FALSE : TRUE, i);
		}
		else if (fpGetCursorItem() == pUnit)
		{
			if (nClickType < 1 || nClickType > 12)
			{
				return args.GetReturnValue().Set(Boolean::New(false));
			}

			clickequip * click = (clickequip*)*(DWORD*)(BodyClickTable + (4 * nClickType));

			if (!click)
			{
				return args.GetReturnValue().Set(Boolean::New(false));
			}

			click(fpGetPlayerUnit(), fpGetPlayerUnit()->pInventory, nClickType);
			return args.GetReturnValue().Set(Boolean::New(true));
		}
	}
	else if (args.Length() == 4)
	{
		if (args[0]->IsNumber() && args[1]->IsNumber() && args[2]->IsNumber() && args[3]->IsNumber())
		{
			int32_t nButton = args[0]->Uint32Value();
			int32_t nX = args[1]->Uint32Value();
			int32_t nY = args[2]->Uint32Value();
			int32_t nLoc = args[3]->Uint32Value();

			int clickTarget = LOCATION_NULL;
			InventoryLayout* pLayout = NULL;

			*vpCursorHoverX = nX;
			*vpCursorHoverY = nY;

			// Fixing the x/y click spot for items taking more than one inventory square- so Diablo can handle it!
			if (nLoc != LOCATION_BELT)
			{
				UnitAny* pItem = fpGetCursorItem();
				if (pItem)
				{
					ItemText* pTxt = fpGetItemText(pItem->dwTxtFileNo);
					if (pTxt)
					{
						if (pTxt->ySize > 1)
							nY += 1;

						if (pTxt->xSize > 1)
							nX += 1;
					}

				}
			}
			//nLoc is click target locations=: LOCATION_INVENTORY=inventory, LOCATION_TRADE=player trade, LOCATION_CUBE=cube, LOCATION_STASH=stash, LOCATION_BELT=belt
			if (nLoc == LOCATION_INVENTORY || nLoc == LOCATION_TRADE || nLoc == LOCATION_CUBE || nLoc == LOCATION_STASH)
			{
				switch (nLoc)
				{
				case LOCATION_INVENTORY:
					pLayout = (InventoryLayout*)vpInventoryLayout;
					clickTarget = CLICKTARGET_INVENTORY;
					break;
				case LOCATION_TRADE:
					pLayout = (InventoryLayout*)vpTradeLayout;
					clickTarget = CLICKTARGET_TRADE;
					break;
				case LOCATION_CUBE:
					pLayout = (InventoryLayout*)vpCubeLayout;
					clickTarget = CLICKTARGET_CUBE;
					break;
				case LOCATION_STASH:
					pLayout = (InventoryLayout*)vpStashLayout;
					clickTarget = CLICKTARGET_STASH;
					break;
				}

				int	x = pLayout->Left + nX * pLayout->SlotPixelWidth + 10;
				int	y = pLayout->Top + nY * pLayout->SlotPixelHeight + 10;

				if (nButton == 0) // Left Click
					fpLeftClickItem(fpGetPlayerUnit(), fpGetPlayerUnit()->pInventory, x, y, 1, pLayout, clickTarget);
				else if (nButton == 1) // Right Click
					D2CLIENT_RightClickItem(x, y, clickTarget, fpGetPlayerUnit(), fpGetPlayerUnit()->pInventory);
				else if (nButton == 2) // Shift Left Click
					fpLeftClickItem(fpGetPlayerUnit(), fpGetPlayerUnit()->pInventory, x, y, 5, pLayout, clickTarget);

				return args.GetReturnValue().Set(Boolean::New(true));
			}
			else if (nLoc == LOCATION_BELT) // Belt
			{

				int z = -1;

				for (UINT i = 0; i < ArraySize(Belt); i++)
				{
					if (Belt[i].x == nX && Belt[i].y == nY)
					{
						z = (int)i;
						break;
					}
				}

				if (z == -1)
				{
					return args.GetReturnValue().Set(Boolean::New(false));
				}

				int x = NULL;
				int y = NULL;

				if (fpGetScreenSize() == 2)
				{
					x = 440 + (Belt[z].x * 29);
					y = 580 - (Belt[z].y * 29);
				}
				else {
					x = 360 + (Belt[z].x * 29);
					y = 460 - (Belt[z].y * 29);
				}

				if (nButton == 0)
					D2CLIENT_ClickBelt(x, y, fpGetPlayerUnit()->pInventory);
				else if (nButton == 1)
					D2CLIENT_ClickBeltRight(fpGetPlayerUnit(), fpGetPlayerUnit()->pInventory, FALSE, z);
				else if (nButton == 2)
					D2CLIENT_ClickBeltRight(fpGetPlayerUnit(), fpGetPlayerUnit()->pInventory, TRUE, z);

				return args.GetReturnValue().Set(Boolean::New(true));
			}
		}
	}
	return args.GetReturnValue().Set(Boolean::New(true));
}

JS_METHOD(meTakeWayPoint)
{
	HandleScope handle_scope(args.GetIsolate());
	UnitAny* pUnit = Vars.thisUnit;// D2CLIENT_FindUnit(lpUnit->dwUnitId, lpUnit->dwType);

	uint32_t nWaypointID = args[0]->Uint32Value();

	int retVal = 0;
	if (FillBaseStat("levels", nWaypointID, "Waypoint", &retVal, sizeof(int)))
		if (retVal == 255)
			return args.GetReturnValue().Set(Boolean::New(false));

	D2CLIENT_TakeWaypoint(pUnit->dwUnitId, nWaypointID);
	if (!D2CLIENT_GetUIState(UI_GAME))
		fpCloseInteract();
	return args.GetReturnValue().Set(Boolean::New(true));
}

JS_METHOD(meSetSkill)
{
	HandleScope handle_scope(args.GetIsolate());
	args.GetReturnValue().Set(Boolean::New(D2Funcs::SetSkill(args[0]->Uint32Value(), args[1]->BooleanValue())));
}

JS_METHOD(meCancel)
{
	HandleScope handle_scope(args.GetIsolate());
	DWORD automapOn = *vpAutomapOn;
	//need to make this more like d2nt
	int val = args[0]->Uint32Value();

	if (val == 0)
	{
		if (IsScrollingText())
			fpClearScreen(); 
		if (fpGetCurrentInteractingNPC())
			fpCloseNPCInteract();
		if (fpGetCursorItem())
			fpClickMap(0, 10, 10, 0x08);
		fpCloseInteract();
	}
	else if (val == 1)
	{
		if (IsScrollingText())
			fpClearScreen();
		if (fpGetCurrentInteractingNPC())
			fpCloseNPCInteract();
	}

	*vpAutomapOn = automapOn;

	args.GetReturnValue().Set(Boolean::New(true));
}

JS_METHOD(meRepair)
{
	HandleScope handle_scope(args.GetIsolate());
	myUnit* lpUnit = (myUnit*)fpGetPlayerUnit();

	if (!lpUnit || (lpUnit->_dwPrivateType & PRIVATE_UNIT) != PRIVATE_UNIT)
		args.GetReturnValue().Set(Boolean::New(false));

	UnitAny* pUnit = D2CLIENT_FindUnit(lpUnit->dwUnitId, lpUnit->dwType);

	if (!pUnit)
		args.GetReturnValue().Set(Boolean::New(false));

	BYTE aPacket[17] = { NULL };
	aPacket[0] = 0x35;
	*(DWORD*)&aPacket[1] = *vpRecentInteractId;
	aPacket[16] = 0x80;
	fpSendPacket(17, 1, aPacket);

	args.GetReturnValue().Set(Boolean::New(true));
}

JS_METHOD(meGetState)
{
	HandleScope handle_scope(args.GetIsolate());
	UnitAny* pUnit = fpGetPlayerUnit();

	if (!pUnit)
		return args.GetReturnValue().Set(Boolean::New(false));

	int32_t nState;
	nState = args[0]->Uint32Value();


	if (nState > 159 || nState < 0)
		return args.GetReturnValue().Set(Boolean::New(false));

	return args.GetReturnValue().Set(Boolean::New(!!fpGetUnitState(pUnit, nState)));
}

JS_METHOD(meGetQuest)
{
	HandleScope handle_scope(args.GetIsolate());
	if (args.Length() < 2 || !args[0]->IsNumber() || !args[1]->IsNumber())
		args.GetReturnValue().Set(Boolean::New(false));

	uint32_t nAct = args[0]->Uint32Value();
	uint32_t nQuest = args[1]->Uint32Value();	
	args.GetReturnValue().Set(Number::New(fpGetQuestFlag(fpGetQuestInfo(), nAct, nQuest)));
}

JS_METHOD(meSwapWeapons)
{
	HandleScope handle_scope(args.GetIsolate());
	INT32 val = Vars.nWeaponsTab;
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
				args.GetReturnValue().Set(Boolean::New(false));
		}
		BYTE aPacket[1];
		aPacket[0] = 0x60;
		fpSendPacket(1, 1, aPacket);
		args.GetReturnValue().Set(Boolean::New(true));
	}
	else
		return args.GetReturnValue().Set(Integer::New((*vpbWeapSwitch)));
}

JS_METHOD(meGetStat)
{
	HandleScope handle_scope(args.GetIsolate());
	UnitAny* pUnit = fpGetPlayerUnit();

	if (!pUnit)
		args.GetReturnValue().Set(Boolean::New(false));

	int32_t nStat = 0;
	int32_t nSubIndex = 0;

	nStat = args[0]->Uint32Value();

	if (nStat >= STAT_HP && nStat <= STAT_MAXSTAMINA)
		args.GetReturnValue().Set(Integer::New((D2COMMON_GetUnitStat(pUnit, nStat, nSubIndex) >> 8)));
	else if (nStat == STAT_EXP || nStat == STAT_LASTEXP || nStat == STAT_NEXTEXP) {
		int32_t rval = 0;
		rval = (unsigned int)D2COMMON_GetUnitStat(pUnit, nStat, nSubIndex);
		args.GetReturnValue().Set(Integer::New(rval));
	}
	else if (nStat == STAT_ITEMLEVELREQ)
		args.GetReturnValue().Set(Integer::New((fpGetItemLevelRequirements(pUnit, fpGetPlayerUnit()))));
	else if (nStat == -1)
	{
		Stat aStatList[256] = { NULL };
		StatList* pStatList = D2COMMON_GetStatList(pUnit, NULL, 0x40);

		if (pStatList)
		{
			DWORD dwStats = D2COMMON_CopyStatList(pStatList, (Stat*)aStatList, 256);
			Local<Array> pReturnArray = Array::New();
			int32_t nIndex, nSubIndex, nValue = NULL;
			for (int j = 0; j < pUnit->pStats->StatVec.wCount; j++)
			{
				bool inListAlready = false;
				for (DWORD k = 0; k < dwStats; k++) {
					if (aStatList[k].dwStatValue == pUnit->pStats->StatVec.pStats[j].dwStatValue &&
						aStatList[k].wStatIndex == pUnit->pStats->StatVec.pStats[j].wStatIndex &&
						aStatList[k].wSubIndex == pUnit->pStats->StatVec.pStats[j].wSubIndex)
						inListAlready = true;
				}
				if (!inListAlready) {

					aStatList[dwStats].dwStatValue = pUnit->pStats->StatVec.pStats[j].dwStatValue;
					aStatList[dwStats].wStatIndex = pUnit->pStats->StatVec.pStats[j].wStatIndex;
					aStatList[dwStats].wSubIndex = pUnit->pStats->StatVec.pStats[j].wSubIndex;
					dwStats++;
				}

			}
			for (UINT i = 0; i < dwStats; i++)
			{
				Local<Array> pArrayInsert = Array::New();
				nIndex = (aStatList[i].wStatIndex);
				nSubIndex = (aStatList[i].wSubIndex);
				nValue = (aStatList[i].dwStatValue);

				pArrayInsert->Set(0, Integer::New(nIndex));
				pArrayInsert->Set(1, Integer::New(nSubIndex));
				pArrayInsert->Set(1, Integer::New(nValue));
				pReturnArray->Set(i, pArrayInsert);

			}
			args.GetReturnValue().Set(pReturnArray);
		}
	}
	else
	{
		long result = D2COMMON_GetUnitStat(pUnit, nStat, nSubIndex);
		if (result == 0) // if stat isnt found look up preset list
		{
			StatList* pStatList = D2COMMON_GetStatList(pUnit, NULL, 0x40);
			Stat aStatList[256] = { NULL };
			if (pStatList)
			{
				DWORD dwStats = D2COMMON_CopyStatList(pStatList, (Stat*)aStatList, 256);
				for (UINT i = 0; i < dwStats; i++)
				{
					if (nStat == aStatList[i].wStatIndex && nSubIndex == aStatList[i].wSubIndex)
						result = (aStatList[i].dwStatValue);
				}
			}
		}
		args.GetReturnValue().Set(Integer::New(result));
	}
}

JS_METHOD(meSelectNPCMenu)
{
	HandleScope handle_scope(args.GetIsolate());
	UnitAny* pNPC = fpGetCurrentInteractingNPC();

	if (!pNPC)
		args.GetReturnValue().Set(Boolean::New(false));

	return args.GetReturnValue().Set(Boolean::New(ClickNPCMenu(pNPC->dwTxtFileNo, args[0]->Uint32Value())));
}

#pragma endregion 

//Unit Methods
#pragma region UNIT

JS_METHOD(unitGetItems)
{

	Isolate* isolate = args.GetIsolate();
	HandleScope scope(isolate);

	uint32_t nClassId = (uint32_t)-1;
	uint32_t nMode = (uint32_t)-1;
	uint32_t nUnitId = (uint32_t)-1;
	uint32_t nType = (uint32_t)4;

	char szName[128] = "";
	char tmp[256] = "";
	if (args.Length() == 0)
	{
		myUnit* lpUnit = (myUnit*)Vars.thisUnit;

		UnitAny* pUnit = Vars.thisUnit;

		Array* pReturnArray = *Array::New();

		int dwArrayCount = 0;

		for (UnitAny* pItem = pUnit->pInventory->pFirstItem; pItem; pItem = fpGetNextItemFromInventory(pItem), dwArrayCount++)
		{
			invUnit* pmyUnit = new invUnit;

			if (!pmyUnit)
				continue;

			GetUnitName(pItem, tmp, 256);
			pmyUnit->_dwPrivateType = PRIVATE_UNIT;
			pmyUnit->szName[0] = NULL;
			pmyUnit->dwMode = pItem->dwMode;
			pmyUnit->dwClassId = pItem->dwTxtFileNo;
			pmyUnit->dwUnitId = pItem->dwUnitId;
			pmyUnit->dwType = UNIT_ITEM;
			pmyUnit->dwOwnerId = pUnit->dwUnitId;
			pmyUnit->dwOwnerType = pUnit->dwType;

			Object* node = *Object::New();

			node->Set(String::NewFromUtf8(isolate, "name"), String::New(tmp));
			node->Set(String::NewFromUtf8(isolate, "quality"), Integer::New(pItem->pItemData->dwQuality));
			node->Set(String::NewFromUtf8(isolate, "itemloc"), Integer::New(pItem->pItemData->GameLocation));
			node->Set(String::NewFromUtf8(isolate, "itemlevel"), Integer::New(pItem->pItemData->dwItemLevel));
			//node->Set(String::NewFromUtf8(isolate, "itemclass"), Integer::New(Itemclass(pUnit)));
			node->Set(String::NewFromUtf8(isolate, "itemdesc"), String::New(Itemdesc(pItem)));
			//node->Set(String::NewFromUtf8(isolate, "itemflag"), String::New(Itemflag(pUnit)));
			//node->Set(String::NewFromUtf8(isolate, "ip"), String::New(Itemprefix(pItem)));
			//node->Set(String::NewFromUtf8(isolate, "itemsuffix"), String::New(Itemsuffix(pUnit)));
			//node->Set(String::NewFromUtf8(isolate, "itemtype"), Integer::New(Itemtype(pUnit)));
			node->Set(String::NewFromUtf8(isolate, "classid"), Integer::New(pmyUnit->dwClassId));
			node->Set(String::NewFromUtf8(isolate, "mode"), Integer::New(pmyUnit->dwMode));
			node->Set(String::NewFromUtf8(isolate, "unitid"), Integer::New(pmyUnit->dwUnitId));
			node->Set(String::NewFromUtf8(isolate, "type"), Integer::New(pmyUnit->dwType));
			node->Set(String::NewFromUtf8(isolate, "xsize"), Integer::New(ItemXsize(pItem)));
			node->Set(String::NewFromUtf8(isolate, "ysize"), Integer::New(ItemYsize(pItem)));
			//node->Set(String::NewFromUtf8(isolate, "GetStat"), FunctionTemplate::New(unitGetStat, args.This()));
			Vars.thisUnit = pItem;
			pReturnArray->Set(dwArrayCount, node->ToObject());

		}
		args.GetReturnValue().Set(pReturnArray->ToObject());
	}
	else
	{
		if (args.Length() > 0 && args[0]->IsString()) {
			String::Utf8Value str(args[0]);
			char* cstr = (char*)cstring(str);
			strcpy_s(szName, sizeof(szName), cstr);
		}
		if (args.Length() > 0 && args[0]->IsNumber())
			nClassId = args[0]->Uint32Value();

		if (args.Length() > 1 && args[1]->IsNumber())
			nMode = args[1]->Uint32Value();

		if (args.Length() > 2 && args[2]->IsNumber())
			nUnitId = args[2]->Uint32Value();

		UnitAny* pItem = GetUnit(szName, nClassId, nType, nMode, nUnitId);

		pItem = GetUnit(szName, nClassId, nType, nMode, nUnitId);

		if (!pItem)
			args.GetReturnValue().Set(Boolean::New(true));

		Vars.thisUnit = pItem;

		myUnit *pmyUnit = (myUnit*)pItem;

		invUnit* pmyItem = new invUnit; // leaked?

		if (!pmyItem)
			return args.GetReturnValue().Set(Boolean::New(false));

		GetUnitName(pItem, tmp, 256);
		pmyItem->_dwPrivateType = PRIVATE_ITEM;
		pmyItem->dwClassId = nClassId;
		pmyItem->dwMode = nMode;
		pmyItem->dwType = pItem->dwType;
		pmyItem->dwUnitId = pItem->dwUnitId;
		pmyItem->dwOwnerId = pmyUnit->dwUnitId;
		pmyItem->dwOwnerType = pmyUnit->dwType;
		strcpy_s(pmyItem->szName, sizeof(pmyItem->szName), szName);

		ObjectTemplate* node = *ObjectTemplate::New();

		node->Set(String::NewFromUtf8(isolate, "name"), String::New(tmp));
		node->Set(String::NewFromUtf8(isolate, "quality"), Integer::New(pItem->pItemData->dwQuality));
		node->Set(String::NewFromUtf8(isolate, "itemloc"), Integer::New(pItem->pItemData->GameLocation));
		node->Set(String::NewFromUtf8(isolate, "itemlevel"), Integer::New(pItem->pItemData->dwItemLevel));
		//node->Set(String::NewFromUtf8(isolate, "itemclass"), Integer::New(Itemclass(pUnit)));
		node->Set(String::NewFromUtf8(isolate, "itemdesc"), String::New(Itemdesc(pItem)));
		//node->Set(String::NewFromUtf8(isolate, "itemflag"), String::New(Itemflag(pUnit)));
		//node->Set(String::NewFromUtf8(isolate, "ip"), String::New(Itemprefix(pItem)));
		//node->Set(String::NewFromUtf8(isolate, "itemsuffix"), String::New(Itemsuffix(pUnit)));
		//node->Set(String::NewFromUtf8(isolate, "itemtype"), Integer::New(Itemtype(pUnit)));
		node->Set(String::NewFromUtf8(isolate, "classid"), Integer::New(pmyItem->dwClassId));
		node->Set(String::NewFromUtf8(isolate, "mode"), Integer::New(pmyItem->dwMode));
		node->Set(String::NewFromUtf8(isolate, "unitid"), Integer::New(pmyItem->dwUnitId));
		node->Set(String::NewFromUtf8(isolate, "type"), Integer::New(pmyItem->dwType));
		node->Set(String::NewFromUtf8(isolate, "xsize"), Integer::New(ItemXsize(pItem)));
		node->Set(String::NewFromUtf8(isolate, "ysize"), Integer::New(ItemYsize(pItem)));
		//node->Set(String::NewFromUtf8(isolate, "GetStat"), FunctionTemplate::New(unitGetStat, args.This()));

		args.GetReturnValue().Set(node->NewInstance());
	}
}

JS_METHOD(unitGetState)
{
	HandleScope handle_scope(args.GetIsolate());
	UnitAny* pUnit = Vars.thisUnit;

	if (!pUnit)
		return args.GetReturnValue().Set(Boolean::New(false));

	int32_t nState;
	nState = args[0]->Uint32Value();


	if (nState > 159 || nState < 0)
		return args.GetReturnValue().Set(Boolean::New(false));

	return args.GetReturnValue().Set(Boolean::New(!!fpGetUnitState(pUnit, nState)));
}

JS_METHOD(unitGetStat)
{
	HandleScope handle_scope(args.GetIsolate());

	UnitAny* pUnit = Vars.thisUnit;

	if (!pUnit)
		args.GetReturnValue().Set(Boolean::New(false));

	int32_t nStat = 0;
	int32_t nSubIndex = 0;

	nStat = args[0]->Uint32Value();
	
	if (nStat >= STAT_HP && nStat <= STAT_MAXSTAMINA)
		args.GetReturnValue().Set(Integer::New((D2COMMON_GetUnitStat(pUnit, nStat, nSubIndex) >> 8)));
	else if (nStat == STAT_EXP || nStat == STAT_LASTEXP || nStat == STAT_NEXTEXP) {
		int32_t rval = 0;
		rval = (unsigned int)D2COMMON_GetUnitStat(pUnit, nStat, nSubIndex);
		args.GetReturnValue().Set(Integer::New(rval));
	}
	else if (nStat == STAT_ITEMLEVELREQ)
		args.GetReturnValue().Set(Integer::New((fpGetItemLevelRequirements(pUnit, fpGetPlayerUnit()))));
	else if (nStat == -1)
	{
		Stat aStatList[256] = { NULL };
		StatList* pStatList = D2COMMON_GetStatList(pUnit, NULL, 0x40);

		if (pStatList)
		{
			DWORD dwStats = D2COMMON_CopyStatList(pStatList, (Stat*)aStatList, 256);
			Local<Array> pReturnArray = Array::New();
			int32_t nIndex, nSubIndex, nValue = NULL;
			for (int j = 0; j < pUnit->pStats->StatVec.wCount; j++)
			{
				bool inListAlready = false;
				for (DWORD k = 0; k < dwStats; k++) {
					if (aStatList[k].dwStatValue == pUnit->pStats->StatVec.pStats[j].dwStatValue &&
						aStatList[k].wStatIndex == pUnit->pStats->StatVec.pStats[j].wStatIndex &&
						aStatList[k].wSubIndex == pUnit->pStats->StatVec.pStats[j].wSubIndex)
						inListAlready = true;
				}
				if (!inListAlready) {

					aStatList[dwStats].dwStatValue = pUnit->pStats->StatVec.pStats[j].dwStatValue;
					aStatList[dwStats].wStatIndex = pUnit->pStats->StatVec.pStats[j].wStatIndex;
					aStatList[dwStats].wSubIndex = pUnit->pStats->StatVec.pStats[j].wSubIndex;
					dwStats++;
				}

			}
			for (UINT i = 0; i < dwStats; i++)
			{
				Local<Array> pArrayInsert = Array::New();
				nIndex = (aStatList[i].wStatIndex);
				nSubIndex = (aStatList[i].wSubIndex);
				nValue = (aStatList[i].dwStatValue);

				pArrayInsert->Set(0, Integer::New(nIndex));
				pArrayInsert->Set(1, Integer::New(nSubIndex));
				pArrayInsert->Set(1, Integer::New(nValue));
				pReturnArray->Set(i, pArrayInsert);

			}
			args.GetReturnValue().Set(pReturnArray);
		}
	}
	else
	{
		long result = D2COMMON_GetUnitStat(pUnit, nStat, nSubIndex);
		if (result == 0) // if stat isnt found look up preset list
		{
			StatList* pStatList = D2COMMON_GetStatList(pUnit, NULL, 0x40);
			Stat aStatList[256] = { NULL };
			if (pStatList)
			{
				DWORD dwStats = D2COMMON_CopyStatList(pStatList, (Stat*)aStatList, 256);
				for (UINT i = 0; i < dwStats; i++)
				{
					if (nStat == aStatList[i].wStatIndex && nSubIndex == aStatList[i].wSubIndex)
						result = (aStatList[i].dwStatValue);
				}
			}
		}
		args.GetReturnValue().Set(Integer::New(result));
	}
}

//not working
JS_METHOD(unitGetNext)
{
	Isolate* isolate = args.GetIsolate();
	HandleScope handle_scope(isolate);

	int nType = -1;
	uint32_t nClassId = (uint32_t)-1;
	uint32_t nMode = (uint32_t)-1;
	uint32_t nUnitId = (uint32_t)-1;
	char szName[128] = "";
	char tmp[256] = "";
	
	Private* unit = (Private*)Vars.thisUnit;

	if (!unit)
		args.GetReturnValue().Set(Boolean::New(false));

	if (unit->dwPrivateType == PRIVATE_UNIT)
	{
		myUnit* lpUnit = (myUnit*)Vars.thisUnit;

		UnitAny* pUnit = D2CLIENT_FindUnit(lpUnit->dwUnitId, lpUnit->dwType);
				
		if (args.Length() > 0 && args[0]->IsString()) {
			String::Utf8Value str(args[0]);
			char* cstr = (char*)cstring(str);
			strcpy_s(lpUnit->szName, 128, cstr);
		}

		if (args.Length() > 0 && args[0]->IsNumber())
			lpUnit->dwClassId = args[0]->Uint32Value();

		if (args.Length() > 1 && args[1]->IsNumber())
			lpUnit->dwMode = args[1]->Uint32Value();
		MessageBoxA(NULL, "lookin for npcs", "DEBUG", MB_OK);

		pUnit = GetNextUnit(pUnit, lpUnit->szName, lpUnit->dwClassId, lpUnit->dwType, lpUnit->dwMode);

		GetUnitName(pUnit, tmp, 256);

		D2Funcs::Print("Name : %s", tmp);

		if (!pUnit)
		{
			args.GetReturnValue().Set(Boolean::New(false));
		}
		else
		{
			//lpUnit->dwUnitId = pUnit->dwUnitId;
			Vars.thisUnit = pUnit;
			MessageBoxA(NULL, "npc found", "DEBUG", MB_OK);
			ObjectTemplate* node = *ObjectTemplate::New();
			node->Set(String::NewFromUtf8(isolate, "name"), String::New(tmp));
			node->Set(String::NewFromUtf8(isolate, "classid"), Integer::New(pUnit->dwTxtFileNo));
			node->Set(String::NewFromUtf8(isolate, "mode"), Integer::New(pUnit->dwMode));
			node->Set(String::NewFromUtf8(isolate, "unitid"), Integer::New(pUnit->dwUnitId));
			node->Set(String::NewFromUtf8(isolate, "type"), Integer::New(pUnit->dwType));
			node->Set(String::NewFromUtf8(isolate, "x"), Integer::New(fpGetUnitX(pUnit)));
			node->Set(String::NewFromUtf8(isolate, "y"), Integer::New(fpGetUnitY(pUnit)));
			node->Set(String::NewFromUtf8(isolate, "GetStat"), FunctionTemplate::New(unitGetStat, args.This()));
			node->Set(String::NewFromUtf8(isolate, "GetNext"), FunctionTemplate::New(unitGetNext, args.This()));
			args.GetReturnValue().Set(node->NewInstance());
		}
	}
	else if (unit->dwPrivateType == PRIVATE_ITEM)
	{
		invUnit *pmyUnit = (invUnit*)unit;
		if (!pmyUnit)
			args.GetReturnValue().Set(Boolean::New(false));

		UnitAny* pUnit = D2CLIENT_FindUnit(pmyUnit->dwUnitId, pmyUnit->dwType);
		UnitAny* pOwner = D2CLIENT_FindUnit(pmyUnit->dwOwnerId, pmyUnit->dwOwnerType);

		GetUnitName(pUnit, tmp, 256);

		D2Funcs::Print("Name : %s", tmp);

		if (!pUnit || !pOwner)
			args.GetReturnValue().Set(Boolean::New(false));
		
		if (args.Length() > 0 && args[0]->IsString()) {
			String::Utf8Value str(args[0]);
			char* cstr = (char*)cstring(str);
			strcpy_s(pmyUnit->szName, 128, cstr);
		}

		if (args.Length() > 0 && args[0]->IsNumber())
			pmyUnit->dwClassId = args[0]->Uint32Value();

		if (args.Length() > 1 && args[1]->IsNumber())
			pmyUnit->dwMode = args[1]->Uint32Value();

		UnitAny* nextItem = GetInvNextUnit(pUnit, pOwner, pmyUnit->szName, pmyUnit->dwClassId, pmyUnit->dwMode);
		if (!nextItem)
		{
			args.GetReturnValue().Set(Boolean::New(false));
		}
		else
		{
			//pmyUnit->dwUnitId = nextItem->dwUnitId;

			/*if (pUnit->dwType == UNIT_ITEM && pUnit->pItemData) {
			if (!D2COMMON_GetItemText(pUnit->dwTxtFileNo))
			break;
			type = (D2COMMON_GetItemText(pUnit->dwTxtFileNo)->nType);
			}*/

			ObjectTemplate* node = *ObjectTemplate::New();
			node->Set(String::NewFromUtf8(isolate, "name"), String::New(tmp));
			node->Set(String::NewFromUtf8(isolate, "quality"), Integer::New(pUnit->pItemData->dwQuality));
			node->Set(String::NewFromUtf8(isolate, "itemloc"), Integer::New(pUnit->pItemData->GameLocation));
			node->Set(String::NewFromUtf8(isolate, "itemlevel"), Integer::New(pUnit->pItemData->dwItemLevel));
			//node->Set(String::NewFromUtf8(isolate, "itemclass"), Integer::New(Itemclass(pUnit)));
			node->Set(String::NewFromUtf8(isolate, "itemdesc"), String::New(Itemdesc(pUnit)));
			//node->Set(String::NewFromUtf8(isolate, "itemflag"), String::New(Itemflag(pUnit)));
			//node->Set(String::NewFromUtf8(isolate, "itemprefix"), String::New(Itemprefix(pUnit)));
			//node->Set(String::NewFromUtf8(isolate, "itemsuffix"), String::New(Itemsuffix(pUnit)));
			//node->Set(String::NewFromUtf8(isolate, "itemtype"), Integer::New(Itemtype(pUnit)));
			node->Set(String::NewFromUtf8(isolate, "classid"), Integer::New(pmyUnit->dwClassId));
			node->Set(String::NewFromUtf8(isolate, "mode"), Integer::New(pmyUnit->dwMode));
			node->Set(String::NewFromUtf8(isolate, "unitid"), Integer::New(pmyUnit->dwUnitId));
			node->Set(String::NewFromUtf8(isolate, "type"), Integer::New(pmyUnit->dwType));
			node->Set(String::NewFromUtf8(isolate, "xsize"), Integer::New(ItemXsize(pUnit)));
			node->Set(String::NewFromUtf8(isolate, "ysize"), Integer::New(ItemYsize(pUnit)));
			node->Set(String::NewFromUtf8(isolate, "GetStat"), FunctionTemplate::New(unitGetStat, args.This()));
			node->Set(String::NewFromUtf8(isolate, "GetNext"), FunctionTemplate::New(unitGetNext, args.This()));
			args.GetReturnValue().Set(node->NewInstance());
		}
	}
}

#pragma endregion 

//Room Methods
#pragma region Room

JS_METHOD(roomGetFirst)
{
	Isolate* isolate = args.GetIsolate();
	HandleScope handle_scope(isolate);
	//needs more work
	Room2* pRoom2 = (Room2*)Vars.pRoom2;

	if (!pRoom2 || !pRoom2->pLevel || !pRoom2->pLevel->pRoom2First)
		args.GetReturnValue().Set(Boolean::New(false));

	args.GetReturnValue().Set(pRoom2->pLevel->pRoom2First);

}

JS_METHOD(roomGetNearby)
{
	Isolate* isolate = args.GetIsolate();
	HandleScope handle_scope(isolate);
	//needs more work
	Room2* pRoom2 = (Room2*)Vars.pRoom2;

	Array* pReturnArray = *Array::New();

	for (DWORD i = 0; i < pRoom2->dwRoomsNear; ++i)
	{
		//not sure if i have this right
		pRoom2 = pRoom2->pRoom2Near[i];

		Object* node = *Object::New();
		node->Set(String::NewFromUtf8(isolate, "areaid"), Integer::New(pRoom2->pLevel->dwLevelNo));
		node->Set(String::NewFromUtf8(isolate, "correcttomb"), Integer::New(pRoom2->pLevel && pRoom2->pLevel->pMisc && pRoom2->pLevel->pMisc->dwStaffTombLevel ? pRoom2->pLevel->pMisc->dwStaffTombLevel : -1));
		node->Set(String::NewFromUtf8(isolate, "x"), Integer::New(pRoom2->dwPosX));
		node->Set(String::NewFromUtf8(isolate, "xsize"), Integer::New(pRoom2->dwSizeX * 5));
		node->Set(String::NewFromUtf8(isolate, "y"), Integer::New(pRoom2->dwPosY));
		node->Set(String::NewFromUtf8(isolate, "ysize"), Integer::New(pRoom2->dwSizeY * 5));
		node->Set(String::NewFromUtf8(isolate, "number"), Integer::New(pRoom2->dwPresetType != 2 ? -1 : pRoom2->pType2Info->dwRoomNumber));
		pReturnArray->Set(Integer::New(i), node->ToObject());
	}
	args.GetReturnValue().Set(pReturnArray->ToObject());
}

JS_METHOD(roomGetNext)
{
	Isolate* isolate = args.GetIsolate();
	HandleScope handle_scope(isolate);
	Room2* pRoom2 = (Room2*)Vars.pRoom2;
	if (!pRoom2)
		args.GetReturnValue().Set(Boolean::New(false));

	pRoom2 = pRoom2->pRoom2Next;

	if (!pRoom2)
	{
		//JSObject* obj = JS_THIS_OBJECT(cx, vp);
		//if (JS_ValueToObject(cx, JSVAL_NULL, &obj))
		args.GetReturnValue().Set(Boolean::New(false));
	}
	else
	{
		Vars.pRoom2 = pRoom2;
		ObjectTemplate* node = *ObjectTemplate::New();

		//There's an issue somewhere in here, it doesn't return the current unit
		//This print line proves this function works as it should
		D2Funcs::Print("%i", pRoom2->dwPosX);
		//

		node->Set(String::NewFromUtf8(isolate, "areaid"), Integer::New(pRoom2->pLevel->dwLevelNo));
		node->Set(String::NewFromUtf8(isolate, "correcttomb"), Integer::New(pRoom2->pLevel && pRoom2->pLevel->pMisc && pRoom2->pLevel->pMisc->dwStaffTombLevel ? pRoom2->pLevel->pMisc->dwStaffTombLevel : -1));
		node->Set(String::NewFromUtf8(isolate, "x"), Integer::New(pRoom2->dwPosX));
		node->Set(String::NewFromUtf8(isolate, "xsize"), Integer::New(pRoom2->dwSizeX * 5));
		node->Set(String::NewFromUtf8(isolate, "y"), Integer::New(pRoom2->dwPosY));
		node->Set(String::NewFromUtf8(isolate, "ysize"), Integer::New(pRoom2->dwSizeY * 5));
		node->Set(String::NewFromUtf8(isolate, "number"), Integer::New(pRoom2->dwPresetType != 2 ? -1 : pRoom2->pType2Info->dwRoomNumber));
		//methods
		node->Set(String::NewFromUtf8(isolate, "GetFirst"), FunctionTemplate::New(roomGetFirst, args.This()));
		node->Set(String::NewFromUtf8(isolate, "GetNearby"), FunctionTemplate::New(roomGetNearby, args.This()));
		node->Set(String::NewFromUtf8(isolate, "GetNext"), FunctionTemplate::New(roomGetNext, args.This()));
		node->Set(String::NewFromUtf8(isolate, "GetPresetUnits"), FunctionTemplate::New(roomGetPresetUnits, args.This()));
		node->Set(String::NewFromUtf8(isolate, "UnitInRoom"), FunctionTemplate::New(roomUnitInRoom, args.This()));
		args.GetReturnValue().Set(node->NewInstance());
	}
}

JS_METHOD(roomGetPresetUnits)
{
	//needs more work
	Isolate* isolate = args.GetIsolate();
	HandleScope handle_scope(isolate);
	Room2* pRoom2 = (Room2*)Vars.pRoom2;
	char tmp[128] = "";

	DWORD nType = NULL;
	DWORD nClass = NULL;

	if (args.Length() > 0 && args[0]->IsNumber())
		nType = args[0]->Uint32Value();
	if (args.Length() > 1 && args[1]->IsNumber())
		nClass = args[1]->Uint32Value();

	bool bAdded = FALSE;
	DWORD dwArrayCount = NULL;

	if (!pRoom2)
		args.GetReturnValue().Set(Boolean::New(false));

	if (!pRoom2->pRoom1)
	{
		bAdded = TRUE;
		fpAddRoomData(fpGetPlayerUnit()->pAct, pRoom2->pLevel->dwLevelNo, pRoom2->dwPosX, pRoom2->dwPosY, fpGetPlayerUnit()->pPath->pRoom1);
	}

	Array* pReturnArray = *Array::New();

	for (PresetUnit* pUnit = pRoom2->pPreset; pUnit; pUnit = pUnit->pPresetNext)
	{
		if ((pUnit->dwType == nType || nType == NULL) && (pUnit->dwTxtFileNo == nClass || nClass == NULL))
		{
			myPresetUnit* presetUnit = new myPresetUnit;

			presetUnit->dwPosX = pUnit->dwPosX;
			presetUnit->dwPosY = pUnit->dwPosY;
			presetUnit->dwRoomX = pRoom2->dwPosX;
			presetUnit->dwRoomY = pRoom2->dwPosY;
			presetUnit->dwType = pUnit->dwType;
			presetUnit->dwId = pUnit->dwTxtFileNo;
			Vars.nthisPresetX = (presetUnit->dwPosX) + (presetUnit->dwRoomX * 5);
			Vars.nthisPresetY = (presetUnit->dwPosY) + (presetUnit->dwRoomY * 5);
			//GetUnitName((UnitAny*)pUnit, tmp, 256);

			Object* node = *Object::New();
			//node->Set(String::NewFromUtf8(isolate, "name"), String::New(tmp));
			node->Set(String::NewFromUtf8(isolate, "classid"), Integer::New(presetUnit->dwId));
			node->Set(String::NewFromUtf8(isolate, "x"), Integer::New(presetUnit->dwPosX));
			node->Set(String::NewFromUtf8(isolate, "y"), Integer::New(presetUnit->dwPosY));
			node->Set(String::NewFromUtf8(isolate, "roomx"), Integer::New(presetUnit->dwRoomX));
			node->Set(String::NewFromUtf8(isolate, "roomy"), Integer::New(presetUnit->dwRoomY));
			node->Set(String::NewFromUtf8(isolate, "areaid"), Integer::New(presetUnit->dwLevel));
			node->Set(String::NewFromUtf8(isolate, "type"), Integer::New(presetUnit->dwType));
			node->Set(String::NewFromUtf8(isolate, "id"), Integer::New(presetUnit->dwId));
			node->Set(String::NewFromUtf8(isolate, "newx"), Integer::New(Vars.nthisPresetX));
			node->Set(String::NewFromUtf8(isolate, "newy"), Integer::New(Vars.nthisPresetY));
			pReturnArray->Set(dwArrayCount, node->ToObject());
			dwArrayCount++;
		}
	}

	if (bAdded)
		fpRemoveRoomData(fpGetPlayerUnit()->pAct, pRoom2->pLevel->dwLevelNo, pRoom2->dwPosX, pRoom2->dwPosY, fpGetPlayerUnit()->pPath->pRoom1);

	args.GetReturnValue().Set(pReturnArray->ToObject());
}

JS_METHOD(roomUnitInRoom)
{
	//not tested at all
	Isolate* isolate = args.GetIsolate();
	HandleScope handle_scope(isolate);

	Room2* pRoom2 = (Room2*)Vars.pRoom2;

	if (!pRoom2 || args.Length() < 1 || !args[0]->IsObject())
		args.GetReturnValue().Set(Boolean::New(false));

	myUnit* pmyUnit = (myUnit*)Vars.pRoom2;

	if (!pmyUnit || (pmyUnit->_dwPrivateType & PRIVATE_UNIT) != PRIVATE_UNIT)
		args.GetReturnValue().Set(Boolean::New(false));

	UnitAny* pUnit = D2CLIENT_FindUnit(pmyUnit->dwUnitId, pmyUnit->dwType);

	if (!pUnit)
		args.GetReturnValue().Set(Boolean::New(false));

	Room1* pRoom1 = D2COMMON_GetRoomFromUnit(pUnit);

	if (!pRoom1 || !pRoom1->pRoom2)
		args.GetReturnValue().Set(Boolean::New(false));

	args.GetReturnValue().Set(Boolean::New(!!(pRoom1->pRoom2 == pRoom2)));	
}

#pragma endregion 