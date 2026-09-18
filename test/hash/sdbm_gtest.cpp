#include <gtest_utils.h>

#include <scl/utility/hash/sdbm.h>

#include <array>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>
#include <vector>

using namespace ::std::string_view_literals;

namespace
{
    /// Satisfied when @ref scl::hash::sdbm accepts @p Range.
    template <typename Range>
    concept sdbm_takes = requires(Range const & range) { ::scl::hash::sdbm(range); };
} // namespace

using namespace ::scl::hash;

/**
 * @test Empty range returns the SDBM seed (0) unchanged.
 */
TEST(SdbmTest, EmptyRangeReturnsSeed) { STATIC_EXPECT_EQ(sdbm(::std::string_view{}), 0ull); }

/**
 * @test Custom initial seed is preserved for an empty range.
 */
TEST(SdbmTest, EmptyRangeWithCustomSeed)
{
    STATIC_EXPECT_EQ(sdbm(::std::string_view{}, 42ull), 42ull);
}

/**
 * @test Identical inputs produce identical results (determinism).
 */
TEST(SdbmTest, Deterministic) { STATIC_EXPECT_EQ(sdbm("hello"sv), sdbm("hello"sv)); }

/**
 * @test Different inputs produce different hash values.
 */
TEST(SdbmTest, DifferentInputsDifferentHashes)
{
    STATIC_EXPECT_NE(sdbm("hello"sv), sdbm("world"sv));
    STATIC_EXPECT_NE(sdbm("hello"sv), sdbm(::std::string_view{}));
    STATIC_EXPECT_NE(sdbm("ab"sv), sdbm("ba"sv));
}

/**
 * @test Chaining two ranges equals hashing their concatenation.
 */
TEST(SdbmTest, ChainingEquivalentToConcatenation)
{
    STATIC_EXPECT_EQ(sdbm(::std::string_view{"bar"}, sdbm(::std::string_view{"foo"})),
        sdbm(::std::string_view{"foobar"}));
}

/**
 * @test Result type is std::uint64_t.
 */
TEST(SdbmTest, ResultType)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(sdbm("hello"sv)), ::std::uint64_t>));
}

/**
 * @test Constexpr evaluation produces a value distinct from the seed.
 */
TEST(SdbmTest, Constexpr) { STATIC_EXPECT_NE(sdbm("constexpr"sv), 0ull); }

/**
 * @test One text produces one value, however it is spelled.
 */
TEST(SdbmTest, OneTextHashesAlikeHoweverSpelled)
{
    static constexpr ::std::array<char, 5> held{'h', 'e', 'l', 'l', 'o'};
    STATIC_EXPECT_EQ(sdbm("hello"sv), sdbm(held));
    STATIC_EXPECT_EQ(sdbm("hello"sv), sdbm(::std::span{held}));
    EXPECT_EQ(sdbm("hello"sv), sdbm(::std::string{"hello"}));
    EXPECT_EQ(sdbm("hello"sv), sdbm(::std::vector<char>{'h', 'e', 'l', 'l', 'o'}));
}

/**
 * @test An array is refused and a wider element with it, while a span over the array
 *       names the bytes the array holds.
 */
TEST(SdbmTest, ArrayIsRefusedAndASpanNamesItsBytes)
{
    static constexpr char raw[3]{'a', 'b', 'c'};
    STATIC_EXPECT_TRUE(sdbm_takes<::std::string_view>);
    STATIC_EXPECT_FALSE(sdbm_takes<char[3]>);
    STATIC_EXPECT_FALSE(sdbm_takes<::std::u16string_view>);
    STATIC_EXPECT_EQ(sdbm(::std::span{raw}), sdbm(::std::string_view{"abc"}));
}

/**
 * @test A trailing zero is one more byte.
 */
TEST(SdbmTest, ATrailingZeroIsOneMoreByte)
{
    STATIC_EXPECT_NE(sdbm(::std::string_view{"abc", 4}), sdbm("abc"sv));
}

/**
 * @test sdbm_hasher callable produces the same result as the free function.
 */
TEST(SdbmTest, HasherMatchesFreeFunction)
{
    STATIC_EXPECT_EQ(sdbm_hasher{}("hello"sv), sdbm("hello"sv));
}

/**
 * @test The value the SDBM recurrence gives for the three bytes of "abc".
 */
TEST(SdbmTest, ThreeBytesAnswerTheSpecifiedValue)
{
    STATIC_EXPECT_EQ(sdbm("abc"sv), 417419622498ull);
}
