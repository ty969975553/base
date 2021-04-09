#pragma once
#include "precompile_header.h"
#include "util/string_util.h"

namespace ui
{
    enum class Alignment
    {
        Left = 0,
        Top = 0,
        Center = 1,
        Right = 2,
        Bottom = 2,
    };

    enum class AxisDirection
    {
        LeftDown,
        RightDown,
        LeftUp,
        RightUp,
        DownLeft,
        DownRight,
        UpLeft,
        UpRight,
    };

    struct TextPos
    {
        int row;

        int column;

        TextPos() : row(0), column(0) {}

        TextPos(int _row, int _column) : row(_row), column(_column) {}

        int Compare(const TextPos& value) const
        {
            if (row < value.row) return -1;
            if (row > value.row) return 1;
            if (column < value.column) return -1;
            if (column > value.column) return 1;
            return 0;
        }

        bool operator==(const TextPos& value) const
        {
            return Compare(value) == 0;
        }
        bool operator!=(const TextPos& value) const
        {
            return Compare(value) != 0;
        }
        bool operator<(const TextPos& value) const
        {
            return Compare(value) < 0;
        }
        bool operator<=(const TextPos& value) const
        {
            return Compare(value) <= 0;
        }
        bool operator>(const TextPos& value) const
        {
            return Compare(value) > 0;
        }
        bool operator>=(const TextPos& value) const
        {
            return Compare(value) >= 0;
        }
    };

    struct GridPos
    {
        int row;
        int column;

        GridPos() : row(0), column(0) {}

        GridPos(int _row, int _column) : row(_row), column(_column) {}

        int Compare(const GridPos& value) const
        {
            if (row < value.row) return -1;
            if (row > value.row) return 1;
            if (column < value.column) return -1;
            if (column > value.column) return 1;
            return 0;
        }

        bool operator==(const GridPos& value) const
        {
            return Compare(value) == 0;
        }
        bool operator!=(const GridPos& value) const
        {
            return Compare(value) != 0;
        }
        bool operator<(const GridPos& value) const
        {
            return Compare(value) < 0;
        }
        bool operator<=(const GridPos& value) const
        {
            return Compare(value) <= 0;
        }
        bool operator>(const GridPos& value) const
        {
            return Compare(value) > 0;
        }
        bool operator>=(const GridPos& value) const
        {
            return Compare(value) >= 0;
        }
    };

    using GuiCoordinate = int;

    struct NativeCoordinate
    {
        int value;

        NativeCoordinate() : value(0) {}
        NativeCoordinate(int _value) : value(_value) {}
        NativeCoordinate(const NativeCoordinate& _value) = default;
        NativeCoordinate(NativeCoordinate&& _value) = default;
        NativeCoordinate& operator=(const NativeCoordinate& _value) = default;
        NativeCoordinate& operator=(NativeCoordinate&& _value) = default;

        inline bool operator==(NativeCoordinate c) const
        {
            return value == c.value;
        };
        inline bool operator!=(NativeCoordinate c) const
        {
            return value != c.value;
        };
        inline bool operator<(NativeCoordinate c) const
        {
            return value < c.value;
        };
        inline bool operator<=(NativeCoordinate c) const
        {
            return value <= c.value;
        };
        inline bool operator>(NativeCoordinate c) const
        {
            return value > c.value;
        };
        inline bool operator>=(NativeCoordinate c) const
        {
            return value >= c.value;
        };

        inline NativeCoordinate operator+(NativeCoordinate c) const
        {
            return value + c.value;
        };
        inline NativeCoordinate operator-(NativeCoordinate c) const
        {
            return value - c.value;
        };
        inline NativeCoordinate operator*(NativeCoordinate c) const
        {
            return value * c.value;
        };
        inline NativeCoordinate operator/(NativeCoordinate c) const
        {
            return value / c.value;
        };

