#include <gtest_utils.h>

#include <scl/utility/runtime/enum.h>

enum class Color : int
{
    Red   = 1,
    Green = 2,
    Blue  = -3,
};

enum class Flags : unsigned
{
    None = 0,
    A    = 1,
    B    = 2,
};

enum class ByteEnum : unsigned char
{
    X = 255,
};

namespace ns
{
    enum class Status : int
    {
        Ok  = 0,
        Err = 42,
    };
} // namespace ns

enum Unscoped : int // NOLINT(performance-enum-size)
{
    ValA = 7,
};

/**
 * @test Verify that enum_value formats a scoped enum with int underlying type.
 */
TEST(EnumValueTest, ScopedIntPositive)
{
    EXPECT_EQ(::scl::enum_value(Color::Red), "Color::1");
    EXPECT_EQ(::scl::enum_value(Color::Green), "Color::2");
}

/**
 * @test Verify that enum_value formats negative underlying values correctly.
 */
TEST(EnumValueTest, ScopedIntNegative)
{
    EXPECT_EQ(::scl::enum_value(Color::Blue), "Color::-3");
}

/**
 * @test Verify that enum_value formats a scoped enum with unsigned underlying type.
 */
TEST(EnumValueTest, ScopedUnsigned)
{
    EXPECT_EQ(::scl::enum_value(Flags::None), "Flags::0");
    EXPECT_EQ(::scl::enum_value(Flags::B), "Flags::2");
}

/**
 * @test Verify that enum_value promotes unsigned char underlying to unsigned and formats correctly.
 */
TEST(EnumValueTest, UnderlyingBytePromotedToUnsigned)
{
    EXPECT_EQ(::scl::enum_value(ByteEnum::X), "ByteEnum::255");
}

/**
 * @test Verify that enum_value handles an out-of-range (unnamed) enum value.
 */
TEST(EnumValueTest, OutOfRangeValue)
{
    EXPECT_EQ(::scl::enum_value(Color{42}), "Color::42");
}

/**
 * @test Verify that enum_value strips namespace qualifiers from the type name.
 */
TEST(EnumValueTest, NamespacedEnum)
{
    EXPECT_EQ(::scl::enum_value(ns::Status::Ok), "Status::0");
    EXPECT_EQ(::scl::enum_value(ns::Status::Err), "Status::42");
}

/**
 * @test Verify that enum_value works for unscoped enums.
 */
TEST(EnumValueTest, UnscopedEnum)
{
    EXPECT_EQ(::scl::enum_value(ValA), "Unscoped::7");
}
