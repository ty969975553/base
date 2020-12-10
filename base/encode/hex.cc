#include "base/encode/hex.h"

#include <windows.h>

#include <sstream>

namespace base
{
    namespace encoding
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
            std::vector<wchar_t> temp;
            wchar_t c;
            for (size_t i = 0; i < binary.size(); i++)
            {
                if (i % 2)
                { 
                    c = c  + (binary[i]<<8);
                    temp.push_back(c);
                }
                else
                {
                    c = binary[i] ;                  
                }
            }
            return std::wstring(temp.begin(), temp.end());
        }

        void EncodeHex(const std::wstring& input, std::vector<uint8_t>& result)
        {
            if (!result.empty())
            {
                result.clear();
            }
            for (size_t i = 0; i < input.size(); ++i)
            {
                wchar_t c = (wchar_t)input[i];
                result.push_back(c & 0xff);
                result.push_back((c >> 8) & 0xff);
            }
        }

        void ConvertCharacterEncodingToBytes(const std::wstring& input, int code,
                                             std::vector<uint8_t>& result)
        {
            if (!result.empty())
            {
                result.clear();
            }

            if (code == -1)
            {
                return EncodeHex(input, result);
            }
            int nSize =
                WideCharToMultiByte(code, 0, input.c_str(), -1, 0, 0, 0, 0);
            uint8_t* pBuff = new uint8_t[nSize]{0};
            ::WideCharToMultiByte(code, 0, input.c_str(), -1, (LPSTR)pBuff, nSize,
                                  0, 0);
            result.insert(result.end(), pBuff, pBuff + nSize - 1);
            delete[] pBuff;
            return;
        }
    }  // namespace encoding
}  // namespace base