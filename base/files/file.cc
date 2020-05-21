#include "base/files/file.h"

namespace base
{
    FileInfo::FileInfo() : size(0), is_directory(false), is_symbolic_link(false)
    {
    }

    FileInfo::~FileInfo() {}
}  // namespace base
