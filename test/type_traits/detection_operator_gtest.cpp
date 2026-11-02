#include <gtest_utils.h>

#include <scl/utility/type_traits/detection/operator.h>

#include <compare>
#include <cstddef>
#include <type_traits>
#include <vector>

namespace operator_detection
{
    enum class op
    {
        address_of,
        indirection,
        unary_plus,
        negate,
        prefix_increment,
        prefix_decrement,
        bit_not,
        logical_not,
        postfix_increment,
        postfix_decrement,
        equal_to,
        not_equal_to,
        less,
        less_equal,
        greater,
        greater_equal,
        compare_three_way,
        multiplies,
        divides,
        modulus,
        plus,
        minus,
        left_shift,
        right_shift,
        bit_and,
        bit_or,
        bit_xor,
        logical_and,
        logical_or,
        assign,
        multiplies_assign,
        divides_assign,
        modulus_assign,
        plus_assign,
        minus_assign,
        left_shift_assign,
        right_shift_assign,
        bit_and_assign,
        bit_or_assign,
        bit_xor_assign,
        pointer_to_member_access,
        member_access,
        comma,
        subscript,
        call,
    };

    template <op Operator>
    struct tag
    {};

    struct members
    {
        tag<op::address_of> operator&() &;
        tag<op::indirection> operator*() &;
        tag<op::unary_plus> operator+() &;
        tag<op::negate> operator-() &;
        tag<op::prefix_increment> operator++() &;
        tag<op::prefix_decrement> operator--() &;
        tag<op::bit_not> operator~() &;
        tag<op::logical_not> operator!() &;
        tag<op::postfix_increment> operator++(int) &;
        tag<op::postfix_decrement> operator--(int) &;
        tag<op::equal_to> operator==(int) &;
        tag<op::not_equal_to> operator!=(int) &;
        tag<op::less> operator<(int) &;
        tag<op::less_equal> operator<=(int) &;
        tag<op::greater> operator>(int) &;
        tag<op::greater_equal> operator>=(int) &;
        tag<op::compare_three_way> operator<=>(int) &;
        tag<op::multiplies> operator*(int) &;
        tag<op::divides> operator/(int) &;
        tag<op::modulus> operator%(int) &;
        tag<op::plus> operator+(int) &;
        tag<op::minus> operator-(int) &;
        tag<op::left_shift> operator<<(int) &;
        tag<op::right_shift> operator>>(int) &;
        tag<op::bit_and> operator&(int) &;
        tag<op::bit_or> operator|(int) &;
        tag<op::bit_xor> operator^(int) &;
        tag<op::logical_and> operator&&(int) &;
        tag<op::logical_or> operator||(int) &;
        tag<op::assign> operator=(int) &;
        tag<op::multiplies_assign> operator*=(int) &;
        tag<op::divides_assign> operator/=(int) &;
        tag<op::modulus_assign> operator%=(int) &;
        tag<op::plus_assign> operator+=(int) &;
        tag<op::minus_assign> operator-=(int) &;
        tag<op::left_shift_assign> operator<<=(int) &;
        tag<op::right_shift_assign> operator>>=(int) &;
        tag<op::bit_and_assign> operator&=(int) &;
        tag<op::bit_or_assign> operator|=(int) &;
        tag<op::bit_xor_assign> operator^=(int) &;
        tag<op::pointer_to_member_access> operator->*(int) &;
        tag<op::member_access> operator->() &;
        tag<op::comma> operator,(int) &;
        tag<op::subscript> operator[](int) &;
        tag<op::call> operator()(int) &;
    };
} // namespace operator_detection

using operator_detection::members;
using operator_detection::op;
using operator_detection::tag;

// The free operators stand apart from every other operand type: Clang 18 to 20 crash on a postfix
// ++ or -- detection whose candidate set holds a deleted member operator and a free one at once.
namespace free_ops
{
    struct free_operand
    {};

    tag<op::address_of> operator&(free_operand operand);
    tag<op::indirection> operator*(free_operand operand);
    tag<op::unary_plus> operator+(free_operand operand);
    tag<op::negate> operator-(free_operand operand);
    tag<op::prefix_increment> operator++(free_operand operand);
    tag<op::prefix_decrement> operator--(free_operand operand);
    tag<op::bit_not> operator~(free_operand operand);
    tag<op::logical_not> operator!(free_operand operand);
    tag<op::postfix_increment> operator++(free_operand operand, int);
    tag<op::postfix_decrement> operator--(free_operand operand, int);
    tag<op::equal_to> operator==(free_operand left, int right);
    tag<op::not_equal_to> operator!=(free_operand left, int right);
    tag<op::less> operator<(free_operand left, int right);
    tag<op::less_equal> operator<=(free_operand left, int right);
    tag<op::greater> operator>(free_operand left, int right);
    tag<op::greater_equal> operator>=(free_operand left, int right);
    tag<op::compare_three_way> operator<=>(free_operand left, int right);
    tag<op::multiplies> operator*(free_operand left, int right);
    tag<op::divides> operator/(free_operand left, int right);
    tag<op::modulus> operator%(free_operand left, int right);
    tag<op::plus> operator+(free_operand left, int right);
    tag<op::minus> operator-(free_operand left, int right);
    tag<op::left_shift> operator<<(free_operand left, int right);
    tag<op::right_shift> operator>>(free_operand left, int right);
    tag<op::bit_and> operator&(free_operand left, int right);
    tag<op::bit_or> operator|(free_operand left, int right);
    tag<op::bit_xor> operator^(free_operand left, int right);
    tag<op::logical_and> operator&&(free_operand left, int right);
    tag<op::logical_or> operator||(free_operand left, int right);
    tag<op::multiplies_assign> operator*=(free_operand left, int right);
    tag<op::divides_assign> operator/=(free_operand left, int right);
    tag<op::modulus_assign> operator%=(free_operand left, int right);
    tag<op::plus_assign> operator+=(free_operand left, int right);
    tag<op::minus_assign> operator-=(free_operand left, int right);
    tag<op::left_shift_assign> operator<<=(free_operand left, int right);
    tag<op::right_shift_assign> operator>>=(free_operand left, int right);
    tag<op::bit_and_assign> operator&=(free_operand left, int right);
    tag<op::bit_or_assign> operator|=(free_operand left, int right);
    tag<op::bit_xor_assign> operator^=(free_operand left, int right);
    tag<op::pointer_to_member_access> operator->*(free_operand left, int right);
    tag<op::comma> operator,(free_operand left, int right);
} // namespace free_ops

