#include "base/win/sysinfo.h"

#include <iostream>
#include <string>

#include "base/test/unittest.h"
#include "ui/console/console.h"
#include <time.h>

using namespace base::win;

namespace Test
{
    TEST_FILE
    {
        TEST_CATEGORY(L"sysinfo_all")
        {
            TEST_CASE(L"GetPCName")
            {
                std::wstring pc_name = GetPCName();
                TEST_ASSERT(!pc_name.empty());
            });

            TEST_CASE(L"GetBootTime")
            {
                time_t t = 0;
                GetBootTime(&t);
                ui::console::WriteLine(std::to_wstring(t));
                struct tm* timeinfo;
                int sec, toyear, tomonth, today;
                timeinfo = localtime(&t);
                std::cout << timeinfo->tm_year << std::endl;
                std::cout << timeinfo->tm_mon << std::endl;
                std::cout << timeinfo->tm_mday << std::endl;
                std::cout << timeinfo->tm_hour << std::endl;
                std::cout << timeinfo->tm_min << std::endl;
                std::cout << timeinfo->tm_sec << std::endl;
            });
        });
    }
}  // namespace Test