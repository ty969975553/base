#pragma once
//originate from https://github.com/andrivet/ADVobfuscator

#if defined(_MSC_VER)
#define ALWAYS_INLINE __forceinline
#else
#define ALWAYS_INLINE __attribute__((always_inline))
#endif

namespace obfuscator
{
    // std::index_sequence will be available with C++14
    // make_indexes
    template <int... I>
    struct Indexes
    {
        using type = Indexes<I..., sizeof...(I)>;
    };

    template <int N>
    struct Make_Indexes
    {
        using type = typename Make_Indexes<N - 1>::type::type;
    };

    template <>
    struct Make_Indexes<0>
    {
        using type = Indexes<>;
    };

    namespace
    {
        // I use current (compile time) as a seed

        constexpr char time[] = __TIME__;  // __TIME__ has the following format:
                                           // hh:mm:ss in 24-hour time

        // Convert time string (hh:mm:ss) into a number
        constexpr int DigitToInt(char c) { return c - '0'; }
        const int seed = DigitToInt(time[7]) + DigitToInt(time[6]) * 10 +
            DigitToInt(time[4]) * 60 + DigitToInt(time[3]) * 600 +
            DigitToInt(time[1]) * 3600 +
            DigitToInt(time[0]) * 36000;
    }  // namespace

    // 1988, Stephen Park and Keith Miller
    // "Random Number Generators: Good Ones Are Hard To Find", considered as
    // "minimal standard" Park-Miller 31 bit pseudo-random number generator,
    // implemented with G. Carta's optimisation: with 32-bit math and without
    // division

    template <int N>
    struct MetaRandomGenerator
    {
    private:
        static constexpr unsigned a = 16807;       // 7^5
        static constexpr unsigned m = 2147483647;  // 2^31 - 1

        static constexpr unsigned s = MetaRandomGenerator<N - 1>::value;
        static constexpr unsigned lo =
            a * (s & 0xFFFF);  // Multiply lower 16 bits by 16807
        static constexpr unsigned hi =
            a * (s >> 16);  // Multiply higher 16 bits by 16807
        static constexpr unsigned lo2 =
            lo + ((hi & 0x7FFF)
                << 16);  // Combine lower 15 bits of hi with lo's upper bits
        static constexpr unsigned hi2 =
            hi >> 15;  // Discard lower 15 bits of hi
        static constexpr unsigned lo3 = lo2 + hi;

    public:
        static constexpr unsigned max = m;
        static constexpr unsigned value = lo3 > m ? lo3 - m : lo3;
    };

    template <>
    struct MetaRandomGenerator<0>
    {
        static constexpr unsigned value = seed;
    };

    // Note: A bias is introduced by the modulo operation.
    // However, I do belive it is neglictable in this case (M is far lower than
    // 2^31 - 1)

    template <int N, int M>
    struct MetaRandom
    {
        static const int value = MetaRandomGenerator<N + 1>::value % M;
    };

    // Helper to generate a key
    template <class _Elem, int N>
    struct MetaRandomChar;

    template <int N>
    struct MetaRandomChar<char, N>
    {
        // Use 0x7F as maximum value since most of the time, char is signed (we
        // have however 1 bit less of randomness)
        static const char value =
            static_cast<char>(1 + MetaRandom<N, 0x7F - 1>::value);
    };

    template <int N>
    struct MetaRandomChar<wchar_t, N>
    {
        static const wchar_t value =
            static_cast<wchar_t>(1 + MetaRandom<N, 0x7FFF - 1>::value);
    };

    template <class _Elem, int N, _Elem Key, typename Indexes>
    struct MetaString;

    // method 0
    template <class _Elem, _Elem Key, int... I>
    struct MetaString<_Elem, 0, Key, Indexes<I...>>
    {
        // Constructor. Evaluated at compile time.
        constexpr ALWAYS_INLINE MetaString(const _Elem* str)
            : key_{ Key }, buffer_{ encrypt(str[I], Key)... }
        {
        }

        // Runtime decryption. Most of the time, inlined
        inline const _Elem* decrypt()
        {
            for (size_t i = 0; i < sizeof...(I); ++i)
                buffer_[i] = decrypt(buffer_[i]);
            buffer_[sizeof...(I)] = 0;
            return const_cast<const _Elem*>(buffer_);
        }

