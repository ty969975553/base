#pragma once
#include <stdint.h>
#include <time.h>

#include <iosfwd>
#include <limits>

#include "base/base_export.h"
#include "base/numerics/safe_math.h"
#include "config/build_config.h"

#if defined(OS_WIN)
#include <windows.h>
#endif

namespace base {

    class TimeDelta;

    
}