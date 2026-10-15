#include <gtest_utils.h>

#include <scl/utility/preprocessor/exceptions.h>

#ifndef SCL_HAS_EXCEPTIONS
#error "SCL_HAS_EXCEPTIONS must always be defined, so that #if catches a misspelling"
#endif

/**
 * @test Verify that the macro answers as the test framework's own detection of exceptions does.
 */
TEST(PreprocessorExceptions, MatchesTheFrameworksOwnDetection)
{
    STATIC_EXPECT_EQ(SCL_HAS_EXCEPTIONS, GTEST_HAS_EXCEPTIONS);
}

#if SCL_HAS_EXCEPTIONS
/**
 * @test Verify that an enabled build catches a thrown object.
 */
TEST(PreprocessorExceptions, AnEnabledBuildCatchesAThrownObject)
{
    auto const raise = [] { throw 42; };

    EXPECT_THROW(raise(), int);
}
#endif
