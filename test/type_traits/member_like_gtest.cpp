#include <gtest_utils.h>

#include <scl/utility/type_traits/member_like.h>

namespace
{
    struct X
    {
        int a = 0;
        int const b = 1;

        // Methods for invocation tests
        int f0(int v) { return v; }
        int f1(int v) const { return v; }
        int f2(int v) & { return v; }
        int f3(int v) const & { return v; }
        int f4(int v) && { return v; }
        int f5(int v) const && noexcept { return v; }
    };

} // namespace

TEST(MemberFunctionLike, SameAs_NoRefQualifierNoExcept)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_function_like_t<X, void()>, void (X::*)()>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_function_like_t<X const, void()>, void (X::*)() const>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::member_function_like_t<X volatile, void()>, void (X::*)() volatile>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_function_like_t<X const volatile, void()>,
        void (X::*)() const volatile>));
}

TEST(MemberFunctionLike, SameAs_LValueQualifiedNoExcept)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_function_like_t<X &, void()>, void (X::*)() &>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::member_function_like_t<X const &, void()>, void (X::*)() const &>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_function_like_t<X volatile &, void()>,
        void (X::*)() volatile &>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_function_like_t<X const volatile &, void()>,
        void (X::*)() const volatile &>));
}

TEST(MemberFunctionLike, SameAs_RValueQualifiedNoExcept)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_function_like_t<X &&, void()>, void (X::*)() &&>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::member_function_like_t<X const &&, void()>, void (X::*)() const &&>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_function_like_t<X volatile &&, void()>,
        void (X::*)() volatile &&>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_function_like_t<X const volatile &&, void()>,
        void (X::*)() const volatile &&>));
}

TEST(MemberFunctionLike, SameAs_NoRefQualifierWithExcept)
{
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::member_function_like_t<X, void() noexcept>, void (X::*)() noexcept>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_function_like_t<X const, void() noexcept>,
        void (X::*)() const noexcept>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_function_like_t<X volatile, void() noexcept>,
        void (X::*)() volatile noexcept>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_function_like_t<X const volatile, void() noexcept>,
        void (X::*)() const volatile noexcept>));
}

TEST(MemberFunctionLike, SameAs_LValueQualifiedWithExcept)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_function_like_t<X &, void() noexcept>,
        void (X::*)() & noexcept>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_function_like_t<X const &, void() noexcept>,
        void (X::*)() const & noexcept>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_function_like_t<X volatile &, void() noexcept>,
        void (X::*)() volatile & noexcept>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_function_like_t<X const volatile &, void() noexcept>,
        void (X::*)() const volatile & noexcept>));
}

TEST(MemberFunctionLike, SameAs_RValueQualifiedWithExcept)
{
    STATIC_EXPECT_TRUE((::std::is_same_v < ::scl::member_function_like_t<X &&, void() noexcept>,
        void (X::*)() && noexcept >));
    STATIC_EXPECT_TRUE((::std::is_same_v < ::scl::member_function_like_t<X const &&, void() noexcept>,
        void (X::*)() const && noexcept >));
    STATIC_EXPECT_TRUE((::std::is_same_v < ::scl::member_function_like_t<X volatile &&, void() noexcept>,
        void (X::*)() volatile && noexcept >));
    STATIC_EXPECT_TRUE((::std::is_same_v < ::scl::member_function_like_t<X const volatile &&, void() noexcept>,
        void (X::*)() const volatile && noexcept >));
}

TEST(MemberFunctionLike, SameAs_AnyQualifiedWithArguments)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_function_like_t<X const &, int(short)>,
        int (X::*)(short) const &>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_function_like_t<X volatile &&, void(double)>,
        void (X::*)(double) volatile &&>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_function_like_t<X const volatile, void(int) noexcept>,
        void (X::*)(int) const volatile noexcept>));
}

