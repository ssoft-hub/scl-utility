#include <gtest_utils.h>

#include <scl/utility/type_traits/overload_cast.h>

#include <type_traits>

namespace cast_fixture
{
    int twice(int value);
    double twice(double value) noexcept { return value; }

    void bind(int & value);
    void bind(int const & value);

    long spread(long value);
    long spread(int value, ...);

    struct every
    {
        void pick(int);
        void pick(double) noexcept;
        void pick(float) &;
        void pick(long) & noexcept;
        void pick(char) &&;
        void pick(short) && noexcept;
        void pick(unsigned int) const;
        void pick(bool) const noexcept;
        void pick(long double) const &;
        void pick(signed char) const & noexcept;
        void pick(unsigned char) const &&;
        void pick(unsigned short) const && noexcept;
        void pick(unsigned long) volatile;
        void pick(long long) volatile noexcept;
        void pick(unsigned long long) volatile &;
        void pick(void *) volatile & noexcept;
        void pick(void const *) volatile &&;
        void pick(char *) volatile && noexcept;
        void pick(char const *) const volatile;
        void pick(int *) const volatile noexcept;
        void pick(int const *) const volatile &;
        void pick(double *) const volatile & noexcept;
        void pick(double const *) const volatile &&;
        void pick(float *) const volatile && noexcept;
    };

    struct qualified
    {
        int & width() &;
        int width() const &;
        long width() && { return 0; }
        char width() const && noexcept;

        void set(int);
        void set(int) const;
        void set(int) volatile;
        void set(int) const volatile noexcept;
    };

    struct derived_qualified : qualified
    {};
} // namespace cast_fixture

using cast_fixture::bind;
using cast_fixture::derived_qualified;
using cast_fixture::every;
using cast_fixture::qualified;
using cast_fixture::spread;
using cast_fixture::twice;

TEST(TypeTraitsOverloadCast, ChoosesTheFunctionWithExactlyTheParameterTypes)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(::scl::overload_cast<int>(&twice)), int (*)(int)>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<decltype(::scl::overload_cast<double>(&twice)), double (*)(double) noexcept>));
}

TEST(TypeTraitsOverloadCast, TellsTheReferencesOfTheParametersApart)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(::scl::overload_cast<int &>(&bind)), void (*)(int &)>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<decltype(::scl::overload_cast<int const &>(&bind)), void (*)(int const &)>));
}

TEST(TypeTraitsOverloadCast, IgnoresTheTopLevelQualifiersOfTheArguments)
{
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<decltype(::scl::overload_cast<int const>(&twice)), int (*)(int)>));
}

TEST(TypeTraitsOverloadCast, ChoosesNoOverloadWithAnEllipsis)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(::scl::overload_cast<long>(&spread)), long (*)(long)>));
}

TEST(TypeTraitsOverloadCast, ChoosesAMemberFunctionWhateverItsQualifiersAndKeepsThem)
{
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<decltype(::scl::overload_cast<int>(&every::pick)), void (every::*)(int)>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::overload_cast<double>(&every::pick)), void (every::*)(double) noexcept>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<decltype(::scl::overload_cast<float>(&every::pick)), void (every::*)(float) &>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::overload_cast<long>(&every::pick)), void (every::*)(long) & noexcept>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<decltype(::scl::overload_cast<char>(&every::pick)), void (every::*)(char) &&>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v < decltype(::scl::overload_cast<short>(&every::pick)),
        void (every::*)(short) && noexcept >));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::overload_cast<unsigned int>(&every::pick)), void (every::*)(unsigned int) const>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::overload_cast<bool>(&every::pick)), void (every::*)(bool) const noexcept>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::overload_cast<long double>(&every::pick)), void (every::*)(long double) const &>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::overload_cast<signed char>(&every::pick)),
        void (every::*)(signed char) const & noexcept>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::overload_cast<unsigned char>(&every::pick)),
        void (every::*)(unsigned char) const &&>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v < decltype(::scl::overload_cast<unsigned short>(&every::pick)),
        void (every::*)(unsigned short) const && noexcept >));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::overload_cast<unsigned long>(&every::pick)),
        void (every::*)(unsigned long) volatile>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::overload_cast<long long>(&every::pick)),
        void (every::*)(long long) volatile noexcept>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::overload_cast<unsigned long long>(&every::pick)),
        void (every::*)(unsigned long long) volatile &>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::overload_cast<void *>(&every::pick)), void (every::*)(void *) volatile & noexcept>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::overload_cast<void const *>(&every::pick)),
        void (every::*)(void const *) volatile &&>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v < decltype(::scl::overload_cast<char *>(&every::pick)),
        void (every::*)(char *) volatile && noexcept >));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::overload_cast<char const *>(&every::pick)),
        void (every::*)(char const *) const volatile>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::overload_cast<int *>(&every::pick)), void (every::*)(int *) const volatile noexcept>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::overload_cast<int const *>(&every::pick)),
        void (every::*)(int const *) const volatile &>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::overload_cast<double *>(&every::pick)),
        void (every::*)(double *) const volatile & noexcept>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::overload_cast<double const *>(&every::pick)),
        void (every::*)(double const *) const volatile &&>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v < decltype(::scl::overload_cast<float *>(&every::pick)),
        void (every::*)(float *) const volatile && noexcept >));
}

