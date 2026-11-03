#include <gtest_utils.h>

#include <scl/utility/type_traits/member_like.h>
#include <scl/utility/type_traits/overload_cast.h>
#include <scl/utility/type_traits/signature.h>

#include <string>
#include <tuple>
#include <type_traits>

namespace
{
    template <typename Callable>
    concept has_signature = requires { typename ::scl::signature_t<Callable>; };

    template <typename Callable>
    concept has_object = requires { typename ::scl::signature::object_t<Callable>; };

    template <typename Pointer>
    inline constexpr bool round_trips_v = ::std::is_same_v<
        ::scl::member_like_t<::scl::signature::object_t<Pointer>, ::scl::signature_t<Pointer>>, Pointer>;

    struct widget
    {};

    struct call_operator
    {
        int operator()(long, short const &) const;
    };

    struct overloaded
    {
        int operator()(int) const;
        int operator()(double) const;
    };

    struct lvalue_call_operator
    {
        int operator()(long) &;
    };

    struct rvalue_call_operator
    {
        int operator()(long) && noexcept;
    };

    struct base_call_operator
    {
        int operator()(long) const;
    };

    struct derived_call_operator : base_call_operator
    {};

    union union_call_operator
    {
        int value;
        int operator()(long) const;
    };

    struct variadic_call_operator
    {
        int operator()(char const *, ...) const;
    };

    template <typename Callable, ::std::size_t Index>
    concept has_parameter = requires { typename ::scl::signature::parameter_t<Callable, Index>; };
} // namespace

// A function declared and not defined in an anonymous namespace draws -Wunused-function.
namespace signature_test
{
    int overloaded_function(int);
    double overloaded_function(double, char) noexcept;
} // namespace signature_test

TEST(TypeTraitsSignature, IsTheFunctionTypeItselfForAFunctionType)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::signature_t<int(double, int &)>, int(double, int &)>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::signature_t<int(double) noexcept>, int(double) noexcept>));
}

TEST(TypeTraitsSignature, IsTheFunctionTypeForAPointerToAFunction)
{
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::signature_t<void (*)(int, char const *)>, void(int, char const *)>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::signature_t<void (*)(int) noexcept>, void(int) noexcept>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::signature_t<void (*)()>, void()>));
}

TEST(TypeTraitsSignature, IsTheFunctionTypeForAReferenceToAFunction)
{
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::signature_t<int (&)(double, int &&)>, int(double, int &&)>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::signature_t<int (&)(double) noexcept>, int(double) noexcept>));
}

TEST(TypeTraitsSignature, IsTheFunctionTypeWithoutClassAndQualifiersForAPointerToAMemberFunction)
{
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::signature_t<long (widget::*)(int, char)>, long(int, char)>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::signature_t<long (widget::*)(int) const &>, long(int)>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::signature_t<long (widget::*)(int) volatile && noexcept>, long(int) noexcept>));
}

