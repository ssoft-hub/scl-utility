#include <gtest_utils.h>

// Force the fallback implementation regardless of std::format availability.
// This TU is compiled independently: #pragma once in enum.h does not block
// re-inclusion because this is a separate translation unit with its own
// preprocessor state.
#undef __cpp_lib_format

#include <scl/utility/runtime/enum.h>

namespace
{
    enum class FbColor : int
    {
        Red  = 1,
        Blue = -3,
    };

    enum class FbFlags : unsigned
    {
        B = 2,
    };

    enum class FbByte : unsigned char
    {
        X = 255,
    };

    namespace ns
    {
        enum class FbStatus : int
        {
            Err = 42,
        };
    } // namespace ns

} // namespace

/**
 * @test Verify fallback path: positive int underlying value.
 */
TEST(EnumValueFallbackTest, ScopedIntPositive)
{
    EXPECT_EQ(::scl::enum_value(FbColor::Red), "FbColor::1");
}

/**
 * @test Verify fallback path: negative int underlying value.
 */
TEST(EnumValueFallbackTest, ScopedIntNegative)
{
    EXPECT_EQ(::scl::enum_value(FbColor::Blue), "FbColor::-3");
}

/**
 * @test Verify fallback path: unsigned underlying type.
 */
TEST(EnumValueFallbackTest, ScopedUnsigned)
{
    EXPECT_EQ(::scl::enum_value(FbFlags::B), "FbFlags::2");
}

/**
 * @test Verify fallback path: unsigned char underlying type.
 */
TEST(EnumValueFallbackTest, UnderlyingByte)
{
    EXPECT_EQ(::scl::enum_value(FbByte::X), "FbByte::255");
}

/**
 * @test Verify fallback path: out-of-range value.
 */
TEST(EnumValueFallbackTest, OutOfRangeValue)
{
    EXPECT_EQ(::scl::enum_value(FbColor{42}), "FbColor::42");
}

/**
 * @test Verify fallback path: namespace qualifier stripped from type name.
 */
TEST(EnumValueFallbackTest, NamespacedEnum)
{
    EXPECT_EQ(::scl::enum_value(ns::FbStatus::Err), "FbStatus::42");
}