TEST(TypeTraitsOverloadCast, IsAConstantExpression)
{
    STATIC_EXPECT_TRUE(
        ::scl::overload_cast<double>(&twice) == static_cast<double (*)(double) noexcept>(&twice));
}

TEST(TypeTraitsOverloadCast, IsNoexcept)
{
    STATIC_EXPECT_TRUE(noexcept(::scl::overload_cast<int>(&twice)));
}

TEST(TypeTraitsMemberOverloadCast, TakesTheRefQualifierFromTheReferenceOfTheType)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::member_overload_cast<qualified &>(&qualified::width)), int & (qualified::*)() &>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::member_overload_cast<qualified const &>(&qualified::width)),
        int (qualified::*)() const &>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::member_overload_cast<qualified &&>(&qualified::width)), long (qualified::*)() &&>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v < decltype(::scl::member_overload_cast<qualified const &&>(&qualified::width)),
        char (qualified::*)() const && noexcept >));
}

TEST(TypeTraitsMemberOverloadCast, TakesTheCvQualifiersFromTheType)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::member_overload_cast<qualified, int>(&qualified::set)), void (qualified::*)(int)>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::member_overload_cast<qualified const, int>(&qualified::set)),
        void (qualified::*)(int) const>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::member_overload_cast<qualified volatile, int>(&qualified::set)),
        void (qualified::*)(int) volatile>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::member_overload_cast<qualified const volatile, int>(&qualified::set)),
        void (qualified::*)(int) const volatile noexcept>));
}

TEST(TypeTraitsMemberOverloadCast, IgnoresTheTopLevelQualifiersOfTheArguments)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::member_overload_cast<qualified const, int const>(&qualified::set)),
        void (qualified::*)(int) const>));
}

TEST(TypeTraitsMemberOverloadCast, PointsToAMemberOfTheClassThatDeclaresIt)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::member_overload_cast<derived_qualified const &>(&derived_qualified::width)),
        int (qualified::*)() const &>));
}

TEST(TypeTraitsMemberOverloadCast, IsAConstantExpression)
{
    STATIC_EXPECT_TRUE(
        ::scl::member_overload_cast<qualified &&>(&qualified::width) ==
        static_cast<long (qualified::*)() &&>(&qualified::width));
}

TEST(TypeTraitsMemberOverloadCast, IsNoexcept)
{
    STATIC_EXPECT_TRUE(noexcept(::scl::member_overload_cast<qualified &&>(&qualified::width)));
}

#ifdef __cpp_explicit_this_parameter
namespace cast_fixture
{
    struct explicit_gauge
    {
        int read(this explicit_gauge const & self, int value);
        long read(int value) &;
    };
} // namespace cast_fixture

using cast_fixture::explicit_gauge;

TEST(TypeTraitsOverloadCast, ChoosesAnExplicitObjectMemberFunctionByTheObjectParameterFirst)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::overload_cast<explicit_gauge const &, int>(&explicit_gauge::read)),
        int (*)(explicit_gauge const &, int)>));
}

TEST(TypeTraitsMemberOverloadCast, ChoosesNoMemberFunctionWithAnExplicitObjectParameter)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::member_overload_cast<explicit_gauge &, int>(&explicit_gauge::read)),
        long (explicit_gauge::*)(int) &>));
}
#endif
