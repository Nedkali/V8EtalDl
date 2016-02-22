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
