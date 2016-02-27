//////////////////////////////////////////////////////////////////////
// D2Helpers.cpp
//////////////////////////////////////////////////////////////////////
#include "D2Helpers.h"
#include "D2Pointers.h"


BYTE CalcPercent(DWORD dwVal, DWORD dwMaxVal, BYTE iMin)
{
	if (dwVal == 0 || dwMaxVal == 0)
		return 0;

	BYTE iRes = (BYTE)((double)dwVal / (double)dwMaxVal * 100.0);
	if (iRes > 100)
		iRes = 100;

	return max(iRes, iMin);
}

DWORD GetTileLevelNo(Room2* lpRoom2, DWORD dwTileNo)
{
	for (RoomTile* pRoomTile = lpRoom2->pRoomTiles; pRoomTile; pRoomTile = pRoomTile->pNext)
	{
		if (*(pRoomTile->nNum) == dwTileNo)
			return pRoomTile->pRoom2->pLevel->dwLevelNo;
	}

	return NULL;
}

UnitAny* GetMercUnit(UnitAny* pUnit)
{
	for (Room1* pRoom = pUnit->pAct->pRoom1; pRoom; pRoom = pRoom->pRoomNext)
		for (UnitAny* pMerc = pRoom->pUnitFirst; pMerc; pMerc = pMerc->pRoomNext)
			if (pMerc->dwType == UNIT_MONSTER &&
				(pMerc->dwTxtFileNo == MERC_A1 || pMerc->dwTxtFileNo == MERC_A2 ||
					pMerc->dwTxtFileNo == MERC_A3 || pMerc->dwTxtFileNo == MERC_A5) &&
				D2CLIENT_GetMonsterOwner(pMerc->dwUnitId) == pUnit->dwUnitId)
				return pMerc;
	return NULL;

#if 0
	// Wanted way of doing things, but D2CLIENT_GetMercUnit does some wierd internal things (drawing, causing screen flicker)
	for (UnitAny* pMerc = D2CLIENT_GetMercUnit(); pMerc; pMerc = pMerc->pRoomNext)
		if (D2CLIENT_GetMonsterOwner(pMerc->dwUnitId) == pUnit->dwUnitId)
			return pMerc;
	return NULL;
#endif
}

UnitAny* D2CLIENT_FindUnit(DWORD dwId, DWORD dwType)
{
	if (dwId == -1) return NULL;
	UnitAny* pUnit = fpFindServerSideUnit(dwId, dwType);
	return pUnit ? pUnit : fpFindClientSideUnit(dwId, dwType);
}
void D2CLIENT_Interact(UnitAny* pUnit, DWORD dwMoveType) {

	if (!pUnit)
		return;

	if (!D2CLIENT_FindUnit(pUnit->dwUnitId, pUnit->dwType))
		return;

	UnitInteraction pInteract = {
		dwMoveType,
		fpGetPlayerUnit(),
		pUnit,
		fpGetUnitX(pUnit),
		fpGetUnitY(pUnit),
		0, 0
	};

	D2CLIENT_Interact_STUB(&pInteract);
}

DWORD GetPlayerArea(void)
{
	return (ClientGameState() == ClientStateInGame ? fpGetPlayerUnit()->pPath->pRoom1->pRoom2->pLevel->dwLevelNo : NULL);
}

Level* GetLevel(DWORD dwLevelNo)
{
	Level* pLevel = fpGetPlayerUnit()->pAct->pMisc->pLevelFirst;

	while (pLevel)
	{
		if (pLevel->dwLevelNo == dwLevelNo)
		{
			if (!pLevel->pRoom2First)
				D2COMMON_InitLevel(pLevel);

			if (!pLevel->pRoom2First)
				break;
			return pLevel;
		}
		pLevel = pLevel->pNextLevel;
	}

	// this crashes pretty much every time it's called
	//pLevel = D2COMMON_GetLevel(D2CLIENT_GetPlayerUnit()->pAct->pMisc, dwLevelNo);
	return pLevel;

}

// NOTE TO CALLERS: szTmp must be a PRE-INITIALIZED string.
const char* GetUnitName(UnitAny* pUnit, char* szTmp, size_t bufSize)
{
	if (!pUnit)
	{
		strcpy_s(szTmp, bufSize, "Unknown");
		return szTmp;
	}
	if (pUnit->dwType == UNIT_MONSTER) {
		wchar_t* wName = D2CLIENT_GetUnitName(pUnit);
		WideCharToMultiByte(CP_ACP, 0, wName, -1, szTmp, bufSize, 0, 0);
		return szTmp;
	}
	if (pUnit->dwType == UNIT_PLAYER && pUnit->pPlayerData)
	{
		//	return pUnit->pPlayerData->szName;
		strcpy_s(szTmp, bufSize, pUnit->pPlayerData->szName);
		return szTmp;
	}
	if (pUnit->dwType == UNIT_ITEM)
	{
		wchar_t wBuffer[256] = L"";
		fpGetItemName(pUnit, wBuffer, sizeof(wBuffer));
		char* szBuffer = UnicodeToAnsi(wBuffer);
		if (strchr(szBuffer, '\n'))
			*strchr(szBuffer, '\n') = 0x00;

		strcpy_s(szTmp, bufSize, szBuffer);
		delete[] szBuffer;
		return szTmp;
	}
	if (pUnit->dwType == UNIT_OBJECT || pUnit->dwType == UNIT_TILE)
	{
		if (pUnit->pObjectData && pUnit->pObjectData->pTxt)
		{
			strcpy_s(szTmp, bufSize, pUnit->pObjectData->pTxt->szName);
			return szTmp;
		}
	}
	strcpy_s(szTmp, bufSize, "Unknown");
	return szTmp;
}

