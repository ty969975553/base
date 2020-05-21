#include "base/files/file_path_win.h"

#include <Windows.h>
#include <shlobj.h>
#include <memory>

#include "base/environment.h"
#include "base/files/file_path.h"
#include "base/win/scoped_co_mem.h"
#include "base/win/windows_version.h"
#include "base/strings/string_conversion.h"

namespace base
{
    extern "C" IMAGE_DOS_HEADER __ImageBase;

    bool PathProviderWin(int key, fspath* result)
    {
        // We need to go compute the value. It would be nice to support paths
        // with names longer than MAX_PATH, but the system functions don't seem
        // to be designed for it either, with the exception of GetTempPath (but
        // other things will surely break if the temp path is too long, so we
        // don't bother handling it.
        wchar_t system_buffer[MAX_PATH];
        system_buffer[0] = 0;

        fspath cur;
        switch (key)
        {
            case FILE_EXE:
                if (::GetModuleFileName(NULL, system_buffer, MAX_PATH) == 0)
                    return false;
                cur = fspath(system_buffer);
                break;
            case FILE_MODULE: {
                // the resource containing module is assumed to be the one that
                // this code lives in, whether that's a dll or exe
                HMODULE this_module = reinterpret_cast<HMODULE>(&__ImageBase);
                if (::GetModuleFileName(this_module, system_buffer, MAX_PATH) ==
                    0)
                    return false;
                cur = fspath(system_buffer);
                break;
            }
            case DIR_WINDOWS:
                ::GetWindowsDirectory(system_buffer, MAX_PATH);
                cur = fspath(system_buffer);
                break;
            case DIR_SYSTEM:
                ::GetSystemDirectory(system_buffer, MAX_PATH);
                cur = fspath(system_buffer);
                break;
            case DIR_PROGRAM_FILESX86:
                if (base::win::OSInfo::GetInstance()->architecture() !=
                    base::win::OSInfo::X86_ARCHITECTURE)
                {
                    if (FAILED(SHGetFolderPath(NULL, CSIDL_PROGRAM_FILESX86,
                                               NULL, SHGFP_TYPE_CURRENT,
                                               system_buffer)))
                        return false;
                    cur = fspath(system_buffer);
                    break;
                }
                // Fall through to base::DIR_PROGRAM_FILES if we're on an X86
                // machine.
            case DIR_PROGRAM_FILES:
                if (FAILED(SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL,
                                           SHGFP_TYPE_CURRENT, system_buffer)))
                    return false;
                cur = fspath(system_buffer);
                break;
            case DIR_PROGRAM_FILES6432:
#if !defined(_WIN64)
                if (base::win::OSInfo::GetInstance()->wow64_status() ==
                    base::win::OSInfo::WOW64_ENABLED)
                {
                    std::unique_ptr<base::Environment> env(
                        base::Environment::Create());
                    std::string programfiles_w6432;
                    // 32-bit process running in WOW64 sets ProgramW6432
                    // environment variable. See
                    // https://msdn.microsoft.com/library/windows/desktop/aa384274.aspx.
                    if (!env->GetVar("ProgramW6432", &programfiles_w6432))
                        return false;
                    // GetVar returns UTF8 - convert back to Wide.
                    cur = fspath(UTF8ToWide(programfiles_w6432));
                    break;
                }
#endif
                if (FAILED(SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL,
                                           SHGFP_TYPE_CURRENT, system_buffer)))
                    return false;
                cur = fspath(system_buffer);
                break;
            case DIR_IE_INTERNET_CACHE:
                if (FAILED(SHGetFolderPath(NULL, CSIDL_INTERNET_CACHE, NULL,
                                           SHGFP_TYPE_CURRENT, system_buffer)))
                    return false;
                cur = fspath(system_buffer);
                break;
            case DIR_COMMON_START_MENU:
                if (FAILED(SHGetFolderPath(NULL, CSIDL_COMMON_PROGRAMS, NULL,
                                           SHGFP_TYPE_CURRENT, system_buffer)))
                    return false;
                cur = fspath(system_buffer);
                break;
            case DIR_START_MENU:
                if (FAILED(SHGetFolderPath(NULL, CSIDL_PROGRAMS, NULL,
                                           SHGFP_TYPE_CURRENT, system_buffer)))
                    return false;
                cur = fspath(system_buffer);
                break;
            case DIR_APP_DATA:
                if (FAILED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL,
                                           SHGFP_TYPE_CURRENT, system_buffer)))
                    return false;
                cur = fspath(system_buffer);
                break;
            case DIR_COMMON_APP_DATA:
                if (FAILED(SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL,
                                           SHGFP_TYPE_CURRENT, system_buffer)))
                    return false;
                cur = fspath(system_buffer);
                break;
            case DIR_LOCAL_APP_DATA:
                if (FAILED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL,
                                           SHGFP_TYPE_CURRENT, system_buffer)))
                    return false;
                cur = fspath(system_buffer);
                break;
            case DIR_SOURCE_ROOT: {
                fspath executableDir;
                // On Windows, unit tests execute two levels deep from the
                // source root. For example: chrome/{Debug|Release}/ui_tests.exe
                PathService::Get(base::DIR_EXE, &executableDir);
                cur = executableDir.parent_path().parent_path();
                break;
            }
            case DIR_APP_SHORTCUTS: {
                if (win::GetVersion() < win::VERSION_WIN8) return false;

                base::win::ScopedCoMem<wchar_t> path_buf;
                if (FAILED(SHGetKnownFolderPath(FOLDERID_ApplicationShortcuts,
                                                0, NULL, &path_buf)))
                    return false;

                cur = fspath(std::wstring(path_buf));
                break;
            }
            case DIR_USER_DESKTOP:
                if (FAILED(SHGetFolderPath(NULL, CSIDL_DESKTOPDIRECTORY, NULL,
                                           SHGFP_TYPE_CURRENT, system_buffer)))
                {
                    return false;
                }
                cur = fspath(system_buffer);
                break;
            case DIR_COMMON_DESKTOP:
                if (FAILED(SHGetFolderPath(NULL, CSIDL_COMMON_DESKTOPDIRECTORY,
                                           NULL, SHGFP_TYPE_CURRENT,
                                           system_buffer)))
                {
                    return false;
                }
                cur = fspath(system_buffer);
                break;
            case DIR_USER_QUICK_LAUNCH:
                if (!PathService::Get(base::DIR_APP_DATA, &cur)) return false;
                // According to various sources, appending
                // "Microsoft\Internet Explorer\Quick Launch" to %appdata% is
                // the only reliable way to get the quick launch folder across
                // all versions of Windows.
                // http://stackoverflow.com/questions/76080/how-do-you-reliably-get-the-quick-
                // http://www.microsoft.com/technet/scriptcenter/resources/qanda/sept05/hey0901.mspx
                cur = cur.append("Microsoft")
                          .append("Internet Explorer")
                          .append("Quick Launch");
                break;
            case DIR_TASKBAR_PINS:
                if (!PathService::Get(base::DIR_USER_QUICK_LAUNCH, &cur))
                    return false;
                cur = cur.append("User Pinned");
                cur = cur.append("TaskBar");
                break;
            case DIR_WINDOWS_FONTS:
                if (FAILED(SHGetFolderPath(NULL, CSIDL_FONTS, NULL,
                                           SHGFP_TYPE_CURRENT, system_buffer)))
                {
                    return false;
                }
                cur = fspath(system_buffer);
                break;
            default:
                return false;
        }

        *result = cur;
        return true;
    }

}  // namespace base