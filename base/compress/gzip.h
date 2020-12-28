#pragma once
#include <windows.h>

#include <string>

#include "base/base_export.h"

namespace base
{
    BASE_EXPORT bool UncompressGzippedString(const std::string& input,
                                             std::string& output);
    BASE_EXPORT bool DeflateString(const std::string& input,
                                   std::string& output);
    BASE_EXPORT bool InflateString(const std::string& input,
                                   std::string& output, size_t output_length);
}  // namespace base
