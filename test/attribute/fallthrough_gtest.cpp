#include <gtest_utils.h>

#include <scl/utility/attribute/fallthrough.h>

#include "attribute_strict_warnings.h"

namespace
{
    int steps_left(int stage)
    {
        int steps = 0;
        switch (stage)
        {
        case 0:
            ++steps;
            SCL_FALLTHROUGH;
        case 1:
            ++steps;
            SCL_FALLTHROUGH;
        case 2:
            ++steps;
            break;
        default:
            break;
        }
        return steps;
    }
} // namespace

TEST(AttributeFallthrough, MarksACaseThatContinuesWithNoWarning)
{
    EXPECT_EQ(steps_left(0), 3);
    EXPECT_EQ(steps_left(2), 1);
    EXPECT_EQ(steps_left(3), 0);
}
