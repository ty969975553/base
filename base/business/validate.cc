#pragma once
#include "base/business/validate.h"

namespace base
{
    BASE_EXPORT bool ValidatePhoneNumber(std::string mobileNumber)
    {
        std::regex e(
            "^1([38][0-9]|4[579]|5[0-3,5-9]|6[6]|7[0135678]|9[89])\\d{8}$");
        return regex_match(mobileNumber, e);
    }
}  // namespace base