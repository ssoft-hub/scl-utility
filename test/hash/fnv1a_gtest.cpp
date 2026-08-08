#include <gtest_utils.h>

#include <scl/utility/hash/fnv1a.h>

#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

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
TEST(Fnv1aTest, Deterministic) { STATIC_EXPECT_EQ(fnv1a("hello, world"), fnv1a("hello, world")); }

/**
 * @test Different inputs produce different hash values.
 */
TEST(Fnv1aTest, DifferentInputsDifferentHashes)
{
    STATIC_EXPECT_NE(fnv1a("hello"), fnv1a("world"));
    STATIC_EXPECT_NE(fnv1a("hello"), fnv1a(::std::string_view{}));
    STATIC_EXPECT_NE(fnv1a("ab"), fnv1a("ba"));
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
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(fnv1a("hello")), ::std::uint64_t>));
}

/**
 * @test Constexpr evaluation produces a value distinct from the offset basis.
 */
TEST(Fnv1aTest, Constexpr) { STATIC_EXPECT_NE(fnv1a("constexpr"), 14695981039346656037ull); }

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
    ::std::string const s{"\x68\x65\x6c\x6c\x6f"};
    ::std::vector<::std::uint8_t> const bytes{0x68, 0x65, 0x6c, 0x6c, 0x6f};
    EXPECT_EQ(fnv1a(::std::string_view{s}), fnv1a(bytes));
}

/**
 * @test A string literal is hashed as its text — every spelling of it agrees.
 */
TEST(Fnv1aTest, LiteralHashedWithoutTerminatingZero)
{
    STATIC_EXPECT_EQ(fnv1a("hello"), fnv1a(::std::string_view{"hello"}));
    EXPECT_EQ(fnv1a("hello"), fnv1a(::std::string{"hello"}));
}

/**
 * @test An array that does not end in zero keeps every byte.
 */
TEST(Fnv1aTest, ArrayWithoutTerminatingZeroHashedWhole)
{
    static constexpr char raw[3]{'a', 'b', 'c'};
    STATIC_EXPECT_EQ(fnv1a(raw), fnv1a(::std::string_view{"abc"}));
}

/**
 * @test A UTF-8 literal follows the same rule as a narrow one.
 */
TEST(Fnv1aTest, Utf8LiteralHashedWithoutTerminatingZero)
{
    STATIC_EXPECT_EQ(fnv1a(u8"hello"), fnv1a(::std::string_view{"hello"}));
}

/**
 * @test A byte array keeps every byte, a zero at its end included.
 */
TEST(Fnv1aTest, ByteArrayKeepsTrailingZero)
{
    static constexpr ::std::uint8_t data[4]{1, 2, 3, 0};
    static constexpr ::std::array<::std::uint8_t, 4> same{1, 2, 3, 0};
    STATIC_EXPECT_EQ(fnv1a(data), fnv1a(same));
}

/**
 * @test fnv1a_hasher callable produces the same result as the free function.
 */
TEST(Fnv1aTest, HasherMatchesFreeFunction)
{
    STATIC_EXPECT_EQ(fnv1a_hasher{}("hello"), fnv1a("hello"));
}
