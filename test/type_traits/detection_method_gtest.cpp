#include <gtest_utils.h>

#include <scl/utility/type_traits/detection/method.h>

namespace
{

    /**
     * @brief Mock class with various method signatures for testing.
     */
    struct Mock
    {
        void simple() {}
        int query(int x) { return x; }

        // Const vs Non-const
        void only_const() const {}
        void only_non_const() {}

        // Ref-qualifiers
        void lvalue_only() & {}
        void rvalue_only() && {}

        static void foo() {}
    };

    /**
     * @brief Class with no methods.
     */
    struct Empty
    {};

    // Generate traits for testing
    SCL_METHOD_DETECTION(simple)
    SCL_METHOD_DETECTION(query)
    SCL_METHOD_DETECTION(only_const)
    SCL_METHOD_DETECTION(only_non_const)
    SCL_METHOD_DETECTION(lvalue_only)
    SCL_METHOD_DETECTION(rvalue_only)
    SCL_METHOD_DETECTION(non_existent)
    SCL_METHOD_DETECTION(foo)

} // namespace

/**
 * @test Verify basic method existence and return type detection.
 */
TEST(MethodDetectionTest, BasicFunctionality)
{
    // Check simple presence
    STATIC_EXPECT_TRUE(has_simple_method_v<Mock>);
    STATIC_EXPECT_TRUE(has_simple_method_exact_v<Mock>);
    STATIC_EXPECT_FALSE(has_non_existent_method_v<Mock>);

    // Check with arguments and return types
    STATIC_EXPECT_TRUE((has_query_method_v<Mock, int>));
    STATIC_EXPECT_TRUE((has_query_method_exact_v<Mock, int>));
    STATIC_EXPECT_TRUE((has_query_method_v<Mock, double>));
    STATIC_EXPECT_FALSE((has_query_method_exact_v<Mock, double>));

    STATIC_EXPECT_TRUE((std::is_same_v<query_method_t<Mock, int>, int>));

    // Invalid arguments
    STATIC_EXPECT_FALSE((has_query_method_v<Mock, char const *>));
    STATIC_EXPECT_FALSE((has_query_method_exact_v<Mock, char const *>));
}

/**
 * @test Verify behavior with const qualifiers.
 */
TEST(MethodDetectionTest, ConstQualifiers)
{
    // only_const() can be called on both
    STATIC_EXPECT_TRUE(has_only_const_method_v<Mock>);
    STATIC_EXPECT_TRUE(has_only_const_method_v<Mock const>);

    // only_non_const() cannot be called on const object
    STATIC_EXPECT_TRUE(has_only_non_const_method_v<Mock>);
    STATIC_EXPECT_FALSE(has_only_non_const_method_v<Mock const>);

    // Exact detection should follow the same rules
    STATIC_EXPECT_TRUE(has_only_const_method_exact_v<Mock const>);
    STATIC_EXPECT_FALSE(has_only_non_const_method_exact_v<Mock const>);
}

/**
 * @test Verify behavior with ref-qualifiers (lvalue vs rvalue).
 */
TEST(MethodDetectionTest, RefQualifiers)
{
    // lvalue_only() &
    STATIC_EXPECT_TRUE(has_lvalue_only_method_v<Mock &>);
    STATIC_EXPECT_TRUE(has_lvalue_only_method_exact_v<Mock &>);
    STATIC_EXPECT_FALSE(has_lvalue_only_method_exact_v<Mock &&>);

    // rvalue_only() &&
    STATIC_EXPECT_TRUE(has_rvalue_only_method_v<Mock &&>);
    STATIC_EXPECT_TRUE(has_rvalue_only_method_exact_v<Mock &&>);
    STATIC_EXPECT_FALSE(has_rvalue_only_method_exact_v<Mock &>);
}

/**
 * @test Verify behavior with static.
 */
TEST(MethodDetectionTest, Static)
{
    // foo() can be called on any object
    STATIC_EXPECT_TRUE(has_foo_static_method_v<Mock>);
    STATIC_EXPECT_TRUE(has_foo_static_method_exact_v<Mock>);
    STATIC_EXPECT_TRUE(has_foo_static_method_v<Mock const>);
    STATIC_EXPECT_TRUE(has_foo_static_method_exact_v<Mock &&>);

    // foo() cannot be non static member
    STATIC_EXPECT_TRUE(has_foo_method_v<Mock>);
    STATIC_EXPECT_FALSE(has_foo_method_exact_v<Mock>);
    STATIC_EXPECT_TRUE(has_foo_method_v<Mock const>);
    STATIC_EXPECT_FALSE(has_foo_method_exact_v<Mock &&>);

    // lvalue_only() is not a static method
    STATIC_EXPECT_FALSE(has_lvalue_only_static_method_v<Mock>);
    STATIC_EXPECT_FALSE(has_lvalue_only_static_method_exact_v<Mock>);
}
