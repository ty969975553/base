#pragma once
#include <stdint.h>

#include "base/base_export.h"
#include "config/build_config.h"

#if defined(OS_WIN)
#include <windows.h>
using Time = FILETIME;
#endif

namespace base
{
    struct BASE_EXPORT FileInfo
    {
        FileInfo();
        ~FileInfo();

        // The size of the file in bytes.  Undefined when is_directory is true.
        int64_t size;

        // True if the file corresponds to a directory.
        bool is_directory;

        // True if the file corresponds to a symbolic link.  For Windows
        // currently not supported and thus always false.
        bool is_symbolic_link;

        // The last modified time of a file.
        Time last_modified;

        // The last accessed time of a file.
        Time last_accessed;

        // The creation time of a file.
        Time creation_time;
    };
}  // namespace base