TEST(TypeTraitsSignature, RebuildsAPointerToAMemberFunctionThroughMemberLikeT)
{
    STATIC_EXPECT_TRUE(round_trips_v<void (widget::*)(int)>);
    STATIC_EXPECT_TRUE(round_trips_v<void (widget::*)(int) noexcept>);
    STATIC_EXPECT_TRUE(round_trips_v<void (widget::*)(int) &>);
    STATIC_EXPECT_TRUE(round_trips_v<void (widget::*)(int) & noexcept>);
    STATIC_EXPECT_TRUE(round_trips_v<void (widget::*)(int) &&>);
    STATIC_EXPECT_TRUE(round_trips_v < void (widget::*)(int) && noexcept >);

    STATIC_EXPECT_TRUE(round_trips_v<void (widget::*)(int) const>);
    STATIC_EXPECT_TRUE(round_trips_v<void (widget::*)(int) const noexcept>);
    STATIC_EXPECT_TRUE(round_trips_v<void (widget::*)(int) const &>);
    STATIC_EXPECT_TRUE(round_trips_v<void (widget::*)(int) const & noexcept>);
    STATIC_EXPECT_TRUE(round_trips_v<void (widget::*)(int) const &&>);
    STATIC_EXPECT_TRUE(round_trips_v < void (widget::*)(int) const && noexcept >);

    STATIC_EXPECT_TRUE(round_trips_v<void (widget::*)(int) volatile>);
    STATIC_EXPECT_TRUE(round_trips_v<void (widget::*)(int) volatile noexcept>);
    STATIC_EXPECT_TRUE(round_trips_v<void (widget::*)(int) volatile &>);
    STATIC_EXPECT_TRUE(round_trips_v<void (widget::*)(int) volatile & noexcept>);
    STATIC_EXPECT_TRUE(round_trips_v<void (widget::*)(int) volatile &&>);
    STATIC_EXPECT_TRUE(round_trips_v < void (widget::*)(int) volatile && noexcept >);

    STATIC_EXPECT_TRUE(round_trips_v<void (widget::*)(int) const volatile>);
    STATIC_EXPECT_TRUE(round_trips_v<void (widget::*)(int) const volatile noexcept>);
    STATIC_EXPECT_TRUE(round_trips_v<void (widget::*)(int) const volatile &>);
    STATIC_EXPECT_TRUE(round_trips_v<void (widget::*)(int) const volatile & noexcept>);
    STATIC_EXPECT_TRUE(round_trips_v<void (widget::*)(int) const volatile &&>);
    STATIC_EXPECT_TRUE(round_trips_v < void (widget::*)(int) const volatile && noexcept >);
}

TEST(TypeTraitsSignature, ObjectCarriesTheQualifiersOfAMemberFunction)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::signature::object_t<void (widget::*)()>, widget>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::signature::object_t<void (widget::*)() const>, widget const>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::signature::object_t<void (widget::*)() volatile &>, widget volatile &>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::signature::object_t<void (widget::*)() const &&>, widget const &&>));
}

TEST(TypeTraitsSignature, IsTheSignatureOfTheCallOperatorOfAClass)
{
    auto const lambda = [](int, double &) { return 0; };
    auto counter = [calls = 0](long) mutable { return ++calls; };
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::signature_t<call_operator>, int(long, short const &)>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::signature_t<decltype(lambda)>, int(int, double &)>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::signature_t<decltype(counter)>, int(long)>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::signature::object_t<call_operator>, call_operator const>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::signature::object_t<decltype(counter)>, decltype(counter)>));
}

TEST(TypeTraitsSignature, IsTheSameForAQualifiedClassAndAReferenceToAClass)
{
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::signature_t<call_operator const>, int(long, short const &)>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::signature_t<call_operator volatile>, int(long, short const &)>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::signature_t<call_operator const &>, int(long, short const &)>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::signature_t<call_operator &&>, int(long, short const &)>));
}

TEST(TypeTraitsSignature, IsTheSignatureOfTheOverloadThatOverloadCastChooses)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::signature_t<decltype(::scl::overload_cast<double, char>(&signature_test::overloaded_function))>,
        double(double, char) noexcept>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::signature_t<decltype(::scl::overload_cast<int>(&overloaded::operator()))>, int(int)>));
}

TEST(TypeTraitsSignature, ReadsTheResultAndTheParameters)
{
    using pointer = long (widget::*)(int, char const &) const;
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::signature::result_t<pointer>, long>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::signature::parameters_t<pointer>, ::std::tuple<int, char const &>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::signature::parameter_t<pointer, 1>, char const &>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::signature::parameters_t<void (*)()>, ::std::tuple<>>));
}

TEST(TypeTraitsSignature, ReadsTheCvQualifiersOfAMemberFunction)
{
    STATIC_EXPECT_FALSE(::scl::signature::is_const_v<void (widget::*)()>);
    STATIC_EXPECT_TRUE(::scl::signature::is_const_v<void (widget::*)() const &>);
    STATIC_EXPECT_FALSE(::scl::signature::is_volatile_v<void (widget::*)() const>);
    STATIC_EXPECT_TRUE(::scl::signature::is_volatile_v<void (widget::*)() const volatile &&>);
    STATIC_EXPECT_TRUE(::scl::signature::is_const_v<call_operator>);
}

