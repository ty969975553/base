#pragma once

// use <winnt.h> must define UMDF_USING_NTSTATUS
#define UMDF_USING_NTSTATUS
#include <Windows.h>
#include <ntstatus.h>
#include <winnt.h>

#include "base/base_export.h"

#pragma comment(lib, "ntdll.lib")

typedef unsigned char byte;
typedef _Return_type_success_(return >= 0) LONG NTSTATUS;
typedef NTSTATUS* PNTSTATUS;
typedef ULONG LOGICAL;

// Privilege code
#define SeCreateTokenPrivilege 0x2
#define SeAssignPrimaryTokenPrivilege 0x3
#define SeLockMemoryPrivilege 0x4
#define SeIncreaseQuotaPrivilege 0x5
#define SeUnsolicitedInputPrivilege 0x0
#define SeMachineAccountPrivilege 0x6
#define SeTcbPrivilege 0x7
#define SeSecurityPrivilege 0x8
#define SeTakeOwnershipPrivilege 0x9
#define SeLoadDriverPrivilege 0xa
#define SeSystemProfilePrivilege 0xb
#define SeSystemtimePrivilege 0xc
#define SeProfileSingleProcessPrivilege 0xd
#define SeIncreaseBasePriorityPrivilege 0xe
#define SeCreatePagefilePrivilege 0xf
#define SeCreatePermanentPrivilege 0x10
#define SeBackupPrivilege 0x11
#define SeRestorePrivilege 0x12
#define SeShutdownPrivilege 0x13
#define SeDebugPrivilege 0x14
#define SeAuditPrivilege 0x15
#define SeSystemEnvironmentPrivilege 0x16
#define SeChangeNotifyPrivilege 0x17
#define SeRemoteShutdownPrivilege 0x18
#define SeUndockPrivilege 0x19
#define SeSyncAgentPrivilege 0x1a
#define SeEnableDelegationPrivilege 0x1b
#define SeManageVolumePrivilege 0x1c
#define SeImpersonatePrivilege 0x1d
#define SeCreateGlobalPrivilege 0x1e
#define SeTrustedCredManAccessPrivilege 0x1f
#define SeRelabelPrivilege 0x20
#define SeIncreaseWorkingSetPrivilege 0x21
#define SeTimeZonePrivilege 0x22
#define SeCreateSymbolicLinkPrivilege 0x23

extern "C" {

typedef NTSYSAPI NTSTATUS(NTAPI* RtlAdjustPrivilegeFunc)(ULONG Privilege,
                                                         BOOLEAN Enable,
                                                         BOOLEAN CurrentThread,
                                                         PBOOLEAN Enabled);
// NTSYSAPI NTSTATUS NTAPI RtlAdjustPrivilege(ULONG Privilege, BOOLEAN Enable,
//                                           BOOLEAN CurrentThread,
//                                           PBOOLEAN Enabled);

typedef NTSYSAPI PIMAGE_NT_HEADERS(NTAPI* RtlImageNtHeaderFunc)(
    _In_ PVOID BaseAddress);

// NTSYSAPI PIMAGE_NT_HEADERS NTAPI RtlImageNtHeader(_In_ PVOID BaseAddress);

// NTSYSAPI PVOID NTAPI RtlImageRvaToVa(
//    _In_ PIMAGE_NT_HEADERS NtHeader, _In_ PVOID BaseAddress, _In_ ULONG Rva,
//    _Inout_opt_ PIMAGE_SECTION_HEADER* SectionHeader);
typedef NTSYSAPI PVOID(NTAPI* RtlImageRvaToVaFunc)(
    _In_ PIMAGE_NT_HEADERS NtHeader, _In_ PVOID BaseAddress, _In_ ULONG Rva,
    _Inout_opt_ PIMAGE_SECTION_HEADER* SectionHeader);
}

namespace base
{
    namespace win
    {
        namespace detail
        {
        }  // namespace detail

        bool IsProcExists(HMODULE hModule, LPCSTR ProcName);

        //_Check_return_ _Success_(return != nullptr) byte* IsProcExists(
        //    _In_ byte* pBase, _In_z_ LPCSTR ProcName);

        _Check_return_ bool IsProcExists(_In_ HANDLE hFile,
                                         _In_z_ LPCSTR ProcName);

    }  // namespace win
}  // namespace base