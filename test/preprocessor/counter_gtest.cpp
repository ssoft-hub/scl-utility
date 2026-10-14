#include <gtest_utils.h>

#include <scl/utility/preprocessor/counter.h>

#include <concepts>
#include <cstdint>
#include <type_traits>

struct Apples
{};

struct Pears
{};

constexpr auto apples_before = SCL_COUNTER_VALUE(Apples);

SCL_COUNTER_NEXT(Apples)

constexpr auto apples_after_one = SCL_COUNTER_VALUE(Apples);

SCL_COUNTER_NEXT(Apples)
SCL_COUNTER_NEXT(Pears)

/**
 * @test Verify that the value is of type std::uint_fast32_t.
 */
TEST(Counter, ValueIsOfTypeUintFast32)
{
    STATIC_EXPECT_TRUE((::std::same_as<::std::remove_cvref_t<decltype(SCL_COUNTER_VALUE(Apples))>,
        ::std::uint_fast32_t>));
}

/**
 * @test Verify that a counter reads 0 before the first advance and one more after each of them.
 */
TEST(Counter, ReadsZeroThenOneMoreAfterEachAdvance)
{
    STATIC_EXPECT_EQ(apples_before, 0U);
    STATIC_EXPECT_EQ(apples_after_one, 1U);
    STATIC_EXPECT_EQ(SCL_COUNTER_VALUE(Apples), 2U);
}

/**
 * @test Verify that advancing the counter of one tag leaves the counter of another tag alone.
 */
TEST(Counter, CountersOfTwoTagsAreApart) { STATIC_EXPECT_EQ(SCL_COUNTER_VALUE(Pears), 1U); }
