#include <gtest_utils.h>

#include <string_view>

#include "caller_macros.h"

namespace
{
    char const * const reference_expansions[caller_macros_count] = {SCL_TEST_MACROS(SCL_TEST_EXPANSION)};
    char const * const names[caller_macros_count] = {SCL_TEST_MACROS(SCL_TEST_NAME)};

#if defined(_MSC_VER)
    int index_of(std::string_view name)
    {
        for (int index = 0; index < caller_macros_count; ++index)
            if (name == names[index])
                return index;
        return -1;
    }
#endif
} // namespace

TEST(AttributeCallerMacros, CompileUnderMacrosNamedLikeAttributeTokens)
{
    EXPECT_EQ(caller_macros_sum(), 31);
}

TEST(AttributeCallerMacros, ExpandAsWithoutMacrosNamedLikeAttributeTokens)
{
    for (int index = 0; index < caller_macros_count; ++index)
    {
#if defined(_MSC_VER)
        if (index == index_of("SCL_NO_UNIQUE_ADDRESS"))
            continue;
#endif
#if defined(_MSC_VER) && !defined(__clang__)
        if (index == index_of("SCL_NOINLINE") || index == index_of("SCL_LIFETIMEBOUND"))
            continue;
#endif
        EXPECT_STREQ(caller_macros_expansions[index], reference_expansions[index]) << names[index];
    }
}

#if defined(_MSC_VER)
TEST(AttributeCallerMacros, KeepTheDefinitionsTheHeadersAskForUnderMacrosOfTheCaller)
{
    EXPECT_STREQ(caller_macros_expansions[index_of("SCL_NO_UNIQUE_ADDRESS")], "[]");
#if !defined(__clang__)
    EXPECT_STREQ(caller_macros_expansions[index_of("SCL_NOINLINE")], "[]");
    EXPECT_STREQ(caller_macros_expansions[index_of("SCL_LIFETIMEBOUND")], "[]");
#endif
}
#endif
