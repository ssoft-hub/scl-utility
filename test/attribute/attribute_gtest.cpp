#include <gtest_utils.h>

#include <scl/utility/attribute.h>

// ============================================================================
// SCL_NO_UNIQUE_ADDRESS
// ============================================================================

namespace
{
    struct Empty
    {};

    struct WithNoUniqueAddress
    {
        SCL_NO_UNIQUE_ADDRESS Empty e;
        int value;
    };
} // namespace

TEST(AttributeNoUniqueAddress, Compiles)
{
    WithNoUniqueAddress s{};
    s.value = 42;
    EXPECT_EQ(s.value, 42);
}

TEST(AttributeNoUniqueAddress, EnablesEBO)
{
    STATIC_EXPECT_EQ(sizeof(WithNoUniqueAddress), sizeof(int));
}

// ============================================================================
// SCL_ASSUME
// ============================================================================

TEST(AttributeAssume, CompilesTrueExpr)
{
    SCL_ASSUME(true);
    SUCCEED();
}

// ============================================================================
// SCL_UNREACHABLE
// ============================================================================

TEST(AttributeUnreachable, CompilesInDeadBranch)
{
    bool flag = true;
    if (!flag)
        SCL_UNREACHABLE();
    EXPECT_TRUE(flag);
}

// ============================================================================
// SCL_FORCE_INLINE
// ============================================================================

namespace
{
    SCL_FORCE_INLINE int force_add(int a, int b) { return a + b; }
} // namespace

TEST(AttributeForceInline, CompilesAndWorks) { EXPECT_EQ(force_add(2, 3), 5); }

// ============================================================================
// SCL_NOINLINE
// ============================================================================

namespace
{
    SCL_NOINLINE int noinline_mul(int a, int b) { return a * b; }
} // namespace

TEST(AttributeNoInline, CompilesAndWorks) { EXPECT_EQ(noinline_mul(3, 4), 12); }

// ============================================================================
// SCL_NORETURN
// ============================================================================

namespace
{
    SCL_NORETURN void throw_error() { throw 42; }
} // namespace

TEST(AttributeNoReturn, CompilesAndThrows) { EXPECT_THROW(throw_error(), int); }

// ============================================================================
// SCL_DEPRECATED / SCL_DEPRECATED_MSG
// ============================================================================

namespace
{
    // Suppress deprecation warnings within the test itself
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4996)
#elif defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

    SCL_DEPRECATED int deprecated_fn() { return 1; }
    SCL_DEPRECATED_MSG("use new_fn() instead") int deprecated_msg_fn() { return 2; }

    int call_deprecated() { return deprecated_fn(); }
    int call_deprecated_msg() { return deprecated_msg_fn(); }

#if defined(_MSC_VER)
#pragma warning(pop)
#elif defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif
} // namespace

TEST(AttributeDeprecated, CompilesAndWorks) { EXPECT_EQ(call_deprecated(), 1); }
TEST(AttributeDeprecatedMsg, CompilesAndWorks) { EXPECT_EQ(call_deprecated_msg(), 2); }

// ============================================================================
// SCL_FALLTHROUGH
// ============================================================================

namespace
{
    int fallthrough_switch(int x)
    {
        int result = 0;
        switch (x)
        {
        case 1:
            result += 10;
            SCL_FALLTHROUGH;
        case 2:
            result += 20;
            break;
        default:
            break;
        }
        return result;
    }
} // namespace

TEST(AttributeFallthrough, FallsThrough) { EXPECT_EQ(fallthrough_switch(1), 30); }
TEST(AttributeFallthrough, NoFallThrough) { EXPECT_EQ(fallthrough_switch(2), 20); }

// ============================================================================
// SCL_NODISCARD / SCL_NODISCARD_MSG
// ============================================================================

namespace
{
    SCL_NODISCARD int nodiscard_fn() { return 99; }
    SCL_NODISCARD_MSG("check the error code") int nodiscard_msg_fn() { return 88; }
} // namespace

TEST(AttributeNodiscard, CompilesAndWorks) { EXPECT_EQ(nodiscard_fn(), 99); }
TEST(AttributeNodiscardMsg, CompilesAndWorks) { EXPECT_EQ(nodiscard_msg_fn(), 88); }

// ============================================================================
// SCL_MAYBE_UNUSED
// ============================================================================

namespace
{
    SCL_MAYBE_UNUSED static int unused_var = 42;

    int maybe_unused_param(int a, SCL_MAYBE_UNUSED int b) { return a; }
} // namespace

