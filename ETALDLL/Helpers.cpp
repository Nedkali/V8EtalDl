
#include "Main.h"
#include "string.h"
#include "D2Pointers.h"
#include "Helpers.h"
#include "Input.h"
#include "D2Helpers.h"

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
