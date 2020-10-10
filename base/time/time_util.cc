#include "base/time/time_util.h"

namespace base
{
    time_t FileTimeToTime_t(FILETIME ft)
    {
        LONGLONG ll;

        ULARGE_INTEGER ui;
        ui.LowPart = ft.dwLowDateTime;
        ui.HighPart = ft.dwHighDateTime;

        ll = ft.dwHighDateTime << 32 + ft.dwLowDateTime;

        return ((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);
    }
}  // namespace base