TEST(MemberPropertyLike, Qualifiers_DoNotAffectThePointerType)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_property_like_t<X, int>, int X::*>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_property_like_t<X const, int>, int const X::*>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_property_like_t<X volatile, int>, int volatile X::*>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_property_like_t<X const volatile, int>,
        int const volatile X::*>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_property_like_t<X &, int>, int X::*>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_property_like_t<X const &, int>, int const X::*>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::member_property_like_t<X volatile &, int>, int volatile X::*>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_property_like_t<X const volatile &, int>,
        int const volatile X::*>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_property_like_t<X &&, int>, int X::*>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_property_like_t<X const &&, int>, int const X::*>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::member_property_like_t<X volatile &&, int>, int volatile X::*>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_property_like_t<X const volatile &&, int>,
        int const volatile X::*>));
}

TEST(MemberPropertyLike, Qualifiers_OfTheMemberArePreserved)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_property_like_t<X, int const>, int const X::*>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_property_like_t<X, int volatile>, int volatile X::*>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_property_like_t<X, int const volatile>,
        int const volatile X::*>));
}

TEST(MemberLike, Function_NoRefQualifierNoExcept)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_like_t<X, void()>, void (X::*)()>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_like_t<X const, void()>, void (X::*)() const>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_like_t<X volatile, void()>, void (X::*)() volatile>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_like_t<X const volatile, void()>,
        void (X::*)() const volatile>));
}

TEST(MemberLike, Function_LValueQualifiedNoExcept)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_like_t<X &, void()>, void (X::*)() &>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_like_t<X const &, void()>, void (X::*)() const &>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::member_like_t<X volatile &, void()>, void (X::*)() volatile &>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_like_t<X const volatile &, void()>,
        void (X::*)() const volatile &>));
}

TEST(MemberLike, Function_RValueQualifiedNoExcept)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_like_t<X &&, void()>, void (X::*)() &&>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_like_t<X const &&, void()>, void (X::*)() const &&>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::member_like_t<X volatile &&, void()>, void (X::*)() volatile &&>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_like_t<X const volatile &&, void()>,
        void (X::*)() const volatile &&>));
}

TEST(MemberLike, Function_NoRefQualifierWithExcept)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_like_t<X, void() noexcept>, void (X::*)() noexcept>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_like_t<X const, void() noexcept>,
        void (X::*)() const noexcept>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_like_t<X volatile, void() noexcept>,
        void (X::*)() volatile noexcept>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_like_t<X const volatile, void() noexcept>,
        void (X::*)() const volatile noexcept>));
}

TEST(MemberLike, Function_LValueQualifiedWithExcept)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_like_t<X &, void() noexcept>, void (X::*)() & noexcept>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_like_t<X const &, void() noexcept>,
        void (X::*)() const & noexcept>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_like_t<X volatile &, void() noexcept>,
        void (X::*)() volatile & noexcept>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_like_t<X const volatile &, void() noexcept>,
        void (X::*)() const volatile & noexcept>));
}

TEST(MemberLike, Function_RValueQualifiedWithExcept)
{
    STATIC_EXPECT_TRUE((::std::is_same_v < ::scl::member_like_t<X &&, void() noexcept>,
        void (X::*)() && noexcept >));
    STATIC_EXPECT_TRUE((::std::is_same_v < ::scl::member_like_t<X const &&, void() noexcept>,
        void (X::*)() const && noexcept >));
    STATIC_EXPECT_TRUE((::std::is_same_v < ::scl::member_like_t<X volatile &&, void() noexcept>,
        void (X::*)() volatile && noexcept >));
    STATIC_EXPECT_TRUE((::std::is_same_v < ::scl::member_like_t<X const volatile &&, void() noexcept>,
        void (X::*)() const volatile && noexcept >));
}

TEST(MemberLike, Function_KeepsItsArguments)
{
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::member_like_t<X const &, int(short)>, int (X::*)(short) const &>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_like_t<X volatile &&, void(double)>,
        void (X::*)(double) volatile &&>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_like_t<X const volatile, void(int) noexcept>,
        void (X::*)(int) const volatile noexcept>));
}

