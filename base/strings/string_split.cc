#include "base/strings/string_split.h"

namespace base
{
    std::vector<std::string> SplitString(const std::string& str,
                                         const std::string& delims)
    {
        std::vector<std::string> out;
        std::size_t current, previous = 0;
        current = str.find_first_of(delims);
        while (current != std::string::npos)
        {
            out.push_back(str.substr(previous, current - previous));
            previous = current + delims.size();
            current = str.find_first_of(delims, previous);
        }
        out.emplace_back(str.substr(previous, current - previous));
        return out;
    }

    std::vector<std::wstring> SplitString(const std::wstring& str,
                                          const std::wstring& delims)
    {
        std::vector<std::wstring> out;
        std::size_t current, previous = 0;
        current = str.find_first_of(delims);
        while (current != std::wstring::npos)
        {
            out.emplace_back(str.substr(previous, current - previous));
            previous = current + delims.size();
            current = str.find_first_of(delims, previous);
        }
        out.emplace_back(str.substr(previous, current - previous));
        return out;
    }
}  // namespace