namespace
{
    struct empty
    {};

    struct qualified
    {
        long operator+(int value) const;
        long operator-(int value) &;
        long operator*(int value) const &;
        long operator/(int value) &&;
    };

    struct derived : qualified
    {};

    struct private_derived : private qualified
    {
        using qualified::operator-;
    };

    struct protected_derived : protected qualified
    {
        using qualified::operator-;
    };

    class restricted
    {
    public:
        long operator+(long value) const;
        long operator+(int value) const = delete;

        long operator-(long value) const;

    private:
        long operator-(short value) const;
    };

    struct ambiguous
    {
        long operator+(int value) const;
        char operator+(int && value) const;
    };

    struct templated
    {
        template <typename Type>
        Type operator+(Type value) const;
        long operator+(int value) const;

        template <typename Type>
        Type operator-(Type value) const;
        long operator-(int value) const;
        char operator-(int && value) const;
    };

    struct calm
    {
        long operator+(int value) const noexcept;
    };

    struct counter
    {
        counter & operator++();
        counter operator++(int);
    };

    struct indexed
    {
        long operator[](int index) const;
        long operator()(int first, int second) const;
    };
} // namespace

TEST(TypeTraitsOperatorDetection, ExpressionFindsABuiltInOperator)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::plus_t<int, long>, long>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::negate_t<int>, int>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::subscript_t<int *, ::std::size_t>, int &>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::call_t<long (*)(int), int>, long>));
}

TEST(TypeTraitsOperatorDetection, ExpressionIsNonesuchWhereItDoesNotCompile)
{
    STATIC_EXPECT_FALSE((::scl::has_plus_v<empty, int>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::plus_t<empty, int>, ::scl::nonesuch>));
}

TEST(TypeTraitsOperatorDetection, ExpressionTakesTheObjectFirstForSubscriptAndCall)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::subscript_t<::std::vector<int> &, ::std::size_t>, int &>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::call_t<indexed const &, int, int>, long>));
}

TEST(TypeTraitsOperatorDetection, ExpressionTakesAnLvalueOnlyForAnLvalueReferenceType)
{
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<int &&>);
    STATIC_EXPECT_TRUE((::scl::has_assign_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_v<int, int>));
}

TEST(TypeTraitsOperatorDetection, MemberIgnoresABuiltInOperator)
{
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<int *, ::std::size_t>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<long (*)(int), int>));
}

TEST(TypeTraitsOperatorDetection, MemberConvertsTheArgumentsImplicitly)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::plus_member_t<qualified const, short>, long>));
}

TEST(TypeTraitsOperatorDetection, MemberAccessIgnoresABuiltInPointer)
{
    STATIC_EXPECT_FALSE(::scl::has_member_access_v<int *>);
    STATIC_EXPECT_FALSE(::scl::has_member_access_member_v<int *>);
}

TEST(TypeTraitsOperatorDetection, PointerToMemberAccessIgnoresABuiltInPointer)
{
    STATIC_EXPECT_FALSE((::scl::has_pointer_to_member_access_v<empty *, int empty::*>));
}

TEST(TypeTraitsOperatorDetection, PostfixMemberCallsTheOperatorWithADummyInt)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::postfix_increment_member_t<counter &>, counter>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::postfix_increment_member_exact_t<counter>, counter>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::prefix_increment_member_t<counter &>, counter &>));
}

TEST(TypeTraitsOperatorDetectionExact, TakesTheQualifiersFromTheType)
{
    STATIC_EXPECT_TRUE((::scl::has_plus_member_exact_v<qualified const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<qualified const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_exact_v<qualified &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<qualified &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_member_exact_v<qualified const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<qualified &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_member_exact_v<qualified &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<qualified, int>));
}

TEST(TypeTraitsOperatorDetectionExact, IgnoresTheTopLevelQualifiersOfTheArguments)
{
    STATIC_EXPECT_TRUE((::scl::has_plus_member_exact_v<qualified const, int const>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_exact_v<qualified const, int volatile>));
}

TEST(TypeTraitsOperatorDetectionExact, FindsAnOperatorOfAPublicBase)
{
    STATIC_EXPECT_TRUE((::scl::has_minus_member_exact_v<derived &, int>));
}

TEST(TypeTraitsOperatorDetectionExact, IsFalseForADeletedOperator)
{
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<restricted const, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_exact_v<restricted const, long>));
}

TEST(TypeTraitsOperatorDetectionExact, IsFalseForAnOperatorOfAPrivateOrProtectedBase)
{
    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<private_derived &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<private_derived &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<protected_derived &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<protected_derived &, int>));
}

