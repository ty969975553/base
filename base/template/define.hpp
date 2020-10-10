#pragma once

namespace base
{
    // template_helper
    namespace th
    {
        //判断是不是字符串类型
        template <typename Ch>
        struct is_character : public std::false_type
        {
        };
        template <>
        struct is_character<char> : public std::true_type
        {
        };
        template <>
        struct is_character<wchar_t> : public std::true_type
        {
        };

        //
        template <class T>
        struct disable_if_c<true, T>
        {
        };

        template <class Cond, class T = void>
        struct disable_if : public disable_if_c<Cond::value, T>
        {
        };
    }  // namespace th
}  // namespace base