    private:
        // Encrypt / decrypt a character of the original string with the key
        constexpr _Elem key() const { return key_; }
        constexpr _Elem ALWAYS_INLINE encrypt(_Elem c, int k) const
        {
            return c ^ k;
        }
        constexpr _Elem decrypt(_Elem c) const { return encrypt(c, key()); }

        volatile int key_;  // key. "volatile" is important to avoid
                            // uncontrolled over-optimization by the compiler
        volatile _Elem
            buffer_[sizeof...(I) + 1];  // Buffer to store the encrypted string
                                        // + terminating null byte
    };

    // method 1
    template <class _Elem, _Elem Key, int... I>
    struct MetaString<_Elem, 1, Key, Indexes<I...>>
    {
        // Constructor. Evaluated at compile time.
        constexpr ALWAYS_INLINE MetaString(const _Elem* str)
            : key_(Key), buffer_{ encrypt(str[I], I)... }
        {
        }

        // Runtime decryption. Most of the time, inlined
        inline const _Elem* decrypt()
        {
            for (size_t i = 0; i < sizeof...(I); ++i)
                buffer_[i] = decrypt(buffer_[i], i);
            buffer_[sizeof...(I)] = 0;
            return const_cast<const _Elem*>(buffer_);
        }

    private:
        // Encrypt / decrypt a character of the original string with the key
        constexpr _Elem key(size_t position) const
        {
            return static_cast<_Elem>(key_ + position);
        }
        constexpr _Elem ALWAYS_INLINE encrypt(_Elem c, size_t position) const
        {
            return c ^ key(position);
        }
        constexpr _Elem decrypt(_Elem c, size_t position) const
        {
            return encrypt(c, position);
        }

        volatile int key_;  // key. "volatile" is important to avoid
                            // uncontrolled over-optimization by the compiler
        volatile _Elem
            buffer_[sizeof...(I) + 1];  // Buffer to store the encrypted string
                                        // + terminating null byte
    };

    // method 2
    template <class _Elem, _Elem Key, int... I>
    struct MetaString<_Elem, 2, Key, Indexes<I...>>
    {
        // Constructor. Evaluated at compile time. Key is *not* stored
        constexpr ALWAYS_INLINE MetaString(const _Elem* str)
            : buffer_{ encrypt(str[I])..., 0 }
        {
        }

        // Runtime decryption. Most of the time, inlined
        inline const _Elem* decrypt()
        {
            for (size_t i = 0; i < sizeof...(I); ++i)
                buffer_[i] = decrypt(buffer_[i]);
            return const_cast<const _Elem*>(buffer_);
        }

    private:
        // Encrypt / decrypt a character of the original string with the key
        // Be sure that the encryption key is never 0.
        constexpr _Elem key(_Elem key) const { return 1 + (key % 13); }
        constexpr _Elem ALWAYS_INLINE encrypt(_Elem c) const
        {
            return c + key(Key);
        }
        constexpr _Elem decrypt(_Elem c) const { return c - key(Key); }

        // Buffer to store the encrypted string + terminating null byte. Key is
        // not stored
        volatile _Elem buffer_[sizeof...(I) + 1];
    };
}  // namespace obfuscator

#define DEF_OBFUSCATEDA(str)                                  \
    obfuscator::MetaString<                                   \
        char, obfuscator::MetaRandom<__COUNTER__, 3>::value,  \
        obfuscator::MetaRandomChar<char, __COUNTER__>::value, \
        obfuscator::Make_Indexes<sizeof(str) - 1>::type>(str)

#define DEF_OBFUSCATEDW(str)                                                \
    obfuscator::MetaString<                                                 \
        wchar_t, obfuscator::MetaRandom<__COUNTER__, 3>::value,             \
        obfuscator::MetaRandomChar<wchar_t, __COUNTER__>::value,            \
        obfuscator::Make_Indexes<sizeof(str) / sizeof(wchar_t) - 1>::type>( \
        str)

#define OBFUSCATEDA(str) (DEF_OBFUSCATEDA(str).decrypt())
#define OBFUSCATEDW(str) (DEF_OBFUSCATEDW(str).decrypt())