#if !defined(_MSC_VER) || defined(__clang__)
TEST(TypeTraitsOperatorDetectionExact, IsFalseForAPrivateOperator)
{
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<restricted const, short>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_exact_v<restricted const, long>));
}

TEST(TypeTraitsOperatorDetectionExact, FindsEachOverloadBesideAnAmbiguousCall)
{
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<ambiguous const, int>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::plus_member_exact_t<ambiguous const, int>, long>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::plus_member_exact_t<ambiguous const, int &&>, char>));
}
#else
TEST(TypeTraitsOperatorDetectionExact, IsFalseBesideAnAmbiguousCallOnMsvc)
{
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<ambiguous const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<ambiguous const, int>));
}
#endif

TEST(TypeTraitsOperatorDetectionExact, TakesTheOverloadOfTheCallBesideAMemberFunctionTemplate)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::plus_member_exact_t<templated const, int>, long>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::plus_member_exact_t<templated const, double>, double>));
}

TEST(TypeTraitsOperatorDetectionExact, IsFalseBesideAMemberFunctionTemplateWhereTheCallIsAmbiguous)
{
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<templated const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<templated const, int>));
}

TEST(TypeTraitsOperatorDetectionExact, FindsANoexceptOperator)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::plus_member_exact_t<calm const, int>, long>));
}

TEST(TypeTraitsOperatorDetectionStatic, IsFalseForANonStaticOperator)
{
    STATIC_EXPECT_FALSE((::scl::has_call_static_member_v<indexed, int, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_static_member_exact_v<indexed, int, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_static_member_v<indexed, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_static_member_exact_v<indexed, int>));
}

TEST(TypeTraitsOperatorDetectionExplicit, IsFalseForAnOperatorWithAnImplicitObjectParameter)
{
    STATIC_EXPECT_TRUE((::scl::has_plus_member_exact_v<qualified const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_explicit_member_exact_v<qualified const, int>));
}

TEST(TypeTraitsOperatorDetection, AddressOfChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::address_of_t<members &>, tag<op::address_of>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::address_of_t<free_ops::free_operand>, tag<op::address_of>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::address_of_member_t<members &>, tag<op::address_of>>));
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<free_ops::free_operand>);
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::address_of_member_exact_t<members &>, tag<op::address_of>>));
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<members const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_explicit_member_exact_v<members &>);
}

TEST(TypeTraitsOperatorDetection, IndirectionChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::indirection_t<members &>, tag<op::indirection>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::indirection_t<free_ops::free_operand>, tag<op::indirection>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::indirection_member_t<members &>, tag<op::indirection>>));
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<free_ops::free_operand>);
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::indirection_member_exact_t<members &>, tag<op::indirection>>));
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<members const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_explicit_member_exact_v<members &>);
}

TEST(TypeTraitsOperatorDetection, UnaryPlusChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::unary_plus_t<members &>, tag<op::unary_plus>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::unary_plus_t<free_ops::free_operand>, tag<op::unary_plus>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::unary_plus_member_t<members &>, tag<op::unary_plus>>));
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<free_ops::free_operand>);
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::unary_plus_member_exact_t<members &>, tag<op::unary_plus>>));
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<members const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_explicit_member_exact_v<members &>);
}

TEST(TypeTraitsOperatorDetection, NegateChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::negate_t<members &>, tag<op::negate>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::negate_t<free_ops::free_operand>, tag<op::negate>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::negate_member_t<members &>, tag<op::negate>>));
    STATIC_EXPECT_FALSE(::scl::has_negate_member_v<free_ops::free_operand>);
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::negate_member_exact_t<members &>, tag<op::negate>>));
    STATIC_EXPECT_FALSE(::scl::has_negate_member_exact_v<members const &>);
    STATIC_EXPECT_FALSE(::scl::has_negate_explicit_member_exact_v<members &>);
}

TEST(TypeTraitsOperatorDetection, PrefixIncrementChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::prefix_increment_t<members &>, tag<op::prefix_increment>>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::prefix_increment_t<free_ops::free_operand>, tag<op::prefix_increment>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::prefix_increment_member_t<members &>, tag<op::prefix_increment>>));
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<free_ops::free_operand>);
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::prefix_increment_member_exact_t<members &>, tag<op::prefix_increment>>));
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<members const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_explicit_member_exact_v<members &>);
}

TEST(TypeTraitsOperatorDetection, PrefixDecrementChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::prefix_decrement_t<members &>, tag<op::prefix_decrement>>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::prefix_decrement_t<free_ops::free_operand>, tag<op::prefix_decrement>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::prefix_decrement_member_t<members &>, tag<op::prefix_decrement>>));
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<free_ops::free_operand>);
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::prefix_decrement_member_exact_t<members &>, tag<op::prefix_decrement>>));
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<members const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_explicit_member_exact_v<members &>);
}

TEST(TypeTraitsOperatorDetection, BitNotChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::bit_not_t<members &>, tag<op::bit_not>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::bit_not_t<free_ops::free_operand>, tag<op::bit_not>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::bit_not_member_t<members &>, tag<op::bit_not>>));
    STATIC_EXPECT_FALSE(::scl::has_bit_not_member_v<free_ops::free_operand>);
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::bit_not_member_exact_t<members &>, tag<op::bit_not>>));
    STATIC_EXPECT_FALSE(::scl::has_bit_not_member_exact_v<members const &>);
    STATIC_EXPECT_FALSE(::scl::has_bit_not_explicit_member_exact_v<members &>);
}

