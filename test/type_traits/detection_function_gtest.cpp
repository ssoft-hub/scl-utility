#include <gtest/gtest.h>

#include <scl/utility/type_traits/detection/function.h>

#define TEST_EXPECT_TRUE(X) \
    static_assert(X, #X);   \
    EXPECT_TRUE(X);

#define TEST_EXPECT_FALSE(X) \
    static_assert(!X, #X);   \
    EXPECT_FALSE(X);

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
    TEST_EXPECT_TRUE(has_void_func_function_v<>);
    TEST_EXPECT_TRUE(has_void_func_function_exact_v<>);

    // Multiple arguments
    TEST_EXPECT_TRUE((has_multiply_function_v<int, int>));
    TEST_EXPECT_TRUE((has_multiply_function_v<double, double>));
    TEST_EXPECT_TRUE((has_multiply_function_exact_v<int, int>));

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
    TEST_EXPECT_FALSE(has_void_func_function_v<int>);
    TEST_EXPECT_FALSE(has_multiply_function_v<int>);

    // Incompatible argument types
    TEST_EXPECT_FALSE((has_multiply_function_v<int, char const *>));

    // Exact detection should also fail for mismatched types
    TEST_EXPECT_FALSE((has_multiply_function_exact_v<double, double>));
    TEST_EXPECT_FALSE(has_void_func_function_exact_v<double>);
}

TEST(FunctionDetectionTest, DetectionBeforeDefinition)
{
    // Found via marker
    TEST_EXPECT_TRUE((has_multiply_function_exact_v<int, int>));
    TEST_EXPECT_FALSE((has_multiply_function_exact_v<double, double>));
}

int multiply(double a, double b) { return a * b; }

namespace test
{
    SCL_FUNCTION_DETECTION(multiply)
} // namespace test

TEST(FunctionDetectionTest, DetectionAfterDefinition)
{
    // Found via AdlType marker
    TEST_EXPECT_TRUE((test::has_multiply_function_exact_v<int, int>));
    TEST_EXPECT_TRUE((test::has_multiply_function_exact_v<double, double>));
}
