#pragma once

#include <cstdint>
#include <cassert>
#include <cstring>

// TODO: use CMake to detect this
#ifdef HAVE_STD_STRING_VIEW

#include <string_view>
namespace fixing {
using std::string_view;
} /*fixing*/

#else

#include <experimental/string_view>
namespace fixing {
using std::experimental::string_view;
} /*fixing*/

#endif

// TODO: move to a definitions file
#define FIXING_LIKELY(x) __builtin_expect(x, 1)
#define FIXING_UNLIKELY(x) __builtin_expect(x, 0)
#define FIXING_UNREACHABLE() do { assert(0); __builtin_unreachable(); } while(0)
#define FIXING_RESTRICT __restrict
#define FIXING_HOT_FUNCTION __attribute__((hot))
#define FIXING_COLD_FUNCTION __attribute__((cold, noinline))
#define FIXING_CONSTEXPR_FUNCTION constexpr
#define FIXING_CONSTEXPR constexpr

// TODO: detect if have nodiscard
#define FIXING_NODISCARD
// #define FIXING_NODISCARD [[nodiscard]]

#ifdef USE_IACA
    #include "iacaMarks.h"
    #define FIXING_IACA_START IACA_START
    #define FIXING_IACA_END   IACA_START
#else
    #define FIXING_IACA_START
    #define FIXING_IACA_END
#endif

#define FIXING_FIX_SEPARATOR '\001'
#define FIXING_CHECKSUM_TAG 10
