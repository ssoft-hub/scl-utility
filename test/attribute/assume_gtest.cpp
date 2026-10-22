#include <gtest_utils.h>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic error "-Wpedantic"
#endif

#include <scl/utility/attribute/assume.h>

#include "attribute_strict_warnings.h"

namespace
{
    int positive(int value)
    {
        SCL_ASSUME(value > 0);
        return value;
    }

    enum class severity
    {
        low,
        high
    };

    int priority(severity level)
    {
        if (level == severity::low)
            return 1;
        if (level == severity::high)
            return 2;
        SCL_UNREACHABLE();
    }
} // namespace

TEST(AttributeAssume, ExpandsWithNoCpp23ExtensionWarning) { EXPECT_EQ(positive(3), 3); }

TEST(AttributeAssume, LeavesTheConditionUnevaluatedOnTheBasis)
{
    int evaluations = 0;
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wassume"
#endif
    SCL_ASSUME((++evaluations, true));
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    EXPECT_EQ(evaluations, 0);
}

TEST(AttributeUnreachable, EndsAFunctionThatReturnsOnEveryPathWithNoWarning)
{
    EXPECT_EQ(priority(severity::low), 1);
    EXPECT_EQ(priority(severity::high), 2);
}
