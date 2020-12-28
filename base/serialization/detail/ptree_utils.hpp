#pragma once
#include <algorithm>
#include <locale>
#include <string>

namespace base
{
    namespace property_tree
    {
        namespace detail
        {
            template <typename T>
            struct is_translator
            {
            };
        }  // namespace detail
    }      // namespace property_tree
}  // namespace base