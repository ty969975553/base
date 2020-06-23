#pragma once

#include <windows.h>
#include <string>
#include "base/base_export.h"

namespace base
{
    namespace win
    {
        // Returns the string representing the current user sid.
        BASE_EXPORT bool GetUserSidString(std::wstring* user_sid);

        // Returns false if user account control (UAC) has been disabled with
        // the
        // EnableLUA registry flag. Returns true if user account control is
        // enabled. NOTE: The EnableLUA registry flag, which is ignored on
        // Windows XP machines, might still exist and be set to 0 (UAC
        // disabled), in which case this function will return false. You should
        // therefore check this flag only if the OS is Vista or later.
        BASE_EXPORT bool UserAccountControlIsEnabled();

        
        BASE_EXPORT bool GetIsUserAdmin();

        BASE_EXPORT bool IsUserNonElevatedAdmin();
    }  // namespace win
}  // namespace base