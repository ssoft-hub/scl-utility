#include <gtest/gtest.h>

#include <scl/utility/type_traits/forward_like.h>

// ============================================================================
// MACROS
// ============================================================================

#define TEST_EXPECT_TRUE(X) \
    static_assert(X, #X);   \
    EXPECT_TRUE(X);

#define TEST_EXPECT_FALSE(X) \
    static_assert(!(X), #X);   \
    EXPECT_FALSE(X);

using ::scl::forward_like_t;

TEST(ForwardLike, BasicCvPropagation)
{
    // Non-reference Base → rvalue reference result (std::forward_like semantics)
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double>, double &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, double>, double const &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, double>, double volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, double>, double const volatile &&>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double const>, double const &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, double const>, double const &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, double const>, double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, double const>, double const volatile &&>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double volatile>, double volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, double volatile>, double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, double volatile>, double volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, double volatile>, double const volatile &&>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double const volatile>, double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, double const volatile>, double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, double const volatile>, double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, double const volatile>,
        double const volatile &&>));
}

TEST(ForwardLike, ReferencePropagation)
{
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, double>, double &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, double>, double &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, double>, double const &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, double>, double const &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, double>, double volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, double>, double volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, double>, double const volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, double>, double const volatile &&>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, double const>, double const &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, double const>, double const &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, double const>, double const &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, double const>, double const &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, double const>, double const volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, double const>, double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, double const>, double const volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, double const>,
        double const volatile &&>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, double volatile>, double volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, double volatile>, double volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, double volatile>, double const volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, double volatile>, double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, double volatile>, double volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, double volatile>, double volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, double volatile>,
        double const volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, double volatile>,
        double const volatile &&>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, double const volatile>, double const volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, double const volatile>, double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, double const volatile>, double const volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, double const volatile>,
        double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, double const volatile>,
        double const volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, double const volatile>,
        double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, double const volatile>,
        double const volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, double const volatile>,
        double const volatile &&>));
}

TEST(ForwardLike, ReferenceRemoving)
{
    // Existing reference on Type is stripped; ref category comes from Base.
    // Non-reference Base → rvalue reference result.
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double &>, double &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double &&>, double &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, double &>, double const &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, double &&>, double const &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, double &>, double volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, double &&>, double volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, double &>, double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, double &&>, double const volatile &&>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double const &>, double const &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double const &&>, double const &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, double const &>, double const &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, double const &&>, double const &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, double const &>, double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, double const &&>, double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, double const &>, double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, double const &&>, double const volatile &&>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double volatile &>, double volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double volatile &&>, double volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, double volatile &>, double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, double volatile &&>, double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, double volatile &>, double volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, double volatile &&>, double volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, double volatile &>, double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, double volatile &&>, double const volatile &&>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double const volatile &>, double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double const volatile &&>, double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, double const volatile &>, double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, double const volatile &&>, double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, double const volatile &>, double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, double const volatile &&>, double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, double const volatile &>,
        double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, double const volatile &&>,
        double const volatile &&>));
}

TEST(ForwardLike, ReferenceOverExistingRefOnType)
{
    // Existing reference on Type is ignored; cvref is taken from Base.
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, double &>, double &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, double &&>, double &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, double const &>, double const &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, double const &&>, double const &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, double volatile &>, double volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, double volatile &&>, double volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, double const volatile &>, double const volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, double const volatile &&>, double const volatile &>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, double &>, double const &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, double &&>, double const &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, double const &>, double const &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, double const &&>, double const &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, double volatile &>, double const volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, double volatile &&>, double const volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, double const volatile &>, double const volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, double const volatile &&>,
        double const volatile &>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, double &>, double volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, double &&>, double volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, double const &>, double const volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, double const &&>, double const volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, double volatile &>, double volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, double volatile &&>, double volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, double const volatile &>,
        double const volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, double const volatile &&>,
        double const volatile &>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, double &>, double const volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, double &&>, double const volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, double const &>,
        double const volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, double const &&>,
        double const volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, double volatile &>,
        double const volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, double volatile &&>,
        double const volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, double const volatile &>,
        double const volatile &>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, double const volatile &&>,
        double const volatile &>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, double &>, double &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, double &&>, double &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, double const &>, double const &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, double const &&>, double const &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, double volatile &>, double volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, double volatile &&>, double volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, double const volatile &>, double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, double const volatile &&>, double const volatile &&>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, double &>, double const &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, double &&>, double const &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, double const &>, double const &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, double const &&>, double const &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, double volatile &>, double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, double volatile &&>, double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, double const volatile &>,
        double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, double const volatile &&>,
        double const volatile &&>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, double &>, double volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, double &&>, double volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, double const &>, double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, double const &&>, double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, double volatile &>, double volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, double volatile &&>, double volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, double const volatile &>,
        double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, double const volatile &&>,
        double const volatile &&>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, double &>, double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, double &&>, double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, double const &>,
        double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, double const &&>,
        double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, double volatile &>,
        double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, double volatile &&>,
        double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, double const volatile &>,
        double const volatile &&>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, double const volatile &&>,
        double const volatile &&>));
}

