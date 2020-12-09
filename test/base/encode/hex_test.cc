#include "base/encode/hex.h"

#include <string>

#include "base/test/unittest.h"

using namespace base;
namespace Test
{
    TEST_FILE
    {
        TEST_CATEGORY(L"hextest")
        {
            TEST_CASE(L"EncodeHex")
            {
                std::wstring chinese = L"中国加油,NO.1";
                std::vector<uint8_t> data;
                encode::EncodeHex(chinese, data);
                std::vector<uint8_t> expected = {
                    0x4e, 0x2d, 0x56, 0xfd, 0x52, 0xa0, 0x6c, 0xb9,  0x00, 0x2c,
                    0x00, 0x4e, 0x00, 0x4f, 0x00, 0x2e, 0x00, 0x31};
                TEST_ASSERT(std::equal(data.begin(), data.end(),
                                       expected.begin(), expected.end()));
            });

            //TEST_CASE(L"DecodeHex")
            //{
            //    std::vector<uint8_t> data = {
            //        0x4e, 0x2d, 0x56, 0xfd, 0x52, 0xa0, 0x6c, 0xb9, 0x00,
            //        0x2c, 0x00, 0x4e, 0x00, 0x4f, 0x00, 0x2e, 0x00, 0x31};
     
            //    std::wstring actual= encode::DecodeHex(data);
            //    std::wstring expected = L"中国加油,NO.1";
            //    TEST_ASSERT(expected == actual);
            //});
        });
    }
}  // namespace Test