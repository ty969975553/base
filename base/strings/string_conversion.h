#pragma once
#include <string>
#include "windows.h"
#include "base/base_export.h"

namespace base
{
    BASE_EXPORT std::wstring SysMultiByteToWide(const std::string& mb,
                                                uint32_t code_page);

    BASE_EXPORT std::string SysWideToMultiByte(const std::wstring& wide,
                                               uint32_t code_page);

    BASE_EXPORT std::string SysWideToUTF8(const std::wstring& wide);

    BASE_EXPORT std::wstring SysUTF8ToWide(const std::string& utf8);

    BASE_EXPORT std::string SysWideToNativeMB(const std::wstring& wide);

    BASE_EXPORT std::wstring SysNativeMBToWide(const std::string& native_mb);

    BASE_EXPORT std::string WideToUTF8(const std::wstring& wide);

    BASE_EXPORT std::wstring UTF8ToWide(const std::string& utf8);
}  // namespace base