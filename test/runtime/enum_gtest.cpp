#include <gtest_utils.h>

#include <scl/utility/runtime/enum.h>

enum class Color : int
{
    Red = 1,
    Green = 2,
    Blue = -3,
};

enum class Flags : unsigned
{
    None = 0,
    A = 1,
    B = 2,
};

enum class ByteEnum : unsigned char
{
    X = 255,
};

namespace ns
{
    enum class Status : int
    {
        Ok = 0,
        Err = 42,
    };
} // namespace ns

enum Unscoped : int // NOLINT(performance-enum-size)
{
    ValA = 7,
};

/**
 * @test Verify that enum_string formats a scoped enum with int underlying type.
 */
TEST(EnumStringTest, ScopedIntPositive)
{
    EXPECT_EQ(::scl::enum_string(Color::Red), "Color::1");
    EXPECT_EQ(::scl::enum_string(Color::Green), "Color::2");
}

/**
 * @test Verify that enum_string formats negative underlying values correctly.
 */
TEST(EnumStringTest, ScopedIntNegative) { EXPECT_EQ(::scl::enum_string(Color::Blue), "Color::-3"); }

/**
 * @test Verify that enum_string formats a scoped enum with unsigned underlying type.
 */
TEST(EnumStringTest, ScopedUnsigned)
{
    EXPECT_EQ(::scl::enum_string(Flags::None), "Flags::0");
    EXPECT_EQ(::scl::enum_string(Flags::B), "Flags::2");
}

/**
 * @test Verify that enum_string promotes unsigned char underlying to unsigned and formats correctly.
 */
TEST(EnumStringTest, UnderlyingBytePromotedToUnsigned)
{
    EXPECT_EQ(::scl::enum_string(ByteEnum::X), "ByteEnum::255");
}

/**
 * @test Verify that enum_string handles an out-of-range (unnamed) enum value.
 */
TEST(EnumStringTest, OutOfRangeValue) { EXPECT_EQ(::scl::enum_string(Color{42}), "Color::42"); }

/**
 * @test Verify that enum_string strips namespace qualifiers from the type name.
 */
TEST(EnumStringTest, NamespacedEnum)
{
    EXPECT_EQ(::scl::enum_string(ns::Status::Ok), "Status::0");
    EXPECT_EQ(::scl::enum_string(ns::Status::Err), "Status::42");
}

/**
 * @test Verify that enum_string works for unscoped enums.
 */
TEST(EnumStringTest, UnscopedEnum) { EXPECT_EQ(::scl::enum_string(ValA), "Unscoped::7"); }
