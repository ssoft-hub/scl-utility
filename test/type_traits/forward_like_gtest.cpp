#include <gtest_utils.h>

#include <scl/utility/type_traits/forward_like.h>

#include <type_traits>
#include <utility>

using ::scl::forward_like_t;

TEST(TypeTraitsForwardLikeT, AddsTheCvQualifiersOfTheBaseToThoseOfTheType)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double>, double &&>));
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, double>, double const &&>));
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, double>, double volatile &&>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<forward_like_t<int const volatile, double>, double const volatile &&>));

    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double const>, double const &&>));
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, double const>, double const &&>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<forward_like_t<int volatile, double const>, double const volatile &&>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<forward_like_t<int const volatile, double const>, double const volatile &&>));

    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double volatile>, double volatile &&>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<forward_like_t<int const, double volatile>, double const volatile &&>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<forward_like_t<int volatile, double volatile>, double volatile &&>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<forward_like_t<int const volatile, double volatile>, double const volatile &&>));

    STATIC_EXPECT_TRUE(
        (::std::is_same_v<forward_like_t<int, double const volatile>, double const volatile &&>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<forward_like_t<int const, double const volatile>, double const volatile &&>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<forward_like_t<int volatile, double const volatile>, double const volatile &&>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        forward_like_t<int const volatile, double const volatile>, double const volatile &&>));
}

TEST(TypeTraitsForwardLikeT, TakesTheKindOfReferenceFromTheBase)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, double>, double &>));
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, double>, double const &>));
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, double>, double volatile &>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<forward_like_t<int const volatile &, double>, double const volatile &>));
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, double>, double &&>));
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, double>, double const &&>));
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, double>, double volatile &&>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<forward_like_t<int const volatile &&, double>, double const volatile &&>));
}

TEST(TypeTraitsForwardLikeT, IgnoresTheReferenceOfTheType)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double &>, double &&>));
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double &&>, double &&>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<forward_like_t<int const, double volatile &>, double const volatile &&>));
}

TEST(TypeTraitsForwardLikeT, TakesTheReferenceOfTheBaseOverThatOfTheType)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, double &&>, double &>));
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, double &>, double &&>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<forward_like_t<int const &, double volatile &&>, double const volatile &>));
}

TEST(TypeTraitsForwardLikeT, IsVoidWithTheCvQualifiersOfBothTypesForVoid)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, void>, void>));
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, void const>, void const>));
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, void volatile>, void volatile>));
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, void const volatile>, void const volatile>));

    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, void>, void const>));
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, void const>, void const>));
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, void volatile>, void const volatile>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<forward_like_t<int const, void const volatile>, void const volatile>));

    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, void>, void volatile>));
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, void const>, void const volatile>));
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, void volatile>, void volatile>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<forward_like_t<int volatile, void const volatile>, void const volatile>));

    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, void>, void const volatile>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<forward_like_t<int const volatile, void const>, void const volatile>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<forward_like_t<int const volatile, void volatile>, void const volatile>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<forward_like_t<int const volatile, void const volatile>, void const volatile>));

    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, void>, void>));
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, void const>, void const>));
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, void volatile>, void volatile>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<forward_like_t<int &&, void const volatile>, void const volatile>));

    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, void>, void const>));
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, void const>, void const>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<forward_like_t<int const &&, void volatile>, void const volatile>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<forward_like_t<int const &&, void const volatile>, void const volatile>));

    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, void>, void volatile>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<forward_like_t<int volatile &&, void const>, void const volatile>));
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, void volatile>, void volatile>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<forward_like_t<int volatile &&, void const volatile>, void const volatile>));

    STATIC_EXPECT_TRUE(
        (::std::is_same_v<forward_like_t<int const volatile &&, void>, void const volatile>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<forward_like_t<int const volatile &&, void const>, void const volatile>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<forward_like_t<int const volatile &&, void volatile>, void const volatile>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        forward_like_t<int const volatile &&, void const volatile>, void const volatile>));

    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, void>, void>));
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, void const>, void const>));
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, void volatile>, void volatile>));
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, void const volatile>, void const volatile>));

    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, void>, void const>));
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, void const>, void const>));
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, void volatile>, void const volatile>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<forward_like_t<int const &, void const volatile>, void const volatile>));

    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, void>, void volatile>));
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, void const>, void const volatile>));
    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, void volatile>, void volatile>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<forward_like_t<int volatile &, void const volatile>, void const volatile>));

    STATIC_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, void>, void const volatile>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<forward_like_t<int const volatile &, void const>, void const volatile>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<forward_like_t<int const volatile &, void volatile>, void const volatile>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        forward_like_t<int const volatile &, void const volatile>, void const volatile>));
}

namespace
{
    constexpr int stored = 7;
} // namespace

TEST(TypeTraitsForwardLike, ReturnsTheArgumentAsForwardLikeT)
{
    int value = 1;
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(::scl::forward_like<int const &>(value)), int const &>));
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(::scl::forward_like<int>(value)), int &&>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<decltype(::scl::forward_like<int &>(::std::move(value))), int &>));
}

TEST(TypeTraitsForwardLike, ReturnsAReferenceToTheSameObject)
{
    int value = 1;
    int && moved = ::scl::forward_like<int>(value);
    EXPECT_EQ(&moved, &value);
}

TEST(TypeTraitsForwardLike, IsAConstantExpression)
{
    STATIC_EXPECT_EQ(&::scl::forward_like<int const &>(stored), &stored);
}

TEST(TypeTraitsForwardLike, IsNoexcept)
{
    int value = 1;
    STATIC_EXPECT_TRUE(noexcept(::scl::forward_like<int>(value)));
}
