#include <gtest_utils.h>

#include <scl/utility/type_traits/detection.h>
#include <scl/utility/type_traits/member_like.h>

#include <type_traits>

namespace
{
    struct gauge
    {
        int label() &;
        long label() const &;
        char label() &&;
    };

    union variant
    {
        int number;
    };

    template <typename Pointer>
    concept casts_to = requires { static_cast<Pointer>(&gauge::label); };

    template <typename Type, typename Signature>
    using member_function_like_operation = ::scl::member_function_like_t<Type, Signature>;
} // namespace

TEST(TypeTraitsMemberFunctionLikeT, TakesTheCvQualifiersFromTheType)
{
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::member_function_like_t<gauge, void()>, void (gauge::*)()>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::member_function_like_t<gauge const, void()>, void (gauge::*)() const>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::member_function_like_t<gauge volatile, void()>, void (gauge::*)() volatile>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::member_function_like_t<gauge const volatile, void()>, void (gauge::*)() const volatile>));
}

TEST(TypeTraitsMemberFunctionLikeT, TakesTheRefQualifierFromTheReferenceOfTheType)
{
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::member_function_like_t<gauge &, void()>, void (gauge::*)() &>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::member_function_like_t<gauge const &, void()>, void (gauge::*)() const &>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::member_function_like_t<gauge volatile &, void()>, void (gauge::*)() volatile &>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::member_function_like_t<gauge const volatile &, void()>, void (gauge::*)() const volatile &>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::member_function_like_t<gauge &&, void()>, void (gauge::*)() &&>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::member_function_like_t<gauge const &&, void()>, void (gauge::*)() const &&>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::member_function_like_t<gauge volatile &&, void()>, void (gauge::*)() volatile &&>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::member_function_like_t<gauge const volatile &&, void()>,
        void (gauge::*)() const volatile &&>));
}

TEST(TypeTraitsMemberFunctionLikeT, KeepsTheNoexceptOfTheSignature)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::member_function_like_t<gauge, void() noexcept>, void (gauge::*)() noexcept>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::member_function_like_t<gauge const, void() noexcept>, void (gauge::*)() const noexcept>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::member_function_like_t<gauge volatile &, void() noexcept>,
        void (gauge::*)() volatile & noexcept>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v < ::scl::member_function_like_t<gauge const volatile &&, void() noexcept>,
        void (gauge::*)() const volatile && noexcept >));
}

TEST(TypeTraitsMemberFunctionLikeT, KeepsTheResultAndTheParametersOfTheSignature)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::member_function_like_t<gauge const &, int(short)>, int (gauge::*)(short) const &>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v < ::scl::member_function_like_t<gauge volatile &&, long(double, char) noexcept>,
        long (gauge::*)(double, char) volatile && noexcept >));
}

TEST(TypeTraitsMemberFunctionLikeT, AcceptsAUnion)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::member_function_like_t<variant const &, int()>, int (variant::*)() const &>));
}

TEST(TypeTraitsMemberFunctionLikeT, ChoosesTheOverloadOfTheQualifiersOfTheType)
{
    STATIC_EXPECT_TRUE((casts_to<::scl::member_function_like_t<gauge const &, long()>>));
    STATIC_EXPECT_TRUE((casts_to<::scl::member_function_like_t<gauge &&, char()>>));
    STATIC_EXPECT_FALSE((casts_to<::scl::member_function_like_t<gauge const, long()>>));
}

TEST(TypeTraitsMemberFunctionLikeT, FailsSubstitutionForAnEllipsisOrAQualifiedFunctionType)
{
    STATIC_EXPECT_TRUE((::scl::is_detected_v<member_function_like_operation, gauge, void(int)>));
    STATIC_EXPECT_FALSE((::scl::is_detected_v<member_function_like_operation, gauge, void(int, ...)>));
    STATIC_EXPECT_FALSE((::scl::is_detected_v<member_function_like_operation, gauge, void() const>));
    STATIC_EXPECT_FALSE((::scl::is_detected_v<member_function_like_operation, gauge, void() &>));
}

TEST(TypeTraitsMemberObjectLikeT, AddsTheCvQualifiersOfTheType)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_object_like_t<gauge, int>, int gauge::*>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::member_object_like_t<gauge const, int>, int const gauge::*>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::member_object_like_t<gauge volatile, int>, int volatile gauge::*>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::member_object_like_t<gauge const volatile, int>, int const volatile gauge::*>));
}

TEST(TypeTraitsMemberObjectLikeT, IgnoresTheReferenceOfTheType)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_object_like_t<gauge &, int>, int gauge::*>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::member_object_like_t<gauge const &, int>, int const gauge::*>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::member_object_like_t<gauge volatile &&, int>, int volatile gauge::*>));
}

TEST(TypeTraitsMemberObjectLikeT, KeepsTheCvQualifiersOfTheMember)
{
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::member_object_like_t<gauge, int const>, int const gauge::*>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::member_object_like_t<gauge volatile, int const>, int const volatile gauge::*>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::member_object_like_t<gauge const, int const>, int const gauge::*>));
}

TEST(TypeTraitsMemberObjectLikeT, AcceptsAUnion)
{
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::member_object_like_t<variant const &, int>, int const variant::*>));
}

TEST(TypeTraitsMemberLikeT, IsMemberFunctionLikeTForAFunctionType)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::member_like_t<gauge const &, int(short)>, int (gauge::*)(short) const &>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v < ::scl::member_like_t<gauge &&, void() noexcept>,
        void (gauge::*)() && noexcept >));
}

TEST(TypeTraitsMemberLikeT, IsMemberObjectLikeTForAnObjectType)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_like_t<gauge const &, int>, int const gauge::*>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::member_like_t<gauge, int volatile>, int volatile gauge::*>));
}

TEST(TypeTraitsMemberLikeT, PointsToADataMemberForAPointerToAFunction)
{
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::member_like_t<gauge, void (*)(int)>, void (*gauge::*)(int)>));
}
