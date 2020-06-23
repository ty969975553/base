#pragma  once
#include <windows.h>
#include <string>
#include "base/base_export.h"

namespace base
{
    namespace win
    {
        BASE_EXPORT std::wstring GetProcessUsername(HANDLE hProcess);

        BASE_EXPORT std::wstring GetPCName();

        BASE_EXPORT std::wstring GetProcessorId();

        BASE_EXPORT std::string GetProductId();

        BASE_EXPORT int GetCpuUsagePercent();
    }    
}