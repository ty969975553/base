#include "base/win/windows_version.h"

#include <iostream>
#include <string>

#include "base/test/unittest.h"
#include "ui/console/console.h"

using namespace base::win;

namespace Test
{
    TEST_FILE
    {
        TEST_CATEGORY(L"windows_version test")
        {
            TEST_CASE(L"GetVersionExAndKernelVersionMatch")
            {   
                std::cout << "version" << " : " << OSInfo::GetInstance()->version() <<std::endl;
                std::cout << "Kernel32Version" << " : " << OSInfo::GetInstance()->Kernel32Version() <<std::endl;
                std::cout << "version_number major" << " : " << OSInfo::GetInstance()->version_number().major <<std::endl;
                std::cout << "version_number minor" << " : " << OSInfo::GetInstance()->version_number().minor <<std::endl;
                std::cout << "version_number build" << " : " << OSInfo::GetInstance()->version_number().build <<std::endl;
                std::cout << "service_pack major" << " : " << OSInfo::GetInstance()->service_pack().major<<std::endl;
                std::cout << "service_pack minor" << " : " << OSInfo::GetInstance()->service_pack().minor<<std::endl;
                std::cout << "architecture" << " : " << OSInfo::GetInstance()->architecture()<<std::endl;
                std::cout << "processor_model_name" << " : " << OSInfo::GetInstance()->processor_model_name()<<std::endl;
                TEST_ASSERT(OSInfo::GetInstance()->version() ==  OSInfo::GetInstance()->Kernel32Version());
            });
        });
    }
}  // namespace Test