#include <gtest_utils.h>

#include <scl/utility/hash/fnv1a.h>

#include <array>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>
#include <vector>

using namespace ::std::string_view_literals;

namespace
{
    /// Satisfied when @ref scl::hash::fnv1a accepts @p Range.
    template <typename Range>
    concept fnv1a_takes = requires(Range const & range) { ::scl::hash::fnv1a(range); };
} // namespace

using namespace ::scl::hash;

/**
 * @test Empty range returns the FNV-1a 64-bit offset basis unchanged.
 */
TEST(Fnv1aTest, EmptyRangeReturnsOffsetBasis)
{
    STATIC_EXPECT_EQ(fnv1a(::std::string_view{}), 14695981039346656037ull);
    EXPECT_EQ(fnv1a(::std::vector<::std::uint8_t>{}), 14695981039346656037ull);
}

/**
 * @test Custom initial value is used as offset basis when provided.
 */
TEST(Fnv1aTest, EmptyRangeWithCustomInitial)
{
    STATIC_EXPECT_EQ(fnv1a(::std::string_view{}, 42ull), 42ull);
}

/**
 * @test Identical inputs produce identical results (determinism).
 */
TEST(Fnv1aTest, Deterministic)
{
    STATIC_EXPECT_EQ(fnv1a("hello, world"sv), fnv1a("hello, world"sv));
}

/**
 * @test Different inputs produce different hash values.
 */
TEST(Fnv1aTest, DifferentInputsDifferentHashes)
{
    STATIC_EXPECT_NE(fnv1a("hello"sv), fnv1a("world"sv));
    STATIC_EXPECT_NE(fnv1a("hello"sv), fnv1a(::std::string_view{}));
    STATIC_EXPECT_NE(fnv1a("ab"sv), fnv1a("ba"sv));
}

/**
 * @test Chaining two ranges equals hashing their concatenation.
 */
TEST(Fnv1aTest, ChainingEquivalentToConcatenation)
{
    STATIC_EXPECT_EQ(fnv1a(::std::string_view{"bar"}, fnv1a(::std::string_view{"foo"})),
        fnv1a(::std::string_view{"foobar"}));
}

/**
 * @test Result type is std::uint64_t.
 */
TEST(Fnv1aTest, ResultType)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(fnv1a("hello"sv)), ::std::uint64_t>));
}

/**
 * @test Constexpr evaluation produces a value distinct from the offset basis.
 */
TEST(Fnv1aTest, Constexpr) { STATIC_EXPECT_NE(fnv1a("constexpr"sv), 14695981039346656037ull); }

/**
 * @test std::vector<uint8_t> of the same bytes yields the same hash value as string_view.
 */
TEST(Fnv1aTest, VectorRange)
{
    ::std::vector<::std::uint8_t> const bytes{0x68, 0x65, 0x6c, 0x6c, 0x6f}; // "hello"
    EXPECT_EQ(fnv1a(bytes), fnv1a(::std::string_view{"hello"}));
}

/**
 * @test static_cast<uint8_t> prevents sign-extension for high-bit chars.
 */
TEST(Fnv1aTest, SignedCharEquivalentToUnsigned)
{
    ::std::string const s{"\x68\x65\x6c\x6c\x6f"sv};
    ::std::vector<::std::uint8_t> const bytes{0x68, 0x65, 0x6c, 0x6c, 0x6f};
    EXPECT_EQ(fnv1a(::std::string_view{s}), fnv1a(bytes));
}

/**
 * @test One text produces one value, however it is spelled.
 */
TEST(Fnv1aTest, OneTextHashesAlikeHoweverSpelled)
{
    static constexpr ::std::array<char, 5> held{'h', 'e', 'l', 'l', 'o'};
    STATIC_EXPECT_EQ(fnv1a("hello"sv), fnv1a(held));
    STATIC_EXPECT_EQ(fnv1a("hello"sv), fnv1a(::std::span{held}));
    EXPECT_EQ(fnv1a("hello"sv), fnv1a(::std::string{"hello"}));
    EXPECT_EQ(fnv1a("hello"sv), fnv1a(::std::vector<char>{'h', 'e', 'l', 'l', 'o'}));
}

/**
 * @test An array is refused and a wider element with it, while a span over the array
 *       names the bytes the array holds.
 */
TEST(Fnv1aTest, ArrayIsRefusedAndASpanNamesItsBytes)
{
    static constexpr char raw[3]{'a', 'b', 'c'};
    STATIC_EXPECT_TRUE(fnv1a_takes<::std::string_view>);
    STATIC_EXPECT_FALSE(fnv1a_takes<char[3]>);
    STATIC_EXPECT_FALSE(fnv1a_takes<::std::u16string_view>);
    STATIC_EXPECT_EQ(fnv1a(::std::span{raw}), fnv1a(::std::string_view{"abc"}));
}

/**
 * @test A UTF-8 spelling carries the same bytes as a narrow one.
 */
TEST(Fnv1aTest, Utf8SpellingCarriesTheSameBytes)
{
    STATIC_EXPECT_EQ(fnv1a(u8"hello"sv), fnv1a(::std::string_view{"hello"}));
}

/**
 * @test A trailing zero is one more byte.
 */
TEST(Fnv1aTest, ATrailingZeroIsOneMoreByte)
{
    STATIC_EXPECT_NE(fnv1a(::std::string_view{"abc", 4}), fnv1a("abc"sv));
}

/**
 * @test fnv1a_hasher callable produces the same result as the free function.
 */
TEST(Fnv1aTest, HasherMatchesFreeFunction)
{
    STATIC_EXPECT_EQ(fnv1a_hasher{}("hello"sv), fnv1a("hello"sv));
}

/**
 * @test The value the FNV-1a specification gives for the three bytes of "abc", with the
 *       64-bit offset basis and prime that specification fixes.
 */
TEST(Fnv1aTest, ThreeBytesAnswerTheSpecifiedValue)
{
    STATIC_EXPECT_EQ(fnv1a("abc"sv), 16654208175385433931ull);
}
