#include <gtest_utils.h>

#include <scl/utility/preprocessor/counter.h>

#include <cstdint>

/**
 * @brief The tag counter_cross_tu_provider_gtest.cpp advances twice; this translation unit
 *        advances it once.
 */
struct CrossTuTag
{};

SCL_COUNTER_NEXT(::CrossTuTag)

::std::uint_fast32_t cross_tu_provider_count() noexcept;

/**
 * @test Verify that each translation unit keeps its own count of one tag.
 */
TEST(CounterCrossTu, CountIsKeptPerTranslationUnit)
{
    STATIC_EXPECT_EQ(SCL_COUNTER_VALUE(::CrossTuTag), 1U);
    EXPECT_EQ(cross_tu_provider_count(), 2U);
}
