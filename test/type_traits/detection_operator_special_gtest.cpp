#include <gtest_utils.h>

#include <scl/utility/type_traits/detection/operator.h>

#include "detection_operator_helpers.h"

using namespace test;

// ============================================================================
// TESTS - SPECIAL
// ============================================================================

// ----------------------------------------------------------------------------
// SUBSCRIPT ([])
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Subscript)
{
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_subscript_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_subscript_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, SubscriptMember)
{
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, SubscriptMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_subscript_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// FUNCTION CALL (())
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Call)
{
    STATIC_EXPECT_FALSE((::scl::has_call_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_call_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_call_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_call_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_call_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_call_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_call_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, CallMember)
{
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_call_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_call_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, CallMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_call_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// COMMA (,)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Comma)
{
    STATIC_EXPECT_TRUE((::scl::has_comma_v<int, int>)); // built-in
    STATIC_EXPECT_TRUE((::scl::has_comma_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithRvalueMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithRvalueMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_comma_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, CommaMember)
{
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, CommaMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_comma_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// ARROW (->)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, ArrowMember)
{
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<int *>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<int *&>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<int *&&>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<int const *>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<int const *&>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<int const *&&>);

    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, ArrowMemberExact)
{
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_arrow_member_exact_v<WithMember>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithMember &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_exact_v<WithConstMember const>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_exact_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithRvalueMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithLvalueConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithLvalueConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_exact_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// MEMBER POINTER ACCESS (->*)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, ArrowToPointerMember)
{
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, ArrowToPointerMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<NoOperators const &&, int>));
}
