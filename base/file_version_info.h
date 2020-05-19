#pragma once

#include <string>

#include "base_export.h"
#include "base/files/file_path.h"
#include "config/build_config.h"

#if defined(OS_WIN)
#include <windows.h>
extern "C" IMAGE_DOS_HEADER __ImageBase;
#endif

namespace base
{
}

#if defined(OS_WIN)
// Creates a FileVersionInfo for the current module. Returns NULL in case of
// error. The returned object should be deleted when you are done with it. This
// is done as a macro to force inlining of __ImageBase. It used to be inside of
// a method labeled with __forceinline, but inlining through __forceinline
// stopped working for Debug builds in VS2013 (http://crbug.com/516359).
#define CREATE_FILE_VERSION_INFO_FOR_CURRENT_MODULE() \
    FileVersionInfo::CreateFileVersionInfoForModule(  \
        reinterpret_cast<HMODULE>(&__ImageBase))
#endif

class BASE_EXPORT FileVersionInfo
{
public:
    virtual ~FileVersionInfo() {}
#if defined(OS_WIN) || defined(OS_MACOSX)
    // Creates a FileVersionInfo for the specified path. Returns NULL if
    // something goes wrong (typically the file does not exit or cannot be
    // opened). The returned object should be deleted when you are done with it.
    static FileVersionInfo* CreateFileVersionInfo(const fspath& file_path);
#endif  // OS_WIN || OS_MACOSX

#if defined(OS_WIN)
    // Creates a FileVersionInfo for the specified module. Returns NULL in case
    // of error. The returned object should be deleted when you are done with
    // it. See CREATE_FILE_VERSION_INFO_FOR_CURRENT_MODULE() helper above for a
    // CreateFileVersionInfoForCurrentModule() alternative for Windows.
    static FileVersionInfo* CreateFileVersionInfoForModule(HMODULE module);
#else
    // Creates a FileVersionInfo for the current module. Returns NULL in case
    // of error. The returned object should be deleted when you are done with
    // it.
    static FileVersionInfo* CreateFileVersionInfoForCurrentModule();
#endif  // OS_WIN

    // Accessors to the different version properties.
    // Returns an empty string if the property is not found.
    virtual std::wstring company_name() = 0;
    virtual std::wstring company_short_name() = 0;
    virtual std::wstring product_name() = 0;
    virtual std::wstring product_short_name() = 0;
    virtual std::wstring internal_name() = 0;
    virtual std::wstring product_version() = 0;
    virtual std::wstring private_build() = 0;
    virtual std::wstring special_build() = 0;
    virtual std::wstring comments() = 0;
    virtual std::wstring original_filename() = 0;
    virtual std::wstring file_description() = 0;
    virtual std::wstring file_version() = 0;
    virtual std::wstring legal_copyright() = 0;
    virtual std::wstring legal_trademarks() = 0;
    virtual std::wstring last_change() = 0;
    virtual bool is_official_build() = 0;
};
