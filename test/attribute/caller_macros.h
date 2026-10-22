#pragma once

#include <scl/utility/attribute.h>

#include "attribute_text.h"

#define SCL_TEST_EXPANSION(text) SCL_TEST_TEXT(text),
#define SCL_TEST_NAME(text) #text,

// clang-format off
#define SCL_TEST_MACROS(entry)       \
    entry(SCL_ASSUME(x))             \
    entry(SCL_UNREACHABLE())         \
    entry(SCL_DEPRECATED)            \
    entry(SCL_DEPRECATED_MSG("m"))   \
    entry(SCL_FALLTHROUGH)           \
    entry(SCL_HOT)                   \
    entry(SCL_COLD)                  \
    entry(SCL_INDETERMINATE)         \
    entry(SCL_FORCE_INLINE)          \
    entry(SCL_NOINLINE)              \
    entry(SCL_LIFETIMEBOUND)         \
    entry(SCL_LIKELY)                \
    entry(SCL_UNLIKELY)              \
    entry(SCL_LIKELY_EXPR(x))        \
    entry(SCL_UNLIKELY_EXPR(x))      \
    entry(SCL_MAYBE_UNUSED)          \
    entry(SCL_NODISCARD)             \
    entry(SCL_NODISCARD_MSG("m"))    \
    entry(SCL_NORETURN)              \
    entry(SCL_NO_UNIQUE_ADDRESS)     \
    entry(SCL_UNSEQUENCED(int f()))  \
    entry(SCL_REPRODUCIBLE(int f()))
// clang-format on

#define SCL_TEST_ONE(text) +1
inline constexpr int caller_macros_count = 0 SCL_TEST_MACROS(SCL_TEST_ONE);

// The expansions in the translation unit that defines the macros of the caller.
extern char const * const caller_macros_expansions[caller_macros_count];

int caller_macros_sum() noexcept;