        inline NativeCoordinate& operator+=(NativeCoordinate c)
        {
            value += c.value;
            return *this;
        };
        inline NativeCoordinate& operator-=(NativeCoordinate c)
        {
            value -= c.value;
            return *this;
        };
        inline NativeCoordinate& operator*=(NativeCoordinate c)
        {
            value *= c.value;
            return *this;
        };
        inline NativeCoordinate& operator/=(NativeCoordinate c)
        {
            value /= c.value;
            return *this;
        };
    };

    template <typename T>
    struct Point_
    {
        T x;
        T y;

        Point_() : x(0), y(0) {}

        Point_(T _x, T _y) : x(_x), y(_y) {}

        bool operator==(Point_<T> point) const
        {
            return x == point.x && y == point.y;
        }

        bool operator!=(Point_<T> point) const
        {
            return x != point.x || y != point.y;
        }
    };

    using Point = Point_<int>;
    using NativePoint = Point_<NativeCoordinate>;

    template <typename T>
    struct Size_
    {
        T x;
        T y;
        Size_() : x(0), y(0) {}

        Size_(T _x, T _y) : x(_x), y(_y) {}

        bool operator==(Size_<T> size) const
        {
            return x == size.x && y == size.y;
        }

        bool operator!=(Size_<T> size) const
        {
            return x != size.x || y != size.y;
        }
    };

    using Size = Size_<GuiCoordinate>;
    using NativeSize = Size_<NativeCoordinate>;

    template <typename T>
    struct Rect_
    {
        T left;
        T top;
        T right;
        T bottom;

        Rect_() : left(0), top(0), right(0), bottom(0) {}

        Rect_(T _x1, T _y1, T _x2, T _y2)
            : left(_x1), top(_y1), right(_x2), bottom(_y2)
        {
        }

        Rect_(Point_<T> p, Size_<T> s)
            : left(p.x), top(p.y), right(p.x + s.x), bottom(p.y + s.y)
        {
        }

        bool operator==(Rect_<T> rect) const
        {
            return left == rect.left && top == rect.top &&
                   right == rect.right && bottom == rect.bottom;
        }

        bool operator!=(Rect_<T> rect) const
        {
            return left != rect.left || top != rect.top ||
                   right != rect.right || bottom != rect.bottom;
        }

        Point_<T> LeftTop() const { return Point_<T>(left, top); }

        Point_<T> RightBottom() const { return Point_<T>(right, bottom); }

        Size_<T> GetSize() const
        {
            return Size_<T>(right - left, bottom - top);
        }

        T Left() const { return left; }

        T Right() const { return right; }

        T Width() const { return right - left; }

        T Top() const { return top; }

        T Bottom() const { return bottom; }

        T Height() const { return bottom - top; }

        void Expand(T x, T y)
        {
            left -= x;
            top -= y;
            right += x;
            bottom += y;
        }

        void Expand(Size_<T> s)
        {
            left -= s.x;
            top -= s.y;
            right += s.x;
            bottom += s.y;
        }

        void Move(T x, T y)
        {
            left += x;
            top += y;
            right += x;
            bottom += y;
        }

        void Move(Size_<T> s)
        {
            left += s.x;
            top += s.y;
            right += s.x;
            bottom += s.y;
        }

        bool Contains(Point_<T> p)
        {
            return left <= p.x && p.x < right && top <= p.y && p.y < bottom;
        }
    };

    using Rect = Rect_<GuiCoordinate>;
    using NativeRect = Rect_<NativeCoordinate>;

    /***********************************************************************
    2D operations
    ***********************************************************************/

    template <typename T>
    inline Point_<T> operator+(Point_<T> p, Size_<T> s)
    {
        return Point_<T>(p.x + s.x, p.y + s.y);
    }

    template <typename T>
    inline Point_<T> operator+(Size_<T> s, Point_<T> p)
    {
        return Point_<T>(p.x + s.x, p.y + s.y);
    }

