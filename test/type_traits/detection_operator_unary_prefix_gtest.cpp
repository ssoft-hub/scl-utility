#include <gtest_utils.h>

#include <scl/utility/type_traits/detection/operator.h>

#include "detection_operator_helpers.h"

using namespace test;

// ============================================================================
// TESTS - UNARY
// ============================================================================

// ----------------------------------------------------------------------------
// ADDRESS OF (&)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, AddressOf)
{
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<int>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<int const>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<WithMember const>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithRvalueMember>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<WithRvalueMember const>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_address_of_v<WithGlobal>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<WithGlobal const>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_address_of_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<NoOperators const>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<NoOperators const &>); // built-in
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, AddressOfMember)
{
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, AddressOfMemberExact)
{
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_address_of_member_exact_v<WithMember>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithMember &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_exact_v<WithConstMember const>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_exact_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithRvalueMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithLvalueConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithLvalueConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_exact_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// INDIRECTION (*)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Indirection)
{
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<int *>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<int *&>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<int *&&>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<int * const>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<int * const &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<int * const &&>);

    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithGlobal>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithGlobal &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithGlobal &&>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithGlobal const>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithGlobal const &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_indirection_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, IndirectionMember)
{
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, IndirectionMemberExact)
{
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_indirection_member_exact_v<WithMember>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithMember &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_exact_v<WithConstMember const>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_exact_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithRvalueMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithLvalueConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithLvalueConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_exact_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// UNARY PLUS (+)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, UnaryPlus)
{
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<int>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<int &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<int &&>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<int const>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<int const &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithGlobal>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithGlobal &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithGlobal &&>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithGlobal const>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithGlobal const &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_plus_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, UnaryPlusMember)
{
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, UnaryPlusMemberExact)
{
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_exact_v<WithMember>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithMember &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_exact_v<WithConstMember const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_exact_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithRvalueMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithLvalueConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithLvalueConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_exact_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// UNARY MINUS (-)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, UnaryMinus)
{
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<int>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<int &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<int &&>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<int const>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<int const &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithGlobal>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithGlobal &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithGlobal &&>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithGlobal const>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithGlobal const &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_minus_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, UnaryMinusMember)
{
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, UnaryMinusMemberExact)
{
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_exact_v<WithMember>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithMember &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_exact_v<WithConstMember const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_exact_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithRvalueMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithLvalueConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithLvalueConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_exact_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// PREFIX INCREMENT (++)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, PrefixIncrement)
{
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<int>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<WithGlobal>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, PrefixIncrementMember)
{
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, PrefixIncrementMemberExact)
{
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_exact_v<WithMember>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithMember &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_exact_v<WithConstMember const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_exact_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithRvalueMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithLvalueConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithLvalueConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_exact_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// PREFIX DECREMENT (--)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, PrefixDecrement)
{
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<int>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithGlobal>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, PrefixDecrementMember)
{
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, PrefixDecrementMemberExact)
{
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_exact_v<WithMember>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithMember &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_exact_v<WithConstMember const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_exact_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithRvalueMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithLvalueConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithLvalueConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_exact_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// BITWISE NOT (~)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, BitwiseNot)
{
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<int>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<int &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<int &&>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<int const>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<int const &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithGlobal>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithGlobal &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithGlobal &&>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithGlobal const>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithGlobal const &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, BitwiseNotMember)
{
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, BitwiseNotMemberExact)
{
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_exact_v<WithMember>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithMember &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_exact_v<WithConstMember const>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_exact_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithRvalueMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithLvalueConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithLvalueConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_exact_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// LOGICAL NOT (!)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, LogicalNot)
{
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<int>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<int &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<int &&>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<int const>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<int const &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithGlobal>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithGlobal &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithGlobal &&>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithGlobal const>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithGlobal const &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_logical_not_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, LogicalNotMember)
{
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, LogicalNotMemberExact)
{
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_exact_v<WithMember>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithMember &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_exact_v<WithConstMember const>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_exact_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithRvalueMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithLvalueConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithLvalueConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_exact_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<NoOperators const &&>);
}
