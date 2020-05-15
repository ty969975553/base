#include "base/strings/string_util.h"

#include "base/test/unittest.h"

using namespace base;
namespace Test
{
    TEST_FILE
    {
        TEST_CATEGORY(L"string util test")
        {
            TEST_CASE(L"StringCompareNoCase")
            {
                std::string A = "1234";
                std::string B = "1234";

                bool actual = StringCompareNoCase(A, B);
                bool expected = true;
                TEST_ASSERT(expected == actual);

                std::string A2 = "Abc中文";
                std::string B2 = "aBc中文";

                bool actual2 = StringCompareNoCase(A2, B2);
                bool expected2 = true;
                TEST_ASSERT(expected2 == actual2);

                std::string A3 = "Abc中文";
                std::string B3 = "aBc";

                bool actual3 = StringCompareNoCase(A3, B3);
                bool expected3 = false;
                TEST_ASSERT(expected3 == actual3);
            });
        });
    }
}  // namespace Test