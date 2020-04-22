#pragma once

namespace base
{
    class NotCopyable
    {
    private:
        NotCopyable(const NotCopyable&);
        NotCopyable& operator=(const NotCopyable&);

    public:
        NotCopyable();
    };

#if defined _WIN64 || __x86_64 || __LP64__
#define _64
#endif

#if defined _MSC_VER
#define _MSVC
#else
#define _GCC
#if defined(__APPLE__)
#define _APPLE
#endif
#endif

#if defined _MSC_VER || defined _GCC || defined _DEBUG
#define CHECK_ERROR(CONDITION, DESCRIPTION)         \
    do                                              \
    {                                               \
        if (!(CONDITION)) throw Error(DESCRIPTION); \
    } while (0)
#elif defined NDEBUG
#define CHECK_ERROR(CONDITION, DESCRIPTION)
#endif

#define CHECK_FAIL(DESCRIPTION)   \
    do                            \
    {                             \
        throw Error(DESCRIPTION); \
    } while (0)

}  // namespace base
