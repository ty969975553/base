#include "base/win/font_util.h"

#include <iostream>
#include <string>

#include "base/test/unittest.h"
#include "ui/console/console.h"

//using namespace base;

namespace Test
{
    TEST_FILE
    {
        TEST_CATEGORY(L"font_util")
        {
            TEST_CASE(L"printallfonts")
            {
                std::locale::global(std::locale("chs"));
                std::locale loc("chs");
                std::wcout.imbue(loc);

                base::FontManager* manager = base::FontManager::GetInstance();
                manager->EnumFontFamiliesExW(GetDC(NULL), DEFAULT_CHARSET,
                                             false);

                for (auto item : manager->screen_font_vec)
                {
                    std::wcout << L"----------------" << std::endl;
                    std::wcout << item.name << "   " << item.script;
                }
            });
        });
    }
}  // namespace Test