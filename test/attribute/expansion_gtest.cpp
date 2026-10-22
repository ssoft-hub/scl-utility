#include <gtest_utils.h>

#include <scl/utility/attribute.h>

#include "attribute_text.h"

#if defined(_MSC_VER) && !defined(__clang__)
TEST(AttributeExpansion, TakesTheRowsOfMsvc)
{
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_LIFETIMEBOUND), "[[[msvc::lifetimebound]]]");
}
#endif
