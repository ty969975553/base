#include "base/business/validate.h"
#include <iostream>
#include <string>

#include "base/test/unittest.h"
#include "ui/console/console.h"

namespace Test
{
    TEST_FILE
    {
        TEST_CATEGORY(L"validate_Test")
        {
            TEST_CASE(L"validate phone")
            {
                bool b = base::ValidatePhoneNumber("1234");
                TEST_ASSERT(b == false);

                b = base::ValidatePhoneNumber("13333333333");
                TEST_ASSERT(b == true);

                b = base::ValidatePhoneNumber("1730177362");
                TEST_ASSERT(b == false);

                b = base::ValidatePhoneNumber("1730177632b");
                TEST_ASSERT(b == false);

                b = base::ValidatePhoneNumber("173017736226");
                TEST_ASSERT(b == false);

                b = base::ValidatePhoneNumber("16637014795");
                TEST_ASSERT(b == true);
            });
        });
    }
}  // namespace Test