#include <gtest_utils.h>

#include <scl/utility/any/std_any.h>
#include <scl/utility/preprocessor/rtti.h>

#if SCL_HAS_RTTI
#include <any>
#endif
#include <string>
#include <tuple>
#include <type_traits>

#if SCL_HAS_RTTI

namespace
{
    struct boxed_counted
    {
        inline static int copies = 0;
        inline static int moves = 0;

        int id = 0;

        boxed_counted() = default;
        explicit boxed_counted(int value) noexcept
            : id{value}
        {}
        boxed_counted(boxed_counted const & other) noexcept
            : id{other.id}
        {
            ++copies;
        }
        boxed_counted(boxed_counted && other) noexcept
            : id{other.id}
        {
            ++moves;
        }
        boxed_counted & operator=(boxed_counted const &) = default;
        boxed_counted & operator=(boxed_counted &&) = default;
        ~boxed_counted() = default;
    };
} // namespace

namespace
{
    template <typename Source, typename Target>
    concept castable_to = requires(Source * source) { ::scl::any_cast<Target>(source); };

    template <typename Source>
    concept pointer_castable = castable_to<Source, int>;

    template <typename Source>
    concept value_castable = requires(Source & source) { ::scl::any_cast<int>(source); };

    template <typename Source>
    concept bound_castable = requires(Source & source) { ::scl::any_cast<int const &>(source); };

    template <typename Source>
    concept writing_castable = requires(Source & source) { ::scl::any_cast<int &>(source); };
} // namespace

TEST(StdAnyTest, CastReachesTheBoxedObject)
{
    ::std::any const boxed{::std::string{"hello"}};

    ASSERT_NE(::scl::any_cast<::std::string>(&boxed), nullptr);
    EXPECT_EQ(*::scl::any_cast<::std::string>(&boxed), "hello");
}

TEST(StdAnyTest, CastRefusesAnotherType)
{
    ::std::any const boxed{::std::string{"hello"}};

    EXPECT_EQ(::scl::any_cast<int>(&boxed), nullptr);
}

TEST(StdAnyTest, CastRefusesAnEmptyBox)
{
    ::std::any const empty;

    EXPECT_EQ(::scl::any_cast<int>(&empty), nullptr);
}

TEST(StdAnyTest, CastRefusesANullBox)
{
    EXPECT_EQ(::scl::any_cast<int>(static_cast<::std::any const *>(nullptr)), nullptr);
}

TEST(StdAnyTest, RequestQualificationTakesNoPartInTheMatch)
{
    ::std::any const boxed{42};

    EXPECT_EQ(::scl::any_cast<int const>(&boxed), ::scl::any_cast<int>(&boxed));
}

TEST(StdAnyTest, ReferenceFormBindsTheBoxedObject)
{
    ::std::any const boxed{::std::string{"hello"}};

    EXPECT_EQ(::scl::any_cast<::std::string const &>(boxed), "hello");
    EXPECT_EQ(&::scl::any_cast<::std::string const &>(boxed), ::scl::any_cast<::std::string>(&boxed));
}

TEST(StdAnyTest, ReferenceFormDoesNotCopyTheBoxedObject)
{
    ::std::any const boxed{boxed_counted{5}};
    boxed_counted::copies = 0; // ignore copies incurred while boxing

    EXPECT_EQ(::scl::any_cast<boxed_counted const &>(boxed).id, 5);
    EXPECT_EQ(boxed_counted::copies, 0);
}

TEST(StdAnyTest, ValueFormCopiesTheBoxedObjectOut)
{
    ::std::any const boxed{::std::string{"hello"}};

    auto const copied = ::scl::any_cast<::std::string>(boxed);

    EXPECT_EQ(copied, "hello");
    EXPECT_NE(&copied, ::scl::any_cast<::std::string>(&boxed));
}

TEST(StdAnyTest, ReferenceFormThrowsOnAnotherType)
{
    ::std::any const boxed{::std::string{"hello"}};

    EXPECT_THROW(::std::ignore = ::scl::any_cast<int>(boxed), ::scl::bad_any_cast);
}

TEST(StdAnyTest, AVolatileBoxIsRefusedRatherThanConvertedIntoAnotherBox)
{
    STATIC_EXPECT_TRUE(pointer_castable<::std::any>);
    STATIC_EXPECT_TRUE(pointer_castable<::std::any const>);
    STATIC_EXPECT_FALSE(pointer_castable<::std::any volatile>);
    STATIC_EXPECT_FALSE((castable_to<::std::any volatile, int *>));
    STATIC_EXPECT_FALSE(value_castable<::std::any volatile>);
    STATIC_EXPECT_FALSE(bound_castable<::std::any volatile>);
}

TEST(StdAnyTest, TheBoxsOwnConstnessReachesTheBoxedObject)
{
    ::std::any box{42};
    ::std::any const frozen{42};

    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(::scl::any_cast<int>(&box)), int *>));
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(::scl::any_cast<int>(&frozen)), int const *>));
    STATIC_EXPECT_TRUE(writing_castable<::std::any>);
    STATIC_EXPECT_FALSE(writing_castable<::std::any const>);
}

TEST(StdAnyTest, ANonConstBoxHandsOutAWrite)
{
    ::std::any boxed{::std::string{"hello"}};

    ASSERT_NE(::scl::any_cast<::std::string>(&boxed), nullptr);
    *::scl::any_cast<::std::string>(&boxed) += "!";

    EXPECT_EQ(::std::any_cast<::std::string>(boxed), "hello!");
}

TEST(StdAnyTest, ABoxWithNoNameHandsTheObjectOver)
{
    boxed_counted::copies = 0;
    boxed_counted::moves = 0;

    auto const taken = ::scl::any_cast<boxed_counted>(::std::any{boxed_counted{7}});

    EXPECT_EQ(taken.id, 7);
    EXPECT_EQ(boxed_counted::copies, 0);
    EXPECT_GT(boxed_counted::moves, 0);
}

TEST(StdAnyTest, ANamedBoxIsCopiedFrom)
{
    ::std::any const boxed{boxed_counted{9}};
    boxed_counted::copies = 0;
    boxed_counted::moves = 0;

    auto const taken = ::scl::any_cast<boxed_counted>(boxed);

    EXPECT_EQ(taken.id, 9);
    EXPECT_EQ(boxed_counted::copies, 1);
    EXPECT_EQ(boxed_counted::moves, 0);
}

#endif
