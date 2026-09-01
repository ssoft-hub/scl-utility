#include <gtest_utils.h>

#include <scl/utility/any.h>

#include <array>
#include <type_traits>

namespace
{
    constexpr int read_through_an_anchored_view()
    {
        int const probe = 42;
        ::scl::any_anchor const bound{probe};
        ::scl::any_view const view{bound};
        auto const * reached = ::scl::any_cast<int>(&view);

        return (reached != nullptr) ? *reached : -1;
    }

    constexpr int write_through_an_anchored_argument()
    {
        int probe = 21;
        ::scl::any_anchor const bound{probe};
        ::scl::any_argument const subject{bound};
        auto * const reached = ::scl::any_cast<int>(&subject);

        if (reached != nullptr)
            *reached *= 2;

        return probe;
    }

    // A writing handle hands its own description on, so the anchor reaches both the reading
    // view it narrows into and the argument that adopts it. An argument adopting a reading view
    // takes the shared const description instead, which stands beside no object.
    constexpr bool anchor_survives_narrowing()
    {
        int value = 42;
        ::scl::any_anchor const anchored{value};
        ::scl::any_mutable_view const writing{anchored};

        ::scl::any_view const narrowed = writing;
        ::scl::any_argument const adopted{writing};

        return ::scl::any_cast<int>(&narrowed) != nullptr &&
            ::scl::any_cast<int>(&adopted) != nullptr && *::scl::any_cast<int>(&narrowed) == 42;
    }

    void bound_to_nothing();

    template <typename Type>
    concept anchor_over = requires(Type & object) { ::scl::any_anchor{object}; };

    constexpr int one = 1;
    constexpr int two = 2;
    constexpr int three = 3;

    constexpr ::scl::any_anchor anchored_one{one};
    constexpr ::scl::any_anchor anchored_two{two};
    constexpr ::scl::any_anchor anchored_three{three};

    constexpr ::std::array<::scl::any_view, 3> anchored_views{anchored_one, anchored_two, anchored_three};

} // namespace

TEST(AnyAnchorTest, AnAnchoredViewReadsItsReferentDuringConstantEvaluation)
{
    STATIC_EXPECT_TRUE(read_through_an_anchored_view() == 42);
}

TEST(AnyAnchorTest, AnAnchoredArgumentWritesThroughItsReferentDuringConstantEvaluation)
{
    STATIC_EXPECT_TRUE(write_through_an_anchored_argument() == 42);
}

TEST(AnyAnchorTest, AnAnchoredViewIsStoredAndReadAsAConstantExpression)
{
    STATIC_EXPECT_TRUE(*::scl::any_cast<int>(&anchored_views[0]) == 1);
    STATIC_EXPECT_TRUE(*::scl::any_cast<int>(&anchored_views[2]) == 3);
}

TEST(AnyAnchorTest, AHandleBoundThroughAnAnchorNamesTheReferentAndNotTheAnchor)
{
    int const probe = 42;

    ::scl::any_anchor const bound{probe};
    ::scl::any_view const view{bound};

    EXPECT_EQ(view.type_name(), ::scl::type_name<int>());
    EXPECT_EQ(view.type_key(), ::scl::type_key_of<int>());
}

TEST(AnyAnchorTest, AnAnchoredHandleReadsItsReferentAtRunTime)
{
    int probe = 21;

    ::scl::any_anchor const bound{probe};
    ::scl::any_view const view{bound};
    ::scl::any_argument const subject{bound};

    EXPECT_EQ(*::scl::any_cast<int>(&view), 21);
    *::scl::any_cast<int>(&subject) *= 2;
    EXPECT_EQ(probe, 42);
}

TEST(AnyAnchorTest, AnAnchorKeepsTheReferentQualifiers)
{
    int const constant = 42;
    int volatile sensor = 7;

    ::scl::any_anchor const anchored_constant{constant};
    ::scl::any_anchor const anchored_sensor{sensor};

    ::scl::any_view const over_constant{anchored_constant};
    ::scl::any_view const over_sensor{anchored_sensor};

    EXPECT_EQ(*::scl::any_cast<int>(&over_constant), 42);
    EXPECT_EQ(::scl::any_cast<int>(&over_sensor), nullptr); // the request drops volatile
    EXPECT_EQ(*::scl::any_cast<int volatile>(&over_sensor), 7);
}

TEST(AnyAnchorTest, AnAnchorOverAConstReferentGrantsNoWriteAccess)
{
    int const constant = 42;

    ::scl::any_anchor const bound{constant};
    ::scl::any_argument const subject{bound};

    EXPECT_EQ(::scl::any_cast<int>(&subject), nullptr); // the request drops const
    EXPECT_EQ(*::scl::any_cast<int const>(&subject), 42);
}

TEST(AnyAnchorTest, NarrowingAHandleKeepsTheAnchorItWasBoundThrough)
{
    STATIC_EXPECT_TRUE(anchor_survives_narrowing());
}

TEST(AnyAnchorTest, AnAnchorStandsForAnObjectAndForNothingElse)
{
    // A function is no object, and neither the anchor nor a handle built through one admits
    // a function lvalue: the refusal is a constraint rather than an error inside the group.
    STATIC_EXPECT_FALSE(anchor_over<decltype(bound_to_nothing)>);
    STATIC_EXPECT_FALSE((::std::is_constructible_v<::scl::any_anchor<int()>, int (&)()>));
    STATIC_EXPECT_FALSE((::std::is_constructible_v<::scl::any_view, ::scl::any_anchor<int()> const &>));
    STATIC_EXPECT_FALSE(
        (::std::is_constructible_v<::scl::any_mutable_view, ::scl::any_anchor<int()> const &>));
    STATIC_EXPECT_FALSE((::std::is_constructible_v<::scl::any_argument, ::scl::any_anchor<int()> const &>));

    STATIC_EXPECT_TRUE(anchor_over<int>);
    STATIC_EXPECT_TRUE(anchor_over<int const>);
}

TEST(AnyAnchorTest, AnAnchorIsCopiedAndDestroyedByTheCompilersOwnDefinitions)
{
    // What lets an anchor stand beside the handles built through it and be used during
    // constant evaluation as freely as they are.
    STATIC_EXPECT_TRUE(::std::is_trivially_copyable_v<::scl::any_anchor<int>>);
    STATIC_EXPECT_TRUE(::std::is_trivially_copy_constructible_v<::scl::any_anchor<int>>);
    STATIC_EXPECT_TRUE(::std::is_trivially_move_constructible_v<::scl::any_anchor<int>>);
    STATIC_EXPECT_TRUE(::std::is_trivially_destructible_v<::scl::any_anchor<int>>);
    STATIC_EXPECT_FALSE(::std::is_default_constructible_v<::scl::any_anchor<int>>);
}
