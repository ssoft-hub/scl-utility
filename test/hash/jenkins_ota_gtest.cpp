#include <gtest_utils.h>

#include <scl/utility/hash/jenkins_ota.h>

#include <array>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>
#include <vector>

using namespace ::std::string_view_literals;

namespace
{
    /// Satisfied when @ref scl::hash::jenkins_ota accepts @p Range.
    template <typename Range>
    concept jenkins_ota_takes = requires(Range const & range) { ::scl::hash::jenkins_ota(range); };
} // namespace

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
    STATIC_EXPECT_EQ(jenkins_ota("hello"sv), jenkins_ota("hello"sv));
}

/**
 * @test Different inputs produce different hash values.
 */
TEST(JenkinsOtaTest, DifferentInputsDifferentHashes)
{
    STATIC_EXPECT_NE(jenkins_ota("hello"sv), jenkins_ota("world"sv));
    STATIC_EXPECT_NE(jenkins_ota("hello"sv), jenkins_ota(::std::string_view{}));
    STATIC_EXPECT_NE(jenkins_ota("ab"sv), jenkins_ota("ba"sv));
}

/**
 * @test Result type is std::uint32_t (Jenkins OAT is a 32-bit algorithm).
 */
TEST(JenkinsOtaTest, ResultType)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(jenkins_ota("hello"sv)), ::std::uint32_t>));
}

/**
 * @test Constexpr evaluation produces a non-zero value.
 */
TEST(JenkinsOtaTest, Constexpr) { STATIC_EXPECT_NE(jenkins_ota("constexpr"sv), 0u); }

/**
 * @test One text produces one value, however it is spelled.
 */
TEST(JenkinsOtaTest, OneTextHashesAlikeHoweverSpelled)
{
    static constexpr ::std::array<char, 5> held{'h', 'e', 'l', 'l', 'o'};
    STATIC_EXPECT_EQ(jenkins_ota("hello"sv), jenkins_ota(held));
    STATIC_EXPECT_EQ(jenkins_ota("hello"sv), jenkins_ota(::std::span{held}));
    EXPECT_EQ(jenkins_ota("hello"sv), jenkins_ota(::std::string{"hello"}));
    EXPECT_EQ(jenkins_ota("hello"sv), jenkins_ota(::std::vector<char>{'h', 'e', 'l', 'l', 'o'}));
}

/**
 * @test An array is refused and a wider element with it, while a span over the array
 *       names the bytes the array holds.
 */
TEST(JenkinsOtaTest, ArrayIsRefusedAndASpanNamesItsBytes)
{
    static constexpr char raw[3]{'a', 'b', 'c'};
    STATIC_EXPECT_TRUE(jenkins_ota_takes<::std::string_view>);
    STATIC_EXPECT_FALSE(jenkins_ota_takes<char[3]>);
    STATIC_EXPECT_FALSE(jenkins_ota_takes<::std::u16string_view>);
    STATIC_EXPECT_EQ(jenkins_ota(::std::span{raw}), jenkins_ota(::std::string_view{"abc"}));
}

/**
 * @test A trailing zero is one more byte.
 */
TEST(JenkinsOtaTest, ATrailingZeroIsOneMoreByte)
{
    STATIC_EXPECT_NE(jenkins_ota(::std::string_view{"abc", 4}), jenkins_ota("abc"sv));
}

/**
 * @test jenkins_ota_hasher callable produces the same result as the free function.
 */
TEST(JenkinsOtaTest, HasherMatchesFreeFunction)
{
    STATIC_EXPECT_EQ(jenkins_ota_hasher{}("hello"sv), jenkins_ota("hello"sv));
}

/**
 * @test The value Jenkins' one-at-a-time hash gives for the three bytes of "abc",
 *       including the three finalisation steps.
 */
TEST(JenkinsOtaTest, ThreeBytesAnswerTheSpecifiedValue)
{
    STATIC_EXPECT_EQ(jenkins_ota("abc"sv), 3977453403u);
}
