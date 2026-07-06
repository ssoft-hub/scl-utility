#include <gtest_utils.h>

#include <scl/utility/type_traits/detection/operator.h>

#include "detection_operator_helpers.h"

using namespace test;

// ============================================================================
// TESTS - LOGICAL
// ============================================================================

// ----------------------------------------------------------------------------
// BINARY LOGICAL AND (&&)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, LogicalAnd)
{
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_and_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, LogicalAndMember)
{
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, LogicalAndMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY LOGICAL OR (||)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, LogicalOr)
{
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_or_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, LogicalOrMember)
{
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, LogicalOrMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<NoOperators const &&, int>));
}
