#pragma once
#include <string>
#include "base/strings/string_conversion.h"

inline std::wstring atow(const std::string& str)
{
    return base::SysNativeMBToWide(str);
}

inline std::string wtoa(const std::wstring& wstr) {
    return base::SysWideToNativeMB(wstr);
}

inline std::wstring itow(int i)
{
    return std::to_wstring(i);
}