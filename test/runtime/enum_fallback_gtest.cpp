#include <gtest_utils.h>

// Takes the path without std::format: <version> is included first, so the include
// inside enum.h cannot define the macro again.
#include <version>
#undef __cpp_lib_format

#include <scl/utility/runtime/enum.h>

#ifdef __cpp_lib_format
#error "enum.h took the std::format path"
#endif

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

    enum class FbWide : wchar_t
    {
        B = 66,
    };

    enum class FbBool : bool
    {
        Yes = true,
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
 * @test Verify fallback path: unsigned char underlying type.
 */
TEST(EnumStringFallbackTest, UnderlyingByte)
{
    EXPECT_EQ(::scl::enum_string(FbByte::X), "FbByte::255");
}

/**
 * @test Verify fallback path: a character underlying type renders as a number.
 */
TEST(EnumStringFallbackTest, CharUnderlyingRendersNumber)
{
    EXPECT_EQ(::scl::enum_string(FbChar::A), "FbChar::65");
    EXPECT_EQ(::scl::enum_string(FbWide::B), "FbWide::66");
}

/**
 * @test Verify fallback path: a bool underlying type renders as a number.
 */
TEST(EnumStringFallbackTest, BoolUnderlyingRendersNumber)
{
    EXPECT_EQ(::scl::enum_string(FbBool::Yes), "FbBool::1");
}

/**
 * @test Verify fallback path: out-of-range value.
 */
TEST(EnumStringFallbackTest, OutOfRangeValue)
{
    EXPECT_EQ(::scl::enum_string(FbColor{42}), "FbColor::42");
}

/**
 * @test Verify fallback path: namespace qualifier stripped from type name.
 */
TEST(EnumStringFallbackTest, NamespacedEnum)
{
    EXPECT_EQ(::scl::enum_string(ns::FbStatus::Err), "FbStatus::42");
}
