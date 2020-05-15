#include "base/encrypt/base64.h"

#include <string>

#include "base/test/unittest.h"

using namespace base;
namespace Test
{
    TEST_FILE
    {
        TEST_CATEGORY(L"base64")
        {
            TEST_CASE(L"basic")
            {
                const std::string kText = "hello world";
                const std::string kBase64Text = "aGVsbG8gd29ybGQ=";
                std::string encoded;
                std::string decoded;
                bool ok;
                Base64Encode(kText, &encoded);
                TEST_ASSERT(kBase64Text == encoded);
                ok = Base64Decode(encoded, &decoded);
                TEST_ASSERT(ok);
                TEST_ASSERT(kText == decoded);
            });
        });
    }
}  // namespace Test