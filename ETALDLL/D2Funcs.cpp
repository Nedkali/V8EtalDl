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

//bool D2Funcs::Say()
//{
//
//	return 1;
//}

int D2Funcs::Random(WORD a, WORD b)
{
	int random = rand() % a + b;
	return random;
}
int D2Funcs::GetArea()
{
	int nArea = GetPlayerArea();	
	////Level* pLevel = GetLevel(nArea);	
	//myArea* pArea = new myArea;
	//pArea->AreaId = nArea;
	////pArea->ExitArray = NULL;
	return nArea;
}
UnitAny D2Funcs::GetPlayerUnit()
{
	UnitAny *pUnit = fpGetPlayerUnit();
	return *pUnit;
}
bool D2Funcs::ExitGame() {

	fpExitGame();
	return 1;
}

bool D2Funcs::Interact(WORD dwUnitType, WORD dwUnitID) {
	LPBYTE aPacket = new BYTE[9];
	*(BYTE*)&aPacket[0] = (BYTE)0x13;
	*(WORD*)&aPacket[1] = dwUnitType;
	*(WORD*)&aPacket[3] = dwUnitID;
	fpSendPacket(9, 0, aPacket);
	delete[] aPacket;
	return 1;
}
bool D2Funcs::WaypointTo(WORD wpid, BYTE(dest)) {
	LPBYTE aPacket = new BYTE[9];
	*(BYTE*)&aPacket[0] = (BYTE)0x49;
	*(WORD*)&aPacket[1] = wpid;
	*(WORD*)&aPacket[3] = dest;
	fpSendPacket(9, 0, aPacket);
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
	else return FALSE;

	return TRUE;
}
bool D2Funcs::GetSkill(WORD wSkillId)
{
	if (!fpGetPlayerUnit())
		return 0;

	for (Skill* pSkill = fpGetPlayerUnit()->pInfo->pFirstSkill; pSkill; pSkill = pSkill->pNextSkill)
		if (pSkill->pSkillInfo->wSkillId == wSkillId)
			return 1;//D2COMMON_GetSkillLevel(D2CLIENT_GetPlayerUnit(), pSkill, TRUE);

	return 0;
}
bool D2Funcs::MoveTo(WORD x, WORD y) {
	LPBYTE aPacket = new BYTE[5];
	*(BYTE*)&aPacket[0] = (BYTE)0x03;
	*(WORD*)&aPacket[1] = x;
	*(WORD*)&aPacket[3] = y;
	fpSendPacket(5, 0, aPacket);
	delete[] aPacket;
	return 1;
}
void D2Funcs::Print(const char * szFormat, ...)
{
	using namespace std;
	const char REPLACE_CHAR = (char)(unsigned char)0xFE;

	va_list vaArgs;
	va_start(vaArgs, szFormat);
	int len = _vscprintf(szFormat, vaArgs);
	char* str = new char[len + 1];
	vsprintf_s(str, len + 1, szFormat, vaArgs);
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
