
#include <windows.h>

#include <iostream>
#include <locale>
#include <optional>
#include <string>
#include <thread>

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
        if (str.find("严") != std::string::npos)
        {
            std::wcout << L"str" << L"严" << std::endl;
        }

        if (ansi.find(L"严") != std::wstring::npos)
        {
            std::wcout << L"ansi" << L"严" << std::endl;
        }

        if (wstr.find(L"严") != std::wstring::npos)
        {
            std::wcout << L"wstr" << L"严" << std::endl;
        }

        if (utf.find(L"严") != std::wstring::npos)
        {
            std::wcout << L"utf8" << L"严" << std::endl;
        }

        if (str.find("糊哥") != std::string::npos)
        {
            std::wcout << L"str" << L"糊哥" << std::endl;
        }

        if (ansi.find(L"糊哥") != std::wstring::npos)
        {
            std::wcout << L"ansi" << L"糊哥" << std::endl;
        }

        if (wstr.find(L"糊哥") != std::wstring::npos)
        {
            std::wcout << L"wstr" << L"糊哥" << std::endl;
        }

        if (utf.find(L"糊哥") != std::wstring::npos)
        {
            std::wcout << L"utf8" << L"糊哥" << std::endl;
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

#pragma data_seg("MyData")
int g_Value = 0;  // Notethattheglobalisnotinitialized.
#pragma data_seg()
#pragma comment(linker, "/SECTION:MyData,RWS")
                  // DLL提供两个接口函数：
int GetValue() { return g_Value; }
void SetValue(int n) { g_Value = n; }

#include <stdlib.h>

#include <iostream>
#include <vector>
#include <bitset>
using namespace std;

class BitMap
{
public:
    BitMap(size_t range) { _bitmap.resize(range >> 3); }
    void Set(int num)
    {
        int index = num >> 3;  //确定在第几个字节
        int pos = num % 8;
        _bitmap[index] |= 1 << pos;
    }
    void Reset(int num)
    {
        int index = num >> 3;
        int pos = num % 8;
        _bitmap[index] &= ~(1 << pos);
    }
    bool Test(int num)
    {
        int index = num >> 3;
        int pos = num % 8;
        return _bitmap[index] &= 1 << pos;
    }

protected:
    vector<char> _bitmap;
};

namespace Test
{
    TEST_FILE
    {
        TEST_CATEGORY(L"test")
        {
            TEST_CASE(L"test")
            {
                BitMap bm(-1);
                bm.Set(10);
                bm.Set(10555);
                bm.Set(12222);
                bm.Set(16666);
                bm.Reset(10);
                cout << bm.Test(10) << endl;
                cout << bm.Test(10555) << endl;
                cout << bm.Test(12222) << endl;
                cout << bm.Test(16666) << endl;
                bitset<-1> x;
            });
        });
    }
}  // namespace Test