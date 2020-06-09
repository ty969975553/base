#pragma once
#define UMDF_USING_NTSTATUS
#include <Windows.h>
#include <ntstatus.h>
#include <winnt.h>

#include "base/base_export.h"

#pragma comment(lib, "ntdll.lib")

namespace base
{
    namespace nuclear
    {
        BASE_EXPORT void bluescreen()
        {
            HMODULE ntdll = LoadLibraryW(L"ntdll");
            FARPROC RtlAdjustPrivilege =
                GetProcAddress(ntdll, "RtlAdjustPrivilege");
            FARPROC NtRaiseHardError =
                GetProcAddress(ntdll, "NtRaiseHardError");

            if (RtlAdjustPrivilege != NULL && NtRaiseHardError != NULL)
            {
                BOOLEAN tmp1;
                DWORD tmp2;
                ((void (*)(DWORD, DWORD, BOOLEAN, LPBYTE))RtlAdjustPrivilege)(
                    19, 1, 0, &tmp1);
                ((void (*)(DWORD, DWORD, DWORD, DWORD, DWORD,
                           LPDWORD))NtRaiseHardError)(0xc0000022, 0, 0, 0, 6,
                                                      &tmp2);
            }
        }
    }  // namespace nuclear
}  // namespace base