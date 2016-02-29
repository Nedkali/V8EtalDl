#pragma once
#include <string>
#include <sstream>
#include <algorithm>
#include <list>
#include "D2Pointers.h"

bool SplitLines(const std::string & str, size_t maxlen, const char delim, std::list<std::string> & lst);
wchar_t* AnsiToUnicode(const char* str);
char* UnicodeToAnsi(const wchar_t* str);
bool ClickMap(DWORD dwClickType, int wX, int wY, BOOL bShift, UnitAny* pUnit);
//char * IntoChar(int x);
