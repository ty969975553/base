#pragma once
#include "base/base_export.h"
#include "base/basic.h"

#include <string>

namespace ui
{
    namespace console
    {
        /// <summary>Write to the command line window.</summary>
        /// <param name="string">Content to write.</param>
        /// <param name="length">Size of the content in wchar_t. The zero terminator is not
        /// included.</param>
        BASE_EXPORT void Write(const wchar_t* string, int length);

        /// <summary>Write to the command line window.</summary>
        /// <param name="string">Content to write.</param>
        BASE_EXPORT void Write(const wchar_t* string);

        /// <summary>Write to the command line window.</summary>
        /// <param name="string">Content to write.</param>
        BASE_EXPORT void Write(const std::wstring& string);

        /// <summary>Write to the command line window with a CRLF.</summary>
        /// <param name="string">Content to write.</param>
        BASE_EXPORT void WriteLine(const std::wstring& string);

        /// <summary>Read from the command line window.</summary>
        /// <returns>The whole line read from the command line window.</returns>
        BASE_EXPORT std::wstring Read();

        BASE_EXPORT void SetColor(bool red, bool green, bool blue, bool light);
        BASE_EXPORT void SetTitle(const std::wstring& string);
    } // namespace console
} // namespace UI
