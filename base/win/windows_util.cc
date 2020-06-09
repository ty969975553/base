#include "base/win/windows_util.h"

namespace base
{
    namespace win
    {
        namespace detail
        {
            byte* IsProcExistsInternal(byte* pBase, byte* pEnd,
                                       PIMAGE_NT_HEADERS pNtHeaders,
                                       DWORD VirtualAddress, LPCSTR ProcName)
            {
                RtlImageRvaToVaFunc RtlImageRvaToVa = (RtlImageRvaToVaFunc)::GetProcAddress(::GetModuleHandle(L"ntdl.dll"), "RtlImageRvaToVa");
                PIMAGE_EXPORT_DIRECTORY pExportTable =
                    (PIMAGE_EXPORT_DIRECTORY)RtlImageRvaToVa(
                        (PIMAGE_NT_HEADERS)pNtHeaders, pBase, VirtualAddress,
                        NULL);

                if (pExportTable == NULL) return NULL;

                if ((byte*)pExportTable + sizeof(IMAGE_EXPORT_DIRECTORY) >=
                    pEnd)
                {
                    return NULL;
                }

                auto pNames = (PDWORD)RtlImageRvaToVa(
                    (PIMAGE_NT_HEADERS)pNtHeaders, pBase,
                    pExportTable->AddressOfNames, NULL);

                if (pNames == NULL) return NULL;

                if ((byte*)pNames +
                        pExportTable->NumberOfNames * sizeof(*pNames) >=
                    pEnd)
                {
                    return NULL;
                }

                auto cbProcName = strlen(ProcName) + 1;

                for (int i = 0; i != pExportTable->NumberOfNames; i++)
                {
                    LPCSTR szFuncName = (LPCSTR)RtlImageRvaToVa(
                        (PIMAGE_NT_HEADERS)pNtHeaders, pBase, pNames[i], NULL);

                    if (szFuncName == NULL) continue;

                    if ((byte*)szFuncName + cbProcName >= pEnd)
                    {
                        return NULL;
                    }

                    // auto szFuncName = (LPCSTR)((BYTE*)pBase + pNames[i]);

                    if (memcmp(ProcName, szFuncName, cbProcName) == 0)
                    {
                        auto pNameOrdinals = (PWORD)RtlImageRvaToVa(
                            (PIMAGE_NT_HEADERS)pNtHeaders, pBase,
                            pExportTable->AddressOfNameOrdinals, NULL);

                        if (!pNameOrdinals) return NULL;

                        if ((byte*)pNameOrdinals +
                                sizeof(*pNameOrdinals) *
                                    pExportTable->NumberOfNames >=
                            pEnd)
                        {
                            return NULL;
                        }

                        //
                        auto pFunctions = (PDWORD)RtlImageRvaToVa(
                            (PIMAGE_NT_HEADERS)pNtHeaders, pBase,
                            pExportTable->AddressOfFunctions, NULL);

                        pFunctions += pNameOrdinals[i];

                        if ((byte*)pFunctions + sizeof(*pFunctions) >= pEnd)
                        {
                            return NULL;
                        }

                        return (byte*)RtlImageRvaToVa(
                            pNtHeaders, pBase, pFunctions[pNameOrdinals[i]],
                            NULL);
                    }
                }

                return NULL;
            }

        }  // namespace detail

        byte* IsProcExists(byte* pBase, byte* pEnd, LPCSTR ProcName)
        {
            if (pBase == NULL) return NULL;

            RtlImageNtHeaderFunc RtlImageNtHeader = (RtlImageNtHeaderFunc)::GetProcAddress(::GetModuleHandle(L"ntdl.dll"), "RtlImageNtHeader");
            auto pNtHeader = RtlImageNtHeader(pBase);

            if (pNtHeader == NULL) return NULL;

            if ((byte*)pNtHeader + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER) >=
                pEnd)
            {
                // NtHeader
                return NULL;
            }

            switch (pNtHeader->FileHeader.Machine)
            {
                case IMAGE_FILE_MACHINE_I386:
                    return detail::IsProcExistsInternal(
                        pBase, pEnd, pNtHeader,
                        ((PIMAGE_NT_HEADERS32)pNtHeader)
                            ->OptionalHeader
                            .DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]
                            .VirtualAddress,
                        ProcName);
                    break;
                case IMAGE_FILE_MACHINE_AMD64:
                case IMAGE_FILE_MACHINE_IA64:
                    return detail::IsProcExistsInternal(
                        pBase, pEnd, pNtHeader,
                        ((PIMAGE_NT_HEADERS64)pNtHeader)
                            ->OptionalHeader
                            .DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]
                            .VirtualAddress,
                        ProcName);
                    break;
                default:
                    return NULL;
                    break;
            }
        }

        bool IsProcExists(HMODULE hModule, LPCSTR ProcName)
        {
            return IsProcExists((byte*)((SIZE_T)hModule & (-1 ^ 0x3)),
                                ProcName);
        }

        bool IsProcExists(HANDLE hFile, LPCSTR ProcName)
        {
            HANDLE hMap =
                CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);

            auto pBase = (byte*)MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);

            if (!pBase) return false;

            auto ret = IsProcExists(pBase, pBase + GetFileSize(hFile, NULL),
                                    "DllGetClassObject");

            UnmapViewOfFile(pBase);

            return ret;
        }

    }  // namespace win
}  // namespace base