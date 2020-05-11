#include "Console.h"
#if defined _MSVC
#include <Windows.h>
#elif defined _GCC
#include <iostream>
#include <string>
using namespace std;
#endif

namespace ui
{
/***********************************************************************
Console
***********************************************************************/
    namespace console
    {
        void Write(const wchar_t* string, int length)
        {
#if defined _MSVC
            HANDLE outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD fileMode = 0;
            DWORD written = 0;
            if ((GetFileType(outHandle) & FILE_TYPE_CHAR) &&
                GetConsoleMode(outHandle, &fileMode))
            {
                WriteConsole(outHandle, string, (int)length, &written, 0);
            }
            else
            {
                int codePage = GetConsoleOutputCP();
                int charCount =
                    WideCharToMultiByte(codePage, 0, string, -1, 0, 0, 0, 0);
                char* codePageBuffer = new char[charCount];
                WideCharToMultiByte(codePage, 0, string, -1, codePageBuffer,
                                    charCount, 0, 0);
                WriteFile(outHandle, codePageBuffer, charCount - 1, &written,
                          0);
                delete[] codePageBuffer;
            }
#elif defined _GCC
            wstring s(string, string + length);
            wcout << s << ends;
#endif
        }

        void Write(const wchar_t* string) { Write(string, wcslen(string)); }

        void Write(const std::wstring& string)
        {
            Write(string.c_str, string.size());
        }

        void WriteLine(const std::wstring& string)
        {
            Write(string);
            Write(L"\r\n");
        }

        std::wstring Read()
        {
#if defined _MSVC
            std::wstring result;
            DWORD count;
            for (;;)
            {
                wchar_t buffer;
                ReadConsole(GetStdHandle(STD_INPUT_HANDLE), &buffer, 1, &count,
                            0);
                if (buffer == L'\r')
                {
                    ReadConsole(GetStdHandle(STD_INPUT_HANDLE), &buffer, 1,
                                &count, 0);
                    break;
                }
                else if (buffer == L'\n')
                {
                    break;
                }
                else
                {
                    result = result + buffer;
                }
            }
            return result;
#elif defined _GCC
            wstring s;
            getline(wcin, s, L'\n');
            return s.c_str();
#endif
        }

        void SetColor(bool red, bool green, bool blue, bool light)
        {
#if defined _MSVC
            WORD attribute = 0;
            if (red) attribute |= FOREGROUND_RED;
            if (green) attribute |= FOREGROUND_GREEN;
            if (blue) attribute |= FOREGROUND_BLUE;
            if (light) attribute |= FOREGROUND_INTENSITY;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attribute);
            SetConsoleTextAttribute(GetStdHandle(STD_INPUT_HANDLE), attribute);
#elif defined _GCC
            int color =
                (blue ? 1 : 0) * 4 + (green ? 1 : 0) * 2 + (red ? 1 : 0);
            if (light)
                wprintf(L"\x1B[00;3%dm", color);
            else
                wprintf(L"\x1B[01;3%dm", color);
#endif
        }

        void SetTitle(const std::wstring& string)
        {
#if defined _MSVC
            SetConsoleTitle(string.c_str());
#endif
        }
    }  // namespace console
}  // namespace ui