TEST(TypeTraitsOperatorDetection, LogicalNotChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::logical_not_t<members &>, tag<op::logical_not>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::logical_not_t<free_ops::free_operand>, tag<op::logical_not>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::logical_not_member_t<members &>, tag<op::logical_not>>));
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<free_ops::free_operand>);
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::logical_not_member_exact_t<members &>, tag<op::logical_not>>));
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<members const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_explicit_member_exact_v<members &>);
}

TEST(TypeTraitsOperatorDetection, PostfixIncrementChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::postfix_increment_t<members &>, tag<op::postfix_increment>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::postfix_increment_t<free_ops::free_operand>, tag<op::postfix_increment>>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::postfix_increment_member_t<members &>, tag<op::postfix_increment>>));
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<free_ops::free_operand>);
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::postfix_increment_member_exact_t<members &>, tag<op::postfix_increment>>));
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<members const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_explicit_member_exact_v<members &>);
}

TEST(TypeTraitsOperatorDetection, PostfixDecrementChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::postfix_decrement_t<members &>, tag<op::postfix_decrement>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::postfix_decrement_t<free_ops::free_operand>, tag<op::postfix_decrement>>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::postfix_decrement_member_t<members &>, tag<op::postfix_decrement>>));
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<free_ops::free_operand>);
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::postfix_decrement_member_exact_t<members &>, tag<op::postfix_decrement>>));
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<members const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_explicit_member_exact_v<members &>);
}

