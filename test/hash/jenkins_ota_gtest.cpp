#include <gtest_utils.h>

#include <scl/utility/hash/jenkins_ota.h>

#include <cstdint>
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
 * @test jenkins_ota_hasher callable produces the same result as the free function.
 */
TEST(JenkinsOtaTest, HasherMatchesFreeFunction)
{
    STATIC_EXPECT_EQ(jenkins_ota_hasher{}("hello"), jenkins_ota("hello"));
}
