#include "base/time/time_util.h"

namespace base
{
    inline bool LRESULTToBool(LRESULT v) { return (v != FALSE); }
    inline bool BOOLToBool(BOOL v) { return (v != FALSE); }
    inline BOOL BoolToBOOL(bool v) { return (v ? TRUE : FALSE); }

    static const UInt32 kNumTimeQuantumsInSecond = 10000000;
    static const UInt32 kFileTimeStartYear = 1601;
    static const UInt32 kDosTimeStartYear = 1980;
    static const UInt32 kUnixTimeStartYear = 1970;
    static const UInt64 kUnixTimeOffset =
        (UInt64)60 * 60 * 24 *
        (89 + 365 * (kUnixTimeStartYear - kFileTimeStartYear));
    static const UInt64 kNumSecondsInFileTime =
        (UInt64)(Int64)-1 / kNumTimeQuantumsInSecond;

    bool DosTimeToFileTime(UInt32 dosTime, FILETIME &ft) throw()
    {
        return BOOLToBool(::DosDateTimeToFileTime(
            (UInt16)(dosTime >> 16), (UInt16)(dosTime & 0xFFFF), &ft));
    }

    static const UInt32 kHighDosTime = 0xFF9FBF7D;
    static const UInt32 kLowDosTime = 0x210000;

#define PERIOD_4 (4 * 365 + 1)
#define PERIOD_100 (PERIOD_4 * 25 - 1)
#define PERIOD_400 (PERIOD_100 * 4 + 1)

    bool FileTimeToDosTime(const FILETIME &ft, UInt32 &dosTime) throw()
    {
        WORD datePart, timePart;
        if (!::FileTimeToDosDateTime(&ft, &datePart, &timePart))
        {
            dosTime =
                (ft.dwHighDateTime >= 0x01C00000) ? kHighDosTime : kLowDosTime;
            return false;
        }
        dosTime = (((UInt32)datePart) << 16) + timePart;
        return true;
    }

    UInt64 UnixTimeToFileTime64(UInt32 unixTime) throw()
    {
        return (kUnixTimeOffset + (UInt64)unixTime) * kNumTimeQuantumsInSecond;
    }

    void UnixTimeToFileTime(UInt32 unixTime, FILETIME &ft) throw()
    {
        UInt64 v = UnixTimeToFileTime64(unixTime);
        ft.dwLowDateTime = (DWORD)v;
        ft.dwHighDateTime = (DWORD)(v >> 32);
    }

    UInt64 UnixTime64ToFileTime64(Int64 unixTime) throw()
    {
        return (UInt64)(kUnixTimeOffset + unixTime) * kNumTimeQuantumsInSecond;
    }

    bool UnixTime64ToFileTime(Int64 unixTime, FILETIME &ft) throw()
    {
        if (unixTime > (Int64)(kNumSecondsInFileTime - kUnixTimeOffset))
        {
            ft.dwLowDateTime = ft.dwHighDateTime = (UInt32)(Int32)-1;
            return false;
        }
        Int64 v = (Int64)kUnixTimeOffset + unixTime;
        if (v < 0)
        {
            ft.dwLowDateTime = ft.dwHighDateTime = 0;
            return false;
        }
        UInt64 v2 = (UInt64)v * kNumTimeQuantumsInSecond;
        ft.dwLowDateTime = (DWORD)v2;
        ft.dwHighDateTime = (DWORD)(v2 >> 32);
        return true;
    }

    Int64 FileTimeToUnixTime64(const FILETIME &ft) throw()
    {
        UInt64 winTime = (((UInt64)ft.dwHighDateTime) << 32) + ft.dwLowDateTime;
        return (Int64)(winTime / kNumTimeQuantumsInSecond) -
               (Int64)kUnixTimeOffset;
    }

    bool FileTimeToUnixTime(const FILETIME &ft, UInt32 &unixTime) throw()
    {
        UInt64 winTime = (((UInt64)ft.dwHighDateTime) << 32) + ft.dwLowDateTime;
        winTime /= kNumTimeQuantumsInSecond;
        if (winTime < kUnixTimeOffset)
        {
            unixTime = 0;
            return false;
        }
        winTime -= kUnixTimeOffset;
        if (winTime > 0xFFFFFFFF)
        {
            unixTime = 0xFFFFFFFF;
            return false;
        }
        unixTime = (UInt32)winTime;
        return true;
    }

    bool GetSecondsSince1601(unsigned year, unsigned month, unsigned day,
                             unsigned hour, unsigned min, unsigned sec,
                             UInt64 &resSeconds) throw()
    {
        resSeconds = 0;
        if (year < kFileTimeStartYear || year >= 10000 || month < 1 ||
            month > 12 || day < 1 || day > 31 || hour > 23 || min > 59 ||
            sec > 59)
            return false;
        UInt32 numYears = year - kFileTimeStartYear;
        UInt32 numDays =
            numYears * 365 + numYears / 4 - numYears / 100 + numYears / 400;
        Byte ms[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ms[1] = 29;
        month--;
        for (unsigned i = 0; i < month; i++) numDays += ms[i];
        numDays += day - 1;
        resSeconds = ((UInt64)(numDays * 24 + hour) * 60 + min) * 60 + sec;
        return true;
    }

    void GetCurUtcFileTime(FILETIME &ft) throw()
    {
        // Both variants provide same low resolution on WinXP: about 15 ms.
        // But GetSystemTimeAsFileTime is much faster.
        GetSystemTimeAsFileTime(&ft);
    }
}  // namespace base