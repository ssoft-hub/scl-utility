#include <gtest_utils.h>

#include <scl/utility/hash/sdbm.h>

#include <cstdint>
#include <string_view>

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
TEST(SdbmTest, Deterministic) { STATIC_EXPECT_EQ(sdbm("hello"), sdbm("hello")); }

/**
 * @test Different inputs produce different hash values.
 */
TEST(SdbmTest, DifferentInputsDifferentHashes)
{
    STATIC_EXPECT_NE(sdbm("hello"), sdbm("world"));
    STATIC_EXPECT_NE(sdbm("hello"), sdbm(::std::string_view{}));
    STATIC_EXPECT_NE(sdbm("ab"), sdbm("ba"));
}

/**
 * @test Chaining two ranges equals hashing their concatenation.
 * @note Uses std::string_view to control exact byte boundaries (no null terminator).
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
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(sdbm("hello")), ::std::uint64_t>));
}

/**
 * @test Constexpr evaluation produces a value distinct from the seed.
 */
TEST(SdbmTest, Constexpr) { STATIC_EXPECT_NE(sdbm("constexpr"), 0ull); }

/**
 * @test sdbm_hasher callable produces the same result as the free function.
 */
TEST(SdbmTest, HasherMatchesFreeFunction)
{
    STATIC_EXPECT_EQ(sdbm_hasher{}("hello"), sdbm("hello"));
}