TEST(TypeTraitsOperatorDetection, EqualToChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::equal_to_t<members &, int>, tag<op::equal_to>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::equal_to_t<free_ops::free_operand, int>, tag<op::equal_to>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::equal_to_member_t<members &, int>, tag<op::equal_to>>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::equal_to_member_exact_t<members &, int>, tag<op::equal_to>>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, NotEqualToChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::not_equal_to_t<members &, int>, tag<op::not_equal_to>>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::not_equal_to_t<free_ops::free_operand, int>, tag<op::not_equal_to>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::not_equal_to_member_t<members &, int>, tag<op::not_equal_to>>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::not_equal_to_member_exact_t<members &, int>, tag<op::not_equal_to>>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, LessChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::less_t<members &, int>, tag<op::less>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::less_t<free_ops::free_operand, int>, tag<op::less>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::less_member_t<members &, int>, tag<op::less>>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::less_member_exact_t<members &, int>, tag<op::less>>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_less_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, LessEqualChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::less_equal_t<members &, int>, tag<op::less_equal>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::less_equal_t<free_ops::free_operand, int>, tag<op::less_equal>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::less_equal_member_t<members &, int>, tag<op::less_equal>>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::less_equal_member_exact_t<members &, int>, tag<op::less_equal>>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, GreaterChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::greater_t<members &, int>, tag<op::greater>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::greater_t<free_ops::free_operand, int>, tag<op::greater>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::greater_member_t<members &, int>, tag<op::greater>>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::greater_member_exact_t<members &, int>, tag<op::greater>>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_greater_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, GreaterEqualChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::greater_equal_t<members &, int>, tag<op::greater_equal>>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::greater_equal_t<free_ops::free_operand, int>, tag<op::greater_equal>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::greater_equal_member_t<members &, int>, tag<op::greater_equal>>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::greater_equal_member_exact_t<members &, int>, tag<op::greater_equal>>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, CompareThreeWayChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::compare_three_way_t<members &, int>, tag<op::compare_three_way>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::compare_three_way_t<free_ops::free_operand, int>, tag<op::compare_three_way>>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::compare_three_way_member_t<members &, int>, tag<op::compare_three_way>>));
    STATIC_EXPECT_FALSE((::scl::has_compare_three_way_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::compare_three_way_member_exact_t<members &, int>, tag<op::compare_three_way>>));
    STATIC_EXPECT_FALSE((::scl::has_compare_three_way_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_compare_three_way_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, MultipliesChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::multiplies_t<members &, int>, tag<op::multiplies>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::multiplies_t<free_ops::free_operand, int>, tag<op::multiplies>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::multiplies_member_t<members &, int>, tag<op::multiplies>>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::multiplies_member_exact_t<members &, int>, tag<op::multiplies>>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, DividesChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::divides_t<members &, int>, tag<op::divides>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::divides_t<free_ops::free_operand, int>, tag<op::divides>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::divides_member_t<members &, int>, tag<op::divides>>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::divides_member_exact_t<members &, int>, tag<op::divides>>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_divides_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, ModulusChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::modulus_t<members &, int>, tag<op::modulus>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::modulus_t<free_ops::free_operand, int>, tag<op::modulus>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::modulus_member_t<members &, int>, tag<op::modulus>>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::modulus_member_exact_t<members &, int>, tag<op::modulus>>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, PlusChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::plus_t<members &, int>, tag<op::plus>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::plus_t<free_ops::free_operand, int>, tag<op::plus>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::plus_member_t<members &, int>, tag<op::plus>>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::plus_member_exact_t<members &, int>, tag<op::plus>>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_plus_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, MinusChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::minus_t<members &, int>, tag<op::minus>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::minus_t<free_ops::free_operand, int>, tag<op::minus>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::minus_member_t<members &, int>, tag<op::minus>>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::minus_member_exact_t<members &, int>, tag<op::minus>>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_minus_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, LeftShiftChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::left_shift_t<members &, int>, tag<op::left_shift>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::left_shift_t<free_ops::free_operand, int>, tag<op::left_shift>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::left_shift_member_t<members &, int>, tag<op::left_shift>>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::left_shift_member_exact_t<members &, int>, tag<op::left_shift>>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, RightShiftChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::right_shift_t<members &, int>, tag<op::right_shift>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::right_shift_t<free_ops::free_operand, int>, tag<op::right_shift>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::right_shift_member_t<members &, int>, tag<op::right_shift>>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::right_shift_member_exact_t<members &, int>, tag<op::right_shift>>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, BitAndChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::bit_and_t<members &, int>, tag<op::bit_and>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::bit_and_t<free_ops::free_operand, int>, tag<op::bit_and>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::bit_and_member_t<members &, int>, tag<op::bit_and>>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::bit_and_member_exact_t<members &, int>, tag<op::bit_and>>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, BitOrChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::bit_or_t<members &, int>, tag<op::bit_or>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::bit_or_t<free_ops::free_operand, int>, tag<op::bit_or>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::bit_or_member_t<members &, int>, tag<op::bit_or>>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::bit_or_member_exact_t<members &, int>, tag<op::bit_or>>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, BitXorChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::bit_xor_t<members &, int>, tag<op::bit_xor>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::bit_xor_t<free_ops::free_operand, int>, tag<op::bit_xor>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::bit_xor_member_t<members &, int>, tag<op::bit_xor>>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::bit_xor_member_exact_t<members &, int>, tag<op::bit_xor>>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, LogicalAndChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::logical_and_t<members &, int>, tag<op::logical_and>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::logical_and_t<free_ops::free_operand, int>, tag<op::logical_and>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::logical_and_member_t<members &, int>, tag<op::logical_and>>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::logical_and_member_exact_t<members &, int>, tag<op::logical_and>>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, LogicalOrChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::logical_or_t<members &, int>, tag<op::logical_or>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::logical_or_t<free_ops::free_operand, int>, tag<op::logical_or>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::logical_or_member_t<members &, int>, tag<op::logical_or>>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::logical_or_member_exact_t<members &, int>, tag<op::logical_or>>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, AssignChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::assign_t<members &, int>, tag<op::assign>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::assign_member_t<members &, int>, tag<op::assign>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::assign_member_exact_t<members &, int>, tag<op::assign>>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_assign_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, MultipliesAssignChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::multiplies_assign_t<members &, int>, tag<op::multiplies_assign>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::multiplies_assign_t<free_ops::free_operand, int>, tag<op::multiplies_assign>>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::multiplies_assign_member_t<members &, int>, tag<op::multiplies_assign>>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::multiplies_assign_member_exact_t<members &, int>, tag<op::multiplies_assign>>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, DividesAssignChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::divides_assign_t<members &, int>, tag<op::divides_assign>>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::divides_assign_t<free_ops::free_operand, int>, tag<op::divides_assign>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::divides_assign_member_t<members &, int>, tag<op::divides_assign>>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::divides_assign_member_exact_t<members &, int>, tag<op::divides_assign>>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, ModulusAssignChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::modulus_assign_t<members &, int>, tag<op::modulus_assign>>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::modulus_assign_t<free_ops::free_operand, int>, tag<op::modulus_assign>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::modulus_assign_member_t<members &, int>, tag<op::modulus_assign>>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::modulus_assign_member_exact_t<members &, int>, tag<op::modulus_assign>>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, PlusAssignChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::plus_assign_t<members &, int>, tag<op::plus_assign>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::plus_assign_t<free_ops::free_operand, int>, tag<op::plus_assign>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::plus_assign_member_t<members &, int>, tag<op::plus_assign>>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::plus_assign_member_exact_t<members &, int>, tag<op::plus_assign>>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, MinusAssignChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::minus_assign_t<members &, int>, tag<op::minus_assign>>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::minus_assign_t<free_ops::free_operand, int>, tag<op::minus_assign>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::minus_assign_member_t<members &, int>, tag<op::minus_assign>>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::minus_assign_member_exact_t<members &, int>, tag<op::minus_assign>>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, LeftShiftAssignChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::left_shift_assign_t<members &, int>, tag<op::left_shift_assign>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::left_shift_assign_t<free_ops::free_operand, int>, tag<op::left_shift_assign>>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::left_shift_assign_member_t<members &, int>, tag<op::left_shift_assign>>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::left_shift_assign_member_exact_t<members &, int>, tag<op::left_shift_assign>>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, RightShiftAssignChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::right_shift_assign_t<members &, int>, tag<op::right_shift_assign>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::right_shift_assign_t<free_ops::free_operand, int>, tag<op::right_shift_assign>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::right_shift_assign_member_t<members &, int>, tag<op::right_shift_assign>>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::right_shift_assign_member_exact_t<members &, int>, tag<op::right_shift_assign>>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, BitAndAssignChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::bit_and_assign_t<members &, int>, tag<op::bit_and_assign>>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::bit_and_assign_t<free_ops::free_operand, int>, tag<op::bit_and_assign>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::bit_and_assign_member_t<members &, int>, tag<op::bit_and_assign>>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_assign_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::bit_and_assign_member_exact_t<members &, int>, tag<op::bit_and_assign>>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_assign_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_assign_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, BitOrAssignChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::bit_or_assign_t<members &, int>, tag<op::bit_or_assign>>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::bit_or_assign_t<free_ops::free_operand, int>, tag<op::bit_or_assign>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::bit_or_assign_member_t<members &, int>, tag<op::bit_or_assign>>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_assign_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::bit_or_assign_member_exact_t<members &, int>, tag<op::bit_or_assign>>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_assign_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_assign_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, BitXorAssignChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::bit_xor_assign_t<members &, int>, tag<op::bit_xor_assign>>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::bit_xor_assign_t<free_ops::free_operand, int>, tag<op::bit_xor_assign>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::bit_xor_assign_member_t<members &, int>, tag<op::bit_xor_assign>>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_assign_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::bit_xor_assign_member_exact_t<members &, int>, tag<op::bit_xor_assign>>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_assign_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_assign_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, PointerToMemberAccessChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::pointer_to_member_access_t<members &, int>, tag<op::pointer_to_member_access>>));
    STATIC_EXPECT_FALSE((::scl::has_pointer_to_member_access_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::pointer_to_member_access_member_t<members &, int>, tag<op::pointer_to_member_access>>));
    STATIC_EXPECT_FALSE((::scl::has_pointer_to_member_access_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::pointer_to_member_access_member_exact_t<members &, int>, tag<op::pointer_to_member_access>>));
    STATIC_EXPECT_FALSE((::scl::has_pointer_to_member_access_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_pointer_to_member_access_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, MemberAccessChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_access_t<members &>, tag<op::member_access>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::member_access_member_t<members &>, tag<op::member_access>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::member_access_member_exact_t<members &>, tag<op::member_access>>));
    STATIC_EXPECT_FALSE(::scl::has_member_access_member_exact_v<members const &>);
    STATIC_EXPECT_FALSE(::scl::has_member_access_explicit_member_exact_v<members &>);
}

