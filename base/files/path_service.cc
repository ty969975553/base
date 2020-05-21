#include "base/files/path_service.h"

#include "base/files/file_util.h"

namespace base
{
    bool PathProvider(int key, fspath* result)
    {
        // NOTE: DIR_CURRENT is a special case in PathService::Get
        switch (key)
        {
            case DIR_EXE:
                PathService::Get(FILE_EXE, result);
                *result = result->parent_path();
                return true;
            case DIR_MODULE:
                PathService::Get(FILE_MODULE, result);
                *result = result->parent_path();
                return true;
            case DIR_TEMP:
                if (!GetTempDir(result)) return false;
                return true;
            case base::DIR_HOME:
                *result = GetHomeDir();
                return true;
            case DIR_TEST_DATA: {
                fspath test_data_path;
                if (!PathService::Get(DIR_SOURCE_ROOT, &test_data_path))
                    return false;
                test_data_path = test_data_path.append(L"test");
                test_data_path = test_data_path.append(L"base");
                test_data_path = test_data_path.append(L"data");
                if (!PathExists(
                        test_data_path))  // We don't want to create this.
                    return false;
                *result = test_data_path;
                return true;
            }
            default:
                return false;
        }
    }

    bool PathService::Get(int key, fspath* result)
    {
        if (DIR_CURRENT == key)
        {
            GetCurrentDirectory(result);
            return true;
        }
#if defined(OS_WIN)
        else if (PathProviderWin(key, result))
        {
            return true;
        }
#endif
        else if (PathProvider(key, result))
        {
            return true;
        }
        return false;
    }
}  // namespace base