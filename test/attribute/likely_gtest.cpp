#include <gtest_utils.h>

#include <scl/utility/attribute/likely.h>

#include <type_traits>

TEST(AttributeLikelyExpr, YieldsBool)
{
    STATIC_EXPECT_TRUE((std::is_same_v<decltype(SCL_LIKELY_EXPR(1 == 1)), bool>));
    STATIC_EXPECT_TRUE((std::is_same_v<decltype(SCL_UNLIKELY_EXPR(1 == 1)), bool>));
}
