#include <gtest_utils.h>

#include <scl/utility/hash/jenkins_ota.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>

using namespace ::scl::hash;

/**
 * @test Empty range returns 0 after finalization (h=0 is a fixed point).
 */
TEST(JenkinsOtaTest, EmptyRangeReturnsZero)
{
    STATIC_EXPECT_EQ(jenkins_ota(::std::string_view{}), 0u);
}

/**
 * @test Identical inputs produce identical results (determinism).
 */
TEST(JenkinsOtaTest, Deterministic)
{
    STATIC_EXPECT_EQ(jenkins_ota("hello"), jenkins_ota("hello"));
}

/**
 * @test Different inputs produce different hash values.
 */
TEST(JenkinsOtaTest, DifferentInputsDifferentHashes)
{
    STATIC_EXPECT_NE(jenkins_ota("hello"), jenkins_ota("world"));
    STATIC_EXPECT_NE(jenkins_ota("hello"), jenkins_ota(::std::string_view{}));
    STATIC_EXPECT_NE(jenkins_ota("ab"), jenkins_ota("ba"));
}

/**
 * @test Result type is std::uint32_t (Jenkins OAT is a 32-bit algorithm).
 */
TEST(JenkinsOtaTest, ResultType)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(jenkins_ota("hello")), ::std::uint32_t>));
}

/**
 * @test Constexpr evaluation produces a non-zero value.
 */
TEST(JenkinsOtaTest, Constexpr) { STATIC_EXPECT_NE(jenkins_ota("constexpr"), 0u); }

/**
 * @test A string literal is hashed as its text — every spelling of it agrees.
 */
TEST(JenkinsOtaTest, LiteralHashedWithoutTerminatingZero)
{
    STATIC_EXPECT_EQ(jenkins_ota("hello"), jenkins_ota(::std::string_view{"hello"}));
    EXPECT_EQ(jenkins_ota("hello"), jenkins_ota(::std::string{"hello"}));
}

/**
 * @test An array that does not end in zero keeps every byte.
 */
TEST(JenkinsOtaTest, ArrayWithoutTerminatingZeroHashedWhole)
{
    static constexpr char raw[3]{'a', 'b', 'c'};
    STATIC_EXPECT_EQ(jenkins_ota(raw), jenkins_ota(::std::string_view{"abc"}));
}

/**
 * @test A byte array keeps every byte, a zero at its end included.
 */
TEST(JenkinsOtaTest, ByteArrayKeepsTrailingZero)
{
    static constexpr ::std::uint8_t data[4]{1, 2, 3, 0};
    static constexpr ::std::array<::std::uint8_t, 4> same{1, 2, 3, 0};
    STATIC_EXPECT_EQ(jenkins_ota(data), jenkins_ota(same));
}

/**
 * @test jenkins_ota_hasher callable produces the same result as the free function.
 */
TEST(JenkinsOtaTest, HasherMatchesFreeFunction)
{
    STATIC_EXPECT_EQ(jenkins_ota_hasher{}("hello"), jenkins_ota("hello"));
}

/**
 * @test A sequence filled at run time is spelled as a view, and hashes to the value the
 *       array spelling of the same bytes gives.
 */
TEST(JenkinsOtaTest, RunTimeViewMatchesArraySpelling)
{
    char text[8]{};
    ::std::ranges::copy(::std::string_view{"hello"}, text);
    EXPECT_EQ(jenkins_ota(::std::string_view{text, 5}), jenkins_ota("hello"));

    ::std::uint8_t data[4]{};
    for (auto index = ::std::size_t{0}; index < 4; ++index)
        data[index] = static_cast<::std::uint8_t>(index);

    static constexpr ::std::uint8_t expected[4]{0, 1, 2, 3};
    EXPECT_EQ(jenkins_ota(::std::span{data}), jenkins_ota(expected));
}