TEST(TypeTraitsSignature, ReadsTheRefQualifierOfAMemberFunction)
{
    STATIC_EXPECT_FALSE(::scl::signature::is_lvalue_reference_v<void (widget::*)() const>);
    STATIC_EXPECT_FALSE(::scl::signature::is_rvalue_reference_v<void (widget::*)() const>);
    STATIC_EXPECT_TRUE(::scl::signature::is_lvalue_reference_v<void (widget::*)() const &>);
    STATIC_EXPECT_FALSE(::scl::signature::is_rvalue_reference_v<void (widget::*)() const &>);
    STATIC_EXPECT_FALSE(::scl::signature::is_lvalue_reference_v<void (widget::*)() &&>);
    STATIC_EXPECT_TRUE(::scl::signature::is_rvalue_reference_v<void (widget::*)() &&>);
}

TEST(TypeTraitsSignature, ReadsNoexcept)
{
    STATIC_EXPECT_FALSE(::scl::signature::is_noexcept_v<void (*)()>);
    STATIC_EXPECT_TRUE(::scl::signature::is_noexcept_v<void (*)() noexcept>);
    STATIC_EXPECT_TRUE(::scl::signature::is_noexcept_v<void (widget::*)() const & noexcept>);
}

TEST(TypeTraitsSignature, HasNoObjectAndNoQualifiersForAFunction)
{
    STATIC_EXPECT_FALSE(has_object<void (*)(int)>);
    STATIC_EXPECT_FALSE(::scl::signature::is_const_v<void (*)(int)>);
    STATIC_EXPECT_FALSE(::scl::signature::is_volatile_v<void (*)(int)>);
    STATIC_EXPECT_FALSE(::scl::signature::is_lvalue_reference_v<void (*)(int)>);
    STATIC_EXPECT_FALSE(::scl::signature::is_rvalue_reference_v<void (*)(int)>);
}

TEST(TypeTraitsSignature, ReadsACallOperatorWithAnRvalueRefQualifierAndNoexcept)
{
    auto const lambda = [](int) noexcept { return 0; };
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::signature_t<rvalue_call_operator>, int(long) noexcept>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::signature::object_t<rvalue_call_operator>, rvalue_call_operator &&>));
    STATIC_EXPECT_TRUE(::scl::signature::is_rvalue_reference_v<rvalue_call_operator>);
    STATIC_EXPECT_FALSE(::scl::signature::is_const_v<rvalue_call_operator>);
    STATIC_EXPECT_TRUE(::scl::signature::is_noexcept_v<rvalue_call_operator>);
    STATIC_EXPECT_TRUE(::scl::signature::is_noexcept_v<decltype(lambda)>);
    STATIC_EXPECT_TRUE(::scl::signature::is_const_v<decltype(lambda)>);
}

TEST(TypeTraitsSignature, ObjectIsTheClassForACallOperatorOfABase)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::signature_t<derived_call_operator>, int(long)>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::signature::object_t<derived_call_operator>, derived_call_operator const>));
}

TEST(TypeTraitsSignature, IsTheSignatureOfTheCallOperatorOfAUnion)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::signature_t<union_call_operator>, int(long)>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::signature::object_t<union_call_operator>, union_call_operator const>));
}

TEST(TypeTraitsSignature, KeepsAnEllipsisInTheFunctionTypeAndLeavesItOutOfTheParameters)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::signature::parameters_t<int (widget::*)(int, ...) volatile &&>, ::std::tuple<int>>));
    STATIC_EXPECT_TRUE(::scl::signature::is_variadic_v<int (widget::*)(int, ...) const noexcept>);
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::signature_t<int (*)(char const *, ...)>, int(char const *, ...)>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::signature_t<int(char const *, ...) noexcept>, int(char const *, ...) noexcept>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::signature_t<int (widget::*)(int, ...) const &>, int(int, ...)>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::signature::parameters_t<int (*)(char const *, ...)>, ::std::tuple<char const *>>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::signature_t<variadic_call_operator>, int(char const *, ...)>));
    STATIC_EXPECT_TRUE(::scl::signature::is_variadic_v<int (*)(char const *, ...)>);
    STATIC_EXPECT_TRUE(::scl::signature::is_variadic_v<variadic_call_operator>);
    STATIC_EXPECT_FALSE(::scl::signature::is_variadic_v<int (*)(char const *)>);
}