TEST(MemberLike, Property_TakesTheObjectCv)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_like_t<X, int>, int X::*>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_like_t<X const, int>, int const X::*>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_like_t<X volatile, int>, int volatile X::*>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::member_like_t<X const volatile, int>, int const volatile X::*>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_like_t<X &, int>, int X::*>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_like_t<X &&, int>, int X::*>));
}

TEST(MemberLike, Property_KeepsItsOwnCv)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_like_t<X, int const>, int const X::*>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::member_like_t<X, int volatile>, int volatile X::*>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::member_like_t<X, int const volatile>, int const volatile X::*>));
}

TEST(MemberFunctionLike, Invocation_LValueQualified)
{
    // X::f2 is int(int) &; requires lvalue object
    ::scl::member_function_like_t<X &, int(int)> pmf = &X::f2;
    X x;
    EXPECT_EQ((x.*pmf)(5), 5);

    // The next line would fail to compile (rvalue object with &-qualified method):
    // EXPECT_EQ((std::move(x).*pmf)(5), 5);
}

TEST(MemberFunctionLike, Invocation_ConstLValueQualified)
{
    // X::f3 is int(int) const &; works with const lvalue
    ::scl::member_function_like_t<X const &, int(int)> pmf = &X::f3;
    X const cx{};
    EXPECT_EQ((cx.*pmf)(7), 7);
}

TEST(MemberFunctionLike, Invocation_RValueQualified)
{
    // X::f4 is int(int) &&; requires rvalue object
    ::scl::member_function_like_t<X &&, int(int)> pmf = &X::f4;
    X x;
    EXPECT_EQ((std::move(x).*pmf)(9), 9);
}

TEST(MemberFunctionLike, Invocation_RValueQualified_Noexcept)
{
    // X::f5 is int(int) const && noexcept
    ::scl::member_function_like_t<X const &&, int(int) noexcept> pmf = &X::f5;
    X const cx{};
    EXPECT_EQ((std::move(cx).*pmf)(11), 11);
}

TEST(MemberPropertyLike, AccessThroughPointer)
{
    X x;
    x.a = 42;
    ::scl::member_property_like_t<X, int> pm = &X::a;
    EXPECT_EQ(x.*pm, 42);
    x.*pm = 7;
    EXPECT_EQ(x.a, 7);
    X * px = &x;
    EXPECT_EQ(px->*pm, 7);

    X const cx{};
    ::scl::member_property_like_t<X const, int> pb = &X::b;
    EXPECT_EQ(cx.*pb, 1);
}

TEST(MemberLike, Invocation_LValueQualified)
{
    // X::f2 is int(int) &; requires lvalue object
    ::scl::member_like_t<X &, int(int)> pmf = &X::f2;
    X x;
    EXPECT_EQ((x.*pmf)(5), 5);
}

TEST(MemberLike, Invocation_ConstLValueQualified)
{
    // X::f3 is int(int) const &; works with const lvalue
    ::scl::member_like_t<X const &, int(int)> pmf = &X::f3;
    X const cx{};
    EXPECT_EQ((cx.*pmf)(7), 7);
}

TEST(MemberLike, Invocation_RValueQualified)
{
    // X::f4 is int(int) &&; requires rvalue object
    ::scl::member_like_t<X &&, int(int)> pmf = &X::f4;
    X x;
    EXPECT_EQ((std::move(x).*pmf)(9), 9);
}

TEST(MemberLike, Invocation_RValueQualified_Noexcept)
{
    // X::f5 is int(int) const && noexcept
    ::scl::member_like_t<X const &&, int(int) noexcept> pmf = &X::f5;
    X const cx{};
    EXPECT_EQ((std::move(cx).*pmf)(11), 11);
}

TEST(MemberLike, Property_AccessThroughPointer)
{
    X x;
    x.a = 42;
    ::scl::member_like_t<X, int> pm = &X::a;
    EXPECT_EQ(x.*pm, 42);
    x.*pm = 7;
    EXPECT_EQ(x.a, 7);
    X * px = &x;
    EXPECT_EQ(px->*pm, 7);

    X const cx{};
    ::scl::member_like_t<X const, int> pb = &X::b;
    EXPECT_EQ(cx.*pb, 1);
}
