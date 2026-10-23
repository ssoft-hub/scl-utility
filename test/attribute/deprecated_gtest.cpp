#include <gtest_utils.h>

#include <scl/utility/attribute/deprecated.h>

#include "attribute_strict_warnings.h"

namespace
{
    SCL_DEPRECATED [[maybe_unused]]
    int old_function()
    {
        return 1;
    }

    SCL_DEPRECATED_MSG("use current_function instead") [[maybe_unused]]
    int old_function_with_message()
    {
        return 2;
    }

    SCL_DEPRECATED [[maybe_unused]]
    int const old_constant = 4;

    struct SCL_DEPRECATED old_type
    {};

    enum class SCL_DEPRECATED old_enumeration
    {
        value
    };

    struct SCL_DEPRECATED_MSG("use current_type instead") old_type_with_message
    {};

    int current_function() { return 3; }
} // namespace

TEST(AttributeDeprecated, DrawsNoWarningAtTheDeclarationOfAFunctionAVariableOrAType)
{
    EXPECT_EQ(current_function(), 3);
}
