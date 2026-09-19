#include <gtest_utils.h>

#include <scl/utility/hash/djb2.h>

#include <array>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>
#include <vector>

using namespace ::std::string_view_literals;

namespace
{
    /// Satisfied when @ref scl::hash::djb2 accepts @p Range.
    template <typename Range>
    concept djb2_takes = requires(Range const & range) { ::scl::hash::djb2(range); };
} // namespace

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
TEST(Djb2Test, Deterministic) { STATIC_EXPECT_EQ(djb2("hello"sv), djb2("hello"sv)); }

/**
 * @test Different inputs produce different hash values.
 */
TEST(Djb2Test, DifferentInputsDifferentHashes)
{
    STATIC_EXPECT_NE(djb2("hello"sv), djb2("world"sv));
    STATIC_EXPECT_NE(djb2("hello"sv), djb2(::std::string_view{}));
    STATIC_EXPECT_NE(djb2("ab"sv), djb2("ba"sv));
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
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(djb2("hello"sv)), ::std::uint64_t>));
}

/**
 * @test Constexpr evaluation produces a value distinct from the seed.
 */
TEST(Djb2Test, Constexpr) { STATIC_EXPECT_NE(djb2("constexpr"sv), 5381ull); }

/**
 * @test One text produces one value, however it is spelled.
 */
TEST(Djb2Test, OneTextHashesAlikeHoweverSpelled)
{
    static constexpr ::std::array<char, 5> held{'h', 'e', 'l', 'l', 'o'};
    STATIC_EXPECT_EQ(djb2("hello"sv), djb2(held));
    STATIC_EXPECT_EQ(djb2("hello"sv), djb2(::std::span{held}));
    EXPECT_EQ(djb2("hello"sv), djb2(::std::string{"hello"}));
    EXPECT_EQ(djb2("hello"sv), djb2(::std::vector<char>{'h', 'e', 'l', 'l', 'o'}));
}

/**
 * @test An array is refused and a wider element with it, while a span over the array
 *       names the bytes the array holds.
 */
TEST(Djb2Test, ArrayIsRefusedAndASpanNamesItsBytes)
{
    static constexpr char raw[3]{'a', 'b', 'c'};
    STATIC_EXPECT_TRUE(djb2_takes<::std::string_view>);
    STATIC_EXPECT_FALSE(djb2_takes<char[3]>);
    STATIC_EXPECT_FALSE(djb2_takes<::std::u16string_view>);
    STATIC_EXPECT_EQ(djb2(::std::span{raw}), djb2(::std::string_view{"abc"}));
}

/**
 * @test A trailing zero is one more byte.
 */
TEST(Djb2Test, ATrailingZeroIsOneMoreByte)
{
    STATIC_EXPECT_NE(djb2(::std::string_view{"abc", 4}), djb2("abc"sv));
}

/**
 * @test djb2_hasher callable produces the same result as the free function.
 */
TEST(Djb2Test, HasherMatchesFreeFunction)
{
    STATIC_EXPECT_EQ(djb2_hasher{}("hello"sv), djb2("hello"sv));
}

/**
 * @test The value the djb2a recurrence gives for the three bytes of "abc", from the seed
 *       5381 that variant fixes.
 */
TEST(Djb2Test, ThreeBytesAnswerTheSpecifiedValue) { STATIC_EXPECT_EQ(djb2("abc"sv), 193409669ull); }
