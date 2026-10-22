#include <gtest_utils.h>

// The base macros alone are defined before the header, each as its standard row, which carries no
// message.
#define SCL_DEPRECATED [[deprecated]]
#define SCL_NODISCARD [[nodiscard]]

#include <scl/utility/attribute.h>

#include "attribute_text.h"

TEST(AttributeReplacement, GivesTheMessageFormTheDefinitionOfTheBaseMacro)
{
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_DEPRECATED_MSG("m")), "[[[deprecated]]]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_NODISCARD_MSG("m")), "[[[nodiscard]]]");
}