TEST(TypeTraitsOperatorDetection, CommaChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::comma_t<members &, int>, tag<op::comma>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::comma_t<free_ops::free_operand, int>, tag<op::comma>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::comma_member_t<members &, int>, tag<op::comma>>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<free_ops::free_operand, int>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::comma_member_exact_t<members &, int>, tag<op::comma>>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_comma_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, SubscriptChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::subscript_t<members &, int>, tag<op::subscript>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::subscript_member_t<members &, int>, tag<op::subscript>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::subscript_member_exact_t<members &, int>, tag<op::subscript>>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_explicit_member_exact_v<members &, int>));
}

TEST(TypeTraitsOperatorDetection, CallChecksItsOwnOperatorInEveryForm)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::call_t<members &, int>, tag<op::call>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::call_member_t<members &, int>, tag<op::call>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::call_member_exact_t<members &, int>, tag<op::call>>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<members &, long>));
    STATIC_EXPECT_FALSE((::scl::has_call_explicit_member_exact_v<members &, int>));
}

#ifdef __cpp_explicit_this_parameter
namespace operator_detection
{
    struct explicit_members
    {
        tag<op::address_of> operator&(this explicit_members & self);
        tag<op::indirection> operator*(this explicit_members & self);
        tag<op::unary_plus> operator+(this explicit_members & self);
        tag<op::negate> operator-(this explicit_members & self);
        tag<op::prefix_increment> operator++(this explicit_members & self);
        tag<op::prefix_decrement> operator--(this explicit_members & self);
        tag<op::bit_not> operator~(this explicit_members & self);
        tag<op::logical_not> operator!(this explicit_members & self);
        tag<op::postfix_increment> operator++(this explicit_members & self, int value);
        tag<op::postfix_decrement> operator--(this explicit_members & self, int value);
        tag<op::equal_to> operator==(this explicit_members & self, int value);
        tag<op::not_equal_to> operator!=(this explicit_members & self, int value);
        tag<op::less> operator<(this explicit_members & self, int value);
        tag<op::less_equal> operator<=(this explicit_members & self, int value);
        tag<op::greater> operator>(this explicit_members & self, int value);
        tag<op::greater_equal> operator>=(this explicit_members & self, int value);
        tag<op::compare_three_way> operator<=>(this explicit_members & self, int value);
        tag<op::multiplies> operator*(this explicit_members & self, int value);
        tag<op::divides> operator/(this explicit_members & self, int value);
        tag<op::modulus> operator%(this explicit_members & self, int value);
        tag<op::plus> operator+(this explicit_members & self, int value);
        tag<op::minus> operator-(this explicit_members & self, int value);
        tag<op::left_shift> operator<<(this explicit_members & self, int value);
        tag<op::right_shift> operator>>(this explicit_members & self, int value);
        tag<op::bit_and> operator&(this explicit_members & self, int value);
        tag<op::bit_or> operator|(this explicit_members & self, int value);
        tag<op::bit_xor> operator^(this explicit_members & self, int value);
        tag<op::logical_and> operator&&(this explicit_members & self, int value);
        tag<op::logical_or> operator||(this explicit_members & self, int value);
        tag<op::assign> operator=(this explicit_members & self, int value);
        tag<op::multiplies_assign> operator*=(this explicit_members & self, int value);
        tag<op::divides_assign> operator/=(this explicit_members & self, int value);
        tag<op::modulus_assign> operator%=(this explicit_members & self, int value);
        tag<op::plus_assign> operator+=(this explicit_members & self, int value);
        tag<op::minus_assign> operator-=(this explicit_members & self, int value);
        tag<op::left_shift_assign> operator<<=(this explicit_members & self, int value);
        tag<op::right_shift_assign> operator>>=(this explicit_members & self, int value);
        tag<op::bit_and_assign> operator&=(this explicit_members & self, int value);
        tag<op::bit_or_assign> operator|=(this explicit_members & self, int value);
        tag<op::bit_xor_assign> operator^=(this explicit_members & self, int value);
        tag<op::pointer_to_member_access> operator->*(this explicit_members & self, int value);
        tag<op::member_access> operator->(this explicit_members & self);
        tag<op::comma> operator,(this explicit_members & self, int value);
        tag<op::subscript> operator[](this explicit_members & self, int value);
        tag<op::call> operator()(this explicit_members & self, int value);
    };

