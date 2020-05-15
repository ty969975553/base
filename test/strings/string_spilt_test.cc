#include "base/strings/string_split.h"
#include "base/test/unittest.h"

using namespace base;
namespace Test
{
    TEST_FILE
    {
        TEST_CATEGORY(L"string_split test")
        {
            TEST_CASE(L"string split")
            {
                std::vector<std::string> out =
                    SplitString("hello world, i has ready", " ");
                size_t size = out.size();
                size_t expected = 5;
                TEST_ASSERT(size == expected);
            });

            TEST_CASE(L"wstring split")
            {
                std::vector<std::wstring> out =
                    SplitString(L"red; green; blue", L"; ");
                size_t size = out.size();
                size_t expected = 3;
                TEST_ASSERT(size == expected);

                TEST_ASSERT(out[0] == L"red");
            });
        });
    }
}  // namespace Test