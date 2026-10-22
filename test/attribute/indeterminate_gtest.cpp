#include <gtest_utils.h>

#include <scl/utility/attribute/indeterminate.h>

#include "attribute_strict_warnings.h"

namespace
{
    int sum_of_squares(int count)
    {
        SCL_INDETERMINATE
        int squares[8];
        for (int index = 0; index < count; ++index)
            squares[index] = index * index;
        int sum = 0;
        for (int index = 0; index < count; ++index)
            sum += squares[index];
        return sum;
    }

    int identity(SCL_INDETERMINATE int value) { return value; }
} // namespace

TEST(AttributeIndeterminate, StandsBeforeAVariableWrittenBeforeEveryReadWithNoWarning)
{
    EXPECT_EQ(sum_of_squares(4), 14);
}

TEST(AttributeIndeterminate, StandsBeforeAFunctionParameterWithNoWarning)
{
    EXPECT_EQ(identity(5), 5);
}