    template <typename T>
    inline Point_<T> operator-(Point_<T> p, Size_<T> s)
    {
        return Point_<T>(p.x - s.x, p.y - s.y);
    }

    template <typename T>
    inline Size_<T> operator-(Point_<T> p1, Point_<T> p2)
    {
        return Size_<T>(p1.x - p2.x, p1.y - p2.y);
    }

    template <typename T>
    inline Size_<T> operator+(Size_<T> s1, Size_<T> s2)
    {
        return Size_<T>(s1.x + s2.x, s1.y + s2.y);
    }

    template <typename T>
    inline Size_<T> operator-(Size_<T> s1, Size_<T> s2)
    {
        return Size_<T>(s1.x - s2.x, s1.y - s2.y);
    }

    template <typename T>
    inline Size_<T> operator*(Size_<T> s, int i)
    {
        return Size_<T>(s.x * i, s.y * i);
    }

    template <typename T>
    inline Size_<T> operator/(Size_<T> s, int i)
    {
        return Size_<T>(s.x / i, s.y / i);
    }

    template <typename T>
    inline Point_<T> operator+=(Point_<T>& s1, Size_<T> s2)
    {
        s1.x += s2.x;
        s1.y += s2.y;
        return s1;
    }

    template <typename T>
    inline Point_<T> operator-=(Point_<T>& s1, Size_<T> s2)
    {
        s1.x -= s2.x;
        s1.y -= s2.y;
        return s1;
    }

    template <typename T>
    inline Size_<T> operator+=(Size_<T>& s1, Size_<T> s2)
    {
        s1.x += s2.x;
        s1.y += s2.y;
        return s1;
    }

    template <typename T>
    inline Size_<T> operator-=(Size_<T>& s1, Size_<T> s2)
    {
        s1.x -= s2.x;
        s1.y -= s2.y;
        return s1;
    }

    /***********************************************************************
    Color
    ***********************************************************************/
    struct Color
    {
        union
        {
            struct
            {
                unsigned char r;
                unsigned char g;
                unsigned char b;
                unsigned char a;
            };
            vuint32_t value;
        };

        Color() : r(0), g(0), b(0), a(255) {}

        Color(unsigned char _r, unsigned char _g, unsigned char _b,
              unsigned char _a = 255)
            : r(_r), g(_g), b(_b), a(_a)
        {
        }

        int Compare(Color color) const { return value - color.value; }

        static Color Parse(const std::wstring& value)
        {
            const wchar_t* code = L"0123456789ABCDEF";
            if ((value.Length() == 7 || value.Length() == 9) &&
                value[0] == L'#')
            {
                int index[8] = {15, 15, 15, 15, 15, 15, 15, 15};
                for (int i = 0; i < value.Length() - 1; i++)
                {
                    index[i] = wcschr(code, value[i + 1]) - code;
                    if (index[i] < 0 || index[i] > 15)
                    {
                        return Color();
                    }
                }

                Color c;
                c.r = (unsigned char)(index[0] * 16 + index[1]);
                c.g = (unsigned char)(index[2] * 16 + index[3]);
                c.b = (unsigned char)(index[4] * 16 + index[5]);
                c.a = (unsigned char)(index[6] * 16 + index[7]);
                return c;
            }
            return Color();
        }

        std::wstring ToString() const
        {
            const wchar_t* code = L"0123456789ABCDEF";
            wchar_t result[10] = L"#00000000";
            result[1] = code[r / 16];
            result[2] = code[r % 16];
            result[3] = code[g / 16];
            result[4] = code[g % 16];
            result[5] = code[b / 16];
            result[6] = code[b % 16];
            if (a == 255)
            {
                result[7] = L'0';
            }
            else
            {
                result[7] = code[a / 16];
                result[8] = code[a % 16];
            }
            return result;
        }

