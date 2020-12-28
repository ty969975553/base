#pragma once
#include <string>

namespace base
{
    namespace property_tree
    {
        namespace detail
        {
            template <typename T>
            struct less_nocase;
        }

        // basic_ptree
        template <class Key, class Data, class KeyCompare = std::less<Key> >
        class basic_ptree;

        template <typename T>
        struct id_translator;

        template <typename String, typename Translator>
        class string_path;

        template <typename Key>
        struct path_of;

        template <typename Internal, typename External>
        struct translator_between;

        class ptree_error;
        class ptree_bad_data;
        class ptree_bad_path;

        typedef string_path<std::string, id_translator<std::string> > path;

        /**
         * A property tree with std::string for key and data, and default
         * comparison.
         */
        typedef basic_ptree<std::string, std::string> ptree;

        /**
         * A property tree with std::string for key and data, and
         * case-insensitive comparison.
         */
        typedef basic_ptree<std::string, std::string,
                            detail::less_nocase<std::string> >
            iptree;

        typedef string_path<std::wstring, id_translator<std::wstring> > wpath;

        /**
         * A property tree with std::wstring for key and data, and default
         * comparison.
         * @note The type only exists if the platform supports @c wchar_t.
         */
        typedef basic_ptree<std::wstring, std::wstring> wptree;

        /**
         * A property tree with std::wstring for key and data, and
         * case-insensitive comparison.
         * @note The type only exists if the platform supports @c wchar_t.
         */
        typedef basic_ptree<std::wstring, std::wstring,
                            detail::less_nocase<std::wstring> >
            wiptree;

        // Free functions

        /**
         * Swap two property tree instances.
         */
        template <class K, class D, class C>
        void swap(basic_ptree<K, D, C> &pt1, basic_ptree<K, D, C> &pt2);
    }  // namespace property_tree
}  // namespace base