#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "base/base_export.h"
#include "config/build_config.h"

#if defined(OS_POSIX)
#include <stdlib.h>
#elif defined(OS_WIN)
#include <windows.h>
#endif