    struct explicit_plus
    {
        int operator+(this explicit_plus const & self, long value);
    };

    struct generic_explicit_plus
    {
        template <typename Self>
        int operator+(this Self && self, long value);
    };

    struct deleted_explicit_plus
    {
        int operator+(this deleted_explicit_plus const & self, long value) = delete;
    };

    struct explicit_call
    {
        int operator()(this explicit_call const & self, long value);
        int operator[](this explicit_call const & self, long value);
    };
} // namespace operator_detection

using operator_detection::deleted_explicit_plus;
using operator_detection::explicit_call;
using operator_detection::explicit_members;
using operator_detection::explicit_plus;
using operator_detection::generic_explicit_plus;

TEST(TypeTraitsOperatorDetectionExplicit, ChecksEachOperatorWithAnExplicitObjectParameter)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::address_of_explicit_member_exact_t<explicit_members &>, tag<op::address_of>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::indirection_explicit_member_exact_t<explicit_members &>, tag<op::indirection>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::unary_plus_explicit_member_exact_t<explicit_members &>, tag<op::unary_plus>>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::negate_explicit_member_exact_t<explicit_members &>, tag<op::negate>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::prefix_increment_explicit_member_exact_t<explicit_members &>, tag<op::prefix_increment>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::prefix_decrement_explicit_member_exact_t<explicit_members &>, tag<op::prefix_decrement>>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::bit_not_explicit_member_exact_t<explicit_members &>, tag<op::bit_not>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::logical_not_explicit_member_exact_t<explicit_members &>, tag<op::logical_not>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::postfix_increment_explicit_member_exact_t<explicit_members &>, tag<op::postfix_increment>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::postfix_decrement_explicit_member_exact_t<explicit_members &>, tag<op::postfix_decrement>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::equal_to_explicit_member_exact_t<explicit_members &, int>, tag<op::equal_to>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::not_equal_to_explicit_member_exact_t<explicit_members &, int>, tag<op::not_equal_to>>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::less_explicit_member_exact_t<explicit_members &, int>, tag<op::less>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::less_equal_explicit_member_exact_t<explicit_members &, int>, tag<op::less_equal>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::greater_explicit_member_exact_t<explicit_members &, int>, tag<op::greater>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::greater_equal_explicit_member_exact_t<explicit_members &, int>, tag<op::greater_equal>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::compare_three_way_explicit_member_exact_t<explicit_members &, int>,
        tag<op::compare_three_way>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::multiplies_explicit_member_exact_t<explicit_members &, int>, tag<op::multiplies>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::divides_explicit_member_exact_t<explicit_members &, int>, tag<op::divides>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::modulus_explicit_member_exact_t<explicit_members &, int>, tag<op::modulus>>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::plus_explicit_member_exact_t<explicit_members &, int>, tag<op::plus>>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::minus_explicit_member_exact_t<explicit_members &, int>, tag<op::minus>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::left_shift_explicit_member_exact_t<explicit_members &, int>, tag<op::left_shift>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::right_shift_explicit_member_exact_t<explicit_members &, int>, tag<op::right_shift>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::bit_and_explicit_member_exact_t<explicit_members &, int>, tag<op::bit_and>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::bit_or_explicit_member_exact_t<explicit_members &, int>, tag<op::bit_or>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::bit_xor_explicit_member_exact_t<explicit_members &, int>, tag<op::bit_xor>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::logical_and_explicit_member_exact_t<explicit_members &, int>, tag<op::logical_and>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::logical_or_explicit_member_exact_t<explicit_members &, int>, tag<op::logical_or>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::assign_explicit_member_exact_t<explicit_members &, int>, tag<op::assign>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::multiplies_assign_explicit_member_exact_t<explicit_members &, int>,
        tag<op::multiplies_assign>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::divides_assign_explicit_member_exact_t<explicit_members &, int>, tag<op::divides_assign>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::modulus_assign_explicit_member_exact_t<explicit_members &, int>, tag<op::modulus_assign>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::plus_assign_explicit_member_exact_t<explicit_members &, int>, tag<op::plus_assign>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::minus_assign_explicit_member_exact_t<explicit_members &, int>, tag<op::minus_assign>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::left_shift_assign_explicit_member_exact_t<explicit_members &, int>,
        tag<op::left_shift_assign>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::right_shift_assign_explicit_member_exact_t<explicit_members &, int>,
        tag<op::right_shift_assign>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::bit_and_assign_explicit_member_exact_t<explicit_members &, int>, tag<op::bit_and_assign>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::bit_or_assign_explicit_member_exact_t<explicit_members &, int>, tag<op::bit_or_assign>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::bit_xor_assign_explicit_member_exact_t<explicit_members &, int>, tag<op::bit_xor_assign>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::pointer_to_member_access_explicit_member_exact_t<explicit_members &, int>,
        tag<op::pointer_to_member_access>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::member_access_explicit_member_exact_t<explicit_members &>, tag<op::member_access>>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::comma_explicit_member_exact_t<explicit_members &, int>, tag<op::comma>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::subscript_explicit_member_exact_t<explicit_members &, int>, tag<op::subscript>>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::call_explicit_member_exact_t<explicit_members &, int>, tag<op::call>>));
}

