#include <gtest_utils.h>

#include <scl/utility/hash/djb2.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>

using namespace ::scl::hash;

/**
 * @test Empty range returns the djb2 seed (5381) unchanged.
 */
TEST(Djb2Test, EmptyRangeReturnsSeed) { STATIC_EXPECT_EQ(djb2(::std::string_view{}), 5381ull); }

/**
 * @test Custom initial seed is preserved for an empty range.
 */
TEST(Djb2Test, EmptyRangeWithCustomSeed)
{
    STATIC_EXPECT_EQ(djb2(::std::string_view{}, 99ull), 99ull);
}

/**
 * @test Identical inputs produce identical results (determinism).
 */
TEST(Djb2Test, Deterministic) { STATIC_EXPECT_EQ(djb2("hello"), djb2("hello")); }

/**
 * @test Different inputs produce different hash values.
 */
TEST(Djb2Test, DifferentInputsDifferentHashes)
{
    STATIC_EXPECT_NE(djb2("hello"), djb2("world"));
    STATIC_EXPECT_NE(djb2("hello"), djb2(::std::string_view{}));
    STATIC_EXPECT_NE(djb2("ab"), djb2("ba"));
}

/**
 * @test Chaining two ranges equals hashing their concatenation.
 */
TEST(Djb2Test, ChainingEquivalentToConcatenation)
{
    STATIC_EXPECT_EQ(djb2(::std::string_view{"bar"}, djb2(::std::string_view{"foo"})),
        djb2(::std::string_view{"foobar"}));
}

/**
 * @test Result type is std::uint64_t.
 */
TEST(Djb2Test, ResultType)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(djb2("hello")), ::std::uint64_t>));
}

/**
 * @test Constexpr evaluation produces a value distinct from the seed.
 */
TEST(Djb2Test, Constexpr) { STATIC_EXPECT_NE(djb2("constexpr"), 5381ull); }

/**
 * @test A string literal is hashed as its text — every spelling of it agrees.
 */
TEST(Djb2Test, LiteralHashedWithoutTerminatingZero)
{
    STATIC_EXPECT_EQ(djb2("hello"), djb2(::std::string_view{"hello"}));
    EXPECT_EQ(djb2("hello"), djb2(::std::string{"hello"}));
}

/**
 * @test An array that does not end in zero keeps every byte.
 */
TEST(Djb2Test, ArrayWithoutTerminatingZeroHashedWhole)
{
    static constexpr char raw[3]{'a', 'b', 'c'};
    STATIC_EXPECT_EQ(djb2(raw), djb2(::std::string_view{"abc"}));
}

/**
 * @test A byte array keeps every byte, a zero at its end included.
 */
TEST(Djb2Test, ByteArrayKeepsTrailingZero)
{
    static constexpr ::std::uint8_t data[4]{1, 2, 3, 0};
    static constexpr ::std::array<::std::uint8_t, 4> same{1, 2, 3, 0};
    STATIC_EXPECT_EQ(djb2(data), djb2(same));
}

/**
 * @test djb2_hasher callable produces the same result as the free function.
 */
TEST(Djb2Test, HasherMatchesFreeFunction)
{
    STATIC_EXPECT_EQ(djb2_hasher{}("hello"), djb2("hello"));
}

/**
 * @test A sequence filled at run time is spelled as a view, and hashes to the value the
 *       array spelling of the same bytes gives.
 */
TEST(Djb2Test, RunTimeViewMatchesArraySpelling)
{
    char text[8]{};
    ::std::ranges::copy(::std::string_view{"hello"}, text);
    EXPECT_EQ(djb2(::std::string_view{text, 5}), djb2("hello"));

    ::std::uint8_t data[4]{};
    for (auto index = ::std::size_t{0}; index < 4; ++index)
        data[index] = static_cast<::std::uint8_t>(index);

    static constexpr ::std::uint8_t expected[4]{0, 1, 2, 3};
    EXPECT_EQ(djb2(::std::span{data}), djb2(expected));
}
