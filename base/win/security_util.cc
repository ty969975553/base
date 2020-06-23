#pragma once

#include "base/win/security_util.h"

#include <aclapi.h>
#include <lm.h>
#include <sddl.h>

#include <memory>
#include <string>

#include "base/base_export.h"
#include "base/win/registry.h"
#include "base/win/scoped_handle.h"
#include "base/win/windows_version.h"

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
            std::unique_ptr<BYTE[]> user_bytes(new BYTE[size]);
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

            Registry key(HKEY_LOCAL_MACHINE,
                         "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policie"
                         "s\\System",
                         true);
            int uac_enabled = 0;
            if (!key.getInt("EnableLUA", uac_enabled))
            {
                return true;
            }
            // Users can set the EnableLUA value to something arbitrary, like 2,
            // which Vista will treat as UAC enabled, so we make sure it is not
            // set to 0.
            return (uac_enabled != 0);
        }

        bool GetIsUserAdmin()
        {
            // Determine if the user is part of the adminstators group. This
            // will return true in case of XP and 2K if the user belongs to
            // admin group. In case of Vista, it only returns true if the admin
            // is running elevated.
            SID_IDENTIFIER_AUTHORITY nt_authority = SECURITY_NT_AUTHORITY;
            PSID administrators_group = NULL;
            BOOL result = ::AllocateAndInitializeSid(
                &nt_authority, 2, SECURITY_BUILTIN_DOMAIN_RID,
                DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0,
                &administrators_group);
            if (result)
            {
                if (!::CheckTokenMembership(NULL, administrators_group,
                                            &result))
                {
                    result = false;
                }
                ::FreeSid(administrators_group);
            }
            return !!result;
        }

        bool IsUserNonElevatedAdmin()
        {
            // If pre-Vista return false;
            if (!base::win::IsWindowsVistaOrGreater())
            {
                return false;
            }

            bool non_elevated_admin = false;
            HANDLE token;
            if (::OpenProcessToken(::GetCurrentProcess(), TOKEN_READ, &token))
            {
                TOKEN_ELEVATION_TYPE elevation_type = TokenElevationTypeDefault;
                DWORD infoLen = 0;
                if (::GetTokenInformation(
                        token, TokenElevationType,
                        reinterpret_cast<void*>(&elevation_type),
                        sizeof(elevation_type), &infoLen))
                {
                    if (elevation_type == TokenElevationTypeLimited)
                    {
                        non_elevated_admin = true;
                    }
                }
            }

            return non_elevated_admin;
        }

    }  // namespace win
}  // namespace base