        bool operator==(Color color) const { return Compare(color) == 0; }
        bool operator!=(Color color) const { return Compare(color) != 0; }
        bool operator<(Color color) const { return Compare(color) < 0; }
        bool operator<=(Color color) const { return Compare(color) <= 0; }
        bool operator>(Color color) const { return Compare(color) > 0; }
        bool operator>=(Color color) const { return Compare(color) >= 0; }
    };

    /***********************************************************************
    Margin
    ***********************************************************************/

    template <typename T>
    struct Margin_
    {
        T left;
        T top;
        T right;
        T bottom;

        Margin_() : left(0), top(0), right(0), bottom(0) {}

        Margin_(T _left, T _top, T _right, T _bottom)
            : left(_left), top(_top), right(_right), bottom(_bottom)
        {
        }

        bool operator==(Margin_<T> margin) const
        {
            return left == margin.left && top == margin.top &&
                   right == margin.right && bottom == margin.bottom;
        }

        bool operator!=(Margin_<T> margin) const
        {
            return left != margin.left || top != margin.top ||
                   right != margin.right || bottom != margin.bottom;
        }
    };

    using Margin = Margin_<GuiCoordinate>;
    using NativeMargin = Margin_<NativeCoordinate>;

    /***********************************************************************
    Resources
    ***********************************************************************/
    struct FontProperties
    {
        std::wstring fontFamily;

        int size;

        bool bold;

        bool italic;

        bool underline;

        bool strikeline;

        bool antialias;

        bool verticalAntialias;

        FontProperties()
            : size(0),
              bold(false),
              italic(false),
              underline(false),
              strikeline(false),
              antialias(true),
              verticalAntialias(false)
        {
        }

        int Compare(const FontProperties& value) const
        {
            int result = 0;

            result = CompareStringNoCase(fontFamily, value.fontFamily);
            if (result != 0) return result;

            result = size - value.size;
            if (result != 0) return result;

            result = (int)bold - (int)value.bold;
            if (result != 0) return result;

            result = (int)italic - (int)value.italic;
            if (result != 0) return result;

            result = (int)underline - (int)value.underline;
            if (result != 0) return result;

            result = (int)strikeline - (int)value.strikeline;
            if (result != 0) return result;

            result = (int)antialias - (int)value.antialias;
            if (result != 0) return result;

            return 0;
        }

        bool operator==(const FontProperties& value) const
        {
            return Compare(value) == 0;
        }
        bool operator!=(const FontProperties& value) const
        {
            return Compare(value) != 0;
        }
        bool operator<(const FontProperties& value) const
        {
            return Compare(value) < 0;
        }
        bool operator<=(const FontProperties& value) const
        {
            return Compare(value) <= 0;
        }
        bool operator>(const FontProperties& value) const
        {
            return Compare(value) > 0;
        }
        bool operator>=(const FontProperties& value) const
        {
            return Compare(value) >= 0;
        }
    };

