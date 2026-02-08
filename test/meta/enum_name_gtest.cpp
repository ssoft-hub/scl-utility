#include <gtest/gtest.h>

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
} // namespace Namespace

TEST(EnumTraitsTest, ScopedEnumName)
{
    EXPECT_EQ(::scl::enum_name<Namespace::Color::Red>(), "Namespace::Color::Red");
    EXPECT_EQ(::scl::enum_name<Namespace::Color::Green>(), "Namespace::Color::Green");
    EXPECT_EQ(::scl::enum_name<Namespace::Color::Blue>(), "Namespace::Color::Blue");
    EXPECT_EQ(::scl::enum_short_name<Namespace::Color::Red>(), "Red");
    EXPECT_EQ(::scl::enum_short_name<Namespace::Color::Green>(), "Green");
    EXPECT_EQ(::scl::enum_short_name<Namespace::Color::Blue>(), "Blue");
}

TEST(EnumTraitsTest, UnscopedEnumName)
{
    EXPECT_EQ(::scl::enum_name<Namespace::Value>(), "Namespace::Value");
    EXPECT_EQ(::scl::enum_short_name<Namespace::Value>(), "Value");
}
