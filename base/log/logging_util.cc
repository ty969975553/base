#pragma once
#include "base/log/logging_util.h"

namespace loggging
{
    int32 GetMainThreadId()
    {
#if defined OS_LINUX
        return getpid();  // Linux:  getpid returns thread ID when gettid is
                          // absent
#elif defined OS_WIN
        return GetCurrentThreadId();
#endif
    }

    void DumpStackTraceToString(string* stacktrace)
    {
        // DumpStackTrace(1, DebugWriteToString, stacktrace);
    }
}  // namespace loggging
