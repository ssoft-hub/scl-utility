#include <gtest_utils.h>

#include <scl/utility/type_traits/detection/operator.h>

#include "detection_operator_helpers.h"

using namespace test;

// ============================================================================
// TESTS - ARITHMETIC
// ============================================================================

// ----------------------------------------------------------------------------
// BINARY PLUS (+)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Plus)
{
    STATIC_EXPECT_TRUE((::scl::has_plus_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_plus_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, PlusMember)
{
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, PlusMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY MINUS (-)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Minus)
{
    STATIC_EXPECT_TRUE((::scl::has_minus_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_minus_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, MinusMember)
{
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, MinusMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY MULTIPLY (*)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Multiplies)
{
    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiplies_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, MultipliesMember)
{
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiplies_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiplies_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiplies_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiplies_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, MultipliesMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiplies_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY DIVIDE (/)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Divides)
{
    STATIC_EXPECT_TRUE((::scl::has_divides_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divides_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divides_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divides_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divides_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divides_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divides_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, DividesMember)
{
    STATIC_EXPECT_FALSE((::scl::has_divides_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divides_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divides_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divides_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divides_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divides_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divides_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, DividesMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divides_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY MODULO (%)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Modulus)
{
    STATIC_EXPECT_TRUE((::scl::has_modulus_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulus_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulus_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulus_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulus_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulus_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulus_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, ModulusMember)
{
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulus_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulus_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulus_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulus_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulus_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulus_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, ModulusMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulus_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_member_exact_v<NoOperators const &&, int>));
}
