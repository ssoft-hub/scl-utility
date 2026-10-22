#include <gtest_utils.h>

#include <scl/utility/attribute/inline.h>

#include "attribute_strict_warnings.h"
#include "inline_function.h"

namespace
{
    SCL_FORCE_INLINE int twice(int value) { return value * 2; }

    SCL_NOINLINE int called(int value) { return value + 1; }

#if !defined(__GNUC__) || defined(__clang__)
    SCL_FORCE_INLINE int factorial(int value)
    {
        return value <= 1 ? 1 : value * factorial(value - 1);
    }
#endif
} // namespace

TEST(AttributeForceInline, StandsBeforeAFunctionDefinitionWithNoWarning) { EXPECT_EQ(twice(2), 4); }

TEST(AttributeForceInline, LetsEveryTranslationUnitThatIncludesAHeaderDefineItsFunction)
{
    EXPECT_EQ(twice_in_every_unit(2) + twice_from_the_other_unit(3), 10);
}

TEST(AttributeNoInline, StandsBeforeAFunctionDeclarationWithNoWarning) { EXPECT_EQ(called(1), 2); }

#if !defined(__GNUC__) || defined(__clang__)
TEST(AttributeForceInline, AcceptsARecursiveFunctionOnClangAndMsvc)
{
    EXPECT_EQ(factorial(5), 120);
}
#endif
