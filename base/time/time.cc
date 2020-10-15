#include "time.h"
#include "time_util.h"

namespace base
{   
    Time::Time()
        : year(0),
          month(0),
          day(0),
          hour(0),
          minute(0),
          second(0),
          milliseconds(0),
          file_time(0)
    {
    }
    Time Time::LocalTime()
    {
        SYSTEMTIME sys_time;
        ::GetLocalTime(&sys_time);
        Time t;
        t.year = sys_time.wYear;
        t.month = sys_time.wMonth;
        t.day = sys_time.wDay;
        t.dayOfWeek = sys_time.wDayOfWeek;
        t.hour = sys_time.wHour;
        t.minute = sys_time.wMinute;
        t.second = sys_time.wSecond;
        t.milliseconds = sys_time.wMilliseconds;

        FILETIME fileTime;
        SystemTimeToFileTime(&sys_time, &fileTime);
        ULARGE_INTEGER largeInteger;
        largeInteger.HighPart = fileTime.dwHighDateTime;
        largeInteger.LowPart = fileTime.dwLowDateTime;
        t.total_milliseconds = largeInteger.QuadPart / 10000;
        t.file_time = largeInteger.QuadPart;
        return t;
    }

    Time Time::UtcTime()
    {
        SYSTEMTIME sys_time;
        ::GetSystemTime(&sys_time);
        Time t;
        t.year = sys_time.wYear;
        t.month = sys_time.wMonth;
        t.day = sys_time.wDay;
        t.dayOfWeek = sys_time.wDayOfWeek;
        t.hour = sys_time.wHour;
        t.minute = sys_time.wMinute;
        t.second = sys_time.wSecond;
        t.milliseconds = sys_time.wMilliseconds;

        FILETIME fileTime;
        SystemTimeToFileTime(&sys_time, &fileTime);
        ULARGE_INTEGER largeInteger;
        largeInteger.HighPart = fileTime.dwHighDateTime;
        largeInteger.LowPart = fileTime.dwLowDateTime;
        t.total_milliseconds = largeInteger.QuadPart / 10000;
        t.file_time = largeInteger.QuadPart;
        return t;
    }
}  // namespace base
