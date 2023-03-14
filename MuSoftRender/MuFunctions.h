#pragma once
#include <string>
#include "framework.h"

#define ADD_GET_SET_METHOD(ClassName, MemberName) \
ClassName Get##MemberName() const { return MemberName; } \
void Set##MemberName(ClassName In##MemberName) { MemberName = In##MemberName; }

class MuFunctions
{
public:

	// convert a string of multibyte characters to a string of wide characters
    static std::wstring ConvertToWideString(const char* str);
};
