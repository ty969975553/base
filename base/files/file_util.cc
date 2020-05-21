#include "base/files/file_util.h"

#include <algorithm>
#include <fstream>
#include <limits>
#include <memory>

namespace base
{
    bool CopyFile(const fspath& from_path, const fspath& to_path)
    {
        return fs::copy_file(from_path, to_path);
    }

    bool CopyDirectory(const fspath& from_path, const fspath& to_path,
                       bool recursive)
    {
        fs::copy(from_path, to_path);
        return true;
    }
    bool PathExists(const fspath& path) { return fs::exists(path); }

    fspath MakeAbsoluteFilePath(const fspath& input)
    {
        return fs::absolute(input);
    }

    int64_t ComputeDirectorySize(const fspath& root_path)
    {
        return fs::file_size(root_path);
    }

    bool DirectoryExists(const fspath& path) { return fs::exists(path); }

    bool GetTempDir(fspath* path)
    {
        std::error_code code;
        *path = fs::temp_directory_path(code);
        return code.value() == 0;
    }

    bool ContentsEqual(const fspath& filename1, const fspath& filename2)
    {
        std::ifstream file1(filename1.native().c_str(),
                            std::ios::in | std::ios::binary);
        std::ifstream file2(filename2.native().c_str(),
                            std::ios::in | std::ios::binary);

        // Even if both files aren't openable (and thus, in some sense,
        // "equal"), any unusable file yields a result of "false".
        if (!file1.is_open() || !file2.is_open()) return false;

        const int BUFFER_SIZE = 2056;
        char buffer1[BUFFER_SIZE], buffer2[BUFFER_SIZE];
        do
        {
            file1.read(buffer1, BUFFER_SIZE);
            file2.read(buffer2, BUFFER_SIZE);

            if ((file1.eof() != file2.eof()) ||
                (file1.gcount() != file2.gcount()) ||
                (memcmp(buffer1, buffer2, static_cast<size_t>(file1.gcount()))))
            {
                file1.close();
                file2.close();
                return false;
            }
        } while (!file1.eof() || !file2.eof());

        file1.close();
        file2.close();
        return true;
    }
    bool TextContentsEqual(const fspath& filename1, const fspath& filename2)
    {
        std::ifstream file1(filename1.native().c_str(), std::ios::in);
        std::ifstream file2(filename2.native().c_str(), std::ios::in);

        // Even if both files aren't openable (and thus, in some sense,
        // "equal"), any unusable file yields a result of "false".
        if (!file1.is_open() || !file2.is_open()) return false;

        do
        {
            std::string line1, line2;
            getline(file1, line1);
            getline(file2, line2);

            // Check for mismatched EOF states, or any error state.
            if ((file1.eof() != file2.eof()) || file1.bad() || file2.bad())
            {
                return false;
            }

            // Trim all '\r' and '\n' characters from the end of the line.
            std::string::size_type end1 = line1.find_last_not_of("\r\n");
            if (end1 == std::string::npos)
                line1.clear();
            else if (end1 + 1 < line1.length())
                line1.erase(end1 + 1);

            std::string::size_type end2 = line2.find_last_not_of("\r\n");
            if (end2 == std::string::npos)
                line2.clear();
            else if (end2 + 1 < line2.length())
                line2.erase(end2 + 1);

            if (line1 != line2) return false;
        } while (!file1.eof() || !file2.eof());

        return true;
    }

    bool ReadFileToString(const fspath& path, std::string* contents)
    {
        return false;
    }

    bool IsDirectoryEmpty(const fspath& dir_path)
    {
        if (fs::is_directory(dir_path))
        {
            return fs::is_empty(dir_path);
        }
        return false;
    }

    FILE* CreateAndOpenTemporaryFile(fspath* path)
    {
        fspath directory;
        if (!GetTempDir(&directory)) return NULL;

        return CreateAndOpenTemporaryFileInDir(directory, path);
    }

    bool NormalizeToNativefspath(const fspath& path, fspath* nt_path)
    {
        return bool();
    }

    int ReadFile(const fspath& filename, char* data, int max_size)
    {
        return int();
    }
    bool AppendToFile(const fspath& filename, const char* data, int size)
    {
        return bool();
    }
    bool GetCurrentDirectory(fspath* path) { 
        *path = fs::current_path();
        return true; }

    bool SetCurrentDirectory(const fspath& path)
    {
        fs::current_path(path);
        return true;
    }
    int GetUniquePathNumber(const fspath& path,
                            const fspath::string_type& suffix)
    {
        return int();
    }
    bool SetNonBlocking(int fd) { return bool(); }
    int GetMaximumPathComponentLength(const fspath& path) { return int(); }
    bool IsLink(const fspath& file_path) { return bool(); }
}  // namespace base