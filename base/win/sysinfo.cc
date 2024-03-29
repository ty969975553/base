#include "base/win/sysinfo.h"

#include <tchar.h>

#include <algorithm>

#include "base/time/time_util.h"
#include "base/win/nt_function_util.h"

namespace base
{
    namespace win
    {
        std::wstring GetProcessUsername(HANDLE hProcess)
        {
            if (hProcess == NULL)
            {
                hProcess = ::GetCurrentProcess();
            }
            std::wstring ret;
            HANDLE tok = 0;
            TOKEN_USER *ptu;

            DWORD
            nlen, dlen;

            WCHAR name[300], dom[300], tubuf[300];
            WCHAR userName[1024] = {0};
            DWORD userNameLen = 1024;
            int iUse;

            if (OpenProcessToken(hProcess, TOKEN_QUERY, &tok))
            {
                ptu = (TOKEN_USER *)tubuf;
                if (GetTokenInformation(tok, (TOKEN_INFORMATION_CLASS)1, ptu,
                                        300, &nlen))
                {
                    dlen = 300;
                    nlen = 300;
                    if (LookupAccountSid(0, ptu->User.Sid, name, &nlen, dom,
                                         &dlen, (PSID_NAME_USE)&iUse))
                    {
                        nlen = std::min(DWORD(userNameLen) - 1, (DWORD)nlen);
                        _tcsnccpy(userName, name, nlen);
                        userName[nlen] = 0;
                        ret = userName;
                        return ret;
                    }
                }
            }

            if (tok) CloseHandle(tok);
            return ret;
        }

        std::wstring GetPCName()
        {
            TCHAR szHostName[MAX_PATH] = {0};
            DWORD dwSize = MAX_PATH;
            GetComputerName(szHostName, &dwSize);
            return std::wstring(szHostName);
        }

        std::wstring GetProcessorId()
        {
            // struct
            //{
            //    UInt32 eax;
            //    UInt32 ebx;
            //    UInt32 ecx;
            //    UInt32 edx;
            //} id;

            //__cpuid((Int32 *)&id, 1);

            // ::FormatS(Text("%08X%08X"), id.edx, id.eax);
            return std::wstring();
        }

        std::string GetProductId() { return std::string(); }

        int GetCpuUsagePercent() { return 0; }

        bool GetBootTime(time_t *t)
        {
            USE_API(ntdll.dll, NtQuerySystemInformation);
            if (!proc_NtQuerySystemInformation)
            {
                return false;
            }
            LONG status;
            nt::SYSTEM_TIME_INFORMATION sti;
            status = proc_NtQuerySystemInformation(SystemTimeOfDayInformation,
                                                   &sti, sizeof(sti), 0);
            if (NO_ERROR != status) return false;

            FILETIME ft;
            memcpy(&ft, &sti.liKeBootTime, sizeof(ft));
            *t = base::FileTimeToTime_t(ft);
            return true;
        }
    }  // namespace win
}  // namespace base