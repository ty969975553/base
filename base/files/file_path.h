#pragma once
#ifdef _has_cpp17
#include <filesystem>
#endif

namespace base{
#ifndef _has_cpp17
    namespace filesystem {

    }
#else
    using filesystem = std::filestrem;
#endif
}