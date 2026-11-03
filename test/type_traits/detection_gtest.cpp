#include <gtest_utils.h>

#include <scl/utility/type_traits/detection.h>

#include <concepts>
#include <cstddef>
#include <type_traits>
#include <vector>

namespace
{
    struct with_value_type
    {
        using value_type = int;
    };

    struct without_value_type
    {};

    struct with_reset
    {
        void reset();
    };

    struct from_anything
    {
        template <typename Type>
        // NOLINTNEXTLINE(bugprone-forwarding-reference-overload)
        from_anything(Type &&);
    };

    struct only_explicit_from_int
    {
        explicit only_explicit_from_int(int);
    };

    template <typename Container>
    using value_type_operation = Container::value_type;

    template <typename Container>
    using size_operation = decltype(::std::declval<Container const &>().size());

    template <typename Container>
    using front_operation = decltype(::std::declval<Container &>().front());

    template <typename Object>
    using reset_operation = decltype(::std::declval<Object &>().reset());

    template <typename Type>
    struct ill_formed_body
    {
        using type = Type::missing;
    };

    template <typename Type>
        requires ::std::integral<Type>
    struct integral_only
    {};
} // namespace

TEST(TypeTraitsIsDetected, IsTrueTypeForAWellFormedOperation)
{
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::is_detected<size_operation, ::std::vector<int>>, ::std::true_type>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::is_detected<value_type_operation, with_value_type>, ::std::true_type>));
}

TEST(TypeTraitsIsDetected, IsFalseTypeForAnIllFormedOperation)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::is_detected<size_operation, int>, ::std::false_type>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::is_detected<value_type_operation, without_value_type>, ::std::false_type>));
}

TEST(TypeTraitsIsDetected, ValueIsTheValueOfIsDetected)
{
    STATIC_EXPECT_TRUE((::scl::is_detected_v<size_operation, ::std::vector<int>>));
    STATIC_EXPECT_FALSE((::scl::is_detected_v<size_operation, int>));
}

TEST(TypeTraitsIsDetected, ChecksNoBodyOfAClassTemplate)
{
    STATIC_EXPECT_TRUE((::scl::is_detected_v<ill_formed_body, int>));
}

TEST(TypeTraitsIsDetected, ChecksTheRequiresClauseOfAClassTemplate)
{
    STATIC_EXPECT_TRUE((::scl::is_detected_v<integral_only, int>));
    STATIC_EXPECT_FALSE((::scl::is_detected_v<integral_only, double>));
}

TEST(TypeTraitsDetectedT, IsTheTypeOfAWellFormedOperation)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::detected_t<value_type_operation, with_value_type>, int>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::detected_t<size_operation, ::std::vector<int>>, ::std::size_t>));
}

TEST(TypeTraitsDetectedT, KeepsTheReferenceOfTheExpression)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::detected_t<front_operation, ::std::vector<int>>, int &>));
}

TEST(TypeTraitsDetectedT, IsVoidForAWellFormedOperationOfTypeVoid)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::detected_t<reset_operation, with_reset>, void>));
}

TEST(TypeTraitsDetectedT, IsNonesuchForAnIllFormedOperation)
{
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::detected_t<value_type_operation, without_value_type>, ::scl::nonesuch>));
}

TEST(TypeTraitsNonesuch, HasNoObject)
{
    STATIC_EXPECT_FALSE(::std::is_default_constructible_v<::scl::nonesuch>);
    STATIC_EXPECT_FALSE(::std::is_destructible_v<::scl::nonesuch>);
    STATIC_EXPECT_FALSE(::std::is_copy_constructible_v<::scl::nonesuch>);
    STATIC_EXPECT_FALSE(::std::is_move_constructible_v<::scl::nonesuch>);
    STATIC_EXPECT_FALSE(::std::is_copy_assignable_v<::scl::nonesuch>);
    STATIC_EXPECT_FALSE(::std::is_move_assignable_v<::scl::nonesuch>);
}

TEST(TypeTraitsDetectedOr, TypeIsTheTypeOfAWellFormedOperation)
{
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::detected_or<long, value_type_operation, with_value_type>::type, int>));
}

TEST(TypeTraitsDetectedOr, TypeIsDefaultForAnIllFormedOperation)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::detected_or<long, value_type_operation, without_value_type>::type, long>));
}

TEST(TypeTraitsDetectedOr, ValueTIsIsDetectedOfTheOperation)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::detected_or<long, value_type_operation, with_value_type>::value_t,
        ::scl::is_detected<value_type_operation, with_value_type>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::detected_or<long, value_type_operation, without_value_type>::value_t,
        ::scl::is_detected<value_type_operation, without_value_type>>));
}

TEST(TypeTraitsDetectedOrT, IsTheTypeOfDetectedOr)
{
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::detected_or_t<long, value_type_operation, with_value_type>, int>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::detected_or_t<long, value_type_operation, without_value_type>, long>));
}

TEST(TypeTraitsIsDetectedExact, IsTrueTypeForExactlyTheExpectedType)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::is_detected_exact<int &, front_operation, ::std::vector<int>>, ::std::true_type>));
    STATIC_EXPECT_TRUE((::scl::is_detected_exact_v<int &, front_operation, ::std::vector<int>>));
}

TEST(TypeTraitsIsDetectedExact, IsFalseTypeForAnotherType)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::is_detected_exact<int, front_operation, ::std::vector<int>>, ::std::false_type>));
    STATIC_EXPECT_FALSE((::scl::is_detected_exact_v<int const &, front_operation, ::std::vector<int>>));
}

TEST(TypeTraitsIsDetectedExact, IsFalseTypeForAnIllFormedOperationWhateverTheExpectedType)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::is_detected_exact<int, value_type_operation, without_value_type>, ::std::false_type>));
    STATIC_EXPECT_FALSE(
        (::scl::is_detected_exact_v<::scl::nonesuch, value_type_operation, without_value_type>));
    STATIC_EXPECT_FALSE((::scl::is_detected_exact_v<void, reset_operation, without_value_type>));
}

TEST(TypeTraitsIsDetectedConvertible, IsTrueTypeForAnImplicitConversion)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::is_detected_convertible<double, value_type_operation, with_value_type>, ::std::true_type>));
    STATIC_EXPECT_TRUE((::scl::is_detected_convertible_v<int, value_type_operation, with_value_type>));
}

TEST(TypeTraitsIsDetectedConvertible, IsFalseTypeWithoutAnImplicitConversion)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::is_detected_convertible<only_explicit_from_int, value_type_operation, with_value_type>,
        ::std::false_type>));
    STATIC_EXPECT_FALSE((::scl::is_detected_convertible_v<int *, value_type_operation, with_value_type>));
}

TEST(TypeTraitsIsDetectedConvertible, IsFalseTypeForAnIllFormedOperationWhateverTheTargetType)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::is_detected_convertible<int, value_type_operation, without_value_type>, ::std::false_type>));
    STATIC_EXPECT_FALSE((::scl::is_detected_convertible_v<
        ::scl::nonesuch const &, value_type_operation, without_value_type>));
    STATIC_EXPECT_FALSE((
        ::scl::is_detected_convertible_v<::scl::nonesuch &&, value_type_operation, without_value_type>));
    STATIC_EXPECT_FALSE(
        (::scl::is_detected_convertible_v<from_anything, value_type_operation, without_value_type>));
    STATIC_EXPECT_FALSE((::scl::is_detected_convertible_v<void, reset_operation, without_value_type>));
}
