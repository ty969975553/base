#pragma once
#include "base/basic.h"

#include <string>

namespace UI
{
    namespace Console
    {
        class Console
        {
        public:
            /// <summary>Write to the command line window.</summary>
            /// <param name="string">Content to write.</param>
            /// <param name="length">Size of the content in wchar_t. The zero terminator is not
            /// included.</param>
            static void Write(const wchar_t* string, int length);

            /// <summary>Write to the command line window.</summary>
            /// <param name="string">Content to write.</param>
            static void Write(const wchar_t* string);

            /// <summary>Write to the command line window.</summary>
            /// <param name="string">Content to write.</param>
            static void Write(const std::wstring& string);

            /// <summary>Write to the command line window with a CRLF.</summary>
            /// <param name="string">Content to write.</param>
            static void WriteLine(const std::wstring& string);

            /// <summary>Read from the command line window.</summary>
            /// <returns>The whole line read from the command line window.</returns>
            static std::wstring Read();

            static void SetColor(bool red, bool green, bool blue, bool light);
            static void SetTitle(const std::wstring& string);
        };
    } // namespace console
} // namespace UI
