#include "base/encrypt/md5.h"

#include <string>

#include "base/test/unittest.h"

using namespace base;
namespace Test
{
    TEST_FILE
    {
        TEST_CATEGORY(L"md5Test")
        {
            TEST_CASE(L"DigestToBase16")
            {
                MD5Digest digest;
                int data[] = {0xd4, 0x1d, 0x8c, 0xd9, 0x8f, 0x00, 0xb2, 0x04,
                              0xe9, 0x80, 0x09, 0x98, 0xec, 0xf8, 0x42, 0x7e};

                for (int i = 0; i < 16; ++i) digest.a[i] = data[i] & 0xff;

                std::string actual = MD5DigestToBase16(digest);
                std::string expected = "d41d8cd98f00b204e9800998ecf8427e";
                TEST_ASSERT(expected == actual);
            });

            TEST_CASE(L"MD5StringTestSuite1")
            {
                std::string actual = MD5String("");
                std::string expected = "d41d8cd98f00b204e9800998ecf8427e";
                TEST_ASSERT(expected == actual);
            });

            TEST_CASE(L"ContextWithStringData"){
                MD5Context ctx;
                MD5Init(&ctx);

                MD5Update(&ctx, "abc");

                MD5Digest digest;
                MD5Final(&digest, &ctx);

                std::string actual = MD5DigestToBase16(digest);
                std::string expected = "900150983cd24fb0d6963f7d28e17f72";
                TEST_ASSERT(expected == actual);
            });
        });
    }
}  // namespace Test