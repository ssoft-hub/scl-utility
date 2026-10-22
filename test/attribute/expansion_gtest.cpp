#include <gtest_utils.h>

#include <scl/utility/attribute.h>

#include "attribute_text.h"

// The row each compiler of the basis takes in the tables of the pages.
#if defined(_MSC_VER) && !defined(__clang__)
TEST(AttributeExpansion, TakesTheRowsOfMsvc)
{
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_ASSUME(x)), "[__assume(x)]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_UNREACHABLE()), "[__assume(false)]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_HOT), "[]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_COLD), "[]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_FORCE_INLINE), "[__forceinline]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_NOINLINE), "[__declspec(noinline)]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_LIFETIMEBOUND), "[[[msvc::lifetimebound]]]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_LIKELY_EXPR(x)), "[(static_cast<bool>(!!(x)))]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_UNLIKELY_EXPR(x)), "[(static_cast<bool>(!!(x)))]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_NO_UNIQUE_ADDRESS), "[[[msvc::no_unique_address]]]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_UNSEQUENCED(int f())), "[int f()]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_REPRODUCIBLE(int f())), "[int f()]");
}
#else
TEST(AttributeExpansion, TakesTheRowsOfGccAndClang)
{
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_UNREACHABLE()), "[__builtin_unreachable()]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_HOT), "[[[__gnu__::__hot__]]]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_COLD), "[[[__gnu__::__cold__]]]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_FORCE_INLINE), "[[[__gnu__::__always_inline__]] inline]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_NOINLINE), "[[[__gnu__::__noinline__]]]");
    constexpr char const * likely_expr = "[(static_cast<bool>(__builtin_expect(!!(x), 1)))]";
    constexpr char const * unlikely_expr = "[(static_cast<bool>(__builtin_expect(!!(x), 0)))]";
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_LIKELY_EXPR(x)), likely_expr);
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_UNLIKELY_EXPR(x)), unlikely_expr);
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_UNSEQUENCED(int f())), "[[[__gnu__::__const__]] int f()]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_REPRODUCIBLE(int f())), "[[[__gnu__::__pure__]] int f()]");
}

#if defined(__clang__)
TEST(AttributeExpansion, TakesTheRowsOfClang)
{
#if __cplusplus <= 202002L
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_ASSUME(x)), "[__builtin_assume(x)]");
#else
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_ASSUME(x)), "[[[assume(x)]]]");
#endif
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_LIFETIMEBOUND), "[[[_Clang::__lifetimebound__]]]");
#if defined(_MSC_VER)
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_NO_UNIQUE_ADDRESS), "[[[msvc::no_unique_address]]]");
#else
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_NO_UNIQUE_ADDRESS), "[[[no_unique_address]]]");
#endif
}
#else
TEST(AttributeExpansion, TakesTheRowsOfGcc)
{
#if __cplusplus <= 202002L
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_ASSUME(x)), "[__attribute__((__assume__(x)))]");
#else
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_ASSUME(x)), "[[[assume(x)]]]");
#endif
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_LIFETIMEBOUND), "[]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_NO_UNIQUE_ADDRESS), "[[[no_unique_address]]]");
}
#endif
#endif

#if defined(_MSVC_LANG)
#define SCL_TEST_LANG _MSVC_LANG
#else
#define SCL_TEST_LANG __cplusplus
#endif

#if SCL_TEST_LANG <= 202002L
TEST(AttributeExpansion, TakesTheFallbackOfIndeterminateInCpp20)
{
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_INDETERMINATE), "[]");
}
#endif

TEST(AttributeExpansion, TakesTheStandardAttributesOnEveryCompiler)
{
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_DEPRECATED), "[[[deprecated]]]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_DEPRECATED_MSG("m")), "[[[deprecated(\"m\")]]]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_FALLTHROUGH), "[[[fallthrough]]]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_LIKELY), "[[[likely]]]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_UNLIKELY), "[[[unlikely]]]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_MAYBE_UNUSED), "[[[maybe_unused]]]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_NODISCARD), "[[[nodiscard]]]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_NODISCARD_MSG("m")), "[[[nodiscard(\"m\")]]]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_NORETURN), "[[[noreturn]]]");
}
