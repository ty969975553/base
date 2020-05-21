#include "base/files/path_service.h"

#include <iostream>
#include <string>

#include "base/files/file_util.h"
#include "base/test/unittest.h"
#include "ui/console/console.h"
#if defined(OS_WIN)
#include "base/win/windows_version.h"
#endif

using namespace base;

bool ReturnsValidPath(int dir_type)
{
    fspath path;
    bool result = PathService::Get(dir_type, &path);
    std::wcout << path.native() << std::endl;
    // Some paths might not exist on some platforms in which case confirming
    // |result| is true and !path.empty() is the best we can do.
    bool check_path_exists = true;
#if defined(OS_POSIX)
    // If chromium has never been started on this account, the cache path may
    // not exist.
    if (dir_type == DIR_CACHE) check_path_exists = false;
#endif
#if defined(OS_LINUX)
    // On the linux try-bots: a path is returned (e.g.
    // /home/chrome-bot/Desktop), but it doesn't exist.
    if (dir_type == DIR_USER_DESKTOP) check_path_exists = false;
#endif
#if defined(OS_WIN)
    if (dir_type == DIR_TASKBAR_PINS)
    {
        // There is no pinned-to-taskbar shortcuts prior to Win7.
        if (base::win::GetVersion() < base::win::VERSION_WIN7)
            check_path_exists = false;
    }
#endif
#if defined(OS_MACOSX)
    if (dir_type != DIR_EXE && dir_type != DIR_MODULE && dir_type != FILE_EXE &&
        dir_type != FILE_MODULE)
    {
        if (path.ReferencesParent()) return false;
    }
#else
    //if (path.is_relative()) return false;
#endif
    return result && !path.native().empty() &&
           (!check_path_exists || PathExists(path));
}

#if defined(OS_WIN)
// Function to test any directory keys that are not supported on some versions
// of Windows. Checks that the function fails and that the returned path is
// empty.
bool ReturnsInvalidPath(int dir_type)
{
    fspath path;
    bool result = PathService::Get(dir_type, &path);
    std::wcout << path.native() << std::endl;
    return !result && path.empty();
}
#endif

namespace Test
{
    TEST_FILE
    {
        TEST_CATEGORY(L"path_service test")
        {
            // Semantics  语义
            TEST_CASE(L"PathServiceTest")
            {
                for (int key = PATH_WIN_START + 1; key < PATH_WIN_END; ++key)
                {
                    bool valid = true;
                    if (key == DIR_APP_SHORTCUTS)
                        valid =
                            base::win::GetVersion() >= base::win::VERSION_WIN8;

                    if (valid)
                        TEST_ASSERT(ReturnsValidPath(key) == true);
                    else
                        TEST_ASSERT(ReturnsInvalidPath(key) == true);
                }

                for (int key = PATH_START + 1 ; key < PATH_END; ++key)
                {
                    TEST_ASSERT(ReturnsValidPath(key) == true);
                }
            });
        });
    }
}  // namespace Test