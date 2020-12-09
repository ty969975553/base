#pragma once

#include <string>
#include <vector>

#include "base/base_export.h"

namespace base
{
    namespace encode
    {
        std::string EncodeHex(const std::string& src_str, bool upper);

        std::wstring DecodeHex(const std::vector<uint8_t>& binary);

        void EncodeHex(const std::wstring& input, std::vector<uint8_t>& result);
    }  // namespace encode
}  // namespace base