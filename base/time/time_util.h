#pragma once
#include <Windows.h>
#include "base/base_export.h"

namespace base
{
    BASE_EXPORT time_t FileTimeToTime_t(FILETIME ft);
    BASE_EXPORT time_t SystemTimeToTime_t(SYSTEMTIME st);
}  // namespace base