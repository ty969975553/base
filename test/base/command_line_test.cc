#include "base/command_line.h"

#include <iostream>
#include <optional>
#include <string>

#include "base/test/unittest.h"
#include "ui/console/console.h"

using namespace base;
const double kEpsilon = 1e-8;
namespace Test
{
    TEST_FILE
    {
        TEST_CATEGORY(L"command_line test")
        {
            TEST_CASE(L"InitFromArgv")
            {
                const wchar_t* argv[] = {L"--foo=", L"-bAr",
                                         L"-spaetzel=pierogi", L"a"};
                CommandLine cmdline(4, argv);
                cmdline.PrintParseOptions();

                const wchar_t* argv2[] = {L"--foo=1", L"-bAr=2",
                                          L"-spaetzel=pierogi", L"-wjm=false",
                                          L"/foo=2"};

                cmdline.InitFromArgv(5, argv2);
                cmdline.PrintParseOptions();

                bool actual = cmdline.HasOption(L"update");
                bool expected = false;
                TEST_ASSERT(actual == expected);

                actual = cmdline.HasOption(L"foo");
                expected = true;
                TEST_ASSERT(actual == expected);

                std::optional<int> actual2 = cmdline.get<int>(L"foo");
                int expected2 = 2;
                TEST_ASSERT(actual2.value() == expected2);

                std::optional<bool> actual3 = cmdline.get<bool>(L"wjm");
                bool expected3 = false;
                TEST_ASSERT(actual3.value() == expected3);
            });          
        });
    }
}  // namespace Test