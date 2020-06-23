#pragma once
#include <regex>
#include <string>

#include "base/base_export.h"

namespace base
{
    BASE_EXPORT bool ValidatePhoneNumber(std::string mobileNumber);
}  // namespace base