TEST(AttributeMaybeUnused, VariableCompiles) { EXPECT_EQ(unused_var, 42); }
TEST(AttributeMaybeUnused, ParamCompiles) { EXPECT_EQ(maybe_unused_param(7, 0), 7); }

// ============================================================================
// SCL_LIKELY / SCL_UNLIKELY  (statement attributes)
// ============================================================================

TEST(AttributeLikely, CompilesOnIfBranch)
{
    bool hit = false;
    if (true)
        SCL_LIKELY { hit = true; }
    EXPECT_TRUE(hit);
}

TEST(AttributeUnlikely, CompilesOnElseBranch)
{
    bool hit = false;
    if (false)
    {}
    else
        SCL_UNLIKELY { hit = true; }
    EXPECT_TRUE(hit);
}

TEST(AttributeLikely, CompilesOnSwitchLabel)
{
    int result = 0;
    switch (1)
    {
    SCL_LIKELY case 1:
        result = 1;
        break;
        SCL_UNLIKELY default : result = 2;
        break;
    }
    EXPECT_EQ(result, 1);
}

// ============================================================================
// SCL_LIKELY_EXPR / SCL_UNLIKELY_EXPR  (expression wrappers)
// ============================================================================

TEST(AttributeLikelyExpr, ReturnsTrueForTrueExpr) { EXPECT_TRUE(SCL_LIKELY_EXPR(true)); }
TEST(AttributeLikelyExpr, ReturnsFalseForFalseExpr) { EXPECT_FALSE(SCL_LIKELY_EXPR(false)); }
TEST(AttributeUnlikelyExpr, ReturnsTrueForTrueExpr) { EXPECT_TRUE(SCL_UNLIKELY_EXPR(true)); }
TEST(AttributeUnlikelyExpr, ReturnsFalseForFalseExpr) { EXPECT_FALSE(SCL_UNLIKELY_EXPR(false)); }

TEST(AttributeLikelyExpr, WorksInCondition)
{
    int count = 0;
    for (int i = 0; i < 10; ++i)
        if (SCL_LIKELY_EXPR(i < 10))
            ++count;
    EXPECT_EQ(count, 10);
}

TEST(AttributeUnlikelyExpr, WorksInCondition)
{
    int count = 0;
    for (int i = 0; i < 10; ++i)
        if (SCL_UNLIKELY_EXPR(i >= 10))
            ++count;
    EXPECT_EQ(count, 0);
}

// ============================================================================
// SCL_UNSEQUENCED
// ============================================================================

namespace
{
    // clang-format off
    SCL_UNSEQUENCED(int pure_square(int x)) { return x * x; }
    SCL_UNSEQUENCED(int pure_add(int a, int b)) { return a + b; }
    // clang-format on
} // namespace

TEST(AttributeUnsequenced, CompilesAndWorks) { EXPECT_EQ(pure_square(5), 25); }
TEST(AttributeUnsequenced, MultipleArgs) { EXPECT_EQ(pure_add(3, 4), 7); }

// ============================================================================
// SCL_REPRODUCIBLE
// ============================================================================

namespace
{
    static int g_value = 42;

    // clang-format off
    SCL_REPRODUCIBLE(int read_global()) { return g_value; }
    // clang-format on
} // namespace

TEST(AttributeReproducible, CompilesAndWorks) { EXPECT_EQ(read_global(), 42); }

// ============================================================================
// SCL_INDETERMINATE
// ============================================================================

TEST(AttributeIndeterminate, CompilesOnLocalVar)
{
    SCL_INDETERMINATE int x;
    x = 99; // written before read — well-defined
    EXPECT_EQ(x, 99);
}

// ============================================================================
// SCL_HOT / SCL_COLD
// ============================================================================

namespace
{
    SCL_HOT int hot_fn(int x) { return x * 2; }
    SCL_COLD SCL_NOINLINE int cold_fn(int x) { return x + 1; }
} // namespace

TEST(AttributeHot, CompilesAndWorks) { EXPECT_EQ(hot_fn(5), 10); }
TEST(AttributeCold, CompilesAndWorks) { EXPECT_EQ(cold_fn(5), 6); }

// ============================================================================
// SCL_LIFETIMEBOUND
// ============================================================================

namespace
{
    int const & identity(int const & x SCL_LIFETIMEBOUND) { return x; }
} // namespace

TEST(AttributeLifetimebound, CompilesAndWorks)
{
    int v = 42;
    int const & r = identity(v);
    EXPECT_EQ(r, 42);
}
