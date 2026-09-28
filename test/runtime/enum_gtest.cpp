#include <gtest_utils.h>

#include <array>
#include <bitset>
#include <charconv>
#include <climits>
#include <cstdint>
#include <functional>
#include <string>
#include <type_traits>

#include <scl/utility/runtime/enum.h>

namespace
{
    // Spells a number in hexadecimal with a 0x prefix, through to_chars on every library.
    struct hex_format
    {
        template <typename Number>
        [[nodiscard]]
        ::std::string operator()(Number number) const
        {
            ::std::array<char, 32> buf{};
            auto const end = ::std::to_chars(buf.data(), buf.data() + buf.size(), number, 16).ptr;
            return "0x" + ::std::string{buf.data(), end};
        }
    };

    // A call operator overloaded on the number and on a string, which has no single signature.
    struct overloaded_format
    {
        [[nodiscard]]
        ::std::string operator()(::std::uint8_t number) const
        {
            return ::std::to_string(number);
        }

        [[nodiscard]]
        ::std::string operator()(::std::string const & text) const
        {
            return text;
        }
    };

    template <typename Enum, typename Format>
    concept accepts_format = requires(Enum value,
        Format && format) { ::scl::enum_string(value, static_cast<Format &&>(format)); };

    // Takes the function object through the concept written as a type-constraint.
    template <typename Enum, ::scl::concepts::enum_string_format<Enum> Format>
    constexpr bool takes_format_by_constraint = true;

    // Tells whether the function object received the number as a value of the type Expected.
    template <typename Expected>
    auto const receives = [](auto number) {
        return ::std::is_same_v<decltype(number), Expected> ? "same" : "other";
    };

    [[nodiscard]]
    ::std::string spell_wide(::std::uint32_t number)
    {
        return ::std::to_string(number);
    }

    // The type of a function whose parameter is narrower than a char16_t number.
    using narrow_function = ::std::string(::std::uint8_t);
} // namespace

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

enum class Char8Enum : char8_t
{
    X = 200,
};

enum class Char16Enum : char16_t
{
    Max = 0xFFFF,
};

