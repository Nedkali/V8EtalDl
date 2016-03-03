//////////////////////////////////////////////////////////////////////
// D2Helpers.cpp
//////////////////////////////////////////////////////////////////////
#include <io.h>
#include <errno.h>
#include <ctime>
#include <cmath>
#include <sstream>
#include <string>

#include "stringhash.h"
#include "D2Helpers.h"
#include "D2Pointers.h"

bool IsScrollingText()
{
	HWND d2Hwnd = fpGetHwnd();
	WindowHandlerList* whl = vpWindowHandlers->table[(0x534D5347 ^ (DWORD)d2Hwnd) % vpWindowHandlers->length];
	MessageHandlerHashTable* mhht;
	MessageHandlerList* mhl;

	while (whl)
	{
		if (whl->unk_0 == 0x534D5347 && whl->hWnd == d2Hwnd)
		{
			mhht = whl->msgHandlers;
			if (mhht != NULL && mhht->table != NULL && mhht->length != 0)
			{
				// 0x201 - WM_something click
				mhl = mhht->table[0x201 % mhht->length];

				if (mhl != NULL)
				{
					while (mhl)
					{
						if (mhl->message && mhl->unk_4 < 0xffffffff && mhl->handler == D2CLIENT_CloseNPCTalk)
						{
							return true;
						}
						mhl = mhl->next;
					}
				}
			}
		}
		whl = whl->next;
	}

	return false;
}

void GameDrawMenu(void)
{
	D2WIN_DrawSprites();
	if (Vars.bActive && MENU::ClientState() == ClientStateMenu)
	{
		DrawLogo();
	}
	Sleep(10);
}

void GameDraw(void)
{
	if (Vars.bActive && MENU::ClientState() == ClientStateInGame)
	{
		DrawLogo();
	}
	Sleep(10);
}

double GetDistance(long x1, long y1, long x2, long y2, DistanceType type)
{
	double dist = 0;
	switch (type)
	{
	case Euclidean:
	{
		double dx = (double)(x2 - x1);
		double dy = (double)(y2 - y1);
		dx = pow(dx, 2);
		dy = pow(dy, 2);
		dist = sqrt(dx + dy);
	}
	break;
	case Chebyshev:
	{
		long dx = (x2 - x1);
		long dy = (y2 - y1);
		dx = abs(dx);
		dy = abs(dy);
		dist = max(dx, dy);
	}
	break;
	case Manhattan:
	{
		long dx = (x2 - x1);
		long dy = (y2 - y1);
		dx = abs(dx);
		dy = abs(dy);
		dist = (dx + dy);
	}
	break;
	default:
		dist = -1;
		break;
	}
	return dist;
}


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

DWORD ReadFile(HANDLE hFile, void *buf, DWORD len)
//NOTE :- validates len bytes of buf
{
	DWORD numdone = 0;
	return ::ReadFile(hFile, buf, len, &numdone, NULL) != 0 ? numdone : -1;
}

void *memcpy2(void *dest, const void *src, size_t count)
{
	return (char *)memcpy(dest, src, count) + count;
}
HANDLE OpenFileRead(char *filename)
{
	return CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
}

bool InArea(int x, int y, int x2, int y2, int sizex, int sizey) {
	return !!(x >= x2 && x < x2 + sizex && y >= y2 && y < y2 + sizey);
}

int D2GetScreenSizeX()
{
	return GetScreenSize().x;
}

int D2GetScreenSizeY()
{
	return GetScreenSize().y;
}

void myDrawAutomapCell(CellFile *cellfile, int xpos, int ypos, BYTE col)
{
	if (!cellfile)return;
	CellContext ct;
	memset(&ct, 0, sizeof(ct));
	ct.pCellFile = cellfile;

	xpos -= (cellfile->cells[0]->width / 2);
	ypos += (cellfile->cells[0]->height / 2);

	int xpos2 = xpos - cellfile->cells[0]->xoffs, ypos2 = ypos - cellfile->cells[0]->yoffs;
	if ((xpos2 >= D2GetScreenSizeX()) || ((xpos2 + (int)cellfile->cells[0]->width) <= 0) || (ypos2 >= D2GetScreenSizeY()) || ((ypos2 + (int)cellfile->cells[0]->height) <= 0)) return;

	static BYTE coltab[2][256];//, tabno = 0, lastcol = 0;
	if (!coltab[0][1]) for (int k = 0; k < 255; k++) coltab[0][k] = coltab[1][k] = (BYTE)k;
	cellfile->mylastcol = coltab[cellfile->mytabno ^= (col != cellfile->mylastcol)][255] = col;

	fpDrawAutomapCell2(&ct, xpos, ypos, (DWORD)-1, 5, coltab[cellfile->mytabno]);
}