    enum class VKEY
    {
        _UNKNOWN = -1,
        LBUTTON = 0x01,
        RBUTTON = 0x02,
        CANCEL = 0x03,
        MBUTTON = 0x04,  /* NOT contiguous with L & RBUTTON */
        XBUTTON1 = 0x05, /* NOT contiguous with L & RBUTTON */
        XBUTTON2 = 0x06, /* NOT contiguous with L & RBUTTON */
        BACK = 0x08,
        TAB = 0x09,
        CLEAR = 0x0C,
        RETURN = 0x0D,
        SHIFT = 0x10,
        CONTROL = 0x11,
        MENU = 0x12,
        PAUSE = 0x13,
        CAPITAL = 0x14,
        KANA_HANGUL = 0x15,
        JUNJA = 0x17,
        FINAL = 0x18,
        HANJA = 0x19,
        KANJI = 0x19,
        ESCAPE = 0x1B,
        CONVERT = 0x1C,
        NONCONVERT = 0x1D,
        ACCEPT = 0x1E,
        MODECHANGE = 0x1F,
        SPACE = 0x20,
        PRIOR = 0x21,
        NEXT = 0x22,
        END = 0x23,
        HOME = 0x24,
        LEFT = 0x25,
        UP = 0x26,
        RIGHT = 0x27,
        DOWN = 0x28,
        SELECT = 0x29,
        PRINT = 0x2A,
        EXECUTE = 0x2B,
        SNAPSHOT = 0x2C,
        INSERT = 0x2D,
        DELETE = 0x2E,
        HELP = 0x2F,
        /*
         * VKEY_0 - VKEY_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
         * VKEY_A - VKEY_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
         */
        _0 = 0x30,
        _1 = 0x31,
        _2 = 0x32,
        _3 = 0x33,
        _4 = 0x34,
        _5 = 0x35,
        _6 = 0x36,
        _7 = 0x37,
        _8 = 0x38,
        _9 = 0x39,
        A = 0x41,
        B = 0x42,
        C = 0x43,
        D = 0x44,
        E = 0x45,
        F = 0x46,
        G = 0x47,
        H = 0x48,
        I = 0x49,
        J = 0x4A,
        K = 0x4B,
        L = 0x4C,
        M = 0x4D,
        N = 0x4E,
        O = 0x4F,
        P = 0x50,
        Q = 0x51,
        R = 0x52,
        S = 0x53,
        T = 0x54,
        U = 0x55,
        V = 0x56,
        W = 0x57,
        X = 0x58,
        Y = 0x59,
        Z = 0x5A,
        LWIN = 0x5B,
        RWIN = 0x5C,
        APPS = 0x5D,
        SLEEP = 0x5F,
        NUMPAD0 = 0x60,
        NUMPAD1 = 0x61,
        NUMPAD2 = 0x62,
        NUMPAD3 = 0x63,
        NUMPAD4 = 0x64,
        NUMPAD5 = 0x65,
        NUMPAD6 = 0x66,
        NUMPAD7 = 0x67,
        NUMPAD8 = 0x68,
        NUMPAD9 = 0x69,
        MULTIPLY = 0x6A,
        ADD = 0x6B,
        SEPARATOR = 0x6C,

