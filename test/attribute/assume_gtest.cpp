#include <gtest_utils.h>

#if defined(__GNUC__)
#pragma GCC diagnostic error "-Wpedantic"
#endif

#include <scl/utility/attribute/assume.h>

namespace
{
    int positive(int value)
    {
        SCL_ASSUME(value > 0);
        return value;
    }
} // namespace

TEST(AttributeAssume, ExpandsWithNoCpp23ExtensionWarning) { EXPECT_EQ(positive(3), 3); }
