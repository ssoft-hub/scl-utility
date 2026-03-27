#include <gtest_utils.h>

#include <scl/utility/type_traits/detection/function.h>

namespace
{
    void void_func() {}
    int multiply(int a, int b) { return a * b; }

    SCL_FUNCTION_DETECTION(void_func)
    SCL_FUNCTION_DETECTION(multiply)
} // namespace

/**
 * @test Verify detection of global functions with correct arguments.
 */
TEST(FunctionDetectionTest, PositiveCases)
{
    // No arguments
    STATIC_EXPECT_TRUE(has_void_func_function_v<>);
    STATIC_EXPECT_TRUE(has_void_func_function_exact_v<>);

    // Multiple arguments
    STATIC_EXPECT_TRUE((has_multiply_function_v<int, int>));
    STATIC_EXPECT_TRUE((has_multiply_function_v<double, double>));
    STATIC_EXPECT_TRUE((has_multiply_function_exact_v<int, int>));

    // Type verification
    using RetType = multiply_function_t<int, int>;
    static_assert(std::is_same_v<RetType, int>);
}

/**
 * @test Verify that traits return false for incompatible arguments (SFINAE).
 */
TEST(FunctionDetectionTest, NegativeCases)
{
    // Wrong number of arguments
    STATIC_EXPECT_FALSE(has_void_func_function_v<int>);
    STATIC_EXPECT_FALSE(has_multiply_function_v<int>);

    // Incompatible argument types
    STATIC_EXPECT_FALSE((has_multiply_function_v<int, char const *>));

    // Exact detection should also fail for mismatched types
    STATIC_EXPECT_FALSE((has_multiply_function_exact_v<double, double>));
    STATIC_EXPECT_FALSE(has_void_func_function_exact_v<double>);
}

TEST(FunctionDetectionTest, DetectionBeforeDefinition)
{
    // Found via marker
    STATIC_EXPECT_TRUE((has_multiply_function_exact_v<int, int>));
    STATIC_EXPECT_FALSE((has_multiply_function_exact_v<double, double>));
}

int multiply(double a, double b) { return static_cast<int>(a * b); }

namespace test
{
    SCL_FUNCTION_DETECTION(multiply)
} // namespace test

TEST(FunctionDetectionTest, DetectionAfterDefinition)
{
    // Found via AdlType marker
    STATIC_EXPECT_TRUE((test::has_multiply_function_exact_v<int, int>));
    STATIC_EXPECT_TRUE((test::has_multiply_function_exact_v<double, double>));
}
