#include <gtest_utils.h>

#include <scl/utility/any.h>

#include <array>

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
