#pragma once
#include "base/base_export.h"
#include "config/build_config.h"

namespace loggging
{
    int32 GetMainThreadId();

    void DumpStackTraceToString(std::string* stacktrace);

    struct CrashReason
    {
        CrashReason() : filename(0), line_number(0), message(0), depth(0) {}
        
        const char* filename;
        int line_number;
        const char* message;

        // We'll also store a bit of stack trace context at the time of crash as
        // it may not be available later on.
        void* stack[32];
        int depth;
    };
}  // namespace loggging
