#include <gtest_utils.h>

#include <scl/utility/meta/enum.h>

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
} // namespace

TEST(MetaEnumTest, ScopedEnumName)
{
    EXPECT_EQ(::scl::enum_name<Namespace::Color::Red>(), "Namespace::Color::Red");
    EXPECT_EQ(::scl::enum_name<Namespace::Color::Green>(), "Namespace::Color::Green");
    EXPECT_EQ(::scl::enum_name<Namespace::Color::Blue>(), "Namespace::Color::Blue");
    EXPECT_EQ(::scl::enum_short_name<Namespace::Color::Red>(), "Red");
    EXPECT_EQ(::scl::enum_short_name<Namespace::Color::Green>(), "Green");
    EXPECT_EQ(::scl::enum_short_name<Namespace::Color::Blue>(), "Blue");
}

TEST(MetaEnumTest, UnscopedEnumName)
{
    EXPECT_EQ(::scl::enum_name<Namespace::Value>(), "Namespace::Value");
    EXPECT_EQ(::scl::enum_short_name<Namespace::Value>(), "Value");
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
    STATIC_EXPECT_TRUE(::scl::enum_short_name<Namespace::Color{42}>().empty());
    STATIC_EXPECT_TRUE(::scl::enum_short_name<Hidden{5}>().empty());
}

/**
 * @test Verify that a constant whose spelling holds parentheses keeps its name: a constant of a
 *       type in an unnamed namespace, in a function, or in a template with an argument spelled as
 *       a cast.
 */
TEST(MetaEnumTest, ConstantSpelledWithParenthesesKeepsName)
{
    enum class Local
    {
        Inner,
    };

    STATIC_EXPECT_TRUE(::scl::enum_name<Hidden::Shown>().ends_with("Hidden::Shown"));
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
