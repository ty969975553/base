#include "base/strings/latin1.h"

namespace base
{
    namespace detail
    {
        BASE_EXPORT const unsigned char kPropertyBits[256] = {
            0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  // 0x00
            0x40, 0x68, 0x48, 0x48, 0x48, 0x48, 0x40, 0x40,
            0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  // 0x10
            0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
            0x28, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,  // 0x20
            0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
            0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84,  // 0x30
            0x84, 0x84, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
            0x10, 0x85, 0x85, 0x85, 0x85, 0x85, 0x85, 0x05,  // 0x40
            0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
            0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,  // 0x50
            0x05, 0x05, 0x05, 0x10, 0x10, 0x10, 0x10, 0x10,
            0x10, 0x85, 0x85, 0x85, 0x85, 0x85, 0x85, 0x05,  // 0x60
            0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
            0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,  // 0x70
            0x05, 0x05, 0x05, 0x10, 0x10, 0x10, 0x10, 0x40,
        };

        // Array of characters for the ascii_tolower() function. For values 'A'
        // through 'Z', return the lower-case character; otherwise, return the
        // identity of the passed character.
        BASE_EXPORT const char kToLower[256] = {
            '\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07',
            '\x08', '\x09', '\x0a', '\x0b', '\x0c', '\x0d', '\x0e', '\x0f',
            '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17',
            '\x18', '\x19', '\x1a', '\x1b', '\x1c', '\x1d', '\x1e', '\x1f',
            '\x20', '\x21', '\x22', '\x23', '\x24', '\x25', '\x26', '\x27',
            '\x28', '\x29', '\x2a', '\x2b', '\x2c', '\x2d', '\x2e', '\x2f',
            '\x30', '\x31', '\x32', '\x33', '\x34', '\x35', '\x36', '\x37',
            '\x38', '\x39', '\x3a', '\x3b', '\x3c', '\x3d', '\x3e', '\x3f',
            '\x40', 'a',    'b',    'c',    'd',    'e',    'f',    'g',
            'h',    'i',    'j',    'k',    'l',    'm',    'n',    'o',
            'p',    'q',    'r',    's',    't',    'u',    'v',    'w',
            'x',    'y',    'z',    '\x5b', '\x5c', '\x5d', '\x5e', '\x5f',
            '\x60', '\x61', '\x62', '\x63', '\x64', '\x65', '\x66', '\x67',
            '\x68', '\x69', '\x6a', '\x6b', '\x6c', '\x6d', '\x6e', '\x6f',
            '\x70', '\x71', '\x72', '\x73', '\x74', '\x75', '\x76', '\x77',
            '\x78', '\x79', '\x7a', '\x7b', '\x7c', '\x7d', '\x7e', '\x7f',
            '\x80', '\x81', '\x82', '\x83', '\x84', '\x85', '\x86', '\x87',
            '\x88', '\x89', '\x8a', '\x8b', '\x8c', '\x8d', '\x8e', '\x8f',
            '\x90', '\x91', '\x92', '\x93', '\x94', '\x95', '\x96', '\x97',
            '\x98', '\x99', '\x9a', '\x9b', '\x9c', '\x9d', '\x9e', '\x9f',
            '\xa0', '\xa1', '\xa2', '\xa3', '\xa4', '\xa5', '\xa6', '\xa7',
            '\xa8', '\xa9', '\xaa', '\xab', '\xac', '\xad', '\xae', '\xaf',
            '\xb0', '\xb1', '\xb2', '\xb3', '\xb4', '\xb5', '\xb6', '\xb7',
            '\xb8', '\xb9', '\xba', '\xbb', '\xbc', '\xbd', '\xbe', '\xbf',
            '\xc0', '\xc1', '\xc2', '\xc3', '\xc4', '\xc5', '\xc6', '\xc7',
            '\xc8', '\xc9', '\xca', '\xcb', '\xcc', '\xcd', '\xce', '\xcf',
            '\xd0', '\xd1', '\xd2', '\xd3', '\xd4', '\xd5', '\xd6', '\xd7',
            '\xd8', '\xd9', '\xda', '\xdb', '\xdc', '\xdd', '\xde', '\xdf',
            '\xe0', '\xe1', '\xe2', '\xe3', '\xe4', '\xe5', '\xe6', '\xe7',
            '\xe8', '\xe9', '\xea', '\xeb', '\xec', '\xed', '\xee', '\xef',
            '\xf0', '\xf1', '\xf2', '\xf3', '\xf4', '\xf5', '\xf6', '\xf7',
            '\xf8', '\xf9', '\xfa', '\xfb', '\xfc', '\xfd', '\xfe', '\xff',
        };

