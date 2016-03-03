#include "JSGlobalMethods.h"
#include "D2Helpers.h"

//Me Global Methods
#pragma region Me

void meSetSkill(const v8::FunctionCallbackInfo<v8::Value>& args) {
	args.GetReturnValue().Set(Boolean::New(D2Funcs::SetSkill(args[0]->Uint32Value(), args[1]->BooleanValue())));
}

void meCancel(const v8::FunctionCallbackInfo<v8::Value>& args) {
	DWORD automapOn = *vpAutomapOn;

	if (IsScrollingText())
		fpClearScreen();
	else if (fpGetCurrentInteractingNPC())
		fpCloseNPCInteract();
	else if (fpGetCursorItem())
		fpClickMap(0, 10, 10, 0x08);
	else
		fpCloseInteract();

	*vpAutomapOn = automapOn;

	args.GetReturnValue().Set(Boolean::New(true));
}

void meRepair(const v8::FunctionCallbackInfo<v8::Value>& args) {

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

void meGetState(const v8::FunctionCallbackInfo<v8::Value>& args) {

	UnitAny* pUnit = fpGetPlayerUnit();

	if (!pUnit)
		return args.GetReturnValue().Set(Boolean::New(false));

	int32_t nState;
	nState = args[0]->Uint32Value();


	if (nState > 159 || nState < 0)
		return args.GetReturnValue().Set(Boolean::New(false));

	return args.GetReturnValue().Set(Boolean::New(!!fpGetUnitState(pUnit, nState)));
}

void meGetQuest(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	if (args.Length() < 2 || !args[0]->IsNumber() || !args[1]->IsNumber())
		args.GetReturnValue().Set(Boolean::New(false));

	uint32_t nAct = args[0]->Uint32Value();
	uint32_t nQuest = args[1]->Uint32Value();	
	args.GetReturnValue().Set(Number::New(fpGetQuestFlag(fpGetQuestInfo(), nAct, nQuest)));
}


void meSwapWeapons(const v8::FunctionCallbackInfo<v8::Value>& args)
{
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

#pragma endregion 