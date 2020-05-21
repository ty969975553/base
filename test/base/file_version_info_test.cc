#include "base/file_version_info.h"
#include "base/file_version_info_win.h"

#include <iostream>
#include <memory>
#include <optional>
#include <string>

#include "base/macros.h"
#include "base/files/path_service.h"
#include "base/test/unittest.h"
#include "ui/console/console.h"

using namespace base;

namespace
{
#if defined(OS_WIN)
    fspath GetTestDataPath()
    {
        fspath path;
        PathService::Get(base::DIR_SOURCE_ROOT, &path);
        path = path.append("test");
        path = path.append("base");
        path = path.append("data");
        path = path.append("file_version_info_unittest");
        return path;
    }
#endif
}  // namespace

namespace Test
{
    TEST_FILE
    {
        TEST_CATEGORY(L"file_version_info test")
        {
            TEST_CASE(L"HardCodedProperties")
            {
                const wchar_t kDLLName[] = {L"FileVersionInfoTest1.dll"};

                const wchar_t* const kExpectedValues[15] = {
                    // FileVersionInfoTest.dll
                    L"Goooooogle",                      // company_name
                    L"Google",                          // company_short_name
                    L"This is the product name",        // product_name
                    L"This is the product short name",  // product_short_name
                    L"The Internal Name",               // internal_name
                    L"4.3.2.1",                         // product_version
                    L"Private build property",          // private_build
                    L"Special build property",          // special_build
                    L"This is a particularly interesting comment",  // comments
                    L"This is the original filename",  // original_filename
                    L"This is my file description",    // file_description
                    L"1.2.3.4",                        // file_version
                    L"This is the legal copyright",    // legal_copyright
                    L"This is the legal trademarks",   // legal_trademarks
                    L"This is the last change",        // last_change
                };

                fspath dll_path = GetTestDataPath();
                dll_path = dll_path.append(kDLLName);

                std::unique_ptr<FileVersionInfo> version_info(
                    FileVersionInfo::CreateFileVersionInfo(dll_path));

                int j = 0;
                TEST_ASSERT(kExpectedValues[j++] ==
                            version_info->company_name());
                TEST_ASSERT(kExpectedValues[j++] ==
                            version_info->company_short_name());
                TEST_ASSERT(kExpectedValues[j++] ==
                            version_info->product_name());
                TEST_ASSERT(kExpectedValues[j++] ==
                            version_info->product_short_name());
                TEST_ASSERT(kExpectedValues[j++] ==
                            version_info->internal_name());
                TEST_ASSERT(kExpectedValues[j++] ==
                            version_info->product_version());
                TEST_ASSERT(kExpectedValues[j++] ==
                            version_info->private_build());
                TEST_ASSERT(kExpectedValues[j++] ==
                            version_info->special_build());
                TEST_ASSERT(kExpectedValues[j++] == version_info->comments());
                TEST_ASSERT(kExpectedValues[j++] ==
                            version_info->original_filename());
                TEST_ASSERT(kExpectedValues[j++] ==
                            version_info->file_description());
                TEST_ASSERT(kExpectedValues[j++] ==
                            version_info->file_version());
                TEST_ASSERT(kExpectedValues[j++] ==
                            version_info->legal_copyright());
                TEST_ASSERT(kExpectedValues[j++] ==
                            version_info->legal_trademarks());
                TEST_ASSERT(kExpectedValues[j++] ==
                            version_info->last_change());
            });

            TEST_CASE(L"IsOfficialBuild")
            {
                const wchar_t* kDLLNames[] = {L"FileVersionInfoTest1.dll",
                                              L"FileVersionInfoTest2.dll"};

                const bool kExpected[] = {
                    true,
                    false,
                };

                // Test consistency check.
                TEST_ASSERT(arraysize(kDLLNames) == arraysize(kExpected));

                for (size_t i = 0; i < arraysize(kDLLNames); ++i)
                {
                    fspath dll_path = GetTestDataPath();
                    dll_path = dll_path.append(kDLLNames[i]);

                    std::unique_ptr<FileVersionInfo> version_info(
                        FileVersionInfo::CreateFileVersionInfo(dll_path));

                    TEST_ASSERT(kExpected[i] ==
                                version_info->is_official_build());
                }
            });

            TEST_CASE(L"CustomProperties")
            {
                fspath dll_path = GetTestDataPath();
                dll_path = dll_path.append("FileVersionInfoTest1.dll");

                std::unique_ptr<FileVersionInfo> version_info(
                    FileVersionInfo::CreateFileVersionInfo(dll_path));

                // Test few existing properties.
                std::wstring str;
                FileVersionInfoWin* version_info_win =
                    static_cast<FileVersionInfoWin*>(version_info.get());
                TEST_ASSERT(version_info_win->GetValue(L"Custom prop 1", &str));
                TEST_ASSERT(L"Un" == str);
                TEST_ASSERT(L"Un" ==
                            version_info_win->GetStringValue(L"Custom prop 1"));

                TEST_ASSERT(version_info_win->GetValue(L"Custom prop 2", &str));
                TEST_ASSERT(L"Deux" == str);
                TEST_ASSERT(L"Deux" ==
                            version_info_win->GetStringValue(L"Custom prop 2"));

                TEST_ASSERT(version_info_win->GetValue(L"Custom prop 3", &str));
                TEST_ASSERT(
                    L"1600 Amphitheatre Parkway Mountain View, CA 94043" ==
                    str);
                TEST_ASSERT(
                    L"1600 Amphitheatre Parkway Mountain View, CA 94043" ==
                    version_info_win->GetStringValue(L"Custom prop 3"));

                // Test an non-existing property.
                TEST_ASSERT(
                    version_info_win->GetValue(L"Unknown property", &str) == false);
                TEST_ASSERT(L"" == version_info_win->GetStringValue(
                                       L"Unknown property"));
            });
        });
    }
}  // namespace Test