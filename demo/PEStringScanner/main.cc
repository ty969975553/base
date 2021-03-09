#include <algorithm>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>
#include <vector>

#include "base/command_line.h"
#include "base/encode/hex.h"
#include "base/files/file_path.h"
#include "base/files/file_util.h"
#include "base/test/unittest.h"
#include "ui/console/console.h"

namespace
{
    std::vector<std::wstring> scan_strings;
    std::vector<int> encode_vector = {CP_UTF8, 936, -1};  //-1 unicode
    std::vector<std::wstring> result;
    bool case_insensitive = false;

    bool CompareChar(unsigned char c1, unsigned char c2)
    {
        if (!case_insensitive)
        {
            return ::tolower(c1) == ::tolower(c2);
        }
        return c1 == c2;
    }

    void FindString(const std::vector<uint8_t> pe_buffer)
    {
        std::vector<uint8_t> temp;
        for (auto item : scan_strings)
        {
            bool find = false;
            for (auto encode : encode_vector)
            {
                base::encoding::ConvertCharacterEncodingToBytes(item, encode,
                                                                temp);
                auto it = std::search(pe_buffer.begin(), pe_buffer.end(),
                                      temp.begin(), temp.end());
                if (it != pe_buffer.end())
                {
                    find = true;
                    break;
                }
            }
            if (find) result.push_back(item);
        }
    }

    void GetPEBuffer(std::wstring path, std::vector<uint8_t>& pe_buffer) 
    {
        HANDLE hFile =
            CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ,
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

        pe_buffer.assign(pMap, pMap + nLen);

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

int wmain(int argc, wchar_t* argv[])
{
    base::CommandLine cmdline(argc, argv);
    base::UsageVector usevec = {
        {L"exe", L"need scanner exe filepath", false, true, nullptr},
        {L"json", L"strings vector json format", false, false, nullptr},
    };
    scan_strings = {L""};
    cmdline.SetCommandInfos(usevec);
    cmdline.PrintParseOptions();

    std::vector<uint8_t> pe_buffer;
    GetPEBuffer(cmdline.get<std::wstring>(L"exe").value(), pe_buffer);
    FindString(pe_buffer);
    for (auto item : result)
    {
        ui::console::SetColor(1,0,0,1);
        ui::console::WriteLine(item);
    }
    return 0;
}