// szBuf must be a 4-character string
void GetItemCode(UnitAny* pUnit, char* szBuf)
{
	if (pUnit->dwType == UNIT_ITEM)
	{
		ItemText* pTxt = fpGetItemText(pUnit->dwTxtFileNo);
		if (pTxt)
		{
			memcpy(szBuf, pTxt->szCode, 3);
			szBuf[3] = 0x00;
		}
	}
}

bool InArea(int x, int y, int x2, int y2, int sizex, int sizey) {
	return !!(x >= x2 && x < x2 + sizex && y >= y2 && y < y2 + sizey);
}

void LoadMPQ(const char* mpq)
{
	D2WIN_InitMPQ("D2Win.DLL", mpq, NULL, 0, 0);
	*p_BNCLIENT_XPacKey = *p_BNCLIENT_ClassicKey = *p_BNCLIENT_KeyOwner = NULL;
	BNCLIENT_DecodeAndLoadKeys();
}

DWORD SAY_I = Pointer::GetDllOffset("D2Client.dll", 0xB27A6);
DWORD SAY_II = Pointer::GetDllOffset("D2Lang.dll", 0x8C60);

void __declspec(naked) __fastcall Say_ASM(DWORD dwPtr)
{
	__asm
	{
		POP EAX
		PUSH ECX
			PUSH EAX
			SUB ESP, 0x110
			PUSH EBX
			PUSH EBP
			MOV EBP, [SAY_II]
			PUSH ESI
			PUSH EDI
			JMP SAY_I
	}
}

void __fastcall Say(const char *szMessage)
{
	using namespace std;

	const char REPLACE_CHAR = (char)(unsigned char)0xFE;

	va_list vaArgs;
	va_start(vaArgs, szMessage);
	int len = _vscprintf(szMessage, vaArgs);
	char* szBuffer = new char[len + 1];
	vsprintf_s(szBuffer, len + 1, szMessage, vaArgs);
	va_end(vaArgs);

	replace(szBuffer, szBuffer + len, REPLACE_CHAR, '%');

	wchar_t* wBuffer = AnsiToUnicode(szBuffer);
	memcpy((wchar_t*)vpChatMsg, wBuffer, wcslen(wBuffer) * 2 + 1);
	delete[] wBuffer;
	wBuffer = NULL;

	MSG* aMsg = new MSG;
	aMsg->hwnd = fpGetHwnd();
	aMsg->message = WM_CHAR;
	aMsg->wParam = VK_RETURN;
	aMsg->lParam = 0x11C0001;
	aMsg->time = NULL;
	aMsg->pt.x = 0x79;
	aMsg->pt.y = 0x1;

	Say_ASM((DWORD)&aMsg);
	delete aMsg;
	aMsg = NULL;
}


DWORD GetUnitName_I = Pointer::GetDllOffset("D2Client.dll", 0x622E0);
DWORD __declspec(naked) __fastcall D2CLIENT_GetUnitName_STUB(DWORD UnitAny)
{
	__asm
	{
		mov eax, ecx
		jmp GetUnitName_I
	}
}

DWORD Interact_I = Pointer::GetDllOffset("D2Client.dll", 0x2B2E0);
void __declspec(naked) __fastcall D2CLIENT_Interact_ASM(DWORD Struct)
{
	__asm {
		mov esi, ecx
			jmp Interact_I
	}
}

DWORD ClickParty_I = Pointer::GetDllOffset("D2Client.dll", 0xA2250);
DWORD __declspec(naked) __fastcall D2CLIENT_ClickParty_ASM(DWORD RosterUnit, DWORD Mode)
{
	__asm
	{
		mov eax, ecx
		jmp ClickParty_I
	}
}

DWORD GetUiVar_I = Pointer::GetDllOffset("D2Client.dll", 0x17C50);
DWORD __declspec(naked) __fastcall D2CLIENT_GetUIVar_STUB(DWORD varno)
{
	__asm
	{
		mov eax, ecx;
		jmp GetUiVar_I;
	}
}

DWORD TakeWaypoint_I = Pointer::GetDllOffset("D2Client.dll", 0x3F5F3);
__declspec(naked) void __stdcall D2CLIENT_TakeWaypoint(DWORD dwWaypointId, DWORD dwArea)
{
	__asm
	{
		PUSH EBP
		MOV EBP, ESP
			SUB ESP, 0x20
			PUSH EBX
			PUSH ESI
			PUSH EDI
			AND DWORD PTR SS : [EBP - 0x20], 0
			PUSH 0
			CALL _TakeWaypoint
			JMP _Exit

			_TakeWaypoint :
		PUSH EBP
			PUSH ESI
			PUSH EDI
			PUSH EBX
			XOR EDI, EDI
			MOV EBX, 1
			MOV ECX, DWORD PTR SS : [EBP + 8]
			MOV EDX, DWORD PTR SS : [EBP + 0xC]
			LEA EBP, DWORD PTR SS : [EBP - 0x20]
			JMP[TakeWaypoint_I]


			_Exit :
			POP EDI
			POP ESI
			POP EBX
			LEAVE
			RETN 8
	}
}