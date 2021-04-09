#pragma once
#include "base/base_export.h"
#include "base/basic.h"

#include <string>

namespace ui
{
    namespace console
    {
        BASE_EXPORT void Write(const wchar_t* string, int length);

        BASE_EXPORT void Write(const wchar_t* string);

        BASE_EXPORT void Write(const std::wstring& string);

        BASE_EXPORT void WriteLine(const std::wstring& string);
        
        BASE_EXPORT std::wstring Read();

        BASE_EXPORT void SetColor(bool red, bool green, bool blue, bool light);
        BASE_EXPORT void SetTitle(const std::wstring& string);
    } // namespace console
} // namespace UI
