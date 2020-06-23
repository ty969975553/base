#include "base/win/security_util.h"
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
        TEST_CATEGORY(L"security_Test")
        {
            TEST_CASE(L"GetUserSidString")
            {
                std::wstring user_sid;
                TEST_ASSERT(GetUserSidString(&user_sid));
                TEST_ASSERT(!user_sid.empty());
            });

            TEST_CASE(L"TestIsUACEnabled")
            {
                if (base::win::GetVersion() >= base::win::VERSION_VISTA)
                {
                    UserAccountControlIsEnabled();
                }
                else
                {
                    TEST_ASSERT(UserAccountControlIsEnabled());
                }
            });
        });
    }
}  // namespace Test