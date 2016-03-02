#include "D2Funcs.h"
#include "D2Pointers.h"
#include "Helpers.h"
#include <string>
#include <sstream>
#include <algorithm>
#include <list>

D2Funcs::D2Funcs()
{
}

D2Funcs::~D2Funcs()
{
}

Level* D2Funcs::GetLevel(Act* pAct, int level) {
	//Insure that the shit we are getting is good.
	if (level < 0 || !pAct)
		return NULL;

	//Loop all the levels in this act

	for (Level* pLevel = pAct->pMisc->pLevelFirst; pLevel; pLevel = pLevel->pNextLevel) {
		//Check if we have reached a bad level.
		if (!pLevel)
			break;

		//If we have found the level, return it!
		if (pLevel->dwLevelNo == level && pLevel->dwPosX > 0)
			return pLevel;
	}
	//Default old-way of finding level.
	return D2COMMON_GetLevel(pAct->pMisc, level);
}

Level* D2Funcs::GetArea()
{
	UnitAny* pUnit = fpGetPlayerUnit();
	Act* pAct = pUnit->pAct;
	int level = pAct->dwAct;
	
	
	//Insure that the shit we are getting is good.
	if (level < 0 || !pAct)
		return NULL;

	//Loop all the levels in this act

	for (Level* pLevel = pAct->pMisc->pLevelFirst; pLevel; pLevel = pLevel->pNextLevel) {
		//Check if we have reached a bad level.
		if (!pLevel)
			break;

		//If we have found the level, return it!
		if (pLevel->dwLevelNo == level && pLevel->dwPosX > 0)
			return pLevel;
	}
	//Default old-way of finding level.
	return D2COMMON_GetLevel(pAct->pMisc, level);
}
bool D2Funcs::Interact(DWORD dwUnitType, DWORD dwUnitID) {
	LPBYTE aPacket = new BYTE[9];
	*(BYTE*)&aPacket[0] = (BYTE)0x13;
	*(DWORD*)&aPacket[1] = dwUnitType;
	*(DWORD*)&aPacket[3] = dwUnitID;
	fpSendPacket(9, 0, aPacket);
	delete[] aPacket;
	return 1;
}
bool D2Funcs::WaypointTo(DWORD wpid, BYTE(dest)) {
	LPBYTE aPacket = new BYTE[9];
	*(BYTE*)&aPacket[0] = (BYTE)0x49;
	*(DWORD*)&aPacket[1] = wpid;
	*(DWORD*)&aPacket[3] = dest;
	fpSendPacket(13, 1, aPacket);
	delete[] aPacket;
	return 1;
}
bool D2Funcs::TeleTo(WORD x, WORD y, bool Left) {
	LPBYTE aPacket = new BYTE[5];
	*(BYTE*)&aPacket[0] = Left ? (BYTE)0x08 : (BYTE)0x0F;
	*(WORD*)&aPacket[1] = x;
	*(WORD*)&aPacket[3] = y;
	fpSendPacket(5, 0, aPacket);
	delete[] aPacket;
	return 1;
}
bool D2Funcs::SetSkill(WORD wSkillId, bool bLeft)
{
	if (!GetSkill(wSkillId))
		return FALSE;

	if (!fpGetPlayerUnit())
		return FALSE;

	BYTE PutSkill[9] = { 0x3c, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF };
	if (GetSkill(wSkillId))
	{
		*(WORD*)&PutSkill[1] = wSkillId;
		if (bLeft)
			*(BYTE*)&PutSkill[4] = 0x80;
		fpSendPacket(9, 0, PutSkill);
	}
	else
		return FALSE;

	return TRUE;
}
bool D2Funcs::GetSkill(WORD wSkillId)
{
	if (!fpGetPlayerUnit())
		return 0;

	for (Skill* pSkill = fpGetPlayerUnit()->pInfo->pFirstSkill; pSkill; pSkill = pSkill->pNextSkill)
		if (pSkill->pSkillInfo->wSkillId == wSkillId)
			return 1;
	return 0;
}
bool D2Funcs::MoveTo(WORD x, WORD y) {
	ClickMap(0, x, y, FALSE, NULL);
	Sleep(50);
	ClickMap(2, x, y, FALSE, NULL);
	return 1;
}

void D2Funcs::Print(const char * szText, ...)
{
	//Still needs work getting correct d2 colors
	using namespace std;
	const char REPLACE_CHAR = (char)(unsigned char)0xFE;

	va_list vaArgs;
	va_start(vaArgs, szText);
	int len = _vscprintf(szText, vaArgs);
	char* str = new char[len + 1];
	vsprintf_s(str, len + 1, szText, vaArgs);
	va_end(vaArgs);

	replace(str, str + len, REPLACE_CHAR, '%');
	
	const int maxlen = 98;

	// Break into lines through \n.
	list<string> lines;
	string temp;
	stringstream ss(str);
	while (getline(ss, temp))
		SplitLines(temp, maxlen, ' ', lines);

	// Convert and send every line.
	for (list<string>::iterator it = lines.begin(); it != lines.end(); ++it)
	{
		wchar_t * output = AnsiToUnicode(it->c_str());
		fpPrintGameString(output, 0);
		delete[] output;
	}

	delete[] str;
	str = NULL;
}
