#pragma once
#include <algorithm>
#include <string>

#include "base/base_export.h"

namespace base
{
    typedef unsigned char Latin1Char;

    namespace detail
    {
        // Declaration for an array of bitfields holding character information.
        BASE_EXPORT extern const unsigned char kPropertyBits[256];

        // Declaration for the array of characters to upper-case characters.
        BASE_EXPORT extern const char kToUpper[256];

        // Declaration for the array of characters to lower-case characters.
        BASE_EXPORT extern const char kToLower[256];
    }  // namespace detail

    // ascii_isalpha()
    //
    // Determines whether the given character is an alphabetic character.
    inline bool ascii_isalpha(unsigned char c)
    {
        return (detail::kPropertyBits[c] & 0x01) != 0;
    }

    // ascii_isalnum()
    //
    // Determines whether the given character is an alphanumeric character.
    inline bool ascii_isalnum(unsigned char c)
    {
        return (detail::kPropertyBits[c] & 0x04) != 0;
    }

    // ascii_isspace()
    //
    // Determines whether the given character is a whitespace character (space,
    // tab, vertical tab, formfeed, linefeed, or carriage return).
    inline bool ascii_isspace(unsigned char c)
    {
        return (detail::kPropertyBits[c] & 0x08) != 0;
    }

    // ascii_ispunct()
    //
    // Determines whether the given character is a punctuation character.
    inline bool ascii_ispunct(unsigned char c)
    {
        return (detail::kPropertyBits[c] & 0x10) != 0;
    }

    // ascii_isblank()
    //
    // Determines whether the given character is a blank character (tab or
    // space).
    inline bool ascii_isblank(unsigned char c)
    {
        return (detail::kPropertyBits[c] & 0x20) != 0;
    }

    // ascii_iscntrl()
    //
    // Determines whether the given character is a control character.
    inline bool ascii_iscntrl(unsigned char c)
    {
        return (detail::kPropertyBits[c] & 0x40) != 0;
    }

    // ascii_isxdigit()
    //
    // Determines whether the given character can be represented as a
    // hexadecimal digit character (i.e. {0-9} or {A-F}).
    inline bool ascii_isxdigit(unsigned char c)
    {
        return (detail::kPropertyBits[c] & 0x80) != 0;
    }

    // ascii_isdigit()
    //
    // Determines whether the given character can be represented as a decimal
    // digit character (i.e. {0-9}).
    inline bool ascii_isdigit(unsigned char c) { return c >= '0' && c <= '9'; }

    // ascii_isprint()
    //
    // Determines whether the given character is printable, including
    // whitespace.
    inline bool ascii_isprint(unsigned char c) { return c >= 32 && c < 127; }

    // ascii_isgraph()
    //
    // Determines whether the given character has a graphical representation.
    inline bool ascii_isgraph(unsigned char c) { return c > 32 && c < 127; }

    // ascii_isupper()
    //
    // Determines whether the given character is uppercase.
    inline bool ascii_isupper(unsigned char c) { return c >= 'A' && c <= 'Z'; }

    // ascii_islower()
    //
    // Determines whether the given character is lowercase.
    inline bool ascii_islower(unsigned char c) { return c >= 'a' && c <= 'z'; }

    // ascii_isascii()
    //
    // Determines whether the given character is ASCII.
    inline bool ascii_isascii(unsigned char c) { return c < 128; }

    // ascii_tolower()
    //
    // Returns an ASCII character, converting to lowercase if uppercase is
    // passed. Note that character values > 127 are simply returned.
    inline char ascii_tolower(unsigned char c) { return detail::kToLower[c]; }

    // Converts the characters in `s` to lowercase, changing the contents of
    // `s`.
    void AsciiStrToLower(std::string* s);

    // Creates a lowercase string from a given base::string_view.
    inline std::string AsciiStrToLower(base::string_view s)
    {
        std::string result(s);
        base::AsciiStrToLower(&result);
        return result;
    }

    // ascii_toupper()
    //
    // Returns the ASCII character, converting to upper-case if lower-case is
    // passed. Note that characters values > 127 are simply returned.
    inline char ascii_toupper(unsigned char c) { return detail::kToUpper[c]; }

    // Converts the characters in `s` to uppercase, changing the contents of
    // `s`.
    void AsciiStrToUpper(std::string* s);

    // Creates an uppercase string from a given base::string_view.
    inline std::string AsciiStrToUpper(base::string_view s)
    {
        std::string result(s);
        base::AsciiStrToUpper(&result);
        return result;
    }

    // Returns base::string_view with whitespace stripped from the beginning of
    // the given string_view.
    inline base::string_view StripLeadingAsciiWhitespace(base::string_view str)
    {
        auto it = std::find_if_not(str.begin(), str.end(), base::ascii_isspace);
        return str.substr(it - str.begin());
    }

    // Strips in place whitespace from the beginning of the given string.
    inline void StripLeadingAsciiWhitespace(std::string* str)
    {
        auto it =
            std::find_if_not(str->begin(), str->end(), base::ascii_isspace);
        str->erase(str->begin(), it);
    }

    // Returns base::string_view with whitespace stripped from the end of the
    // given string_view.
    inline base::string_view StripTrailingAsciiWhitespace(base::string_view str)
    {
        auto it =
            std::find_if_not(str.rbegin(), str.rend(), base::ascii_isspace);
        return str.substr(0, str.rend() - it);
    }

    // Strips in place whitespace from the end of the given string
    inline void StripTrailingAsciiWhitespace(std::string* str)
    {
        auto it =
            std::find_if_not(str->rbegin(), str->rend(), base::ascii_isspace);
        str->erase(str->rend() - it);
    }

    // Returns base::string_view with whitespace stripped from both ends of the
    // given string_view.
    inline base::string_view StripAsciiWhitespace(base::string_view str)
    {
        return StripTrailingAsciiWhitespace(StripLeadingAsciiWhitespace(str));
    }

    // Strips in place whitespace from both ends of the given string
    inline void StripAsciiWhitespace(std::string* str)
    {
        StripTrailingAsciiWhitespace(str);
        StripLeadingAsciiWhitespace(str);
    }

    // Removes leading, trailing, and consecutive internal whitespace.
    void RemoveExtraAsciiWhitespace(std::string*);

}  // namespace base