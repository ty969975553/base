#pragma once
#include "base/win/security_util.h"

#include <aclapi.h>
#include <lm.h>
#include <string>
#include "base/base_export.h"

namespace base
{
    namespace win
    {
        bool GetUserSidString(std::wstring* user_sid)
        {
            // Get the current token.
            HANDLE token = NULL;
            if (!::OpenProcessToken(::GetCurrentProcess(), TOKEN_QUERY, &token))
                return false;
            ScopedHandle token_scoped(token);

            DWORD size = sizeof(TOKEN_USER) + SECURITY_MAX_SID_SIZE;
            scoped_ptr<BYTE[]> user_bytes(new BYTE[size]);
            TOKEN_USER* user = reinterpret_cast<TOKEN_USER*>(user_bytes.get());

            if (!::GetTokenInformation(token, TokenUser, user, size, &size))
                return false;

            if (!user->User.Sid) return false;

            // Convert the data to a string.
            wchar_t* sid_string;
            if (!::ConvertSidToStringSid(user->User.Sid, &sid_string))
                return false;

            *user_sid = sid_string;

            ::LocalFree(sid_string);

            return true;
        }

        bool UserAccountControlIsEnabled()
        {
            // This can be slow if Windows ends up going to disk.  Should watch
            // this key for changes and only read it once, preferably on the
            // file thread.
            //   http://code.google.com/p/chromium/issues/detail?id=61644
            ThreadRestrictions::ScopedAllowIO allow_io;

            RegKey key(HKEY_LOCAL_MACHINE,
                       L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies"
                       L"\\System",
                       KEY_READ);
            DWORD uac_enabled;
            if (key.ReadValueDW(L"EnableLUA", &uac_enabled) != ERROR_SUCCESS)
                return true;
            // Users can set the EnableLUA value to something arbitrary, like 2,
            // which Vista will treat as UAC enabled, so we make sure it is not
            // set to 0.
            return (uac_enabled != 0);
        }
    }  // namespace win
}  // namespace base
