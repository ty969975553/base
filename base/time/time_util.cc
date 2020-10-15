#include "base/time/time_util.h"

namespace base
{
    time_t FileTimeToTime_t(FILETIME ft)
    {
        //LONGLONG ll;
        ULARGE_INTEGER ui;
        ui.LowPart = ft.dwLowDateTime;
        ui.HighPart = ft.dwHighDateTime;
        //ll = (ft.dwHighDateTime << 32) + ft.dwLowDateTime;
        return ((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);
    }

    time_t SystemTimeToTime_t(SYSTEMTIME st) { 
        FILETIME ft;
        SystemTimeToFileTime(&st, &ft);
        return FileTimeToTime_t(ft);
    }
}  // namespace base