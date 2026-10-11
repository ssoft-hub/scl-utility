#include <gtest_utils.h>

#include <scl/utility/meta/enum.h>

#include <string_view>
#include <type_traits>

namespace Namespace
{
    enum class Color
    {
        Red,
        Green,
        Blue,
    };

    enum Enum
    {
        Value,
    };

    enum Fixed : int
    {
        First,
    };

    enum class Wide : long long
    {
        One = 1,
    };

    enum class Switch : bool
    {
        Off,
    };

    struct Holder
    {
        enum class Kind
        {
            Value,
        };

        enum Plain
        {
            Active,
        };
    };
} // namespace Namespace

namespace
{
    enum class Hidden
    {
        Shown,
    };

    template <auto Argument>
    struct Outer
    {
        enum class Nested
        {
            Inner,
        };
    };

    template <auto V>
    concept meta_enum_name_declared = requires { ::scl::enum_name<V>(); };

    template <auto V>
    concept meta_enum_short_name_declared = requires { ::scl::enum_short_name<V>(); };
} // namespace

/**
 * @test Verify that both function templates return a std::string_view and throw nothing.
 */
TEST(MetaEnumTest, Declarations)
{
    STATIC_EXPECT_TRUE(noexcept(::scl::enum_name<Namespace::Color::Red>()));
    STATIC_EXPECT_TRUE(noexcept(::scl::enum_short_name<Namespace::Color::Red>()));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<decltype(::scl::enum_name<Namespace::Color::Red>()), ::std::string_view>));
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(::scl::enum_short_name<Namespace::Color::Red>()),
        ::std::string_view>));
}

/**
 * @test Verify that both function templates take a value of a scoped or an unscoped enumeration and
 *       refuse any other value.
 */
TEST(MetaEnumTest, TakesEnumerationValue)
{
    STATIC_EXPECT_TRUE(meta_enum_name_declared<Namespace::Color::Red>);
    STATIC_EXPECT_TRUE(meta_enum_name_declared<Namespace::Value>);
    STATIC_EXPECT_FALSE(meta_enum_name_declared<42>);
    STATIC_EXPECT_FALSE(meta_enum_name_declared<true>);
    STATIC_EXPECT_TRUE(meta_enum_short_name_declared<Namespace::Color::Red>);
    STATIC_EXPECT_TRUE(meta_enum_short_name_declared<Namespace::Value>);
    STATIC_EXPECT_FALSE(meta_enum_short_name_declared<42>);
    STATIC_EXPECT_FALSE(meta_enum_short_name_declared<true>);
}

/**
 * @test Verify that a constant of a scoped enumeration is named with its type and namespaces.
 */
TEST(MetaEnumTest, ScopedEnumName)
{
    STATIC_EXPECT_EQ(::scl::enum_name<Namespace::Color::Red>(), "Namespace::Color::Red");
    STATIC_EXPECT_EQ(::scl::enum_name<Namespace::Color::Blue>(), "Namespace::Color::Blue");
    STATIC_EXPECT_EQ(::scl::enum_short_name<Namespace::Color::Red>(), "Red");
    STATIC_EXPECT_EQ(::scl::enum_short_name<Namespace::Color::Blue>(), "Blue");
}

/**
 * @test Verify that a constant of an unscoped enumeration is named with its namespaces alone.
 */
TEST(MetaEnumTest, UnscopedEnumName)
{
    STATIC_EXPECT_EQ(::scl::enum_name<Namespace::Value>(), "Namespace::Value");
    STATIC_EXPECT_EQ(::scl::enum_short_name<Namespace::Value>(), "Value");
}

/**
 * @test Verify that a constant of an enumeration declared in a class is named with that class.
 */
TEST(MetaEnumTest, EnumerationInClass)
{
    STATIC_EXPECT_EQ(::scl::enum_name<Namespace::Holder::Kind::Value>(), "Namespace::Holder::Kind::Value");
    STATIC_EXPECT_EQ(::scl::enum_name<Namespace::Holder::Active>(), "Namespace::Holder::Active");
}

/**
 * @test Verify the spelling of a constant of an enumeration in an unnamed namespace
 *       the documentation states for each compiler.
 */
TEST(MetaEnumTest, UnnamedNamespaceSpelling)
{
#if defined(_MSC_VER) && !defined(__clang__)
    STATIC_EXPECT_EQ(::scl::enum_name<Hidden::Shown>(), "`anonymous-namespace'::Hidden::Shown");
#elif defined(__clang__)
    STATIC_EXPECT_EQ(::scl::enum_name<Hidden::Shown>(), "(anonymous namespace)::Hidden::Shown");
#elif defined(__GNUC__)
    STATIC_EXPECT_EQ(::scl::enum_name<Hidden::Shown>(), "<unnamed>::Hidden::Shown");
#endif
}

/**
 * @test Verify that a value no constant of its type has gets an empty name.
 */
TEST(MetaEnumTest, ValueWithoutConstantHasNoName)
{
    STATIC_EXPECT_TRUE(::scl::enum_name<Namespace::Color{42}>().empty());
    STATIC_EXPECT_TRUE(::scl::enum_name<Namespace::Color{-3}>().empty());
    STATIC_EXPECT_TRUE(::scl::enum_name<Namespace::Fixed{7}>().empty());
    STATIC_EXPECT_TRUE(::scl::enum_name<Namespace::Wide{-9000000000LL}>().empty());
    STATIC_EXPECT_TRUE(::scl::enum_name<Hidden{5}>().empty());
    STATIC_EXPECT_TRUE(::scl::enum_name<Namespace::Switch{true}>().empty());
    STATIC_EXPECT_TRUE(::scl::enum_short_name<Namespace::Color{42}>().empty());
    STATIC_EXPECT_TRUE(::scl::enum_short_name<Namespace::Switch{true}>().empty());
    STATIC_EXPECT_TRUE(::scl::enum_short_name<Hidden{5}>().empty());
}

/**
 * @test Verify that a constant whose spelling holds parentheses keeps its name: the full and
 *       the short name of a constant of a type in a function, and the short name of a constant
 *       of a type in an unnamed namespace or in a template with an argument spelled as a cast.
 */
TEST(MetaEnumTest, ConstantSpelledWithParenthesesKeepsName)
{
    enum class Local
    {
        Inner,
    };

    STATIC_EXPECT_TRUE(::scl::enum_name<Local::Inner>().ends_with("Local::Inner"));
    STATIC_EXPECT_EQ(::scl::enum_short_name<Hidden::Shown>(), "Shown");
    STATIC_EXPECT_EQ(::scl::enum_short_name<Local::Inner>(), "Inner");
    STATIC_EXPECT_EQ(::scl::enum_short_name<Outer<Namespace::Color{42}>::Nested::Inner>(), "Inner");
}

/**
 * @test Verify that a value no constant has gets an empty name in a template with a cast argument.
 */
TEST(MetaEnumTest, ValueOfNestedTypeWithoutConstantHasNoName)
{
    STATIC_EXPECT_TRUE(::scl::enum_name<Outer<Namespace::Color{42}>::Nested{7}>().empty());
}
