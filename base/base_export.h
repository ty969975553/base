#pragma once

#if defined(_MSC_VER)
#if defined(BUILD_DLL)
#define BASE_EXPORT __declspec(dllexport)
#elif defined(CONSUME_DLL)
#define BASE_EXPORT __declspec(dllimport)
#else
#define BASE_EXPORT
#endif
#else
#define BASE_EXPORT
#endif  // defined(_MSC_VER)