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

enum class CharEnum : char
{
    A = 65,
};

enum class SignedCharEnum : signed char
{
    Low = -128,
};

enum class WideEnum : wchar_t
{
    B = 66,
};

enum class BoolEnum : bool
{
    Yes = true,
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
 * @test Verify that enum_string spells an unsigned char underlying value as a number.
 */
TEST(EnumStringTest, UnderlyingByteRendersNumber)
{
    EXPECT_EQ(::scl::enum_string(ByteEnum::X), "ByteEnum::255");
}

/**
 * @test Verify that a character underlying type renders as a number, not as a character.
 */
TEST(EnumStringTest, CharUnderlyingRendersNumber)
{
    EXPECT_EQ(::scl::enum_string(CharEnum::A), "CharEnum::65");
    EXPECT_EQ(::scl::enum_string(SignedCharEnum::Low), "SignedCharEnum::-128");
    EXPECT_EQ(::scl::enum_string(WideEnum::B), "WideEnum::66");
}

/**
 * @test Verify that a bool underlying type renders as a number.
 */
TEST(EnumStringTest, BoolUnderlyingRendersNumber)
{
    EXPECT_EQ(::scl::enum_string(BoolEnum::Yes), "BoolEnum::1");
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
