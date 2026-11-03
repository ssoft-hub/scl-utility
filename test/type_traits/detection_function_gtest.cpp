#include <gtest_utils.h>

#include <scl/utility/type_traits/detection/function.h>

namespace
{
    void void_func() {}
    int multiply(int a, int b) { return a * b; }

    SCL_FUNCTION_DETECTION(void_func)
    SCL_FUNCTION_DETECTION(multiply)
} // namespace

namespace overloads
{
    long pick(int value);
    char pick(int && value);

    template <typename Type>
    Type echo(Type value);
    long echo(int value);

    long settle(int value) noexcept;
    char settle(int && value);

    long retire(int value) = delete;
    long retire(long value);

    SCL_FUNCTION_DETECTION(pick)
    SCL_FUNCTION_DETECTION(echo)
    SCL_FUNCTION_DETECTION(settle)
    SCL_FUNCTION_DETECTION(retire)
} // namespace overloads

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

#if !defined(_MSC_VER) || defined(__clang__)
/**
 * @test Verify that the exact form finds each overload where the plain call is ambiguous.
 */
TEST(FunctionDetectionTest, ExactBesideAmbiguousCall)
{
    STATIC_EXPECT_FALSE(overloads::has_pick_function_v<int>);
    STATIC_EXPECT_TRUE(overloads::has_pick_function_exact_v<int>);
    STATIC_EXPECT_TRUE(overloads::has_pick_function_exact_v<int &&>);
    STATIC_EXPECT_FALSE(overloads::has_pick_function_exact_v<long>);
    STATIC_EXPECT_FALSE(overloads::has_pick_function_exact_v<int &>);
    STATIC_EXPECT_TRUE((std::is_same_v<overloads::pick_function_exact_t<int>, long>));
    STATIC_EXPECT_TRUE((std::is_same_v<overloads::pick_function_exact_t<int &&>, char>));
}
#endif

/**
 * @test Verify the exact form for an overload set holding a function template.
 */
TEST(FunctionDetectionTest, ExactBesideTemplate)
{
    STATIC_EXPECT_TRUE(overloads::has_echo_function_exact_v<int>);
    STATIC_EXPECT_TRUE((std::is_same_v<overloads::echo_function_exact_t<int>, long>));
    STATIC_EXPECT_TRUE(overloads::has_echo_function_exact_v<double>);
}

#if !defined(_MSC_VER) || defined(__clang__)
/**
 * @test Verify the exact form for a noexcept overload beside an ambiguous plain call.
 */
TEST(FunctionDetectionTest, ExactNoexceptBesideAmbiguousCall)
{
    STATIC_EXPECT_TRUE(overloads::has_settle_function_exact_v<int>);
    STATIC_EXPECT_TRUE(overloads::has_settle_function_exact_v<int &&>);
    STATIC_EXPECT_TRUE((std::is_same_v<overloads::settle_function_exact_t<int>, long>));
}
#endif

/**
 * @test Verify that the exact form does not find a deleted function.
 */
TEST(FunctionDetectionTest, ExactDeleted)
{
    STATIC_EXPECT_FALSE(overloads::has_retire_function_exact_v<int>);
    STATIC_EXPECT_TRUE(overloads::has_retire_function_exact_v<long>);
}

/**
 * @test Verify that a void argument type gives false rather than a build error.
 */
TEST(FunctionDetectionTest, ExactVoidArgument)
{
    STATIC_EXPECT_FALSE(overloads::has_pick_function_exact_v<void>);
    STATIC_EXPECT_FALSE(has_void_func_function_exact_v<void>);
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

namespace
{
    struct function_traits
    {
        SCL_FUNCTION_DETECTION(multiply)
    };
} // namespace

TEST(FunctionDetectionTest, ClassScope)
{
    STATIC_EXPECT_TRUE((function_traits::has_multiply_function_v<int, int>));
    STATIC_EXPECT_TRUE((function_traits::has_multiply_function_exact_v<int, int>));
    STATIC_EXPECT_FALSE((function_traits::has_multiply_function_exact_v<double, double>));
    STATIC_EXPECT_FALSE(function_traits::has_multiply_function_v<int>);
    STATIC_EXPECT_TRUE((::std::is_same_v<function_traits::multiply_function_t<int, int>, int>));
}