TEST(TypeTraitsOperatorDetectionExplicit, IsTrueForExactlyTheTypeOfTheObjectParameter)
{
    STATIC_EXPECT_TRUE((::scl::has_plus_explicit_member_exact_v<explicit_plus const &, long>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<explicit_plus &, long>));
    STATIC_EXPECT_FALSE((::scl::has_plus_explicit_member_exact_v<explicit_plus &, long>));
    STATIC_EXPECT_FALSE((::scl::has_plus_explicit_member_exact_v<explicit_plus const &, int>));
}

TEST(TypeTraitsOperatorDetectionExplicit, DeducesTheObjectParameterOfATemplate)
{
    STATIC_EXPECT_TRUE((::scl::has_plus_explicit_member_exact_v<generic_explicit_plus &, long>));
    STATIC_EXPECT_TRUE((::scl::has_plus_explicit_member_exact_v<generic_explicit_plus const &&, long>));
}

TEST(TypeTraitsOperatorDetectionExplicit, IsFalseForADeletedOperator)
{
    STATIC_EXPECT_FALSE((::scl::has_plus_explicit_member_exact_v<deleted_explicit_plus const &, long>));
}

TEST(TypeTraitsOperatorDetectionExact, IsFalseForAnOperatorWithAnExplicitObjectParameter)
{
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<explicit_plus const &, long>));
}

#if !defined(_MSC_VER) || defined(__clang__)
TEST(TypeTraitsOperatorDetectionStatic, ExactIsTrueForAnExplicitObjectParameterAmongTheArguments)
{
    STATIC_EXPECT_FALSE((::scl::has_call_static_member_v<explicit_call, explicit_call const &, long>));
    STATIC_EXPECT_TRUE((::scl::has_call_static_member_exact_v<explicit_call, explicit_call const &, long>));
    STATIC_EXPECT_TRUE(
        (::scl::has_subscript_static_member_exact_v<explicit_call, explicit_call const &, long>));
}
#else
TEST(TypeTraitsOperatorDetectionStatic, ExactIsFalseForAnExplicitObjectParameterOnMsvc)
{
    STATIC_EXPECT_FALSE(
        (::scl::has_call_static_member_exact_v<explicit_call, explicit_call const &, long>));
    STATIC_EXPECT_FALSE(
        (::scl::has_subscript_static_member_exact_v<explicit_call, explicit_call const &, long>));
}
#endif
#endif

#if defined(__cpp_static_call_operator) && \
    (__cplusplus > 202002L || (defined(_MSVC_LANG) && _MSVC_LANG > 202002L))
namespace operator_detection
{
    struct static_call
    {
        static int operator()(long value);
    };

    struct static_call_beside_implicit
    {
        static int operator()(static_call_beside_implicit const & self, long value);
        int operator()(long value) const;
    };
} // namespace operator_detection

using operator_detection::static_call;
using operator_detection::static_call_beside_implicit;

TEST(TypeTraitsOperatorDetectionStatic, CallIsTrueWhereTheCallThroughTheClassNameCompiles)
{
    STATIC_EXPECT_TRUE((::scl::has_call_static_member_v<static_call, int>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::call_static_member_t<static_call, long>, int>));
}

TEST(TypeTraitsOperatorDetectionStatic, CallExactIsTrueForExactlyTheParameterTypes)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::call_static_member_exact_t<static_call, long>, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_static_member_exact_v<static_call, int>));
}

TEST(TypeTraitsOperatorDetectionStatic, IgnoresTheQualifiersOfTheType)
{
    STATIC_EXPECT_TRUE((::scl::has_call_static_member_exact_v<static_call const &, long>));
}

TEST(TypeTraitsOperatorDetectionExact, IsFalseForAStaticOperator)
{
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<static_call, long>));
}

TEST(TypeTraitsOperatorDetectionExplicit, IsFalseBesideAStaticCallOperatorTakingTheObject)
{
    STATIC_EXPECT_TRUE((::scl::has_call_member_v<static_call_beside_implicit const &, long>));
    STATIC_EXPECT_FALSE(
        (::scl::has_call_explicit_member_exact_v<static_call_beside_implicit const &, long>));
}
#endif

#if defined(__cpp_static_call_operator) && defined(__cpp_multidimensional_subscript) && \
    __cpp_multidimensional_subscript >= 202211L &&                                      \
    (__cplusplus > 202002L || (defined(_MSVC_LANG) && _MSVC_LANG > 202002L))
namespace operator_detection
{
    struct static_subscript
    {
        static int operator[](long value);
    };
} // namespace operator_detection

using operator_detection::static_subscript;

TEST(TypeTraitsOperatorDetectionStatic, SubscriptExactIsTrueForExactlyTheParameterTypes)
{
    STATIC_EXPECT_TRUE((::scl::has_subscript_static_member_v<static_subscript, int>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::subscript_static_member_exact_t<static_subscript, long>, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_static_member_exact_v<static_subscript, int>));
}
#endif

#if defined(__cpp_multidimensional_subscript) && __cpp_multidimensional_subscript >= 202110L
namespace operator_detection
{
    struct grid
    {
        int operator[](int row, int column) const;
    };
} // namespace operator_detection

using operator_detection::grid;

TEST(TypeTraitsOperatorDetection, MemberChecksASubscriptWithSeveralIndices)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::subscript_member_t<grid const &, int, int>, int>));
}
#endif
