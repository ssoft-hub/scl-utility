#include <gtest_utils.h>

#if defined(__GNUC__) || defined(__clang__)
// The row of __builtin_unreachable, defined before the header where its condition holds.
// clang-format off
#define SCL_ASSUME(expr) (static_cast<bool>(expr) ? (void)0 : __builtin_unreachable())
// clang-format on
#endif

#include <scl/utility/attribute/assume.h>

#if defined(__GNUC__) || defined(__clang__)
TEST(AttributeAssume, EvaluatesTheConditionUnderTheRowOfBuiltinUnreachable)
{
    int evaluations = 0;
    SCL_ASSUME((++evaluations, true));
    EXPECT_EQ(evaluations, 1);
}
#endif
