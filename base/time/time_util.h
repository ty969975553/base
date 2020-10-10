#pragma once
#include <Windows.h>
#include "base/base_export.h"

namespace base
{
    time_t FileTimeToTime_t(FILETIME ft);
}  // namespace base