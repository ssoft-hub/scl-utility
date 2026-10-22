#include <gtest_utils.h>

#include <scl/utility/preprocessor/forward.h>

#include <string_view>
#include <type_traits>

#define SCL_TEST_ONE_ARGUMENT(argument) argument
#define SCL_TEST_STRINGIZE(argument) #argument
#define SCL_TEST_PASTE(argument) SCL_TEST_PASTED_##argument
#define SCL_TEST_PASTED_SCL_FORWARD(...) true

namespace
{
    template <typename... Types>
    struct type_list
    {};

    constexpr int add(int left, int right) noexcept { return left + right; }
} // namespace

/**
 * @test Verify that the macro expands to its arguments with no parentheses around them.
 */
TEST(Forward, ExpandsToItsArguments)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<type_list<SCL_FORWARD(int, long)>, type_list<int, long>>));
}

/**
 * @test Verify that the macro written as an argument of another macro stays one argument and the
 *       parameter receives the whole list.
 */
TEST(Forward, StaysOneArgumentOfAnotherMacro)
{
    STATIC_EXPECT_EQ(add(SCL_TEST_ONE_ARGUMENT(SCL_FORWARD(1, 2))), 3);
}

/**
 * @test Verify that a parameter under # receives the argument unexpanded.
 */
TEST(Forward, ParameterUnderStringizingReceivesItUnexpanded)
{
    STATIC_EXPECT_EQ(
        ::std::string_view{SCL_TEST_STRINGIZE(SCL_FORWARD(1, 2))}, ::std::string_view{"SCL_FORWARD(1, 2)"});
}

/**
 * @test Verify that a parameter under ## receives the argument unexpanded.
 */
TEST(Forward, ParameterUnderPastingReceivesItUnexpanded)
{
    STATIC_EXPECT_TRUE(SCL_TEST_PASTE(SCL_FORWARD(1, 2)));
}

#undef SCL_TEST_PASTED_SCL_FORWARD
#undef SCL_TEST_PASTE
#undef SCL_TEST_STRINGIZE
#undef SCL_TEST_ONE_ARGUMENT
