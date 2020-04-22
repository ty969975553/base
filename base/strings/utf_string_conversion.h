#pragma once
#include "base/base_export.h"

namespace base
{
    BASE_EXPORT bool WideToUTF8(const wchar_t* src, size_t src_len,
                                std::string* output);
    BASE_EXPORT std::string WideToUTF8( std::wstring wide);

    BASE_EXPORT bool UTF8ToWide(const char* src, size_t src_len,
                                std::wstring* output);
    BASE_EXPORT std::wstring UTF8ToWide( std::string utf8);
}  // namespace base