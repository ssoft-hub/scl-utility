#include <gtest/gtest.h>

#include <scl/utility/type_traits/forward_like.h>

using ::scl::forward_like_t;

TEST(ForwardLike, BasicCvPropagation)
{
    static_assert(::std::is_same_v<forward_like_t<int, double>, double>);
    static_assert(::std::is_same_v<forward_like_t<int const, double>, double const>);
    static_assert(::std::is_same_v<forward_like_t<int volatile, double>, double volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile, double>, double const volatile>);

    static_assert(::std::is_same_v<forward_like_t<int, double const>, double const>);
    static_assert(::std::is_same_v<forward_like_t<int const, double const>, double const>);
    static_assert(::std::is_same_v<forward_like_t<int volatile, double const>, double const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile, double const>, double const volatile>);

    static_assert(::std::is_same_v<forward_like_t<int, double volatile>, double volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const, double volatile>, double const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int volatile, double volatile>, double volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile, double volatile>, double const volatile>);

    static_assert(::std::is_same_v<forward_like_t<int, double const volatile>, double const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const, double const volatile>, double const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int volatile, double const volatile>, double const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile, double const volatile>,
        double const volatile>);

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double>, double>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, double>, double const>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, double>, double volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, double>, double const volatile>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double const>, double const>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, double const>, double const>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, double const>, double const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, double const>, double const volatile>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double volatile>, double volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, double volatile>, double const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, double volatile>, double volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, double volatile>, double const volatile>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double const volatile>, double const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, double const volatile>, double const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, double const volatile>, double const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, double const volatile>,
        double const volatile>));
}

TEST(ForwardLike, ReferencePropagation)
{
    static_assert(::std::is_same_v<forward_like_t<int &, double>, double &>);
    static_assert(::std::is_same_v<forward_like_t<int &&, double>, double &&>);
    static_assert(::std::is_same_v<forward_like_t<int const &, double>, double const &>);
    static_assert(::std::is_same_v<forward_like_t<int const &&, double>, double const &&>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &, double>, double volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &&, double>, double volatile &&>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &, double>, double const volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &&, double>, double const volatile &&>);

    static_assert(::std::is_same_v<forward_like_t<int &, double const>, double const &>);
    static_assert(::std::is_same_v<forward_like_t<int &&, double const>, double const &&>);
    static_assert(::std::is_same_v<forward_like_t<int const &, double const>, double const &>);
    static_assert(::std::is_same_v<forward_like_t<int const &&, double const>, double const &&>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &, double const>, double const volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &&, double const>, double const volatile &&>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &, double const>, double const volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &&, double const>, double const volatile &&>);

    static_assert(::std::is_same_v<forward_like_t<int &, double volatile>, double volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int &&, double volatile>, double volatile &&>);
    static_assert(::std::is_same_v<forward_like_t<int const &, double volatile>, double const volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int const &&, double volatile>, double const volatile &&>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &, double volatile>, double volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &&, double volatile>, double volatile &&>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &, double volatile>,
        double const volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &&, double volatile>,
        double const volatile &&>);

    static_assert(::std::is_same_v<forward_like_t<int &, double const volatile>, double const volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int &&, double const volatile>, double const volatile &&>);
    static_assert(::std::is_same_v<forward_like_t<int const &, double const volatile>, double const volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int const &&, double const volatile>, double const volatile &&>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &, double const volatile>,
        double const volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &&, double const volatile>,
        double const volatile &&>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &, double const volatile>,
        double const volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &&, double const volatile>,
        double const volatile &&>);

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, double>, double &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, double>, double &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, double>, double const &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, double>, double const &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, double>, double volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, double>, double volatile &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, double>, double const volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, double>, double const volatile &&>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, double const>, double const &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, double const>, double const &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, double const>, double const &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, double const>, double const &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, double const>, double const volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, double const>, double const volatile &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, double const>, double const volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, double const>,
        double const volatile &&>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, double volatile>, double volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, double volatile>, double volatile &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, double volatile>, double const volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, double volatile>, double const volatile &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, double volatile>, double volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, double volatile>, double volatile &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, double volatile>,
        double const volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, double volatile>,
        double const volatile &&>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, double const volatile>, double const volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, double const volatile>, double const volatile &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, double const volatile>, double const volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, double const volatile>,
        double const volatile &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, double const volatile>,
        double const volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, double const volatile>,
        double const volatile &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, double const volatile>,
        double const volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, double const volatile>,
        double const volatile &&>));
}