        // Array of characters for the ascii_toupper() function. For values 'a'
        // through 'z', return the upper-case character; otherwise, return the
        // identity of the passed character.
        BASE_EXPORT const char kToUpper[256] = {
            '\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07',
            '\x08', '\x09', '\x0a', '\x0b', '\x0c', '\x0d', '\x0e', '\x0f',
            '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17',
            '\x18', '\x19', '\x1a', '\x1b', '\x1c', '\x1d', '\x1e', '\x1f',
            '\x20', '\x21', '\x22', '\x23', '\x24', '\x25', '\x26', '\x27',
            '\x28', '\x29', '\x2a', '\x2b', '\x2c', '\x2d', '\x2e', '\x2f',
            '\x30', '\x31', '\x32', '\x33', '\x34', '\x35', '\x36', '\x37',
            '\x38', '\x39', '\x3a', '\x3b', '\x3c', '\x3d', '\x3e', '\x3f',
            '\x40', '\x41', '\x42', '\x43', '\x44', '\x45', '\x46', '\x47',
            '\x48', '\x49', '\x4a', '\x4b', '\x4c', '\x4d', '\x4e', '\x4f',
            '\x50', '\x51', '\x52', '\x53', '\x54', '\x55', '\x56', '\x57',
            '\x58', '\x59', '\x5a', '\x5b', '\x5c', '\x5d', '\x5e', '\x5f',
            '\x60', 'A',    'B',    'C',    'D',    'E',    'F',    'G',
            'H',    'I',    'J',    'K',    'L',    'M',    'N',    'O',
            'P',    'Q',    'R',    'S',    'T',    'U',    'V',    'W',
            'X',    'Y',    'Z',    '\x7b', '\x7c', '\x7d', '\x7e', '\x7f',
            '\x80', '\x81', '\x82', '\x83', '\x84', '\x85', '\x86', '\x87',
            '\x88', '\x89', '\x8a', '\x8b', '\x8c', '\x8d', '\x8e', '\x8f',
            '\x90', '\x91', '\x92', '\x93', '\x94', '\x95', '\x96', '\x97',
            '\x98', '\x99', '\x9a', '\x9b', '\x9c', '\x9d', '\x9e', '\x9f',
            '\xa0', '\xa1', '\xa2', '\xa3', '\xa4', '\xa5', '\xa6', '\xa7',
            '\xa8', '\xa9', '\xaa', '\xab', '\xac', '\xad', '\xae', '\xaf',
            '\xb0', '\xb1', '\xb2', '\xb3', '\xb4', '\xb5', '\xb6', '\xb7',
            '\xb8', '\xb9', '\xba', '\xbb', '\xbc', '\xbd', '\xbe', '\xbf',
            '\xc0', '\xc1', '\xc2', '\xc3', '\xc4', '\xc5', '\xc6', '\xc7',
            '\xc8', '\xc9', '\xca', '\xcb', '\xcc', '\xcd', '\xce', '\xcf',
            '\xd0', '\xd1', '\xd2', '\xd3', '\xd4', '\xd5', '\xd6', '\xd7',
            '\xd8', '\xd9', '\xda', '\xdb', '\xdc', '\xdd', '\xde', '\xdf',
            '\xe0', '\xe1', '\xe2', '\xe3', '\xe4', '\xe5', '\xe6', '\xe7',
            '\xe8', '\xe9', '\xea', '\xeb', '\xec', '\xed', '\xee', '\xef',
            '\xf0', '\xf1', '\xf2', '\xf3', '\xf4', '\xf5', '\xf6', '\xf7',
            '\xf8', '\xf9', '\xfa', '\xfb', '\xfc', '\xfd', '\xfe', '\xff',
        };
    }  // namespace detail

    void AsciiStrToLower(std::string* s)
    {
        for (auto& ch : *s)
        {
            ch = base::ascii_tolower(ch);
        }
    }

    void AsciiStrToUpper(std::string* s)
    {
        for (auto& ch : *s)
        {
            ch = base::ascii_toupper(ch);
        }
    }

    void RemoveExtraAsciiWhitespace(std::string* str)
    {
        auto stripped = StripAsciiWhitespace(*str);

        if (stripped.empty())
        {
            str->clear();
            return;
        }

        auto input_it = stripped.begin();
        auto input_end = stripped.end();
        auto output_it = &(*str)[0];
        bool is_ws = false;

        for (; input_it < input_end; ++input_it)
        {
            if (is_ws)
            {
                // Consecutive whitespace?  Keep only the last.
                is_ws = base::ascii_isspace(*input_it);
                if (is_ws) --output_it;
            }
            else
            {
                is_ws = base::ascii_isspace(*input_it);
            }

            *output_it = *input_it;
            ++output_it;
        }

        str->erase(output_it - &(*str)[0]);
    }
}  // namespace base