TEST(TypeTraitsSignature, CountsTheParametersWithoutTheEllipsis)
{
    STATIC_EXPECT_EQ(::scl::signature::parameter_count_v<void (*)()>, 0U);
    STATIC_EXPECT_EQ(::scl::signature::parameter_count_v<call_operator>, 2U);
    STATIC_EXPECT_EQ(::scl::signature::parameter_count_v<int (*)(char const *, ...)>, 1U);
}

TEST(TypeTraitsSignature, HasNoParameterTypeForAnIndexNotBelowTheCount)
{
    STATIC_EXPECT_TRUE((has_parameter<call_operator, 1>));
    STATIC_EXPECT_FALSE((has_parameter<call_operator, 2>));
    STATIC_EXPECT_FALSE((has_parameter<void (*)(), 0>));
    STATIC_EXPECT_FALSE((has_parameter<int, 0>));
}

TEST(TypeTraitsSignature, ReadsACallOperatorWithAnLvalueRefQualifier)
{
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::signature::object_t<lvalue_call_operator>, lvalue_call_operator &>));
    STATIC_EXPECT_TRUE(::scl::signature::is_lvalue_reference_v<lvalue_call_operator>);
    STATIC_EXPECT_FALSE(::scl::signature::is_rvalue_reference_v<lvalue_call_operator>);
}

TEST(TypeTraitsSignature, IsTheSignatureOfThePointerForAReferenceToAPointerToAMemberFunction)
{
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::signature_t<long (widget::* const &)(int) &&>, long(int)>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::signature::object_t<long (widget::* volatile)(int) const>, widget const>));
}

#if defined(_MSC_VER) && defined(_M_X64) && !defined(_M_ARM64EC)
#define SCL_TEST_CALLING_CONVENTION __vectorcall
#elif (defined(_WIN32) || defined(__CYGWIN__)) && defined(__x86_64__)
#define SCL_TEST_CALLING_CONVENTION __attribute__((sysv_abi))
#elif defined(__x86_64__)
#define SCL_TEST_CALLING_CONVENTION __attribute__((ms_abi))
#elif defined(_MSC_VER) && defined(_M_IX86)
#define SCL_TEST_CALLING_CONVENTION __fastcall
#elif defined(__i386__)
#define SCL_TEST_CALLING_CONVENTION __attribute__((fastcall))
#endif

#ifdef SCL_TEST_CALLING_CONVENTION
namespace
{
    struct convention_call_operator
    {
        void SCL_TEST_CALLING_CONVENTION operator()(int) const;
    };
} // namespace

TEST(TypeTraitsSignature, IsAbsentForACallingConventionOtherThanTheDefault)
{
    STATIC_EXPECT_FALSE(has_signature<void SCL_TEST_CALLING_CONVENTION(int)>);
    STATIC_EXPECT_FALSE(has_signature<void(SCL_TEST_CALLING_CONVENTION *)(int)>);
    STATIC_EXPECT_FALSE(has_signature<convention_call_operator>);
}
#endif

TEST(TypeTraitsSignature, IsAbsentForAGenericLambda)
{
    auto const generic = [](auto) { return 0; };
    STATIC_EXPECT_FALSE(has_signature<decltype(generic)>);
}

TEST(TypeTraitsSignature, IsAbsentForAnOverloadedCallOperator)
{
    STATIC_EXPECT_FALSE(has_signature<overloaded>);
}

