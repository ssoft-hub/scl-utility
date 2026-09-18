#include <gtest_utils.h>

#include <scl/utility/hash/siphash.h>

#include <array>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>

using namespace ::std::string_view_literals;

namespace
{
    /// Standard test key from the SipHash paper (bytes 0x00..0x0f, little-endian).
    inline constexpr ::scl::hash::siphash_key test_key = ::scl::hash::siphash_default_key;

    /// Satisfied when @ref scl::hash::siphash accepts @p Range.
    template <typename Range>
    concept siphash_takes = requires(Range const & range) { ::scl::hash::siphash(range, test_key); };
} // namespace

using namespace ::scl::hash;

/**
 * @test Reference vector: empty string with the standard test key.
 *       Source: SipHash-2-4 reference implementation, Aumasson & Bernstein (2012).
 */
TEST(SipHashTest, ReferenceVectorEmpty)
{
    STATIC_EXPECT_EQ(siphash(::std::string_view{}, test_key), 0x726fdb47dd0e0e31ull);
}

/**
 * @test Reference vector: single byte 0x00 with the standard test key.
 */
TEST(SipHashTest, ReferenceVectorOneByte)
{
    constexpr char bytes[] = {'\x00'};
    STATIC_EXPECT_EQ(siphash(::std::string_view{bytes, 1}, test_key), 0x74f839c593dc67fdull);
}

/**
 * @test Identical inputs and keys produce identical results (determinism).
 */
TEST(SipHashTest, Deterministic)
{
    STATIC_EXPECT_EQ(siphash("hello"sv, test_key), siphash("hello"sv, test_key));
}

/**
 * @test Different inputs produce different hash values.
 */
TEST(SipHashTest, DifferentInputsDifferentHashes)
{
    STATIC_EXPECT_NE(siphash("hello"sv, test_key), siphash("world"sv, test_key));
    STATIC_EXPECT_NE(siphash("hello"sv, test_key), siphash(::std::string_view{}, test_key));
    STATIC_EXPECT_NE(siphash("ab"sv, test_key), siphash("ba"sv, test_key));
}

/**
 * @test Different keys produce different hash values for the same input
 *       (hash-flooding resistance property).
 */
TEST(SipHashTest, DifferentKeysDifferentHashes)
{
    constexpr siphash_key key_a{0xdeadbeefcafe0000ull, 0x0000cafebabe0001ull};
    constexpr siphash_key key_b{0xdeadbeefcafe0000ull, 0x0000cafebabe0002ull};
    STATIC_EXPECT_NE(siphash("hello"sv, key_a), siphash("hello"sv, key_b));
}

/**
 * @test Result type is std::uint64_t.
 */
TEST(SipHashTest, ResultType)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(siphash("hello"sv, test_key)), ::std::uint64_t>));
}

/**
 * @test Constexpr evaluation produces a non-zero value.
 */
TEST(SipHashTest, Constexpr) { STATIC_EXPECT_NE(siphash("constexpr"sv, test_key), 0ull); }

/**
 * @test One text produces one value, however it is spelled.
 */
TEST(SipHashTest, OneTextHashesAlikeHoweverSpelled)
{
    static constexpr ::std::array<char, 5> held{'h', 'e', 'l', 'l', 'o'};
    STATIC_EXPECT_EQ(siphash("hello"sv, test_key), siphash(held, test_key));
    STATIC_EXPECT_EQ(siphash("hello"sv, test_key), siphash(::std::span{held}, test_key));
    EXPECT_EQ(siphash("hello"sv, test_key), siphash(::std::string{"hello"}, test_key));
}

/**
 * @test An array is refused and a wider element with it, while a span over the array
 *       names the bytes the array holds.
 */
TEST(SipHashTest, ArrayIsRefusedAndASpanNamesItsBytes)
{
    static constexpr char raw[3]{'a', 'b', 'c'};
    STATIC_EXPECT_TRUE(siphash_takes<::std::string_view>);
    STATIC_EXPECT_FALSE(siphash_takes<char[3]>);
    STATIC_EXPECT_FALSE(siphash_takes<::std::u16string_view>);
    STATIC_EXPECT_EQ(siphash(::std::span{raw}, test_key), siphash(::std::string_view{"abc"}, test_key));
}

/**
 * @test A trailing zero is one more byte.
 */
TEST(SipHashTest, ATrailingZeroIsOneMoreByte)
{
    STATIC_EXPECT_NE(siphash(::std::string_view{"abc", 4}, test_key), siphash("abc"sv, test_key));
}

/**
 * @test siphash_hasher<Key> callable produces the same result as the free function.
 */
TEST(SipHashTest, HasherMatchesFreeFunction)
{
    STATIC_EXPECT_EQ(siphash_hasher<test_key>{}("hello"sv), siphash("hello"sv, test_key));
}

/**
 * @test Two siphash_hasher instantiations with different keys are distinct types.
 */
TEST(SipHashTest, DifferentKeysDifferentHasherTypes)
{
    constexpr siphash_key key_a{1ull, 2ull};
    constexpr siphash_key key_b{3ull, 4ull};
    STATIC_EXPECT_FALSE((::std::is_same_v<siphash_hasher<key_a>, siphash_hasher<key_b>>));
}