TEST(ForwardLike, ReferenceRemoving)
{
    static_assert(::std::is_same_v<forward_like_t<int, double &>, double>);
    static_assert(::std::is_same_v<forward_like_t<int, double &&>, double>);
    static_assert(::std::is_same_v<forward_like_t<int const, double &>, double const>);
    static_assert(::std::is_same_v<forward_like_t<int const, double &&>, double const>);
    static_assert(::std::is_same_v<forward_like_t<int volatile, double &>, double volatile>);
    static_assert(::std::is_same_v<forward_like_t<int volatile, double &&>, double volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile, double &>, double const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile, double &&>, double const volatile>);

    static_assert(::std::is_same_v<forward_like_t<int, double const &>, double const>);
    static_assert(::std::is_same_v<forward_like_t<int, double const &&>, double const>);
    static_assert(::std::is_same_v<forward_like_t<int const, double const &>, double const>);
    static_assert(::std::is_same_v<forward_like_t<int const, double const &&>, double const>);
    static_assert(::std::is_same_v<forward_like_t<int volatile, double const &>, double const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int volatile, double const &&>, double const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile, double const &>, double const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile, double const &&>, double const volatile>);

    static_assert(::std::is_same_v<forward_like_t<int, double volatile &>, double volatile>);
    static_assert(::std::is_same_v<forward_like_t<int, double volatile &&>, double volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const, double volatile &>, double const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const, double volatile &&>, double const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int volatile, double volatile &>, double volatile>);
    static_assert(::std::is_same_v<forward_like_t<int volatile, double volatile &&>, double volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile, double volatile &>, double const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile, double volatile &&>, double const volatile>);

    static_assert(::std::is_same_v<forward_like_t<int, double const volatile &>, double const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int, double const volatile &&>, double const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const, double const volatile &>, double const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const, double const volatile &&>, double const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int volatile, double const volatile &>, double const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int volatile, double const volatile &&>, double const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile, double const volatile &>,
        double const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile, double const volatile &&>,
        double const volatile>);

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double &>, double>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double &&>, double>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, double &>, double const>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, double &&>, double const>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, double &>, double volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, double &&>, double volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, double &>, double const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, double &&>, double const volatile>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double const &>, double const>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double const &&>, double const>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, double const &>, double const>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, double const &&>, double const>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, double const &>, double const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, double const &&>, double const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, double const &>, double const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, double const &&>, double const volatile>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double volatile &>, double volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double volatile &&>, double volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, double volatile &>, double const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, double volatile &&>, double const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, double volatile &>, double volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, double volatile &&>, double volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, double volatile &>, double const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, double volatile &&>, double const volatile>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double const volatile &>, double const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int, double const volatile &&>, double const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, double const volatile &>, double const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, double const volatile &&>, double const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, double const volatile &>, double const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, double const volatile &&>, double const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, double const volatile &>,
        double const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, double const volatile &&>,
        double const volatile>));
}