        SUBTRACT = 0x6D,
        DECIMAL = 0x6E,
        DIVIDE = 0x6F,
        F1 = 0x70,
        F2 = 0x71,
        F3 = 0x72,
        F4 = 0x73,
        F5 = 0x74,
        F6 = 0x75,
        F7 = 0x76,
        F8 = 0x77,
        F9 = 0x78,
        F10 = 0x79,
        F11 = 0x7A,
        F12 = 0x7B,
        F13 = 0x7C,
        F14 = 0x7D,
        F15 = 0x7E,
        F16 = 0x7F,
        F17 = 0x80,
        F18 = 0x81,
        F19 = 0x82,
        F20 = 0x83,
        F21 = 0x84,
        F22 = 0x85,
        F23 = 0x86,
        F24 = 0x87,
        NUMLOCK = 0x90,
        SCROLL = 0x91,
        /*
         * NEC PC-9800 kbd definitions
         */
        OEM_NEC_EQUAL = 0x92, /* '=' key on numpad */
        /*
         * Fujitsu/OASYS kbd definitions
         */
        OEM_FJ_JISHO = 0x92,   /* 'Dictionary' key */
        OEM_FJ_MASSHOU = 0x93, /* 'Unregister word' key */
        OEM_FJ_TOUROKU = 0x94, /* 'Register word' key */
        OEM_FJ_LOYA = 0x95,    /* 'Left OYAYUBI' key */
        OEM_FJ_ROYA = 0x96,    /* 'Right OYAYUBI' key */
        /*
         * VKEY_L* & VKEY_R* - left and right Alt, Ctrl and Shift virtual keys.
         * Used only as parameters to GetAsyncKeyState() and GetKeyState().
         * No other API or message will distinguish left and right keys in this
         * way.
         */
        LSHIFT = 0xA0,
        RSHIFT = 0xA1,
        LCONTROL = 0xA2,
        RCONTROL = 0xA3,
        LMENU = 0xA4,
        RMENU = 0xA5,
        BROWSER_BACK = 0xA6,
        BROWSER_FORWARD = 0xA7,
        BROWSER_REFRESH = 0xA8,
        BROWSER_STOP = 0xA9,
        BROWSER_SEARCH = 0xAA,
        BROWSER_FAVORITES = 0xAB,
        BROWSER_HOME = 0xAC,
        VOLUME_MUTE = 0xAD,
        VOLUME_DOWN = 0xAE,
        VOLUME_UP = 0xAF,
        MEDIA_NEXT_TRACK = 0xB0,
        MEDIA_PREV_TRACK = 0xB1,
        MEDIA_STOP = 0xB2,
        MEDIA_PLAY_PAUSE = 0xB3,
        LAUNCH_MAIL = 0xB4,
        LAUNCH_MEDIA_SELECT = 0xB5,
        LAUNCH_APP1 = 0xB6,
        LAUNCH_APP2 = 0xB7,
        OEM_1 = 0xBA,      /* ';:' for US */
        OEM_PLUS = 0xBB,   /* '+' any country */
        OEM_COMMA = 0xBC,  /* ',' any country */
        OEM_MINUS = 0xBD,  /* '-' any country */
        OEM_PERIOD = 0xBE, /* '.' any country */
        OEM_2 = 0xBF,      /* '/?' for US */
        OEM_3 = 0xC0,      /* '`~' for US */
        OEM_4 = 0xDB,      /* '[{' for US */
        OEM_5 = 0xDC,      /* '|' for US */
        OEM_6 = 0xDD,      /* ']}' for US */
        OEM_7 = 0xDE,      /* ''"' for US */
        OEM_8 = 0xDF,
        /*
         * Various extended or enhanced keyboards
         */
        OEM_AX = 0xE1,   /* 'AX' key on Japanese AX kbd */
        OEM_102 = 0xE2,  /* "<>" or "|" on RT 102-key kbd */
        ICO_HELP = 0xE3, /* Help key on ICO */
        ICO_00 = 0xE4,   /* 00 key on ICO */
        PROCESSKEY = 0xE5,
        ICO_CLEAR = 0xE6,
        PACKET = 0xE7,
        /*
         * Nokia/Ericsson definitions
         */
        OEM_RESET = 0xE9,
        OEM_JUMP = 0xEA,
        OEM_PA1 = 0xEB,
        OEM_PA2 = 0xEC,
        OEM_PA3 = 0xED,
        OEM_WSCTRL = 0xEE,
        OEM_CUSEL = 0xEF,
        OEM_ATTN = 0xF0,
        OEM_FINISH = 0xF1,
        OEM_COPY = 0xF2,
        OEM_AUTO = 0xF3,
        OEM_ENLW = 0xF4,
        OEM_BACKTAB = 0xF5,
        ATTN = 0xF6,
        CRSEL = 0xF7,
        EXSEL = 0xF8,
        EREOF = 0xF9,
        PLAY = 0xFA,
        ZOOM = 0xFB,
        NONAME = 0xFC,
        PA1 = 0xFD,
        OEM_CLEAR = 0xFE,
        /*
         * Friendly names for common keys (US)
         */
        SEMICOLON = 0xBA,     /* OEM_1 */
        SLASH = 0xBF,         /* OEM_2 */
        GRAVE_ACCENT = 0xC0,  /* OEM_3 */
        RIGHT_BRACKET = 0xDB, /* OEM_4 */
        BACKSLASH = 0xDC,     /* OEM_5 */
        LEFT_BRACKET = 0xDD,  /* OEM_6 */
        APOSTROPHE = 0xDE,    /* OEM_7 */
    };
}  // namespace ui