TEST(ForwardLike, VoidSpecialCase)
{
    // Reference is not added to void; remains void with cv qualifiers.
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, void>, void>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, void const>, void const>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, void volatile>, void volatile>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, void const volatile>, void const volatile>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, void>, void const>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, void const>, void const>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, void volatile>, void const volatile>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, void const volatile>, void const volatile>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, void>, void volatile>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, void const>, void const volatile>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, void volatile>, void volatile>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, void const volatile>, void const volatile>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, void>, void const volatile>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, void const>, void const volatile>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, void volatile>, void const volatile>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, void const volatile>, void const volatile>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, void>, void>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, void const>, void const>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, void volatile>, void volatile>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, void const volatile>, void const volatile>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, void>, void const>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, void const>, void const>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, void volatile>, void const volatile>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, void const volatile>, void const volatile>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, void>, void volatile>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, void const>, void const volatile>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, void volatile>, void volatile>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, void const volatile>, void const volatile>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, void>, void const volatile>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, void const>, void const volatile>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, void volatile>, void const volatile>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, void const volatile>, void const volatile>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, void>, void>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, void const>, void const>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, void volatile>, void volatile>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, void const volatile>, void const volatile>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, void>, void const>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, void const>, void const>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, void volatile>, void const volatile>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, void const volatile>, void const volatile>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, void>, void volatile>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, void const>, void const volatile>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, void volatile>, void volatile>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, void const volatile>, void const volatile>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, void>, void const volatile>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, void const>, void const volatile>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, void volatile>, void const volatile>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, void const volatile>, void const volatile>));
}

TEST(ForwardLike, ArraysAndOtherTypes)
{
    // Non-reference Base → rvalue reference to array
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, int[3]>, int (&&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, int[3]>, int const (&&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, int[3]>, int volatile (&&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, int[3]>, int const volatile (&&)[3]>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, int const[3]>, int const (&&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, int const[3]>, int const (&&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, int const[3]>, int const volatile (&&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, int const[3]>, int const volatile (&&)[3]>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, int volatile[3]>, int volatile (&&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, int volatile[3]>, int const volatile (&&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, int volatile[3]>, int volatile (&&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, int volatile[3]>,
        int const volatile (&&)[3]>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int, int const volatile[3]>, int const volatile (&&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, int const volatile[3]>,
        int const volatile (&&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, int const volatile[3]>,
        int const volatile (&&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, int const volatile[3]>,
        int const volatile (&&)[3]>));

    // Rvalue reference Base
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, int[3]>, int (&&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, int[3]>, int const(&&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, int[3]>, int volatile(&&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, int[3]>, int const volatile(&&)[3]>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, int const[3]>, int const(&&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, int const[3]>, int const(&&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, int const[3]>, int const volatile(&&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, int const[3]>,
        int const volatile(&&)[3]>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, int volatile[3]>, int volatile(&&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, int volatile[3]>, int const volatile(&&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, int volatile[3]>, int volatile(&&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, int volatile[3]>,
        int const volatile(&&)[3]>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, int const volatile[3]>, int const volatile(&&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, int const volatile[3]>,
        int const volatile(&&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, int const volatile[3]>,
        int const volatile(&&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, int const volatile[3]>,
        int const volatile(&&)[3]>));

    // Lvalue reference Base
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, int[3]>, int (&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, int[3]>, int const(&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, int[3]>, int volatile(&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, int[3]>, int const volatile(&)[3]>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, int const[3]>, int const(&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, int const[3]>, int const(&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, int const[3]>, int const volatile(&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, int const[3]>,
        int const volatile(&)[3]>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, int volatile[3]>, int volatile(&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, int volatile[3]>, int const volatile(&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, int volatile[3]>, int volatile(&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, int volatile[3]>,
        int const volatile(&)[3]>));

    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, int const volatile[3]>, int const volatile(&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, int const volatile[3]>,
        int const volatile(&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, int const volatile[3]>,
        int const volatile(&)[3]>));
    TEST_EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, int const volatile[3]>,
        int const volatile(&)[3]>));
}