TEST(TypeTraitsSignature, IsAbsentForATypeThatIsNotCallable)
{
    STATIC_EXPECT_FALSE(has_signature<int>);
    STATIC_EXPECT_FALSE(has_signature<int *>);
    STATIC_EXPECT_FALSE(has_signature<widget>);
    STATIC_EXPECT_FALSE(has_signature<void>);
    STATIC_EXPECT_FALSE(has_signature<int widget::*>);
    STATIC_EXPECT_FALSE(has_signature<int() const &>);
}

namespace signature_test
{
    template <typename... Types>
    int pack(Types... values);

    void take(::std::string value) noexcept;
} // namespace signature_test

TEST(TypeTraitsSignature, IsNotVariadicForASpecializationOfAPackTemplate)
{
    STATIC_EXPECT_FALSE(::scl::signature::is_variadic_v<decltype(&signature_test::pack<int, long>)>);
    STATIC_EXPECT_EQ(::scl::signature::parameter_count_v<decltype(&signature_test::pack<int, long>)>, 2U);
}

TEST(TypeTraitsSignature, ReadsNoexceptFromTheDeclaration)
{
    STATIC_EXPECT_TRUE(::scl::signature::is_noexcept_v<decltype(&signature_test::take)>);
}

#ifdef __cpp_explicit_this_parameter
namespace
{
    struct explicit_object
    {
        int operator()(this explicit_object const &, long, char);
    };

    struct explicit_rvalue_object
    {
        int operator()(this explicit_rvalue_object &&, long);
    };

    struct explicit_value_object
    {
        int operator()(this explicit_value_object, long);
    };

    struct explicit_variadic_object
    {
        int operator()(this explicit_variadic_object const &, char const *, ...);
    };

    struct explicit_variadic_noexcept_object
    {
        int operator()(this explicit_variadic_noexcept_object &&, int, ...) noexcept;
    };

    struct explicit_base
    {
        int operator()(this explicit_base const &, long);
    };

    struct explicit_derived : explicit_base
    {};
} // namespace

TEST(TypeTraitsSignature, ObjectIsTheExplicitObjectParameterOfACallOperator)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::signature_t<explicit_object>, int(long, char)>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::signature::object_t<explicit_object>, explicit_object const &>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::signature_t<explicit_rvalue_object>, int(long)>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::signature::object_t<explicit_rvalue_object>, explicit_rvalue_object &&>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::signature_t<explicit_value_object>, int(long)>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::signature::object_t<explicit_value_object>, explicit_value_object>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::signature_t<explicit_variadic_object>, int(char const *, ...)>));
    STATIC_EXPECT_TRUE(::scl::signature::is_variadic_v<explicit_variadic_object>);
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<::scl::signature_t<explicit_variadic_noexcept_object>, int(int, ...) noexcept>));
    STATIC_EXPECT_TRUE(::scl::signature::is_rvalue_reference_v<explicit_variadic_noexcept_object>);
    STATIC_EXPECT_FALSE(::scl::signature::is_const_v<explicit_variadic_noexcept_object>);
    STATIC_EXPECT_TRUE(::scl::signature::is_const_v<explicit_object>);
    STATIC_EXPECT_TRUE(::scl::signature::is_lvalue_reference_v<explicit_object>);
}

TEST(TypeTraitsSignature, ObjectIsTheExplicitObjectParameterOfACallOperatorOfABase)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::signature_t<explicit_derived>, int(long)>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::signature::object_t<explicit_derived>, explicit_base const &>));
}
#endif

#if defined(__cpp_static_call_operator) && \
    (__cplusplus > 202002L || (defined(_MSVC_LANG) && _MSVC_LANG > 202002L))
namespace
{
    struct static_call_operator
    {
        static int operator()(static_call_operator const &, long);
    };
} // namespace

TEST(TypeTraitsSignature, KeepsEveryParameterOfAStaticCallOperator)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::signature_t<static_call_operator>, int(static_call_operator const &, long)>));
    STATIC_EXPECT_FALSE(has_object<static_call_operator>);
}
#endif
