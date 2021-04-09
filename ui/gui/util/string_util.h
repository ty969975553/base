#pragma once
#include "precompile_header.h"
#include <exception>

#include "base/strings/string_conversion.h"
//#include "base/base_export.h"

inline std::wstring atow(const std::string& str)
{
    return base::SysNativeMBToWide(str);
}

inline std::string wtoa(const std::wstring& wstr)
{
    return base::SysWideToNativeMB(wstr);
}

inline std::wstring itow(int i) { return std::to_wstring(i); }

inline std::string itoa(int i) { return std::to_string(i); }

inline std::string toUpper(const char* str)
{
    std::string temp(str);
    std::transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
    return temp;
}

inline std::string toLower(const char* str)
{
    std::string temp(str);
    std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
    return temp;
}

inline std::string toUpper(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

inline std::wstring toUpper(std::wstring& str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::towupper);
    return str;
}

inline std::string toLower(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

inline std::wstring toLower(std::wstring& str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::towlower);
    return str;
}

inline bool StringCompareNoCase(const std::string& a, std::string& b)
{
    return std::equal(a.begin(), a.end(), b.begin(), b.end(),
                      [](char chara, char charb) -> bool {
                          return ::tolower(chara) == ::tolower(charb);
                      });
}

inline bool StringCompareNoCase(const char* s1, const char* s2)
{
    if (s1 == nullptr && s2 == nullptr) return true;
    if (s1 == nullptr || s2 == nullptr) return false;

    // With thanks to cygwin for this code
    int d = 0;

    while (true)
    {
        const int c1 = ::toupper(*s1++);
        const int c2 = ::toupper(*s2++);

        if (((d = c1 - c2) != 0) || (c2 == '\0'))
        {
            break;
        }
    }
    return d == 0;
}

// TODO add test
inline bool StringToInt(const std::string& str, unsigned int* out)
{
    bool result = true;
    size_t index = 0;
    try
    {
        *out = std::stoi(str, &index);
    }
    catch (const std::exception& e)
    {
        result = false;
        std::cerr << e.what() << str << '\n';
    }
    if (index != str.size())

    {
        result = false;
    }
    return result;
}

inline bool endWith(const std::string& str, const std::string& tail)
{
    if (str.size() == 0 || str.size() < tail.size())
    {
        return false;
    }
    return str.compare(str.size() - tail.size(), tail.size(), tail) == 0;
}

inline bool startWith(const std::string& str, const std::string& head)
{
    if (str.size() == 0 || str.size() < head.size())
    {
        return false;
    }
    return str.compare(0, head.size(), head) == 0;
}

inline bool ReplaceString(std::wstring& str, size_t offset,
                          const std::wstring& find_this,
                          const std::wstring& replace_with,
                          bool whole_word_only, bool replace_all_instances)
{
    if (find_this.empty() || find_this == replace_with ||
        str.length() < find_this.length() || offset >= str.length())
        return false;

    bool found_and_replaced = false;

    for (size_t pos = str.find(find_this, offset); pos != std::wstring::npos;
         pos = str.find(find_this, pos))
    {
        auto is_whole_word = [&]() {
            auto is_boundary = [](wchar_t c) {
                return iswspace(c) || iswpunct(c);
            };
            if (pos == 0 || is_boundary(str.at(pos - 1)))
            {
                size_t pos_end = pos + find_this.length();
                if (pos_end >= str.length() || is_boundary(str.at(pos_end)))
                    return true;
            }
            return false;
        };

        if (whole_word_only && !is_whole_word())
        {
            pos += find_this.length();
        }
        else
        {
            str.replace(pos, find_this.length(), replace_with);
            pos += replace_with.length();
            found_and_replaced = true;
            if (!replace_all_instances) break;
        }
    }
    return found_and_replaced;
}
