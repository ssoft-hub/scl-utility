#include <gtest_utils.h>

#include <scl/utility/preprocessor/rtti.h>

#ifndef SCL_HAS_RTTI
#error "SCL_HAS_RTTI must always be defined, so that #if catches a misspelling"
#endif

/**
 * @test Verify that the macro answers as the test framework's own detection of RTTI does.
 */
TEST(PreprocessorRtti, MatchesTheFrameworksOwnDetection)
{
    STATIC_EXPECT_EQ(SCL_HAS_RTTI, GTEST_HAS_RTTI);
}

#if SCL_HAS_RTTI

#include <typeinfo>

namespace
{
    struct Base
    {
        virtual ~Base() = default;
    };

    struct Derived : Base
    {};
} // namespace

/**
 * @test Verify that an enabled build tells the dynamic type of an object.
 */
TEST(PreprocessorRtti, AnEnabledBuildTellsTheDynamicType)
{
    Derived derived;
    Base const & base = derived;
    EXPECT_TRUE(typeid(base) == typeid(Derived));
    EXPECT_NE(dynamic_cast<Derived const *>(&base), nullptr);
}

#endif
