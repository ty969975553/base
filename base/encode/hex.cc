#include "base/encode/hex.h"

#include <sstream>

namespace base
{
    namespace encode
    {
        std::string EncodeHex(const std::string& src_str, bool upper)
        {
            const static char hex1[] = "0123456789abcdef";
            const static char hex2[] = "0123456789ABCDEF";
            const char* hexp = upper ? hex2 : hex1;
            std::string dst;
            dst.assign(src_str.size() * 2, ' ');
            for (size_t i = 0; i < src_str.size(); ++i)
            {
                unsigned char c = (unsigned char)src_str[i];
                dst[i * 2 + 1] = hexp[c & 0xf];
                dst[i * 2] = hexp[(c >> 4) & 0xf];
            }
            return dst;
        }

        std::wstring DecodeHex(const std::vector<uint8_t>& binary)
        {
            return L"";
        }

        void EncodeHex(const std::wstring& input, std::vector<uint8_t>& result)
        {
            for (size_t i = 0; i < input.size(); ++i)
            {
                wchar_t c = (wchar_t)input[i];
                result.push_back((c >> 8) & 0xff);
                result.push_back(c & 0xff);
            }
        }
    }  // namespace encode
}  // namespace base