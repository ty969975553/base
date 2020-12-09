#include <algorithm>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>
#include <vector>

#include "base/command_line.h"
#include "base/files/file_path.h"
#include "base/files/file_util.h"
#include "base/test/unittest.h"

using namespace base;

using uchar = unsigned char;

namespace
{
    std::vector<std::wstring> scan_strings;
    std::vector<int> encode_vector = {CP_UTF8, 936};
    enum UnicodeMode
    {
        BE, LE
    };
    std::vector<std::wstring> result;
    bool case_insensitive = false;

    void ConvertCharacterEncodingToBytes(const std::wstring str, int code,
                                         std::vector<uchar>& data)
    {
        if (!data.empty())
        {
            data.clear();
        }
        int nSize = WideCharToMultiByte(code, 0, str.c_str(), -1, 0, 0, 0, 0);
        uchar* pBuff = new uchar[nSize]{0};
        ::WideCharToMultiByte(code, 0, str.c_str(), -1, (LPSTR)pBuff, nSize, 0,
                              0);
        data.insert(data.end(), pBuff, pBuff + nSize);
        delete[] pBuff;
        return;
    }

    void ConvertUnicodeEncodingToBytes(const std::wstring str, int code,
                                       std::vector<uchar>& data)
    {
        if (!data.empty())
        {
            data.clear();
        }
    }

    bool CompareChar(char c1, char c2)
    {
        if (!case_insensitive)
        {
            return ::tolower(c1) == ::tolower(c2);
        }
        return c1 == c2;
    }

    void FindString(const std::vector<char> pe_buffer)
    {
        bool find = false;
        std::vector<uchar> temp;
        for (auto item : scan_strings)
        {
            for (auto encode : encode_vector)
            {
                ConvertCharacterEncodingToBytes(item, encode, temp);
                auto it = std::search(pe_buffer.begin(), pe_buffer.end(),
                                      temp.begin(), temp.end(), CompareChar);
                if (it != std::end(pe_buffer))
                {
                    find = true;
                    break;
                }
            }

            if (find) result.push_back(item);
        }
    }
}  // namespace

namespace test
{
    TEST_FILE
    {
        TEST_CATEGORY(L"function_test")
        {
            TEST_CASE(L"Convert")
            {
                std::vector<uchar> data;
                // 4142 43bf b4cd bc
                ConvertCharacterEncodingToBytes(L"ABC¿´Í¼", 936, data);
                TEST_ASSERT(data[0] == 0x41);
                TEST_ASSERT(data[1] == 0x42);
                TEST_ASSERT(data[2] == 0x43);
                TEST_ASSERT(data[3] == 0xbf);
                TEST_ASSERT(data[4] == 0xb4);
                TEST_ASSERT(data[5] == 0xcd);
                TEST_ASSERT(data[6] == 0xbc);

                // 41 42	43 e79c 8be5 9bbe
                ConvertCharacterEncodingToBytes(L"ABC¿´Í¼", CP_UTF8, data);
                std::vector expected = {0x41, 0x42, 0x43, 0xe7, 0x9c,
                                        0x8b, 0xe5, 0x9b, 0xbe, 0x00};
                TEST_ASSERT(std::equal(data.begin(), data.end(),
                                       expected.begin(), expected.end()));
            });

            TEST_CASE(L"FindString")
            {

            });
        });
    }
}  // namespace test

int wmain(int argc, wchar_t* argv[])
{
#define UNITTEST
#if defined UNITTEST
    Test::UnitTest::RunAndDisposeTests(argc, argv);
#endif
    // scan_strings = {L"ABC¿´Í¼"};

    // 0, 0);

    // std::ifstream t("file.txt");
    // std::string str(std::istreambuf_iterator<char>(t),
    //                std::istreambuf_iterator<char>());

    // std::vector<char> pe_buffer;
    // std::copy(std::istreambuf_iterator<char>(t),
    //          std::istreambuf_iterator<char>(), back_inserter(pe_buffer));

    // std::vector<unsigned char> buffer;
    // for (size_t i = 0; i < str1.length(); i++)
    //{
    //    buffer.push_back(str1[0] >> 8);
    //    buffer.push_back(str1[0] & 0x00FF);
    //}
    return 0;
    /* CommandLine cmdline(argc, argv);
     UsageVector usevec = {
         {L"exe", L"need scanner exe filepath", false, true, nullptr},
         {L"json", L"strings vector json format", false, true, nullptr},
     };
     cmdline.SetCommandInfos(usevec);
     cmdline.PrintParseOptions();

     fspath exe_path = fspath(cmdline.get<std::string>(L"exe").value());*/
    /* HANDLE hFile = CreateFile(exe_path.native().c_str(), GENERIC_READ,
     FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); if
     (INVALID_HANDLE_VALUE == hFile)
     {
         return;
     }

     int nLen = GetFileSize(hFile, NULL);

     if (0 == nLen)
     {
         return;
     }

     HANDLE hMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, nLen, NULL);

     UCHAR* pMap = (UCHAR*)MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, nLen);
     if (NULL == pMap)
     {
         int n = GetLastError();
         return;
     }



     if (pMap)
     {
         UnmapViewOfFile(pMap);
     }
     if (hMap)
     {
         CloseHandle(hMap);
     }
     CloseHandle(hFile);*/

    // std::vector<string>

    return 0;
}