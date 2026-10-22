#include <gtest_utils.h>

#include <scl/utility/attribute/likely.h>

#include <type_traits>

#include "attribute_strict_warnings.h"

namespace
{
    int classify(int value)
    {
        if (value > 0)
            SCL_LIKELY { return 1; }
        else
            SCL_UNLIKELY { return -1; }
    }

    int label(int value)
    {
        switch (value)
        {
        SCL_LIKELY case 0:
            return 10;
            SCL_UNLIKELY default : return 20;
        }
    }

    struct negatable
    {
        bool value;

        bool operator!() const noexcept { return !value; }
    };
} // namespace

TEST(AttributeLikely, StandsBeforeTheStatementOfABranchWithNoWarning)
{
    EXPECT_EQ(classify(3), 1);
    EXPECT_EQ(classify(-3), -1);
}

TEST(AttributeLikely, StandsBeforeACaseOrDefaultLabelWithNoWarning)
{
    EXPECT_EQ(label(0), 10);
    EXPECT_EQ(label(1), 20);
}

TEST(AttributeLikelyExpr, YieldsBool)
{
    STATIC_EXPECT_TRUE((std::is_same_v<decltype(SCL_LIKELY_EXPR(1 == 1)), bool>));
    STATIC_EXPECT_TRUE((std::is_same_v<decltype(SCL_UNLIKELY_EXPR(1 == 1)), bool>));
}

TEST(AttributeLikelyExpr, YieldsTheTruthValueOfItsArgument)
{
    EXPECT_TRUE(SCL_LIKELY_EXPR(7));
    EXPECT_FALSE(SCL_LIKELY_EXPR(0));
    EXPECT_TRUE(SCL_UNLIKELY_EXPR(7));
    EXPECT_FALSE(SCL_UNLIKELY_EXPR(0));
}

TEST(AttributeLikelyExpr, StandsInTheConditionOfIfWhileAndTheConditionalOperator)
{
    int passes = 0;
    while (SCL_LIKELY_EXPR(passes < 3))
        ++passes;
    if (SCL_UNLIKELY_EXPR(passes != 3))
        passes = 0;
    EXPECT_EQ(SCL_LIKELY_EXPR(passes == 3) ? passes : 0, 3);
}

TEST(AttributeLikelyExpr, AcceptsATypeWithOnlyLogicalNot)
{
    EXPECT_TRUE(SCL_LIKELY_EXPR(negatable{true}));
    EXPECT_FALSE(SCL_UNLIKELY_EXPR(negatable{false}));
}
