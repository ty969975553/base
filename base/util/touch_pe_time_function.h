#include <time.h>
#include <windows.h>

#include <algorithm>
#include <experimental/filesystem>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> suffixs = {"exe", "dll"};

bool ModifyTimeDateStamp(std::string file, time_t createtime = time(0))
{
    HANDLE hFile =
        CreateFileA(file.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
                    OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) return false;

    DWORD fileSize = GetFileSize(hFile, NULL);
    BYTE *pByte = new BYTE[fileSize];
    DWORD dw;
    ReadFile(hFile, pByte, fileSize, &dw, NULL);

    PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)pByte;
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) return false;  // invalid PE

    // PIMAGE_FILE_HEADER FH =
    //	(PIMAGE_FILE_HEADER)(pByte + dos->e_lfanew + sizeof(DWORD));

    SetFilePointer(hFile, dos->e_lfanew + 8, NULL, FILE_BEGIN);
    WriteFile(hFile, &createtime, sizeof(time_t), &dw, 0);
    CloseHandle(hFile);
    std::cout << file << " time: " << createtime << std::endl;
    return true;
}

void example()
{
    using fspath = std::experimental::filesystem::path;
    fspath fs(file);
    file = canonical(fs).string();
    transform(file.begin(), file.end(), file.begin(), tolower);
    if (file.find_last_of(".") != std::string::npos)
    {
        size_t pos = file.find_last_of(".");
        std::string suffix = file.substr(pos + 1, file.size() - pos);
        if (std::find(suffixs.begin(), suffixs.end(), suffix) == suffixs.end())
        {
            return -1;
        }
        time_t build_time;
        if (argc == 3)
        {
            try
            {
                build_time = atol(argv[2]);
                ModifyTimeDateStamp(file, build_time);
            }
            catch (...)
            {
                std::cout << " FILE ERROR: " << file << std::endl;
            }
        }
        else
        {
            ModifyTimeDateStamp(file);
        }
    }
}
