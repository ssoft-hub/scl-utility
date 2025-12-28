#include <gtest/gtest.h>

#include <scl/utility/preprocessor.h>

#include <string>
#include <utility>
#include <vector>

// Local helper macros used in tests
#define CALL(fn, ...) fn(__VA_ARGS__)
#define MAKE_PAIR(a, b) a, b
#define FORWARD_TO_EMPLACE(obj, ...) obj.emplace_back(__VA_ARGS__)
#define ID(x) x

namespace
{
    int add(int a, int b) { return a + b; }

    template <typename... Ts>
    struct type_list
    {};
} // namespace

TEST(ExpandMacros, ForwardsFunctionArgs)
{
    // SCL_FORWARD should forward the two arguments unchanged
    int result = CALL(add, SCL_FORWARD(1, 2));
    EXPECT_EQ(result, 3);
}

TEST(ExpandMacros, ExpandsTemplateParameterList)
{
// SCL_FORWARD should expand a macro producing a comma-separated type list
#define BASIC_TYPES int, long, double
    using TL = type_list<SCL_FORWARD(BASIC_TYPES)>;
    using Expected = type_list<int, long, double>;
    EXPECT_TRUE((std::is_same_v<TL, Expected>));
#undef BASIC_TYPES
}

TEST(ExpandMacros, ChainingMacrosToMakeStdPairType)
{
    using PairT = std::pair<SCL_FORWARD(MAKE_PAIR(int, double))>;
    EXPECT_TRUE((std::is_same_v<PairT, std::pair<int, double>>));

    PairT p{1, 2.0};
    EXPECT_EQ(p.first, 1);
    EXPECT_DOUBLE_EQ(p.second, 2.0);
}

TEST(ExpandMacros, ForwardingArgumentsToEmplaceBack)
{
    std::vector<std::pair<int, std::string>> v;
    FORWARD_TO_EMPLACE(v, SCL_FORWARD(42, "answer"));
    ASSERT_EQ(v.size(), 1u);
    EXPECT_EQ(v[0].first, 42);
    EXPECT_EQ(v[0].second, "answer");
}

TEST(ExpandMacros, ForcesExpansionThroughIdentityMacro)
{
    // Using ID to ensure tokens produced by MAKE_PAIR are forwarded as two separate arguments
    auto sum = [](int a, int b) { return a + b; };
    int s = sum(ID(SCL_FORWARD(MAKE_PAIR(7, 9))));
    EXPECT_EQ(s, 16);

    using PairT2 = std::pair<ID(SCL_FORWARD(MAKE_PAIR(float, char)))>;
    EXPECT_TRUE((std::is_same_v<PairT2, std::pair<float, char>>));
}

// Cleanup local helper macros to avoid leaking into other compilation units
#undef CALL
#undef MAKE_PAIR
#undef FORWARD_TO_EMPLACE
#undef ID
