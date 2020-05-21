#pragma once
#include <string>
#include <vector>
#include "base/base_export.h"

namespace base
{
    BASE_EXPORT std::vector<std::string> SplitString(
        const std::string& str, const std::string& delims = " ");

    BASE_EXPORT std::vector<std::wstring> SplitString(
        const std::wstring& str,
        const std::wstring& delims = L" ");
}  // namespace base