#include <gtest_utils.h>

// The base macros alone are defined before the header, each as its standard row, which carries no
// message.
#define SCL_DEPRECATED [[deprecated]]
#define SCL_NODISCARD [[nodiscard]]

#include <scl/utility/attribute.h>

#include "attribute_text.h"

namespace
{
    [[maybe_unused]] SCL_DEPRECATED_MSG("use kept") int retired();

    SCL_NODISCARD_MSG("the value is the result") int kept() { return 1; }
} // namespace

TEST(AttributeReplacement, GivesTheMessageFormTheDefinitionOfTheBaseMacro)
{
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_DEPRECATED_MSG("m")), "[[[deprecated]]]");
    EXPECT_STREQ(SCL_TEST_TEXT(SCL_NODISCARD_MSG("m")), "[[[nodiscard]]]");
}

TEST(AttributeReplacement, StandsInAPlaceInTheMessageFormUnderTheDefinitionOfTheBaseMacro)
{
    EXPECT_EQ(kept(), 1);
}
