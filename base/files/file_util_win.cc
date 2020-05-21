
// windows.h
#include <windows.h>
//

#include <psapi.h>
#include <shellapi.h>
#include <shlobj.h>

#include "base/files/file_util.h"
#include "base/strings/string_util.h"

namespace base
{
    const DWORD kFileShareAll =
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;

    bool DeleteFile(const fspath& path, bool recursive) { return bool(); }

    bool DeleteFileAfterReboot(const fspath& path)
    {
        if (path.native().length() >= MAX_PATH) return false;

        return ::MoveFileEx(path.native().c_str(), NULL,
                            MOVEFILE_DELAY_UNTIL_REBOOT |
                                MOVEFILE_REPLACE_EXISTING) != FALSE;
    }

    bool Move(const fspath& from_path, const fspath& to_path)
    {
        /*      if (from_path.)
              {
              }
              return fs::rename(from_path, to_path);*/
        return false;
    }
    bool ReplaceFile(const fspath& from_path, const fspath& to_path,
                     std::error_code* error)
    {
        return bool();
    }

    bool PathIsWritable(const fspath& path)
    {
        HANDLE dir =
            ::CreateFile(path.native().c_str(), FILE_ADD_FILE, kFileShareAll,
                         NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);

        if (dir == INVALID_HANDLE_VALUE) return false;

        CloseHandle(dir);
        return true;
    }

    fspath GetHomeDir()
    {
        wchar_t result[MAX_PATH];
        if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROFILE, NULL,
                                      SHGFP_TYPE_CURRENT, result)) &&
            result[0])
        {
            return fspath(result);
        }

        // Fall back to the temporary directory on failure.
        fspath temp;
        if (GetTempDir(&temp)) return temp;

        // Last resort.
        return fspath(L"C:\\");
    }

    bool CreateTemporaryFile(fspath* path)
    {
        fspath temp_file;

        if (!GetTempDir(path)) return false;

        if (CreateTemporaryFileInDir(*path, &temp_file))
        {
            *path = temp_file;
            return true;
        }

        return false;
    }

    bool CreateTemporaryFileInDir(const fspath& dir, fspath* temp_file)
    {
        wchar_t temp_name[MAX_PATH + 1];

        if (!GetTempFileName(dir.native().c_str(), L"", 0, temp_name))
        {
            return false;
        }

        wchar_t long_temp_name[MAX_PATH + 1];
        DWORD long_name_len =
            GetLongPathName(temp_name, long_temp_name, MAX_PATH);
        if (long_name_len > MAX_PATH || long_name_len == 0)
        {
            // GetLongPathName() failed, but we still have a temporary file.
            *temp_file = fspath(temp_name);
            return true;
        }

        fspath::string_type long_temp_name_str;
        long_temp_name_str.assign(long_temp_name, long_name_len);
        *temp_file = fspath(long_temp_name_str);
        return true;
    }

    FILE* OpenFile(const fspath& filename, const char* mode)
    {
        std::wstring w_mode = atow(mode);
        return _wfsopen(filename.native().c_str(), w_mode.c_str(), _SH_DENYNO);
    }

    FILE* CreateAndOpenTemporaryFileInDir(const fspath& dir, fspath* path)
    {
        if (!CreateTemporaryFileInDir(dir, path))
        {
            return NULL;
        }
        // Open file in binary mode, to avoid problems with fwrite. On Windows
        // it replaces \n's with \r\n's, which may surprise you.
        // Reference:
        // http://msdn.microsoft.com/en-us/library/h9t88zwz(VS.71).aspx
        return OpenFile(*path, "wb+");
    }
    bool CreateNewTempDirectory(const fspath::string_type& prefix,
                                fspath* new_temp_path)
    {
        return bool();
    }
    bool CreateTemporaryDirInDir(const fspath& base_dir,
                                 const std::wstring& prefix, fspath* new_dir)
    {
        return bool();
    }
    bool CreateDirectoryAndGetError(const fspath& full_path,
                                    std::error_code* error)
    {
        return bool();
    }
    bool CreateDirectory(const fspath& full_path) { return bool(); }
    bool GetFileSize(const fspath& file_path, int64_t* file_size)
    {
        return bool();
    }
    bool Normalizefspath(const fspath& path, fspath* real_path)
    {
        return bool();
    }
    bool DevicePathToDriveLetterPath(const fspath& device_path,
                                     fspath* drive_letter_path)
    {
        return bool();
    }
}  // namespace base