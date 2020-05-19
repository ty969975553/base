#pragma once

#include <memory>
#include <string>

#include "base/base_export.h"
#include "base/file_version_info.h"
#include "base/macros.h"

namespace base
{
    struct FreeDeleter
    {
        inline void operator()(void* ptr) const { free(ptr); }
    };
}  // namespace
struct tagVS_FIXEDFILEINFO;

typedef tagVS_FIXEDFILEINFO VS_FIXEDFILEINFO;

class BASE_EXPORT FileVersionInfoWin : public FileVersionInfo
{
public:
    FileVersionInfoWin(void* data, WORD language, WORD code_page);
    ~FileVersionInfoWin() override;

    // Accessors to the different version properties.
    // Returns an empty string if the property is not found.
    std::wstring company_name() override;
    std::wstring company_short_name() override;
    std::wstring product_name() override;
    std::wstring product_short_name() override;
    std::wstring internal_name() override;
    std::wstring product_version() override;
    std::wstring private_build() override;
    std::wstring special_build() override;
    std::wstring comments() override;
    std::wstring original_filename() override;
    std::wstring file_description() override;
    std::wstring file_version() override;
    std::wstring legal_copyright() override;
    std::wstring legal_trademarks() override;
    std::wstring last_change() override;
    bool is_official_build() override;

    // Lets you access other properties not covered above.
    bool GetValue(const wchar_t* name, std::wstring* value);

    // Similar to GetValue but returns a wstring (empty string if the property
    // does not exist).
    std::wstring GetStringValue(const wchar_t* name);

    // Get the fixed file info if it exists. Otherwise NULL
    VS_FIXEDFILEINFO* fixed_file_info() { return fixed_file_info_; }

private:
    std::unique_ptr<char, base::FreeDeleter> data_;
    WORD language_;
    WORD code_page_;
    // This is a pointer into the data_ if it exists. Otherwise NULL.
    VS_FIXEDFILEINFO* fixed_file_info_;

    DISALLOW_COPY_AND_ASSIGN(FileVersionInfoWin);
};
