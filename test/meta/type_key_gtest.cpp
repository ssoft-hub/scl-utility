#include <gtest_utils.h>

#include <type_traits>
#include <vector>

#include <scl/utility/meta/type_key.h>

/**
 * @brief TU-local type: its key must carry a per-TU discriminator.
 */
namespace
{
    struct KeyAnonDuck
    {};
} // namespace

/**
 * @brief External-linkage type at global namespace scope.
 */
struct KeyGlobalDuck
{};

/**
 * @brief External-linkage type inside a named namespace.
 */
namespace TypeKeyNs
{
    struct Duck
    {};

} // namespace TypeKeyNs

/**
 * @test Verify a key is an ordinary value: default-built, copied, moved and
 *       assigned, so a handle can answer one without handing out a pointer.
 */
TEST(TypeKeyTest, TypeProperties)
{
    STATIC_EXPECT_TRUE(::std::is_default_constructible_v<::scl::type_key>);
    STATIC_EXPECT_TRUE(::std::is_copy_constructible_v<::scl::type_key>);
    STATIC_EXPECT_TRUE(::std::is_move_constructible_v<::scl::type_key>);
    STATIC_EXPECT_TRUE(::std::is_copy_assignable_v<::scl::type_key>);
    STATIC_EXPECT_TRUE(::std::is_move_assignable_v<::scl::type_key>);
}

/**
 * @test Verify the empty key names no type and matches a default-built one.
 */
TEST(TypeKeyTest, EmptyKeyNamesNothing)
{
    STATIC_EXPECT_TRUE(::scl::type_key{} == ::scl::type_key{});
    STATIC_EXPECT_TRUE(::scl::type_key{}.name().empty());
    STATIC_EXPECT_FALSE(::scl::type_key_of<int>() == ::scl::type_key{});
}

/**
 * @test Verify a copy of a key keeps naming the same type as its source.
 */
TEST(TypeKeyTest, ACopyNamesTheSameType)
{
    ::scl::type_key copy = ::scl::type_key_of<int>();

    EXPECT_TRUE(copy == ::scl::type_key_of<int>());
    EXPECT_FALSE(copy == ::scl::type_key_of<double>());

    copy = ::scl::type_key_of<double>();
    EXPECT_TRUE(copy == ::scl::type_key_of<double>());
}

/**
 * @test Verify the name accessor exposes the type_name rendering.
 */
TEST(TypeKeyTest, Name)
{
    STATIC_EXPECT_EQ(::scl::type_key_of<KeyGlobalDuck>().name(), ::scl::type_name<KeyGlobalDuck>());
    STATIC_EXPECT_EQ(::scl::type_key_of<KeyAnonDuck>().name(), ::scl::type_name<KeyAnonDuck>());
}

/**
 * @test Verify type_key_of returns a reference to one per-type object.
 */
TEST(TypeKeyTest, PerTypeAddressIdentity)
{
    STATIC_EXPECT_TRUE(&::scl::type_key_of<int>() == &::scl::type_key_of<int>());
    STATIC_EXPECT_TRUE(&::scl::type_key_of<KeyAnonDuck>() == &::scl::type_key_of<KeyAnonDuck>());
    STATIC_EXPECT_TRUE(&::scl::type_key_of<int>() != &::scl::type_key_of<long>());
}

/**
 * @test Verify key equality and inequality in constant expressions.
 */
TEST(TypeKeyTest, Equality)
{
    STATIC_EXPECT_EQ(::scl::type_key_of<int>(), ::scl::type_key_of<int>());
    STATIC_EXPECT_NE(::scl::type_key_of<int>(), ::scl::type_key_of<long>());
    STATIC_EXPECT_NE(::scl::type_key_of<KeyGlobalDuck>(), ::scl::type_key_of<TypeKeyNs::Duck>());

    STATIC_EXPECT_EQ(::scl::type_key_of<KeyAnonDuck>(), ::scl::type_key_of<KeyAnonDuck>());
    STATIC_EXPECT_NE(::scl::type_key_of<KeyAnonDuck>(), ::scl::type_key_of<KeyGlobalDuck>());
    STATIC_EXPECT_NE(::scl::type_key_of<KeyAnonDuck>(), ::scl::type_key_of<KeyAnonDuck *>());
    STATIC_EXPECT_NE(::scl::type_key_of<KeyAnonDuck>(), ::scl::type_key_of<::std::vector<KeyAnonDuck>>());
}

