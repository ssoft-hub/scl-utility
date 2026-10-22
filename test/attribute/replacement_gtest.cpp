#include <gtest_utils.h>

// Every macro of the group is defined before the header as the last row of its table.
#define SCL_ASSUME(expr) ((void)(expr))
#define SCL_UNREACHABLE() ((void)0)
#define SCL_DEPRECATED
#define SCL_DEPRECATED_MSG(msg) SCL_DEPRECATED
// Spaces of its own between the tokens leave the spelling of the row the same.
// clang-format off
#define SCL_FALLTHROUGH ( ( void ) 0 )
// clang-format on
#define SCL_HOT
#define SCL_COLD
#define SCL_INDETERMINATE
#define SCL_FORCE_INLINE inline
#define SCL_NOINLINE
#define SCL_LIFETIMEBOUND
#define SCL_LIKELY
#define SCL_UNLIKELY
#define SCL_LIKELY_EXPR(expr) (static_cast<bool>(!!(expr)))
#define SCL_UNLIKELY_EXPR(expr) (static_cast<bool>(!!(expr)))
#define SCL_MAYBE_UNUSED
#define SCL_NODISCARD
#define SCL_NODISCARD_MSG(msg) SCL_NODISCARD
#define SCL_NORETURN
#define SCL_NO_UNIQUE_ADDRESS
#define SCL_UNSEQUENCED(...) __VA_ARGS__
#define SCL_REPRODUCIBLE(...) __VA_ARGS__

#include <scl/utility/attribute.h>

#include "attribute_text.h"

namespace
{
    SCL_FORCE_INLINE int twice(int value) { return value * 2; }

    SCL_UNSEQUENCED(int square(int value)) { return value * value; }

    SCL_REPRODUCIBLE(int first(int const * values)) { return values[0]; }

    void count_stage(int stage, int & steps)
    {
        switch (stage)
        {
        case 0:
            ++steps;
            return;
        default:
            SCL_UNREACHABLE();
        }
    }

    int positive(int value)
    {
        SCL_ASSUME(value > 0);
        return value;
    }

    int steps_from(int stage)
    {
        int steps = 0;
        switch (stage)
        {
        case 0:
            ++steps;
            SCL_FALLTHROUGH;
        case 1:
            ++steps;
            break;
        default:
            break;
        }
        return steps;
    }
} // namespace

TEST(AttributeReplacement, KeepsADefinitionOfTheLastRowMadeBeforeTheHeader)
{
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_ASSUME(x)), "[((void)(x))]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_UNREACHABLE()), "[((void)0)]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_DEPRECATED), "[]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_DEPRECATED_MSG("m")), "[]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_FALLTHROUGH), "[( ( void ) 0 )]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_HOT), "[]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_COLD), "[]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_INDETERMINATE), "[]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_FORCE_INLINE), "[inline]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_NOINLINE), "[]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_LIFETIMEBOUND), "[]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_LIKELY), "[]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_UNLIKELY), "[]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_LIKELY_EXPR(x)), "[(static_cast<bool>(!!(x)))]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_UNLIKELY_EXPR(x)), "[(static_cast<bool>(!!(x)))]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_MAYBE_UNUSED), "[]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_NODISCARD), "[]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_NODISCARD_MSG("m")), "[]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_NORETURN), "[]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_NO_UNIQUE_ADDRESS), "[]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_UNSEQUENCED(int f())), "[int f()]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_REPRODUCIBLE(int f())), "[int f()]");
}

TEST(AttributeReplacement, EvaluatesTheConditionOfAssumeUnderTheLastRow)
{
    int evaluations = 0;
    SCL_ASSUME((++evaluations, true));
    EXPECT_EQ(evaluations, 1);
}

TEST(AttributeReplacement, ComputesAsWithoutTheMacrosUnderTheLastRowsThatAreNotEmpty)
{
    int const values[] = {8};
    int steps = 0;
    count_stage(0, steps);
    EXPECT_EQ(steps, 1);
    EXPECT_EQ(first(values), 8);
    EXPECT_EQ(twice(2), 4);
    EXPECT_EQ(square(3), 9);
    EXPECT_EQ(positive(5), 5);
    EXPECT_EQ(steps_from(0), 2);
    EXPECT_TRUE(SCL_LIKELY_EXPR(twice(1) == 2));
    EXPECT_FALSE(SCL_UNLIKELY_EXPR(twice(1) != 2));
}
