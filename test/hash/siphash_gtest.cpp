#include <gtest_utils.h>

#include <scl/utility/hash/siphash.h>

#include <cstdint>
#include <string_view>
#include <type_traits>

using namespace ::scl::hash;

/// Standard test key from the SipHash paper (bytes 0x00..0x0f, little-endian).
inline constexpr siphash_key test_key = siphash_default_key;

/**
 * @test Reference vector: empty string with the standard test key.
 *       Source: SipHash-2-4 reference implementation, Aumasson & Bernstein (2012).
 */
TEST(SipHashTest, ReferenceVectorEmpty)
{
    STATIC_EXPECT_EQ(siphash(::std::string_view{}, test_key), 0x726fdb47dd0e0e31ULL);
}

/**
 * @test Reference vector: single byte 0x00 with the standard test key.
 */
TEST(SipHashTest, ReferenceVectorOneByte)
{
    constexpr char bytes[] = {'\x00'};
    STATIC_EXPECT_EQ(siphash(::std::string_view{bytes, 1}, test_key), 0x74f839c593dc67fdULL);
}

/**
 * @test Identical inputs and keys produce identical results (determinism).
 */
TEST(SipHashTest, Deterministic)
{
    STATIC_EXPECT_EQ(siphash("hello", test_key), siphash("hello", test_key));
}

/**
 * @test Different inputs produce different hash values.
 */
TEST(SipHashTest, DifferentInputsDifferentHashes)
{
    STATIC_EXPECT_NE(siphash("hello", test_key), siphash("world", test_key));
    STATIC_EXPECT_NE(siphash("hello", test_key), siphash(::std::string_view{}, test_key));
    STATIC_EXPECT_NE(siphash("ab", test_key), siphash("ba", test_key));
}

/**
 * @test Different keys produce different digests for the same input
 *       (hash-flooding resistance property).
 */
TEST(SipHashTest, DifferentKeysDifferentHashes)
{
    constexpr siphash_key key_a{0xdeadbeefcafe0000ULL, 0x0000cafebabe0001ULL};
    constexpr siphash_key key_b{0xdeadbeefcafe0000ULL, 0x0000cafebabe0002ULL};
    STATIC_EXPECT_NE(siphash("hello", key_a), siphash("hello", key_b));
}

/**
 * @test Result type is std::uint64_t.
 */
TEST(SipHashTest, ResultType)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(siphash("hello", test_key)), ::std::uint64_t>));
}

/**
 * @test Constexpr evaluation produces a non-zero value.
 */
TEST(SipHashTest, Constexpr) { STATIC_EXPECT_NE(siphash("constexpr", test_key), 0ULL); }

/**
 * @test siphash_hasher<Key> callable produces the same result as the free function.
 */
TEST(SipHashTest, HasherMatchesFreeFunction)
{
    STATIC_EXPECT_EQ(siphash_hasher<test_key>{}("hello"), siphash("hello", test_key));
}

/**
 * @test Two siphash_hasher instantiations with different keys are distinct types.
 */
TEST(SipHashTest, DifferentKeysDifferentHasherTypes)
{
    constexpr siphash_key key_a{1ULL, 2ULL};
    constexpr siphash_key key_b{3ULL, 4ULL};
    STATIC_EXPECT_FALSE((::std::is_same_v<siphash_hasher<key_a>, siphash_hasher<key_b>>));
}