namespace TypeKeySpelling
{
    struct Pointing
    {
        Pointing const * operator->() const { return this; }

        int operator-(Pointing) const { return 0; }

        bool operator>(Pointing) const { return true; }

        bool operator>>(Pointing) const { return true; }

        bool operator<=(Pointing) const { return true; }

        int operator<=>(Pointing) const { return 0; }

        int operator->*(int) const { return 0; }
    };

    template <auto Function>
    struct Holder
    {
        template <auto Other>
        struct Inner
        {};
    };

    template <typename T>
    struct Wrap
    {
        template <auto Other>
        struct Inner
        {};
    };

    template <auto Function, typename... Rest>
    struct Tagged
    {};

    template <typename... Types>
    struct Pack
    {};

    using arrow_first = Wrap<Holder<&Pointing::operator- >>::Inner<&Pointing::operator-> >;
    using arrow_second = Wrap<Holder<&Pointing::operator-> >::Inner<&Pointing::operator- >>;
    using order_first = Wrap<Holder<&Pointing::operator<= >>::Inner<&Pointing::operator<=> >;
    using order_second = Wrap<Holder<&Pointing::operator<=> >::Inner<&Pointing::operator<= >>;
#if defined(__clang__)
    using shift_first = Wrap<Holder<&Pointing::operator> >>::Inner<&Pointing::operator>> >;
    using shift_second = Wrap<Holder<&Pointing::operator>> >::Inner<&Pointing::operator> >>;
#elif defined(__GNUC__)
    using star_first = Pack<Tagged<&Pointing::operator- > *, Tagged<&Pointing::operator->* >>;
    using star_second = Pack<Tagged<&Pointing::operator->*, Tagged<&Pointing::operator- > *>>;
#endif
} // namespace TypeKeySpelling

namespace
{
    struct AnonPointing
    {
        AnonPointing const * operator->() const { return this; }

        int operator-(AnonPointing) const { return 0; }
    };

    using anon_arrow_first = TypeKeySpelling::Wrap<
        TypeKeySpelling::Holder<&AnonPointing::operator- >>::Inner<&AnonPointing::operator-> >;
    using anon_arrow_second = TypeKeySpelling::Wrap<
        TypeKeySpelling::Holder<&AnonPointing::operator-> >::Inner<&AnonPointing::operator- >>;
} // namespace

/**
 * @test Verify that two types GCC and Clang spell with one name get keys of their own, with
 *       external linkage and in an unnamed namespace alike.
 */
TEST(TypeKeyTest, TypesOfOneSpellingGetKeysOfTheirOwn)
{
    using ::TypeKeySpelling::arrow_first;
    using ::TypeKeySpelling::arrow_second;
    using ::TypeKeySpelling::order_first;
    using ::TypeKeySpelling::order_second;
#if !defined(_MSC_VER) || defined(__clang__)
    STATIC_EXPECT_EQ(::scl::type_key_of<arrow_first>().name(), ::scl::type_key_of<arrow_second>().name());
    STATIC_EXPECT_EQ(::scl::type_key_of<order_first>().name(), ::scl::type_key_of<order_second>().name());
    STATIC_EXPECT_EQ(::scl::type_key_of<anon_arrow_first>().name(),
        ::scl::type_key_of<anon_arrow_second>().name());
#endif
    STATIC_EXPECT_NE(::scl::type_key_of<arrow_first>(), ::scl::type_key_of<arrow_second>());
    STATIC_EXPECT_NE(::scl::type_key_of<order_first>(), ::scl::type_key_of<order_second>());
    STATIC_EXPECT_NE(::scl::type_key_of<anon_arrow_first>(), ::scl::type_key_of<anon_arrow_second>());
#if defined(__clang__)
    using ::TypeKeySpelling::shift_first;
    using ::TypeKeySpelling::shift_second;
    STATIC_EXPECT_EQ(::scl::type_key_of<shift_first>().name(), ::scl::type_key_of<shift_second>().name());
    STATIC_EXPECT_NE(::scl::type_key_of<shift_first>(), ::scl::type_key_of<shift_second>());
#elif defined(__GNUC__)
    using ::TypeKeySpelling::star_first;
    using ::TypeKeySpelling::star_second;
    STATIC_EXPECT_EQ(::scl::type_key_of<star_first>().name(), ::scl::type_key_of<star_second>().name());
    STATIC_EXPECT_NE(::scl::type_key_of<star_first>(), ::scl::type_key_of<star_second>());
#endif
}