enum class Char32Enum : char32_t
{
    Max = 0x10FFFF,
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
    EXPECT_EQ(::scl::enum_string(Char8Enum::X), "Char8Enum::200");
    EXPECT_EQ(::scl::enum_string(Char16Enum::Max), "Char16Enum::65535");
    EXPECT_EQ(::scl::enum_string(Char32Enum::Max), "Char32Enum::1114111");
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

/**
 * @test Verify that a function object spells the number in the base it chooses.
 */
TEST(EnumStringTest, FormatSpellsNumber)
{
    EXPECT_EQ(::scl::enum_string(Flags::B, hex_format{}), "Flags::0x2");
}

/**
 * @test Verify that the function object may return a pointer to characters or a reference to a string.
 */
TEST(EnumStringTest, FormatReturnsAnyStringView)
{
    static ::std::string const text{"two"};
    EXPECT_EQ(::scl::enum_string(Flags::B, [](auto) { return "two"; }), "Flags::two");
    EXPECT_EQ(::scl::enum_string(Flags::B, [](auto) -> ::std::string const & { return text; }), "Flags::two");
    EXPECT_EQ(::scl::enum_string(Flags::B, [](auto) { return ::std::string_view{"two"}; }), "Flags::two");
}

/**
 * @test Verify that a function object whose call operator is not const is accepted.
 */
TEST(EnumStringTest, FormatMutableAccepted)
{
    auto counted = [calls = 0](auto) mutable { return ++calls == 1 ? "first" : "again"; };
    EXPECT_EQ(::scl::enum_string(Flags::B, counted), "Flags::first");
    EXPECT_EQ(::scl::enum_string(Flags::B, counted), "Flags::again");
    EXPECT_EQ(
        ::scl::enum_string(Flags::B,
            [calls = 0](auto) mutable { return ++calls == 1 ? "first" : "again"; }),
        "Flags::first");

    auto wide_mutable = [calls = 0](::std::uint32_t) mutable {
        return ++calls == 1 ? "first" : "again";
    };
    STATIC_EXPECT_TRUE((accepts_format<Char16Enum, decltype(wide_mutable) &>));
    EXPECT_EQ(::scl::enum_string(Char16Enum::Max, wide_mutable), "Char16Enum::first");
}

/**
 * @test Verify that the function object receives an integer type of the width and signedness of the underlying type.
 */
TEST(EnumStringTest, FormatReceivesIntegerOfUnderlyingType)
{
    using char_number = ::std::conditional_t<::std::is_signed_v<char>, signed char, unsigned char>;
    using wide_number = ::std::conditional_t<::std::is_signed_v<wchar_t>,
        ::std::make_signed_t<wchar_t>, ::std::make_unsigned_t<wchar_t>>;
    EXPECT_EQ(::scl::enum_string(Color::Blue, receives<int>), "Color::same");
    EXPECT_EQ(::scl::enum_string(Flags::B, receives<unsigned>), "Flags::same");
    EXPECT_EQ(::scl::enum_string(ByteEnum::X, receives<unsigned char>), "ByteEnum::same");
    EXPECT_EQ(::scl::enum_string(SignedCharEnum::Low, receives<signed char>), "SignedCharEnum::same");
    EXPECT_EQ(::scl::enum_string(CharEnum::A, receives<char_number>), "CharEnum::same");
    EXPECT_EQ(::scl::enum_string(WideEnum::B, receives<wide_number>), "WideEnum::same");
    EXPECT_EQ(::scl::enum_string(Char8Enum::X, receives<unsigned char>), "Char8Enum::same");
    EXPECT_EQ(::scl::enum_string(Char16Enum::Max, receives<::std::make_unsigned_t<char16_t>>), "Char16Enum::same");
    EXPECT_EQ(::scl::enum_string(Char32Enum::Max, receives<::std::make_unsigned_t<char32_t>>), "Char32Enum::same");
    EXPECT_EQ(::scl::enum_string(BoolEnum::Yes, receives<unsigned char>), "BoolEnum::same");
}

/**
 * @test Verify that a function object whose parameter is an integer, bool or character type no narrower and of the same signedness is accepted.
 */
TEST(EnumStringTest, FormatParameterNoNarrowerAccepted)
{
    auto const unsigned_32 = [](::std::uint32_t number) { return ::std::to_string(number); };
    auto const by_reference = [](::std::uint16_t const & number) { return ::std::to_string(number); };
    auto const any_type = [](auto number) { return ::std::to_string(number); };
    auto const signed_64 = [](::std::int64_t number) { return ::std::to_string(number); };
    auto const char_number = [](::std::conditional_t<::std::is_signed_v<char>, signed char, unsigned char>) {
        return "";
    };
    auto const plain_char = [](char) { return ""; };
    auto const same_character = [](char16_t) { return "character"; };
    auto const wider_character = [](char32_t) { return ""; };
    auto const boolean = [](bool flag) { return flag ? "true" : "false"; };
    auto const unsigned_16 = [](::std::uint16_t number) { return ::std::to_string(number); };
    auto const wide_character = [](wchar_t) { return ""; };
    auto const any_arguments = [](...) { return "any"; };
    STATIC_EXPECT_TRUE((accepts_format<Char16Enum, decltype(unsigned_32)>));
    STATIC_EXPECT_TRUE((accepts_format<Char16Enum, decltype(unsigned_16)>));
    STATIC_EXPECT_TRUE((accepts_format<WideEnum, decltype(wide_character)>));
    STATIC_EXPECT_TRUE((accepts_format<Char16Enum, decltype(any_arguments)>));
    STATIC_EXPECT_TRUE((accepts_format<Char16Enum, ::std::string (*)(...)>));
    STATIC_EXPECT_TRUE((accepts_format<Char16Enum, decltype(by_reference)>));
    STATIC_EXPECT_TRUE((accepts_format<Char16Enum, decltype(any_type)>));
    STATIC_EXPECT_TRUE((accepts_format<Char16Enum, decltype(&spell_wide)>));
    STATIC_EXPECT_TRUE((accepts_format<Char16Enum, ::std::string (*)(::std::uint32_t, ...)>));
    STATIC_EXPECT_TRUE((accepts_format<Color, decltype(signed_64)>));
    STATIC_EXPECT_TRUE((accepts_format<CharEnum, decltype(char_number)>));
    STATIC_EXPECT_TRUE((accepts_format<CharEnum, decltype(plain_char)>));
    STATIC_EXPECT_TRUE((accepts_format<Char16Enum, decltype(same_character)>));
    STATIC_EXPECT_TRUE((accepts_format<Char16Enum, decltype(wider_character)>));
    STATIC_EXPECT_TRUE((accepts_format<BoolEnum, decltype(boolean)>));
    STATIC_EXPECT_TRUE((accepts_format<BoolEnum, decltype(unsigned_32)>));
    EXPECT_EQ(::scl::enum_string(Char16Enum::Max, same_character), "Char16Enum::character");
    EXPECT_EQ(::scl::enum_string(BoolEnum::Yes, boolean), "BoolEnum::true");
    EXPECT_EQ(::scl::enum_string(Char16Enum::Max, any_arguments), "Char16Enum::any");
    EXPECT_EQ(::scl::enum_string(Char16Enum::Max, unsigned_32), "Char16Enum::65535");
    EXPECT_EQ(::scl::enum_string(Char16Enum::Max, spell_wide), "Char16Enum::65535");
}

/**
 * @test Verify that a function object whose parameter is narrower, of the other signedness, not of an integer type or a reference to non-const is refused.
 */
TEST(EnumStringTest, FormatParameterOutsideRuleRefused)
{
    auto const unsigned_8 = [](::std::uint8_t number) { return ::std::to_string(number); };
    auto const signed_32 = [](::std::int32_t number) { return ::std::to_string(number); };
    auto const narrow_character = [](char16_t) { return ""; };
    auto const boolean = [](bool) { return ""; };
    auto const mutable_reference = [](::std::uint32_t &) { return ""; };
    auto const floating = [](double) { return ""; };
    auto const other_char = [](::std::conditional_t<::std::is_signed_v<char>, unsigned char, signed char>) {
        return "";
    };
    auto const other_wide =
        [](::std::conditional_t<::std::is_signed_v<wchar_t>, ::std::make_unsigned_t<wchar_t>,
            ::std::make_signed_t<wchar_t>>) { return ""; };
    auto narrow_mutable = [calls = 0](::std::uint8_t) mutable {
        return ++calls == 1 ? "first" : "again";
    };
    STATIC_EXPECT_FALSE((accepts_format<Char16Enum, decltype(unsigned_8)>));
    STATIC_EXPECT_FALSE((accepts_format<Char16Enum, decltype(signed_32)>));
    STATIC_EXPECT_FALSE((accepts_format<Char32Enum, decltype(narrow_character)>));
    STATIC_EXPECT_FALSE((accepts_format<Char16Enum, narrow_function *>));
    STATIC_EXPECT_FALSE((accepts_format<Char16Enum, narrow_function &>));
    STATIC_EXPECT_FALSE((accepts_format<Char16Enum, decltype(mutable_reference)>));
    STATIC_EXPECT_FALSE((accepts_format<Char16Enum, decltype(narrow_mutable) &>));
    STATIC_EXPECT_FALSE((accepts_format<ByteEnum, decltype(boolean)>));
    STATIC_EXPECT_FALSE((accepts_format<SignedCharEnum, decltype(&spell_wide)>));
    STATIC_EXPECT_FALSE((accepts_format<Char16Enum, ::std::string (*)(::std::uint8_t, ...)>));
    STATIC_EXPECT_FALSE((accepts_format<Char16Enum, decltype(floating)>));
    STATIC_EXPECT_FALSE((accepts_format<CharEnum, decltype(other_char)>));
    STATIC_EXPECT_FALSE((accepts_format<WideEnum, decltype(other_wide)>));
}

/**
 * @test Verify that the concept enum_string_format accepts what the call accepts and refuses what
 *       it refuses.
 */
TEST(EnumStringTest, FormatConceptMatchesCall)
{
    auto const unsigned_32 = [](::std::uint32_t number) { return ::std::to_string(number); };
    auto const unsigned_8 = [](::std::uint8_t number) { return ::std::to_string(number); };
    STATIC_EXPECT_TRUE((::scl::concepts::enum_string_format<decltype(unsigned_32), Char16Enum>));
    STATIC_EXPECT_TRUE((accepts_format<Char16Enum, decltype(unsigned_32)>));
    STATIC_EXPECT_FALSE((::scl::concepts::enum_string_format<decltype(unsigned_8), Char16Enum>));
    STATIC_EXPECT_FALSE((accepts_format<Char16Enum, decltype(unsigned_8)>));
    STATIC_EXPECT_FALSE((::scl::concepts::enum_string_format<decltype(unsigned_32), int>));
    STATIC_EXPECT_FALSE((accepts_format<int, decltype(unsigned_32)>));
    STATIC_EXPECT_TRUE((takes_format_by_constraint<Char16Enum, decltype(unsigned_32)>));
}

/**
 * @test Verify that a function object with no single signature is checked by the call alone.
 */
TEST(EnumStringTest, FormatWithoutSignatureCheckedByCall)
{
    STATIC_EXPECT_TRUE((accepts_format<Char16Enum, overloaded_format>));
    STATIC_EXPECT_TRUE((accepts_format<Char16Enum, hex_format>));
    auto const floating = [](double) { return ""; };
    STATIC_EXPECT_FALSE((accepts_format<Char16Enum, decltype(floating)>));
    STATIC_EXPECT_TRUE((accepts_format<Char16Enum, decltype(::std::ref(floating))>));
    STATIC_EXPECT_TRUE((accepts_format<Char16Enum, decltype(::std::bind_front(floating))>));
    EXPECT_EQ(::scl::enum_string(ByteEnum::X, overloaded_format{}), "ByteEnum::255");
}

/**
 * @test Verify that the function object spells the number in binary as wide as its type.
 */
TEST(EnumStringTest, FormatSpellsBinary)
{
    auto const binary = [](auto number) {
        return ::std::bitset<sizeof(number) * CHAR_BIT>(number).to_string();
    };
    EXPECT_EQ(::scl::enum_string(CharEnum::A, binary), "CharEnum::" + ::std::bitset<CHAR_BIT>(65).to_string());
    EXPECT_EQ(::scl::enum_string(SignedCharEnum::Low, binary),
        "SignedCharEnum::" + ::std::bitset<CHAR_BIT>(static_cast<unsigned char>(-128)).to_string());
}

/**
 * @test Verify that a function object whose result is no text is refused.
 */
TEST(EnumStringTest, FormatWithoutTextRefused)
{
    STATIC_EXPECT_TRUE((accepts_format<Color, hex_format>));
    STATIC_EXPECT_FALSE((accepts_format<Color, int (*)(long long)>));
}
