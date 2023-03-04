#pragma once
#include <string>
#include "framework.h"

class MuFunctions
{
public:

	// convert a string of multibyte characters to a string of wide characters
    static std::wstring ConvertToWideString(const char* str);
};