TEST(ForwardLike, ReferenceOverExistingRefOnType)
{
    // Existing reference on Type is ignored; cvref is taken from Base.
    static_assert(::std::is_same_v<forward_like_t<int &, double &>, double &>);
    static_assert(::std::is_same_v<forward_like_t<int &, double &&>, double &>);
    static_assert(::std::is_same_v<forward_like_t<int &, double const &>, double const &>);
    static_assert(::std::is_same_v<forward_like_t<int &, double const &&>, double const &>);
    static_assert(::std::is_same_v<forward_like_t<int &, double volatile &>, double volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int &, double volatile &&>, double volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int &, double const volatile &>, double const volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int &, double const volatile &&>, double const volatile &>);

    static_assert(::std::is_same_v<forward_like_t<int const &, double &>, double const &>);
    static_assert(::std::is_same_v<forward_like_t<int const &, double &&>, double const &>);
    static_assert(::std::is_same_v<forward_like_t<int const &, double const &>, double const &>);
    static_assert(::std::is_same_v<forward_like_t<int const &, double const &&>, double const &>);
    static_assert(::std::is_same_v<forward_like_t<int const &, double volatile &>, double const volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int const &, double volatile &&>, double const volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int const &, double const volatile &>, double const volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int const &, double const volatile &&>,
        double const volatile &>);

    static_assert(::std::is_same_v<forward_like_t<int volatile &, double &>, double volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &, double &&>, double volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &, double const &>, double const volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &, double const &&>, double const volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &, double volatile &>, double volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &, double volatile &&>, double volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &, double const volatile &>,
        double const volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &, double const volatile &&>,
        double const volatile &>);

    static_assert(::std::is_same_v<forward_like_t<int const volatile &, double &>, double const volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &, double &&>, double const volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &, double const &>, double const volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &, double const &&>,
        double const volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &, double volatile &>,
        double const volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &, double volatile &&>,
        double const volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &, double const volatile &>,
        double const volatile &>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &, double const volatile &&>,
        double const volatile &>);

    static_assert(::std::is_same_v<forward_like_t<int &&, double &>, double &&>);
    static_assert(::std::is_same_v<forward_like_t<int &&, double &&>, double &&>);
    static_assert(::std::is_same_v<forward_like_t<int &&, double const &>, double const &&>);
    static_assert(::std::is_same_v<forward_like_t<int &&, double const &&>, double const &&>);
    static_assert(::std::is_same_v<forward_like_t<int &&, double volatile &>, double volatile &&>);
    static_assert(::std::is_same_v<forward_like_t<int &&, double volatile &&>, double volatile &&>);
    static_assert(::std::is_same_v<forward_like_t<int &&, double const volatile &>, double const volatile &&>);
    static_assert(::std::is_same_v<forward_like_t<int &&, double const volatile &&>, double const volatile &&>);

    static_assert(::std::is_same_v<forward_like_t<int const &&, double &>, double const &&>);
    static_assert(::std::is_same_v<forward_like_t<int const &&, double &&>, double const &&>);
    static_assert(::std::is_same_v<forward_like_t<int const &&, double const &>, double const &&>);
    static_assert(::std::is_same_v<forward_like_t<int const &&, double const &&>, double const &&>);
    static_assert(::std::is_same_v<forward_like_t<int const &&, double volatile &>, double const volatile &&>);
    static_assert(::std::is_same_v<forward_like_t<int const &&, double volatile &&>, double const volatile &&>);
    static_assert(::std::is_same_v<forward_like_t<int const &&, double const volatile &>,
        double const volatile &&>);
    static_assert(::std::is_same_v<forward_like_t<int const &&, double const volatile &&>,
        double const volatile &&>);

    static_assert(::std::is_same_v<forward_like_t<int volatile &&, double &>, double volatile &&>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &&, double &&>, double volatile &&>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &&, double const &>, double const volatile &&>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &&, double const &&>, double const volatile &&>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &&, double volatile &>, double volatile &&>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &&, double volatile &&>, double volatile &&>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &&, double const volatile &>,
        double const volatile &&>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &&, double const volatile &&>,
        double const volatile &&>);

    static_assert(::std::is_same_v<forward_like_t<int const volatile &&, double &>, double const volatile &&>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &&, double &&>, double const volatile &&>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &&, double const &>,
        double const volatile &&>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &&, double const &&>,
        double const volatile &&>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &&, double volatile &>,
        double const volatile &&>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &&, double volatile &&>,
        double const volatile &&>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &&, double const volatile &>,
        double const volatile &&>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &&, double const volatile &&>,
        double const volatile &&>);

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, double &>, double &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, double &&>, double &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, double const &>, double const &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, double const &&>, double const &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, double volatile &>, double volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, double volatile &&>, double volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, double const volatile &>, double const volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, double const volatile &&>, double const volatile &>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, double &>, double const &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, double &&>, double const &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, double const &>, double const &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, double const &&>, double const &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, double volatile &>, double const volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, double volatile &&>, double const volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, double const volatile &>,
        double const volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, double const volatile &&>,
        double const volatile &>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, double &>, double volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, double &&>, double volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, double const &>, double const volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, double const &&>, double const volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, double volatile &>, double volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, double volatile &&>, double volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, double const volatile &>,
        double const volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, double const volatile &&>,
        double const volatile &>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, double &>, double const volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, double &&>, double const volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, double const &>,
        double const volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, double const &&>,
        double const volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, double volatile &>,
        double const volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, double volatile &&>,
        double const volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, double const volatile &>,
        double const volatile &>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, double const volatile &&>,
        double const volatile &>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, double &>, double &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, double &&>, double &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, double const &>, double const &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, double const &&>, double const &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, double volatile &>, double volatile &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, double volatile &&>, double volatile &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, double const volatile &>, double const volatile &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, double const volatile &&>, double const volatile &&>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, double &>, double const &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, double &&>, double const &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, double const &>, double const &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, double const &&>, double const &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, double volatile &>, double const volatile &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, double volatile &&>, double const volatile &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, double const volatile &>,
        double const volatile &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, double const volatile &&>,
        double const volatile &&>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, double &>, double volatile &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, double &&>, double volatile &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, double const &>, double const volatile &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, double const &&>, double const volatile &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, double volatile &>, double volatile &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, double volatile &&>, double volatile &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, double const volatile &>,
        double const volatile &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, double const volatile &&>,
        double const volatile &&>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, double &>, double const volatile &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, double &&>, double const volatile &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, double const &>,
        double const volatile &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, double const &&>,
        double const volatile &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, double volatile &>,
        double const volatile &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, double volatile &&>,
        double const volatile &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, double const volatile &>,
        double const volatile &&>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, double const volatile &&>,
        double const volatile &&>));
}

