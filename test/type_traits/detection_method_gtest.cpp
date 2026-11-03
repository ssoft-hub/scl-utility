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
     * @brief Overloads a plain call cannot tell apart, and overloads beside a member template.
     */
    struct Overloads
    {
        long choose(int value) &;
        char choose(int && value) &;

        template <typename Type>
        Type echo(Type value) const;
        long echo(int value) const;

        static long shared(int value);
        static char shared(int && value);
    };

    /**
     * @brief Class inheriting the overloads.
     */
    struct DerivedOverloads : Overloads
    {};

    /**
     * @brief Class with a deleted and a private overload.
     */
    class Restricted
    {
    public:
        long retire(int value) & = delete;
        long retire(long value) &;

    private:
        long retire(short value) &;
    };

    /**
     * @brief Union with a member function.
     */
    union Variant
    {
        int number;

        int query(int x) & { return x; }
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
    SCL_METHOD_DETECTION(choose)
    SCL_METHOD_DETECTION(echo)
    SCL_METHOD_DETECTION(shared)
    SCL_METHOD_DETECTION(retire)

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

#if !defined(_MSC_VER) || defined(__clang__)
/**
 * @test Verify that the exact forms find each overload where the plain call is ambiguous.
 */
TEST(MethodDetectionTest, ExactBesideAmbiguousCall)
{
    STATIC_EXPECT_FALSE((has_choose_method_v<Overloads &, int>));
    STATIC_EXPECT_TRUE((has_choose_method_exact_v<Overloads &, int>));
    STATIC_EXPECT_TRUE((has_choose_method_exact_v<Overloads &, int &&>));
    STATIC_EXPECT_TRUE((has_choose_method_exact_v<DerivedOverloads &, int>));
    STATIC_EXPECT_FALSE((has_choose_method_exact_v<Overloads, int>));
    STATIC_EXPECT_TRUE((std::is_same_v<choose_method_exact_t<Overloads &, int>, long>));
    STATIC_EXPECT_TRUE((std::is_same_v<choose_method_exact_t<Overloads &, int &&>, char>));

    STATIC_EXPECT_FALSE((has_shared_static_method_v<Overloads, int>));
    STATIC_EXPECT_TRUE((has_shared_static_method_exact_v<Overloads, int>));
    STATIC_EXPECT_TRUE((std::is_same_v<shared_static_method_exact_t<Overloads, int &&>, char>));
}
#endif

/**
 * @test Verify the exact form for an overload set holding a member function template.
 */
TEST(MethodDetectionTest, ExactBesideTemplate)
{
    STATIC_EXPECT_TRUE((has_echo_method_exact_v<Overloads const, int>));
    STATIC_EXPECT_TRUE((std::is_same_v<echo_method_exact_t<Overloads const, int>, long>));
    STATIC_EXPECT_TRUE((has_echo_method_exact_v<Overloads const, double>));
    STATIC_EXPECT_FALSE((has_echo_method_exact_v<Overloads, int>));
    STATIC_EXPECT_TRUE((has_echo_method_exact_v<DerivedOverloads const, int>));
    STATIC_EXPECT_TRUE((has_echo_method_exact_v<DerivedOverloads const, double>));
}

/**
 * @test Verify that the exact form finds neither a deleted nor a private member function.
 */
TEST(MethodDetectionTest, ExactDeletedAndPrivate)
{
    STATIC_EXPECT_FALSE((has_retire_method_exact_v<Restricted &, int>));
    STATIC_EXPECT_TRUE((has_retire_method_exact_v<Restricted &, long>));
#if !defined(_MSC_VER) || defined(__clang__)
    STATIC_EXPECT_FALSE((has_retire_method_exact_v<Restricted &, short>));
#endif
}

/**
 * @test Verify that a void argument type gives false rather than a build error.
 */
TEST(MethodDetectionTest, ExactVoidArgument)
{
    STATIC_EXPECT_FALSE((has_choose_method_exact_v<Overloads &, void>));
    STATIC_EXPECT_FALSE((has_shared_static_method_exact_v<Overloads, void>));
    STATIC_EXPECT_FALSE((has_query_method_exact_v<Mock &, void>));
}

/**
 * @test Verify the exact form for a member function of a union.
 */
TEST(MethodDetectionTest, Union)
{
    STATIC_EXPECT_TRUE((has_query_method_v<Variant &, int>));
    STATIC_EXPECT_TRUE((has_query_method_exact_v<Variant &, int>));
    STATIC_EXPECT_FALSE((has_query_method_exact_v<Variant, int>));
}

namespace
{
    struct method_traits
    {
        SCL_METHOD_DETECTION(query)
    };
} // namespace

TEST(MethodDetectionTest, ClassScope)
{
    STATIC_EXPECT_TRUE((method_traits::has_query_method_v<Mock, int>));
    STATIC_EXPECT_TRUE((method_traits::has_query_method_exact_v<Mock, int>));
    STATIC_EXPECT_FALSE((method_traits::has_query_method_v<Mock const, int>));
}

namespace
{
    struct implicit_pick
    {
        int pick(long) const &;
    };

    struct static_pick
    {
        static int pick(static_pick const &, long);
    };

    struct static_beside_implicit_pick
    {
        static int pick(static_beside_implicit_pick const &, long);
        int pick(long) const;
    };

    SCL_METHOD_DETECTION(pick)
} // namespace

/**
 * @test Verify that the explicit object exact form finds no other kind of member function.
 */
TEST(MethodDetectionTest, ExplicitObjectExactIgnoresOtherKinds)
{
    STATIC_EXPECT_TRUE((has_pick_method_exact_v<implicit_pick const &, long>));
    STATIC_EXPECT_FALSE((has_pick_explicit_method_exact_v<implicit_pick const &, long>));
    STATIC_EXPECT_FALSE((has_pick_explicit_method_exact_v<static_pick const &, long>));
    STATIC_EXPECT_TRUE((has_pick_method_v<static_beside_implicit_pick const &, long>));
    STATIC_EXPECT_FALSE((has_pick_explicit_method_exact_v<static_beside_implicit_pick const &, long>));
}

#ifdef __cpp_explicit_this_parameter
namespace
{
    struct explicit_pick
    {
        int pick(this explicit_pick const &, long);
    };

    struct explicit_value_pick
    {
        int pick(this explicit_value_pick, long);
    };

    struct explicit_template_pick
    {
        template <typename Self>
        int pick(this Self &&, long);
    };

    struct derived_template_pick : explicit_template_pick
    {};

    struct explicit_overloaded_pick
    {
        int pick(this explicit_overloaded_pick const &, long);
        double pick(this explicit_overloaded_pick &, long);
    };
} // namespace

/**
 * @test Verify the exact form for a member function with an explicit object parameter.
 */
TEST(MethodDetectionTest, ExplicitObjectExact)
{
    STATIC_EXPECT_TRUE((has_pick_explicit_method_exact_v<explicit_pick const &, long>));
    STATIC_EXPECT_FALSE((has_pick_explicit_method_exact_v<explicit_pick &, long>));
    STATIC_EXPECT_FALSE((has_pick_explicit_method_exact_v<explicit_pick const &, int>));
    STATIC_EXPECT_FALSE((has_pick_method_exact_v<explicit_pick const &, long>));
    STATIC_EXPECT_TRUE((::std::is_same_v<pick_explicit_method_exact_t<explicit_pick const &, long>, int>));

    STATIC_EXPECT_TRUE((has_pick_explicit_method_exact_v<explicit_value_pick, long>));
    STATIC_EXPECT_TRUE((has_pick_explicit_method_exact_v<explicit_template_pick &, long>));
    STATIC_EXPECT_TRUE((has_pick_explicit_method_exact_v<explicit_template_pick const &&, long>));
    STATIC_EXPECT_TRUE((has_pick_explicit_method_exact_v<derived_template_pick const &, long>));

    STATIC_EXPECT_TRUE((
        ::std::is_same_v<pick_explicit_method_exact_t<explicit_overloaded_pick const &, long>, int>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<pick_explicit_method_exact_t<explicit_overloaded_pick &, long>, double>));
}
#endif
