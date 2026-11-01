#include <gtest_utils.h>

#include <scl/utility/type_traits/detection/operator.h>

#include <type_traits>

// ============================================================================
// TESTS - EXPLICIT OBJECT AND STATIC MEMBER OPERATORS
// ============================================================================

namespace
{
    struct implicit_operators
    {
        int operator+(long) const &;
        int operator()(long) const &;
        int operator[](long) const &;
        int operator++(int) &;
    };
} // namespace

TEST(OperatorDetectionTest, ExplicitAndStaticExactIgnoreImplicitMembers)
{
    STATIC_EXPECT_TRUE((::scl::has_plus_member_exact_v<implicit_operators const &, long>));
    STATIC_EXPECT_FALSE((::scl::has_plus_explicit_member_exact_v<implicit_operators const &, long>));
    STATIC_EXPECT_FALSE((::scl::has_call_explicit_member_exact_v<implicit_operators const &, long>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_explicit_member_exact_v<implicit_operators const &, long>));
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_explicit_member_exact_v<implicit_operators &>);
    STATIC_EXPECT_FALSE((::scl::has_call_static_member_v<implicit_operators, long>));
    STATIC_EXPECT_FALSE((::scl::has_call_static_member_exact_v<implicit_operators, long>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_static_member_v<implicit_operators, long>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_static_member_exact_v<implicit_operators, long>));
}

#ifdef __cpp_explicit_this_parameter
namespace
{
    struct explicit_operators
    {
        int operator+(this explicit_operators const &, long);
        int operator()(this explicit_operators const &, long);
        int operator[](this explicit_operators const &, long);
        int operator++(this explicit_operators &, int);
    };

    struct generic_explicit_operators
    {
        template <typename Self>
        int operator-(this Self &&, long);
    };
} // namespace

TEST(OperatorDetectionTest, ExplicitMemberExact)
{
    STATIC_EXPECT_TRUE((::scl::has_plus_explicit_member_exact_v<explicit_operators const &, long>));
    STATIC_EXPECT_FALSE((::scl::has_plus_explicit_member_exact_v<explicit_operators &, long>));
    STATIC_EXPECT_FALSE((::scl::has_plus_explicit_member_exact_v<explicit_operators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<explicit_operators const &, long>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::plus_explicit_member_exact_t<explicit_operators const &, long>, int>));

    STATIC_EXPECT_TRUE((::scl::has_call_explicit_member_exact_v<explicit_operators const &, long>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_explicit_member_exact_v<explicit_operators const &, long>));
    STATIC_EXPECT_FALSE((::scl::has_call_static_member_exact_v<explicit_operators const &, long>));

    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_explicit_member_exact_v<explicit_operators &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_explicit_member_exact_v<explicit_operators const &>);

    STATIC_EXPECT_TRUE((::scl::has_minus_explicit_member_exact_v<generic_explicit_operators &, long>));
    STATIC_EXPECT_TRUE(
        (::scl::has_minus_explicit_member_exact_v<generic_explicit_operators const &&, long>));
}
#endif

#if defined(__cpp_static_call_operator) && __cplusplus > 202002L
namespace
{
    struct static_call
    {
        static int operator()(long);
    };

    struct static_call_beside_implicit
    {
        static int operator()(static_call_beside_implicit const &, long);
        int operator()(long) const;
    };
} // namespace

TEST(OperatorDetectionTest, StaticCallMemberExact)
{
    STATIC_EXPECT_TRUE((::scl::has_call_static_member_v<static_call, long>));
    STATIC_EXPECT_TRUE((::scl::has_call_static_member_v<static_call, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_static_member_exact_v<static_call, long>));
    STATIC_EXPECT_TRUE((::scl::has_call_static_member_exact_v<static_call const &, long>));
    STATIC_EXPECT_FALSE((::scl::has_call_static_member_exact_v<static_call, int>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::call_static_member_exact_t<static_call, long>, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<static_call, long>));

    STATIC_EXPECT_TRUE((::scl::has_call_member_v<static_call_beside_implicit const &, long>));
    STATIC_EXPECT_FALSE(
        (::scl::has_call_explicit_member_exact_v<static_call_beside_implicit const &, long>));
}
#endif

#if defined(__cpp_multidimensional_subscript) && __cpp_multidimensional_subscript >= 202211L && \
    __cplusplus > 202002L
namespace
{
    struct static_subscript
    {
        static int operator[](long);
    };
} // namespace

TEST(OperatorDetectionTest, StaticSubscriptMemberExact)
{
    STATIC_EXPECT_TRUE((::scl::has_subscript_static_member_v<static_subscript, long>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_static_member_exact_v<static_subscript, long>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_static_member_exact_v<static_subscript, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_explicit_member_exact_v<static_subscript, long>));
}
#endif
