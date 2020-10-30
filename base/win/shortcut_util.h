#pragma once
#include <windows.h>

#include <string>

#include "base/base_export.h"

namespace base
{
    namespace win
    {
        BASE_EXPORT bool CreateShortCut(
            const std::wstring& target_file, const std::wstring& target_args,
            const std::wstring& link_file, const std::wstring& description,
            int show_mode, const std::wstring& cur_dir,
            const std::wstring& icon_file, int icon_index);

        BASE_EXPORT bool CreateShortCutToURL(const std::wstring& url,
                                             const std::wstring& link_file);
    }  // namespace win
}  // namespace base