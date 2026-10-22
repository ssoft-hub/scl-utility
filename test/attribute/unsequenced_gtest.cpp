#include <gtest_utils.h>

#include <scl/utility/attribute/unsequenced.h>

#include <cstddef>
#include <tuple>

#include "attribute_strict_warnings.h"

namespace
{
    SCL_UNSEQUENCED(int square(int value));

    SCL_UNSEQUENCED(int square(int value)) { return value * value; }

    SCL_REPRODUCIBLE(std::size_t count_zeros(int const * values, std::size_t size))
    {
        std::size_t zeros = 0;
        for (std::size_t index = 0; index < size; ++index)
            if (values[index] == 0)
                ++zeros;
        return zeros;
    }
} // namespace

TEST(AttributeUnsequenced, WrapsADeclarationAndADefinition) { EXPECT_EQ(square(7), 49); }

TEST(AttributeUnsequenced, KeepsTheSideEffectOfAnArgument)
{
    int counter = 0;
    std::ignore = square(++counter);
    EXPECT_EQ(counter, 1);
}

TEST(AttributeReproducible, WrapsAFunctionThatReadsThroughAPointer)
{
    int const values[] = {0, 3, 0, 5};
    EXPECT_EQ(count_zeros(values, 4), 2U);
}
