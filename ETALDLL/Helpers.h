#pragma once
#include <string>
#include <sstream>
#include <algorithm>
#include <list>

bool SplitLines(const std::string & str, size_t maxlen, const char delim, std::list<std::string> & lst);
wchar_t* AnsiToUnicode(const char* str);

