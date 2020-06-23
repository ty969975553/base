
#include <windows.h>

#include <iostream>
#include <locale>
#include <optional>
#include <string>

#include "base/files/file_path.h"
#include "base/macros.h"
#include "base/strings/string_conversion.h"
#include "base/test/unittest.h"
#include "ui/console/console.h"

using namespace base;

namespace
{
    void ScannerFile(const fspath& strPEFileName)
    {
        if (strPEFileName.native().empty())
        {
            return;
        }

        HANDLE hFile =
            CreateFile(strPEFileName.c_str(), GENERIC_READ, FILE_SHARE_READ,
                       NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (INVALID_HANDLE_VALUE == hFile)
        {
            return;
        }

        int nLen = GetFileSize(hFile, NULL);

        if (0 == nLen)
        {
            return;
        }

        HANDLE hMap =
            CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, nLen, NULL);

        UCHAR* pMap = (UCHAR*)MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, nLen);
        if (NULL == pMap)
        {
            int n = GetLastError();
            return;
        }

        std::string str((char*)pMap, nLen);
        std::wstring wstr((wchar_t*)pMap, nLen / 2);
        std::wstring ansi = SysNativeMBToWide(str);
        std::wstring utf = SysUTF8ToWide(str);
        std::wcout.imbue(std::locale("chs"));
        if (str.find("ÑÏ") != std::string::npos)
        {
            std::wcout << L"str" << L"ÑÏ" << std::endl;
        }

        if (ansi.find(L"ÑÏ") != std::wstring::npos)
        {
            std::wcout << L"ansi" << L"ÑÏ" << std::endl;
        }

        if (wstr.find(L"ÑÏ") != std::wstring::npos)
        {
            std::wcout << L"wstr" << L"ÑÏ" << std::endl;
        }

        if (utf.find(L"ÑÏ") != std::wstring::npos)
        {
            std::wcout << L"utf8" << L"ÑÏ" << std::endl;
        }

        if (str.find("ºý¸ç") != std::string::npos)
        {
            std::wcout << L"str" << L"ºý¸ç" << std::endl;
        }

        if (ansi.find(L"ºý¸ç") != std::wstring::npos)
        {
            std::wcout << L"ansi" << L"ºý¸ç" << std::endl;
        }

        if (wstr.find(L"ºý¸ç") != std::wstring::npos)
        {
            std::wcout << L"wstr" << L"ºý¸ç" << std::endl;
        }

        if (utf.find(L"ºý¸ç") != std::wstring::npos)
        {
            std::wcout << L"utf8" << L"ºý¸ç" << std::endl;
        }

        if (pMap)
        {
            UnmapViewOfFile(pMap);
        }
        if (hMap)
        {
            CloseHandle(hMap);
        }
        CloseHandle(hFile);
    }
}  // namespace
namespace Test
{
    TEST_FILE
    {
        TEST_CATEGORY(L"test")
        {
            TEST_CASE(L"test")
            {
                fspath file("E:/nodepad/trunk/build/Release/Test.exe");
                ScannerFile(file);
                //decltype(MessageBoxA)* func = GetProce;
                //(*func)(nullptr,"1","1",MB_OK);
            });
        });
    }
}  // namespace Test