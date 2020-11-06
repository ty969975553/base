#pragma once
#include <windows.h>
#include <winternl.h>

#define USE_API(NAME, API) static auto proc_##API = (decltype(&API))(GetProcAddress(GetModuleHandle(L#NAME), #API))
//#define NT_SUCCESS(x) ((x) >= 0)

namespace nt
{
    //static const int SystemTimeInformation = 3;
    typedef struct
    {
        LARGE_INTEGER liKeBootTime;
        LARGE_INTEGER liKeSystemTime;
        LARGE_INTEGER liExpTimeZoneBias;
        ULONG uCurrentTimeZoneId;
        DWORD dwReserved;
    } SYSTEM_TIME_INFORMATION;

    //typedef NTSTATUS(__stdcall *PNT_QUERY_SYSTEM_INFORMATION)(
    //    __in SYSTEM_INFORMATION_CLASS SystemInformationClass,
    //    __inout PVOID SystemInformation, __in ULONG SystemInformationLength,
    //    __out_opt PULONG ReturnLength);
}

