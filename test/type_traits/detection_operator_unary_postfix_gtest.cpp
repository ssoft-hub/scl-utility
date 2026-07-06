#include <gtest_utils.h>

#include <scl/utility/type_traits/detection/operator.h>

#include "detection_operator_helpers.h"

using namespace test;

// ============================================================================
// TESTS - POSTFIX UNARY
// ============================================================================

// ----------------------------------------------------------------------------
// POSTFIX INCREMENT (++)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, PostfixIncrement)
{
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<int>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<WithGlobal>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, PostfixIncrementMember)
{
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, PostfixIncrementMemberExact)
{
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_exact_v<WithMember>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithMember &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_exact_v<WithConstMember const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_exact_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithRvalueMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithLvalueConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithLvalueConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_exact_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// POSTFIX DECREMENT (--)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, PostfixDecrement)
{
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<int>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithGlobal>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, PostfixDecrementMember)
{
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, PostfixDecrementMemberExact)
{
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_exact_v<WithMember>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithMember &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_exact_v<WithConstMember const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_exact_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithRvalueMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithLvalueConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithLvalueConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_exact_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<NoOperators const &&>);
}
