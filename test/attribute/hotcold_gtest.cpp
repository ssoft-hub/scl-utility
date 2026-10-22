#include <gtest_utils.h>

#include <scl/utility/attribute/hotcold.h>

#include "attribute_strict_warnings.h"

namespace
{
    SCL_HOT int often(int value) { return value + 1; }

    SCL_COLD int rarely(int value) { return value - 1; }
} // namespace

TEST(AttributeHot, StandsBeforeAFunctionDeclarationWithNoWarning) { EXPECT_EQ(often(1), 2); }

TEST(AttributeCold, StandsBeforeAFunctionDeclarationWithNoWarning) { EXPECT_EQ(rarely(1), 0); }
