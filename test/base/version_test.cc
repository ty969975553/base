#include "base/version.h"
#include "base/strings/string_util.h"

#include <iostream>
#include <optional>
#include <string>

#include "base/macros.h"
#include "base/test/unittest.h"
#include "ui/console/console.h"

using namespace base;
namespace Test
{
    TEST_FILE
    {
        TEST_CATEGORY(L"verison test")
        {
            // Semantics  语义
            TEST_CASE(L"ValueSemantics")
            {
                Version v1("1.2.3.4");
                TEST_ASSERT(v1.IsValid() == true);

                Version v2;
                TEST_ASSERT(v2.IsValid() == false);

                std::string value = "+1234.56";
                unsigned int a = 0;
                StringToInt(value, &a);
            });

            TEST_CASE(L"VersionCompare")
            {
                static const struct version_compare
                {
                    const char* lhs;
                    const char* rhs;
                    int expected;
                } cases[] = {
                    {"1.0", "1.0", 0},
                    {"1.0", "0.0", 1},
                    {"1.0", "2.0", -1},
                    {"1.0", "1.1", -1},
                    {"1.1", "1.0", 1},
                    {"1.0", "1.0.1", -1},
                    {"1.1", "1.0.1", 1},
                    {"1.1", "1.0.1", 1},
                    {"1.0.0", "1.0", 0},
                    {"1.0.3", "1.0.20", -1},
                    {"11.0.10", "15.007.20011", -1},
                    {"11.0.10", "15.5.28.130162", -1},
                };
                for (size_t i = 0; i < arraysize(cases); ++i)
                {
                    Version lhs(cases[i].lhs);
                    Version rhs(cases[i].rhs);
                    TEST_ASSERT(lhs.CompareTo(rhs) == (cases[i].expected));

                    TEST_ASSERT(lhs.IsOlderThan(cases[i].rhs) ==
                                (cases[i].expected == -1));
                }
            });

            TEST_CASE(L"IsValidWildcardString")
            {
                static const struct version_compare
                {
                    const char* version;
                    bool expected;
                } cases[] = {
                    {"1.0", true},         {"", false},
                    {"1.2.3.4.5.6", true}, {"1.2.3.*", true},
                    {"1.2.3.5*", false},   {"1.2.3.56*", false},
                    {"1.*.3", false},      {"20.*", true},
                    {"+2.*", false},       {"*", false},
                    {"*.2", false},
                };
                for (size_t i = 0; i < arraysize(cases); ++i)
                {
                    TEST_ASSERT(Version::IsValidWildcardString(
                                    cases[i].version) == cases[i].expected);
                    //<< cases[i].version << "?" << cases[i].expected;
                }
            });
        });
    }
}  // namespace Test