void WorldToScreen(POINT* pPos)
{
	fpMapToAbsScreen(&pPos->x, &pPos->y);
	pPos->x -= fpGetMouseXOffset();
	pPos->y -= fpGetMouseYOffset();
}

void ScreenToWorld(POINT *pPos)
{
	fpAbsScreenToMap(&pPos->x, &pPos->y);
	pPos->x += fpGetMouseXOffset();
	pPos->y += fpGetMouseYOffset();
}

POINT ScreenToAutomap(int x, int y)
{
	POINT result = { 0,0 };
	x *= 32;
	y *= 32;
	result.x = ((x - y) / 2 / (*vpDivisor)) - (*vpOffset).x + 8;
	result.y = ((x + y) / 4 / (*vpDivisor)) - (*vpOffset).y - 8;

	if (D2CLIENT_GetAutomapSize())
	{
		--result.x;
		result.y += 5;
	}
	return result;
}

void AutomapToScreen(POINT* pPos)
{
	pPos->x = 8 - vpOffset->x + (pPos->x * (*vpAutomapMode));
	pPos->y = 8 + vpOffset->y + (pPos->y * (*vpAutomapMode));
}

BYTE *AllocReadFile(char *filename)
{
	HANDLE hFile = OpenFileRead(filename);
	int filesize = GetFileSize(hFile, 0);
	if (filesize <= 0) return 0;
	BYTE *buf = new BYTE[filesize];
	ReadFile(hFile, buf, filesize);
	CloseHandle(hFile);
	return buf;
}
CellFile *LoadBmpCellFile(BYTE *buf1, int width, int height)
{
	BYTE *buf2 = new BYTE[(width*height * 2) + height], *dest = buf2;

	for (int i = 0; i < height; i++) {
		BYTE *src = buf1 + (i*((width + 3)&-4)), *limit = src + width;
		while (src < limit) {
			BYTE *start = src, *limit2 = min(limit, src + 0x7f), trans = !*src;
			do src++; while ((trans == (BYTE)!*src) && (src < limit2));
			if (!trans || (src < limit)) *dest++ = (BYTE)((trans ? 0x80 : 0) + (src - start));
			if (!trans) while (start < src) *dest++ = *start++;
		}
		*dest++ = 0x80;
	}

	static DWORD dc6head[] = { 6, 1, 0, 0xeeeeeeee, 1, 1, 0x1c,  0, (DWORD)-1, (DWORD)-1, 0, 0, 0, (DWORD)-1, (DWORD)-1 };
	dc6head[8] = width;
	dc6head[9] = height;
	dc6head[14] = dest - buf2;
	dc6head[13] = sizeof(dc6head) + (dc6head[14]) + 3;
	BYTE *ret = new BYTE[dc6head[13]];
	memset(memcpy2(memcpy2(ret, dc6head, sizeof(dc6head)), buf2, dc6head[14]), 0xee, 3);
	delete[] buf2;

	return (CellFile *)ret;
}
CellFile *LoadBmpCellFile(char *filename)
{
	BYTE *ret = 0;

	BYTE *buf1 = AllocReadFile(filename);
	BITMAPFILEHEADER *bmphead1 = (BITMAPFILEHEADER *)buf1;
	BITMAPINFOHEADER *bmphead2 = (BITMAPINFOHEADER *)(buf1 + sizeof(BITMAPFILEHEADER));
	if (buf1 && (bmphead1->bfType == 'MB') && (bmphead2->biBitCount == 8) && (bmphead2->biCompression == BI_RGB)) {
		ret = (BYTE *)LoadBmpCellFile(buf1 + bmphead1->bfOffBits, bmphead2->biWidth, bmphead2->biHeight);
	}
	delete[] buf1;

	return (CellFile *)ret;
}
CellFile *myInitCellFile(CellFile *cf)
{
	if (cf)
		D2CMP_InitCellFile(cf, &cf, "?", 0, (DWORD)-1, "?");
	return cf;
}
CellFile* LoadCellFile(char* lpszPath, DWORD bMPQ)
{
	// AutoDetect the Cell File
	if (bMPQ == 3)
	{
		// Check in our directory first
		char path[_MAX_FNAME + _MAX_PATH];
		sprintf_s(path, sizeof(path), "%s\\%s", Vars.szScriptPath, lpszPath);


		HANDLE hFile = OpenFileRead(path);

		if (hFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hFile);
			return LoadCellFile(path, FALSE);
		}
		else
		{
			return LoadCellFile(lpszPath, TRUE);
		}

		//return NULL;
	}

	unsigned __int32 hash = sfh(lpszPath, (int)strlen(lpszPath));
	if (Vars.mCachedCellFiles.count(hash) > 0)
		return Vars.mCachedCellFiles[hash];
	if (bMPQ == TRUE)
	{
		CellFile* result = (CellFile*)fpLoadCellFile(lpszPath, 0);
		Vars.mCachedCellFiles[hash] = result;
		return result;
	}
	else if (bMPQ == FALSE)
	{
		// see if the file exists first
		if (!(_access(lpszPath, 0) != 0 && errno == ENOENT))
		{
			CellFile* result = myInitCellFile((CellFile*)LoadBmpCellFile(lpszPath));
			Vars.mCachedCellFiles[hash] = result;
			return result;
		}
	}

	return NULL;
}
void myDrawText(const char* szwText, int x, int y, int color, int font)
{
	size_t found;
	wchar_t* text = AnsiToUnicode(szwText);
	std::string temp(szwText);
	found = temp.find_first_of(-1);

	while (found != std::string::npos)
	{
		text[found] = 0xff;
		found = temp.find_first_of(-1, found + 1);
	}


	DWORD dwOld = D2WIN_SetTextSize(font);
	fpDrawText(text, x, y, color, 0);
	D2WIN_SetTextSize(dwOld);

	delete[] text;
}

