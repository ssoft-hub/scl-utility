#include <gtest_utils.h>

#include <compare>
#include <functional>
#include <string_view>
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

struct KeyTwin
{};

namespace
{
    struct KeyTwin
    {};

    using KeyMovedTwin = KeyTwin;
} // namespace

namespace
{
    template <typename Key>
    concept meta_key_ordered = requires(Key const & left, Key const & right) { left < right; };
} // namespace

/**
 * @test Verify the declarations: type_key_of, name, the comparison and the default constructor
 *       throw nothing, type_key_of returns a reference to a const key, and name returns
 *       a std::string_view.
 */
TEST(TypeKeyTest, Declarations)
{
    STATIC_EXPECT_TRUE(noexcept(::scl::type_key_of<int>()));
    STATIC_EXPECT_TRUE(noexcept(::scl::type_key_of<int>().name()));
    STATIC_EXPECT_TRUE(noexcept(::scl::type_key_of<int>() == ::scl::type_key_of<long>()));
    STATIC_EXPECT_TRUE(noexcept(::scl::type_key{}));
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(::scl::type_key_of<int>()), ::scl::type_key const &>));
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(::scl::type_key_of<int>().name()), ::std::string_view>));
}

/**
 * @test Verify that no key that identifies a type can be built apart from type_key_of, and that keys
 *       carry neither an order nor a hash.
 */
TEST(TypeKeyTest, NoConstructionOrderOrHash)
{
    STATIC_EXPECT_FALSE((::std::is_constructible_v<::scl::type_key, ::std::string_view, void const *>));
    STATIC_EXPECT_FALSE((::std::is_constructible_v<::scl::type_key, ::std::string_view>));
    STATIC_EXPECT_FALSE(::std::three_way_comparable<::scl::type_key>);
    STATIC_EXPECT_FALSE(meta_key_ordered<::scl::type_key>);
    STATIC_EXPECT_FALSE(::std::is_default_constructible_v<::std::hash<::scl::type_key>>);
}

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
    constexpr auto copy_matches = [] {
        ::scl::type_key copy = ::scl::type_key_of<int>();
        bool const first = copy == ::scl::type_key_of<int>() && copy != ::scl::type_key_of<double>();
        copy = ::scl::type_key_of<double>();
        return first && copy == ::scl::type_key_of<double>();
    };
    constexpr auto move_matches = [] {
        ::scl::type_key source = ::scl::type_key_of<int>();
        ::scl::type_key moved = static_cast<::scl::type_key &&>(source);
        ::scl::type_key assigned{};
        assigned = static_cast<::scl::type_key &&>(moved);
        return assigned == ::scl::type_key_of<int>();
    };
    STATIC_EXPECT_TRUE(copy_matches());
    STATIC_EXPECT_TRUE(move_matches());
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
 * @test Verify that a class moved into an unnamed namespace gets a key of its own beside a class of
 *       the same name at namespace scope.
 */
TEST(TypeKeyTest, UnnamedNamespaceKeyOfItsOwn)
{
    STATIC_EXPECT_NE(::scl::type_key_of<KeyMovedTwin>(), ::scl::type_key_of<::KeyTwin>());
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
