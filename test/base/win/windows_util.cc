#include "base/win/windows_util.h"
#include "base/win/windows_version.h"

#include <iostream>
#include <string>

#include "base/test/unittest.h"
#include "ui/console/console.h"

using namespace base::win;

namespace Test
{
    TEST_FILE
    {
        TEST_CATEGORY(L"Windows_util_Test")
        {
            TEST_CASE(L"IsProcExists")
            {
                //bool b = IsProcExists(::GetModuleHandle(NULL), "test");
                //TEST_ASSERT(b == true);
            });
        });
    }
}  // namespace Test