TEST(ForwardLike, VoidSpecialCase)
{
    // Reference is not added to void; remains void with cv qualifiers.
    static_assert(::std::is_same_v<forward_like_t<int, void>, void>);
    static_assert(::std::is_same_v<forward_like_t<int, void const>, void const>);
    static_assert(::std::is_same_v<forward_like_t<int, void volatile>, void volatile>);
    static_assert(::std::is_same_v<forward_like_t<int, void const volatile>, void const volatile>);

    static_assert(::std::is_same_v<forward_like_t<int const, void>, void const>);
    static_assert(::std::is_same_v<forward_like_t<int const, void const>, void const>);
    static_assert(::std::is_same_v<forward_like_t<int const, void volatile>, void const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const, void const volatile>, void const volatile>);

    static_assert(::std::is_same_v<forward_like_t<int volatile, void>, void volatile>);
    static_assert(::std::is_same_v<forward_like_t<int volatile, void const>, void const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int volatile, void volatile>, void volatile>);
    static_assert(::std::is_same_v<forward_like_t<int volatile, void const volatile>, void const volatile>);

    static_assert(::std::is_same_v<forward_like_t<int const volatile, void>, void const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile, void const>, void const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile, void volatile>, void const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile, void const volatile>, void const volatile>);

    static_assert(::std::is_same_v<forward_like_t<int &&, void>, void>);
    static_assert(::std::is_same_v<forward_like_t<int &&, void const>, void const>);
    static_assert(::std::is_same_v<forward_like_t<int &&, void volatile>, void volatile>);
    static_assert(::std::is_same_v<forward_like_t<int &&, void const volatile>, void const volatile>);

    static_assert(::std::is_same_v<forward_like_t<int const &&, void>, void const>);
    static_assert(::std::is_same_v<forward_like_t<int const &&, void const>, void const>);
    static_assert(::std::is_same_v<forward_like_t<int const &&, void volatile>, void const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const &&, void const volatile>, void const volatile>);

    static_assert(::std::is_same_v<forward_like_t<int volatile &&, void>, void volatile>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &&, void const>, void const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &&, void volatile>, void volatile>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &&, void const volatile>, void const volatile>);

    static_assert(::std::is_same_v<forward_like_t<int const volatile &&, void>, void const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &&, void const>, void const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &&, void volatile>, void const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &&, void const volatile>, void const volatile>);

    static_assert(::std::is_same_v<forward_like_t<int &, void>, void>);
    static_assert(::std::is_same_v<forward_like_t<int &, void const>, void const>);
    static_assert(::std::is_same_v<forward_like_t<int &, void volatile>, void volatile>);
    static_assert(::std::is_same_v<forward_like_t<int &, void const volatile>, void const volatile>);

    static_assert(::std::is_same_v<forward_like_t<int const &, void>, void const>);
    static_assert(::std::is_same_v<forward_like_t<int const &, void const>, void const>);
    static_assert(::std::is_same_v<forward_like_t<int const &, void volatile>, void const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const &, void const volatile>, void const volatile>);

    static_assert(::std::is_same_v<forward_like_t<int volatile &, void>, void volatile>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &, void const>, void const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &, void volatile>, void volatile>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &, void const volatile>, void const volatile>);

    static_assert(::std::is_same_v<forward_like_t<int const volatile &, void>, void const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &, void const>, void const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &, void volatile>, void const volatile>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &, void const volatile>, void const volatile>);

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int, void>, void>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int, void const>, void const>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int, void volatile>, void volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int, void const volatile>, void const volatile>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, void>, void const>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, void const>, void const>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, void volatile>, void const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, void const volatile>, void const volatile>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, void>, void volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, void const>, void const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, void volatile>, void volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, void const volatile>, void const volatile>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, void>, void const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, void const>, void const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, void volatile>, void const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, void const volatile>, void const volatile>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, void>, void>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, void const>, void const>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, void volatile>, void volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, void const volatile>, void const volatile>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, void>, void const>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, void const>, void const>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, void volatile>, void const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, void const volatile>, void const volatile>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, void>, void volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, void const>, void const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, void volatile>, void volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, void const volatile>, void const volatile>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, void>, void const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, void const>, void const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, void volatile>, void const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, void const volatile>,
        void const volatile>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, void>, void>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, void const>, void const>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, void volatile>, void volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, void const volatile>, void const volatile>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, void>, void const>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, void const>, void const>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, void volatile>, void const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, void const volatile>, void const volatile>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, void>, void volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, void const>, void const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, void volatile>, void volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, void const volatile>, void const volatile>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, void>, void const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, void const>, void const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, void volatile>, void const volatile>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, void const volatile>,
        void const volatile>));
}

