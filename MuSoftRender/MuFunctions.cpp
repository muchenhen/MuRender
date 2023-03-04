#include "MuFunctions.h"

std::wstring MuFunctions::ConvertToWideString(const char* str)
{
    const int len = MultiByteToWideChar(CP_ACP, 0, str, -1, nullptr, 0);
    std::wstring wstr(len, L'\0');
    MultiByteToWideChar(CP_ACP, 0, str, -1, &wstr[0], len);
    return wstr;
}
