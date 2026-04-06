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
    static_assert(sizeof(WithNoUniqueAddress) == sizeof(int));
    SUCCEED();
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