TEST(ForwardLike, ArraysAndOtherTypes)
{
    // forward_like_t applies cv qualifiers to the element type and preserves array type
    static_assert(::std::is_same_v<forward_like_t<int, int[3]>, int[3]>);
    static_assert(::std::is_same_v<forward_like_t<int const, int[3]>, int const[3]>);
    static_assert(::std::is_same_v<forward_like_t<int volatile, int[3]>, int volatile[3]>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile, int[3]>, int const volatile[3]>);

    static_assert(::std::is_same_v<forward_like_t<int, int const[3]>, int const[3]>);
    static_assert(::std::is_same_v<forward_like_t<int const, int const[3]>, int const[3]>);
    static_assert(::std::is_same_v<forward_like_t<int volatile, int const[3]>, int const volatile[3]>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile, int const[3]>, int const volatile[3]>);

    static_assert(::std::is_same_v<forward_like_t<int, int volatile[3]>, int volatile[3]>);
    static_assert(::std::is_same_v<forward_like_t<int const, int volatile[3]>, int const volatile[3]>);
    static_assert(::std::is_same_v<forward_like_t<int volatile, int volatile[3]>, int volatile[3]>);
    static_assert(
        ::std::is_same_v<forward_like_t<int const volatile, int volatile[3]>, int const volatile[3]>);

    static_assert(::std::is_same_v<forward_like_t<int, int const volatile[3]>, int const volatile[3]>);
    static_assert(::std::is_same_v<forward_like_t<int const, int const volatile[3]>, int const volatile[3]>);
    static_assert(
        ::std::is_same_v<forward_like_t<int volatile, int const volatile[3]>, int const volatile[3]>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile, int const volatile[3]>,
        int const volatile[3]>);

    static_assert(::std::is_same_v<forward_like_t<int &&, int[3]>, int (&&)[3]>);
    static_assert(::std::is_same_v<forward_like_t<int const &&, int[3]>, int const(&&)[3]>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &&, int[3]>, int volatile(&&)[3]>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &&, int[3]>, int const volatile(&&)[3]>);

    static_assert(::std::is_same_v<forward_like_t<int &&, int const[3]>, int const(&&)[3]>);
    static_assert(::std::is_same_v<forward_like_t<int const &&, int const[3]>, int const(&&)[3]>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &&, int const[3]>, int const volatile(&&)[3]>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &&, int const[3]>,
        int const volatile(&&)[3]>);

    static_assert(::std::is_same_v<forward_like_t<int &&, int volatile[3]>, int volatile(&&)[3]>);
    static_assert(::std::is_same_v<forward_like_t<int const &&, int volatile[3]>, int const volatile(&&)[3]>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &&, int volatile[3]>, int volatile(&&)[3]>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &&, int volatile[3]>,
        int const volatile(&&)[3]>);

    static_assert(::std::is_same_v<forward_like_t<int &&, int const volatile[3]>, int const volatile(&&)[3]>);
    static_assert(::std::is_same_v<forward_like_t<int const &&, int const volatile[3]>,
        int const volatile(&&)[3]>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &&, int const volatile[3]>,
        int const volatile(&&)[3]>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &&, int const volatile[3]>,
        int const volatile(&&)[3]>);

    static_assert(::std::is_same_v<forward_like_t<int &, int[3]>, int (&)[3]>);
    static_assert(::std::is_same_v<forward_like_t<int const &, int[3]>, int const(&)[3]>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &, int[3]>, int volatile(&)[3]>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &, int[3]>, int const volatile(&)[3]>);

    static_assert(::std::is_same_v<forward_like_t<int &, int const[3]>, int const(&)[3]>);
    static_assert(::std::is_same_v<forward_like_t<int const &, int const[3]>, int const(&)[3]>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &, int const[3]>, int const volatile(&)[3]>);
    static_assert(
        ::std::is_same_v<forward_like_t<int const volatile &, int const[3]>, int const volatile(&)[3]>);

    static_assert(::std::is_same_v<forward_like_t<int &, int volatile[3]>, int volatile(&)[3]>);
    static_assert(::std::is_same_v<forward_like_t<int const &, int volatile[3]>, int const volatile(&)[3]>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &, int volatile[3]>, int volatile(&)[3]>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &, int volatile[3]>,
        int const volatile(&)[3]>);

    static_assert(::std::is_same_v<forward_like_t<int &, int const volatile[3]>, int const volatile(&)[3]>);
    static_assert(
        ::std::is_same_v<forward_like_t<int const &, int const volatile[3]>, int const volatile(&)[3]>);
    static_assert(::std::is_same_v<forward_like_t<int volatile &, int const volatile[3]>,
        int const volatile(&)[3]>);
    static_assert(::std::is_same_v<forward_like_t<int const volatile &, int const volatile[3]>,
        int const volatile(&)[3]>);

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int, int[3]>, int[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, int[3]>, int const[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, int[3]>, int volatile[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, int[3]>, int const volatile[3]>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int, int const[3]>, int const[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, int const[3]>, int const[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, int const[3]>, int const volatile[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, int const[3]>, int const volatile[3]>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int, int volatile[3]>, int volatile[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, int volatile[3]>, int const volatile[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile, int volatile[3]>, int volatile[3]>));
    EXPECT_TRUE((
        ::std::is_same_v<forward_like_t<int const volatile, int volatile[3]>, int const volatile[3]>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int, int const volatile[3]>, int const volatile[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const, int const volatile[3]>, int const volatile[3]>));
    EXPECT_TRUE((
        ::std::is_same_v<forward_like_t<int volatile, int const volatile[3]>, int const volatile[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile, int const volatile[3]>,
        int const volatile[3]>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, int[3]>, int (&&)[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, int[3]>, int const(&&)[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, int[3]>, int volatile(&&)[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, int[3]>, int const volatile(&&)[3]>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, int const[3]>, int const(&&)[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, int const[3]>, int const(&&)[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, int const[3]>, int const volatile(&&)[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, int const[3]>,
        int const volatile(&&)[3]>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, int volatile[3]>, int volatile(&&)[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, int volatile[3]>, int const volatile(&&)[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, int volatile[3]>, int volatile(&&)[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, int volatile[3]>,
        int const volatile(&&)[3]>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &&, int const volatile[3]>, int const volatile(&&)[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &&, int const volatile[3]>,
        int const volatile(&&)[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &&, int const volatile[3]>,
        int const volatile(&&)[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &&, int const volatile[3]>,
        int const volatile(&&)[3]>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, int[3]>, int (&)[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, int[3]>, int const(&)[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, int[3]>, int volatile(&)[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, int[3]>, int const volatile(&)[3]>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, int const[3]>, int const(&)[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, int const[3]>, int const(&)[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, int const[3]>, int const volatile(&)[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, int const[3]>,
        int const volatile(&)[3]>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, int volatile[3]>, int volatile(&)[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, int volatile[3]>, int const volatile(&)[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, int volatile[3]>, int volatile(&)[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, int volatile[3]>,
        int const volatile(&)[3]>));

    EXPECT_TRUE((::std::is_same_v<forward_like_t<int &, int const volatile[3]>, int const volatile(&)[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const &, int const volatile[3]>,
        int const volatile(&)[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int volatile &, int const volatile[3]>,
        int const volatile(&)[3]>));
    EXPECT_TRUE((::std::is_same_v<forward_like_t<int const volatile &, int const volatile[3]>,
        int const volatile(&)[3]>));
}
