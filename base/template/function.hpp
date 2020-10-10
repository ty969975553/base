#pragma once
#include <locale>
#include <string>

namespace base
{
    // template_helper
    namespace th
    {
        //类型比较
        template <typename T>
        struct less_nocase
        {
            typedef typename T::value_type Ch;
            std::locale m_locale;
            inline bool operator()(Ch c1, Ch c2) const
            {
                return std::toupper(c1, m_locale) < std::toupper(c2, m_locale);
            }
            inline bool operator()(const T &t1, const T &t2) const
            {
                return std::lexicographical_compare(
                    t1.begin(), t1.end(), t2.begin(), t2.end(), *this);
            }
        };

        //
        template <typename Str>
        Str trim(const Str &s, const std::locale &loc = std::locale())
        {
            typename Str::const_iterator first = s.begin();
            typename Str::const_iterator end = s.end();
            while (first != end && std::isspace(*first, loc)) ++first;
            if (first == end) return Str();
            typename Str::const_iterator last = end;
            do
                --last;
            while (std::isspace(*last, loc));
            if (first != s.begin() || last + 1 != end)
                return Str(first, last + 1);
            else
                return s;
        }
    }  // namespace th
}  // namespace base