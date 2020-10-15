#pragma once
#include <stdint.h>
#include <time.h>

#include <iosfwd>
#include <limits>

#include "base/base_export.h"
#include "config/build_config.h"

#if defined(OS_WIN)
#include <windows.h>
#endif

namespace base
{
    struct BASE_EXPORT Time
    {
        int year;
        int month;
        int dayOfWeek;
        int day;
        int hour;
        int minute;
        int second;
        int milliseconds;

        int64_t total_milliseconds;

        //filetime
        int64_t file_time;

        Time();
        static Time LocalTime();
        static Time UtcTime();
    };
}  // namespace base