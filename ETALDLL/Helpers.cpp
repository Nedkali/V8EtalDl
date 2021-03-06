
#include "Main.h"
#include "string.h"
#include "D2Pointers.h"
#include "Helpers.h"
#include "Input.h"
#include "D2Helpers.h"

bool ClickMap(DWORD dwClickType, int wX, int wY, BOOL bShift, UnitAny* pUnit)
{

	POINT Click = { wX, wY };
	if (pUnit)
	{
		Click.x = fpGetUnitX(pUnit);
		Click.y = fpGetUnitY(pUnit);
	}

	fpMapToAbsScreen(&Click.x, &Click.y);

	Click.x -= *vpViewportX;
	Click.y -= *vpViewportY;

	POINT OldMouse = { 0, 0 };
	OldMouse.x = *vpMouseX;
	OldMouse.y = *vpMouseY;
	*vpMouseX = 0;
	*vpMouseY = 0;

	if (pUnit && pUnit != fpGetPlayerUnit())
	{
		Vars.dwSelectedUnitId = pUnit->dwUnitId;
		Vars.dwSelectedUnitType = pUnit->dwType;

		Vars.bClickAction = TRUE;

		fpClickMap(dwClickType, Click.x, Click.y, bShift ? 0x0C : (*vpAlwaysRun ? 0x08 : 0));
		D2CLIENT_SetSelectedUnit(NULL);

		Vars.bClickAction = FALSE;
		Vars.dwSelectedUnitId = NULL;
		Vars.dwSelectedUnitType = NULL;
	}
	else
	{
		Vars.dwSelectedUnitId = NULL;
		Vars.dwSelectedUnitType = NULL;

		Vars.bClickAction = TRUE;
		fpClickMap(dwClickType, Click.x, Click.y, bShift ? 0x0C : (*vpAlwaysRun ? 0x08 : 0));
		Vars.bClickAction = FALSE;
	}
	*vpMouseX = OldMouse.x;
	*vpMouseY = OldMouse.y;
	return true;
}

char* UnicodeToAnsi(const wchar_t* str)
{
	char* buf = NULL;
	int len = WideCharToMultiByte(CP_ACP, 0, str, -1, buf, 0, "?", NULL);
	buf = new char[len];
	WideCharToMultiByte(CP_ACP, 0, str, -1, buf, len, "?", NULL);
	return buf;
}
wchar_t* AnsiToUnicode(const char* str)
{
	wchar_t* buf = NULL;
	int len = MultiByteToWideChar(CP_ACP, 0, str, -1, buf, 0);
	buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, str, -1, buf, len);
	return buf;
}
bool SplitLines(const std::string & str, size_t maxlen, const char delim, std::list<std::string> & lst)
{
	using namespace std;

	if (str.length() < 1 || maxlen < 2)
		return false;

	size_t pos, len;
	string tmp(str);

	while (tmp.length() > maxlen)
	{
		len = tmp.length();
		// maxlen-1 since std::string::npos indexes from 0
		pos = tmp.find_last_of(delim, maxlen - 1);
		if (!pos || pos == string::npos)
		{
			//Target delimiter was not found, breaking at maxlen
			// maxlen-1 since std::string::npos indexes from 0
			lst.push_back(tmp.substr(0, maxlen - 1));
			tmp.erase(0, maxlen - 1);
			continue;
		}
		pos = tmp.find_last_of(delim, maxlen - 1);
		if (pos && pos != string::npos)
		{
			//We found the last delimiter before maxlen
			lst.push_back(tmp.substr(0, pos) + delim);
			tmp.erase(0, pos);
		}
		else
			DebugBreak();
	}
	if (!tmp.length())
		DebugBreak();

	if (tmp.length())
		lst.push_back(tmp);

	return true;
}
//char * IntoChar(int x)
//{
//	char numberstring[(((sizeof x) * CHAR_BIT) + 2) / 3 + 2];
//	sprintf(numberstring, "%d", x);
//	return numberstring;
//}
