#include "base/encode/hex.h"

#include <windows.h>

#include <string>

#include "base/test/unittest.h"

using namespace base;
namespace Test
{
    TEST_FILE
    {
        TEST_CATEGORY(L"hextest")
        {
            TEST_CASE(L"Convert")
            {
                std::vector<uint8_t> data;
                // 4142 43bf b4cd bc
                encoding::ConvertCharacterEncodingToBytes(L"ABC看图", 936,
                                                          data);
                TEST_ASSERT(data[0] == 0x41);
                TEST_ASSERT(data[1] == 0x42);
                TEST_ASSERT(data[2] == 0x43);
                TEST_ASSERT(data[3] == 0xbf);
                TEST_ASSERT(data[4] == 0xb4);
                TEST_ASSERT(data[5] == 0xcd);
                TEST_ASSERT(data[6] == 0xbc);

                // 41 42	43 e79c 8be5 9bbe
                encoding::ConvertCharacterEncodingToBytes(L"ABC看图", CP_UTF8,
                                                          data);
                std::vector expected = {0x41, 0x42, 0x43, 0xe7, 0x9c,
                                        0x8b, 0xe5, 0x9b, 0xbe};
                TEST_ASSERT(std::equal(data.begin(), data.end(),
                                       expected.begin(), expected.end()));
            });

            TEST_CASE(L"EncodeHex")
            {
                std::wstring chinese = L"中国加油,NO.1";
                std::vector<uint8_t> data;
                encoding::EncodeHex(chinese, data);
                std::vector<uint8_t> expected = {
                    0x2d, 0x4e, 0xfd, 0x56, 0xa0, 0x52, 0xb9, 0x6c, 0x2c,
                    0x00, 0x4e, 0x00, 0x4f, 0x00, 0x2e, 0x00, 0x31, 0x00};
                TEST_ASSERT(std::equal(data.begin(), data.end(),
                                       expected.begin(), expected.end()));
            });

            TEST_CASE(L"DecodeHex")
            {
                std::vector<uint8_t> data = {
                    0x2d, 0x4e, 0xfd, 0x56, 0xa0, 0x52, 0xb9, 0x6c, 0x2c,
                    0x00, 0x4e, 0x00, 0x4f, 0x00, 0x2e, 0x00, 0x31, 0x00};

                std::wstring actual = encoding::DecodeHex(data);
                std::wstring expected = L"中国加油,NO.1";
                TEST_ASSERT(expected == actual);
            });
        });
    }
}  // namespace Test