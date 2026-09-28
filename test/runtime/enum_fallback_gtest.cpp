#include <gtest_utils.h>

// Takes the path without std::format: <version> is included first, so the include
// inside enum.h cannot define the macro again.
#include <version>
#undef __cpp_lib_format

#include <scl/utility/runtime/enum.h>

#ifdef __cpp_lib_format
#error "enum.h took the std::format path"
#endif

#include <cstdint>
#include <limits>
#include <string>

namespace
{
    enum class FbColor : int
    {
        Red = 1,
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

    enum class FbChar : char
    {
        A = 65,
    };

    enum class FbSignedChar : signed char
    {
        Low = -128,
    };

    enum class FbChar16 : char16_t
    {
        Max = 0xFFFF,
    };

    enum class FbWide : wchar_t
    {
        B = 66,
    };

    enum class FbSigned64 : ::std::int64_t
    {
        Min = ::std::numeric_limits<::std::int64_t>::min(),
    };

    enum class FbUnsigned64 : ::std::uint64_t
    {
        Max = ::std::numeric_limits<::std::uint64_t>::max(),
    };

    enum class FbBool : bool
    {
        Yes = true,
    };

} // namespace

/**
 * @test Verify fallback path: positive int underlying value.
 */
TEST(EnumStringFallbackTest, ScopedIntPositive)
{
    EXPECT_EQ(::scl::enum_string(FbColor::Red), "FbColor::1");
}

/**
 * @test Verify fallback path: negative int underlying value.
 */
TEST(EnumStringFallbackTest, ScopedIntNegative)
{
    EXPECT_EQ(::scl::enum_string(FbColor::Blue), "FbColor::-3");
}

/**
 * @test Verify fallback path: unsigned underlying type.
 */
TEST(EnumStringFallbackTest, ScopedUnsigned)
{
    EXPECT_EQ(::scl::enum_string(FbFlags::B), "FbFlags::2");
}

/**
 * @test Verify fallback path: a character underlying type renders as a number.
 */
TEST(EnumStringFallbackTest, CharUnderlyingRendersNumber)
{
    EXPECT_EQ(::scl::enum_string(FbByte::X), "FbByte::255");
    EXPECT_EQ(::scl::enum_string(FbChar::A), "FbChar::65");
    EXPECT_EQ(::scl::enum_string(FbWide::B), "FbWide::66");
    EXPECT_EQ(::scl::enum_string(FbSignedChar::Low), "FbSignedChar::-128");
    EXPECT_EQ(::scl::enum_string(FbChar16::Max), "FbChar16::65535");
}

/**
 * @test Verify fallback path: a bool underlying type renders as a number.
 */
TEST(EnumStringFallbackTest, BoolUnderlyingRendersNumber)
{
    EXPECT_EQ(::scl::enum_string(FbBool::Yes), "FbBool::1");
}

/**
 * @test Verify fallback path: the extreme values of a 64-bit underlying type are spelled in full.
 */
TEST(EnumStringFallbackTest, ExtremeValues)
{
    EXPECT_EQ(::scl::enum_string(FbSigned64::Min), "FbSigned64::-9223372036854775808");
    EXPECT_EQ(::scl::enum_string(FbUnsigned64::Max), "FbUnsigned64::18446744073709551615");
}
