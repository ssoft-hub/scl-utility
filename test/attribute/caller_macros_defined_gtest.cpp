// No header precedes these macros, so a header of the group is the first to meet them.
#define noinline __attribute__((__noinline__))
#define always_inline __attribute__((__always_inline__))
#define hot hot_is_a_macro_of_the_caller
#define cold cold_is_a_macro_of_the_caller
#define pure pure_is_a_macro_of_the_caller
#define unused unused_is_a_macro_of_the_caller
#define warn_unused_result warn_unused_result_is_a_macro_of_the_caller
#define lifetimebound lifetimebound_is_a_macro_of_the_caller
#define gnu gnu_is_a_macro_of_the_caller
#define clang clang_is_a_macro_of_the_caller
#define msvc msvc_is_a_macro_of_the_caller
#define likely(condition) (condition)
#define unlikely(condition) (condition)
#ifdef __has_cpp_attribute
#if !__has_cpp_attribute(unsequenced)
#define unsequenced
#endif
#if !__has_cpp_attribute(reproducible)
#define reproducible
#endif
#else
#define unsequenced
#define reproducible
#endif
#if defined(_MSVC_LANG)
#define SCL_TEST_LANG _MSVC_LANG
#else
#define SCL_TEST_LANG __cplusplus
#endif
#if SCL_TEST_LANG <= 202002L
#define assume
#endif
#if SCL_TEST_LANG < 202400L
#define indeterminate
#endif

// The headers ask for these definitions: SCL_NO_UNIQUE_ADDRESS under msvc for the MSVC ABI, and on
// MSVC SCL_NOINLINE under noinline.
#if defined(_MSC_VER)
#define SCL_NO_UNIQUE_ADDRESS
#endif
#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_NOINLINE
#endif

#include "caller_macros.h"

namespace
{
    SCL_NOINLINE int called() { return 1; }
    SCL_FORCE_INLINE int inlined() { return 2; }
    SCL_HOT int often() { return 3; }
    SCL_COLD int rarely() { return 4; }
    SCL_UNSEQUENCED(int square(int value)) { return value * value; }
    SCL_REPRODUCIBLE(int first(int const * values)) { return values[0]; }
    SCL_NODISCARD int kept() { return 5; }

    int const & same(int const & value SCL_LIFETIMEBOUND) { return value; }

    int sign(int value)
    {
        if (value >= 0)
            SCL_LIKELY { return 1; }
        else
            SCL_UNLIKELY { return -1; }
    }
} // namespace

char const * const caller_macros_expansions[caller_macros_count] = {SCL_TEST_MACROS(SCL_TEST_EXPANSION)};

int caller_macros_sum() noexcept
{
    SCL_MAYBE_UNUSED int const unread = 0;
    int const values[] = {6};
    int const seven = 7;
    return called() + inlined() + often() + rarely() + square(2) + first(values) + kept() +
        same(seven) + sign(-2);
}
