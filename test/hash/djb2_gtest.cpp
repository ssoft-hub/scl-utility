#include <gtest_utils.h>

#include <scl/utility/hash/djb2.h>

#include <cstdint>
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
 * @note Uses std::string_view to control exact byte boundaries (no null terminator).
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
 * @test djb2_hasher callable produces the same result as the free function.
 */
TEST(Djb2Test, HasherMatchesFreeFunction)
{
    STATIC_EXPECT_EQ(djb2_hasher{}("hello"), djb2("hello"));
}