POINT GetScreenSize()
{
	// HACK: p_D2CLIENT_ScreenSize is wrong for out of game, which is hardcoded to 800x600
	POINT ingame = { *vpScreenSizeX, *vpScreenSizeY },
		oog = { 800, 600 },
		p = { 0 };
	if (MENU::ClientState() == ClientStateMenu) p = oog;
	else p = ingame;
	return p;
}
// TODO: make this use SIZE for clarity
POINT CalculateTextLen(const char* szwText, int Font)
{
	POINT ret = { 0,0 };

	if (!szwText)
		return ret;

	wchar_t* Buffer = AnsiToUnicode(szwText);

	DWORD dwWidth, dwFileNo;
	DWORD dwOldSize = D2WIN_SetTextSize(Font);
	ret.y = D2WIN_GetTextSize(Buffer, &dwWidth, &dwFileNo);
	ret.x = dwWidth;
	D2WIN_SetTextSize(dwOldSize);

	delete[] Buffer;
	return ret;
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

DWORD GoldDialogAction = Pointer::GetDllOffset("D2Client.dll", 0x11C86C);
DWORD GoldDialogAmount = Pointer::GetDllOffset("D2Client.dll", 0x11D568);
void SendGold(int nGold, int nMode)
{
	GoldDialogAction = nGold;
	GoldDialogAmount = nMode;
	fpPerformGoldDialogAction();
}

void __fastcall UseStatPoint(WORD stat, int count)
{
	if (D2COMMON_GetUnitStat(fpGetPlayerUnit(), STAT_STATPOINTSLEFT, 0) < count)
		return;

	BYTE packet[3] = { 0x3A };
	*(WORD*)&packet[1] = stat;

	for (int i = 0; i < count; i++)
	{
		D2CLIENT_SendGamePacket(3, packet);
		if (i != count - 1) Sleep(500);
	}
}

void __fastcall UseSkillPoint(WORD skill, int count)
{
	if (D2COMMON_GetUnitStat(fpGetPlayerUnit(), STAT_SKILLPOINTSLEFT, 0) < count)
		return;

	BYTE packet[3] = { 0x3B };
	*(WORD*)&packet[1] = skill;

	for (int i = 0; i < count; i++)
	{
		D2CLIENT_SendGamePacket(3, packet);
		if (i != count - 1) Sleep(500);
	}
}

DWORD SendGamePacket_I = Pointer::GetDllOffset("D2Client.dll", 0xB61F0);
__declspec(naked) DWORD __fastcall D2CLIENT_SendGamePacket_ASM(DWORD dwLen, BYTE* bPacket)
{
	__asm
	{
		push ebx
		mov ebx, ecx
			push edx
			call SendGamePacket_I
			pop ebx
			ret
	}
}

void __declspec(naked) __fastcall D2CLIENT_SetSelectedUnit_STUB(DWORD UnitAny)
{
	__asm
	{
		mov eax, ecx
		jmp D2CLIENT_SetSelectedUnit_I
	}
}

void __declspec(naked) __fastcall D2GFX_DrawRectFrame_STUB(RECT* rect)
{
	__asm
	{
		mov eax, ecx;
		jmp D2CLIENT_DrawRectFrame;
	}
}
//AutomapLayer* InitAutomapLayer(DWORD levelno)
//{
//	AutomapLayer2 *pLayer = fpGetLayer(levelno);
//	return D2CLIENT_InitAutomapLayer(pLayer->nLayerNo);
//}
//
////DWORD InitAutomapLayer_I = Pointer::GetDllOffset("D2Client.dll", 0x733D0);
//DWORD __declspec(naked) __fastcall D2CLIENT_InitAutomapLayer_STUB(DWORD nLayerNo)
//{
//	__asm
//	{
//		push eax;
//		mov eax, ecx;
//		call fpInitAutomapLayer_I;
//		pop eax;
//		ret;
//	}
//}