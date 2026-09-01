#include <gtest_utils.h>

#include <scl/utility/any/any.h>
#include <scl/utility/meta/type_key.h>
#include <scl/utility/preprocessor/exceptions.h>

#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

namespace
{
    struct allocation_counter
    {
        inline static int allocations = 0;
        inline static int deallocations = 0;
        inline static ::std::vector<void *> live;

        // The counters start over, the ledger does not: a block taken before a reset is still
        // this allocator's to release, and clearing the ledger would leak it.
        static void reset()
        {
            allocations = 0;
            deallocations = 0;
        }
    };

    template <typename Type>
    struct counting_allocator
    {
        using value_type = Type;

        counting_allocator() = default;

        template <typename Other>
        // cppcheck-suppress noExplicitConstructor
        counting_allocator(counting_allocator<Other> const &) noexcept // NOLINT(*-explicit-*)
        {}

        [[nodiscard]]
        Type * allocate(::std::size_t count)
        {
            ++allocation_counter::allocations;
            Type * const acquired = ::std::allocator<Type>{}.allocate(count);
            allocation_counter::live.push_back(acquired);
            return acquired;
        }

        void deallocate(Type * pointer, ::std::size_t count) noexcept
        {
            ++allocation_counter::deallocations;

            auto const released = ::std::ranges::find(allocation_counter::live, static_cast<void *>(pointer));
            if (released == allocation_counter::live.end())
                return;

            allocation_counter::live.erase(released);
            ::std::allocator<Type>{}.deallocate(pointer, count);
        }

        friend bool operator==(counting_allocator const &, counting_allocator const &) = default;
    };

    using counted_any = ::scl::basic_any<counting_allocator<::std::byte>>;

    struct aggregate
    {
        int first = 0;
        double second = 0.0;
    };

    struct small_aggregate
    {
        char letter = '\0';
    };

    int doubled(int number) { return number * 2; }

    struct alignas(4 * alignof(void *)) wide_aligned
    {
        int value = 0;
    };

    // Aligned far more strictly than the buffer, so only the allocator path can hold it.
    struct alignas(128) hyper_aligned
    {
        char letter = 0;
    };

    struct immovable
    {
        int value;

        explicit immovable(int initial) noexcept
            : value{initial}
        {}

        immovable(immovable const &) = delete;
        immovable(immovable &&) = delete;
        immovable & operator=(immovable const &) = delete;
        immovable & operator=(immovable &&) = delete;
        ~immovable() = default;
    };

    template <typename Type>
    concept any_converts_from = ::std::is_convertible_v<Type, ::scl::any>;

    template <typename Type>
    concept any_assigns_value = requires(::scl::any & target,
        Type value) { target = ::std::move(value); };

    template <typename Type>
    concept any_constructs_in_place = requires { ::scl::any{::std::in_place_type<Type>}; };

    template <typename Type>
    concept any_emplaces = requires(::scl::any & target) { target.emplace<Type>(); };

    template <typename Type>
    concept any_reserves_space_for =
        requires(::scl::any & target) { target.reserve_space_for<Type>(); };

    template <typename Type>
    concept any_answers_space_for =
        requires(::scl::any const & target) { target.has_space_for<Type>(); };

    struct printable
    {
        virtual int printed() const = 0;

        printable() = default;
        printable(printable const &) = default;
        printable(printable &&) = default;
        printable & operator=(printable const &) = default;
        printable & operator=(printable &&) = default;
        virtual ~printable() = default;
    };

    template <typename Type>
    concept any_casts = requires(::scl::any & target) { ::scl::any_cast<Type>(&target); };

    template <typename Type>
    concept any_stores = requires(Type & object) { ::scl::any{object}; };

    template <typename Type>
    concept any_takes = requires(::scl::any & target, Type & object) { target = object; };

    // The object stands in a holder here rather than in the buffer, so the identity the
    // assignment answers is a different comparison from the one it makes at run time.
    constexpr bool self_assignment_keeps_the_object()
    {
        ::scl::any value{::std::in_place_type<::std::size_t>, 42U};
        auto const * const before = ::scl::any_cast<::std::size_t>(&value);

        value = *::scl::any_cast<::std::size_t>(&value);

        return ::scl::any_cast<::std::size_t>(&value) == before && *before == 42U;
    }

    template <typename Type, typename... Arguments>
    concept any_makes = requires { ::scl::make_any<Type>(::std::declval<Arguments>()...); };

    [[nodiscard]]
    constexpr int read_during_constant_evaluation()
    {
        ::scl::any const value{42};

        return *::scl::any_cast<int>(&value);
    }

    template <typename Type>
    concept any_assigns_lvalue = requires(::scl::any & target, Type value) { target = value; };

    template <typename Type>
    concept any_constructs_in_place_from_an_int =
        requires { ::scl::any{::std::in_place_type<Type>, 0}; };

    template <typename Type>
    concept any_emplaces_from_an_int = requires(::scl::any & target) { target.emplace<Type>(0); };

    // The descriptor carries no way to end such an object, so an owner must not take one.
    struct destructor_may_throw
    {
        destructor_may_throw() = default;
        destructor_may_throw(destructor_may_throw const &) = default;
        destructor_may_throw(destructor_may_throw &&) = default;
        destructor_may_throw & operator=(destructor_may_throw const &) = default;
        destructor_may_throw & operator=(destructor_may_throw &&) = default;
        ~destructor_may_throw() noexcept(false) {}
    };

    struct trace
    {
        int copies = 0;
        int destructions = 0;
    };

    struct traced
    {
        trace * log = nullptr;

        explicit traced(trace & shared) noexcept
            : log{&shared}
        {}

        traced(traced const & other) noexcept
            : log{other.log}
        {
            ++log->copies;
        }

        traced(traced && other) noexcept
            : log{other.log}
        {
            other.log = nullptr;
        }

        traced & operator=(traced const &) = default;
        traced & operator=(traced &&) = default;

        ~traced()
        {
            if (log != nullptr)
                ++log->destructions;
        }
    };

    // Too wide for the buffer, so its lifetime is watched on the allocated path.
    struct traced_pair
    {
        trace * log = nullptr;
        double first = 0.0;
        double second = 0.0;

        explicit traced_pair(trace & shared) noexcept
            : log{&shared}
        {}

        traced_pair(traced_pair const & other) noexcept
            : log{other.log}
            , first{other.first}
            , second{other.second}
        {
            ++log->copies;
        }

        traced_pair(traced_pair && other) noexcept
            : log{other.log}
            , first{other.first}
            , second{other.second}
        {
            other.log = nullptr;
        }

        traced_pair & operator=(traced_pair const &) = default;
        traced_pair & operator=(traced_pair &&) = default;

        ~traced_pair()
        {
            if (log != nullptr)
                ++log->destructions;
        }
    };

    [[nodiscard]]
    constexpr ::std::size_t constant_allocated_lifecycle()
    {
        ::scl::any value{::std::string{"Hello Any!"}};
        ::std::size_t const size = ::scl::any_cast<::std::string>(&value)->size();

        value.reset();
        return size;
    }

    [[nodiscard]]
    constexpr ::std::size_t constant_replacement()
    {
        ::scl::any value{::std::string{"Hello Any!"}};
        value = ::std::string{"Hello Again!"};
        value.emplace<::std::string>("xxx");

        ::std::size_t const length = ::scl::any_cast<::std::string>(&value)->size();

        value.reset();
        return length;
    }

#if SCL_HAS_EXCEPTIONS
    struct construction_failure
    {};

    struct throwing
    {
        throwing() { throw construction_failure{}; }
    };
#endif
} // namespace

TEST(AnyTest, DefaultConstructedAnyHoldsNothing)
{
    ::scl::any const value;

    EXPECT_FALSE(value.has_value());
    EXPECT_TRUE(value.type_key() == ::scl::type_key{});
    EXPECT_TRUE(value.type_name().empty());
}

TEST(AnyTest, ATypeWhoseDestructorMayThrowIsRefused)
{
    STATIC_EXPECT_FALSE(any_converts_from<destructor_may_throw>);
    STATIC_EXPECT_FALSE(any_assigns_value<destructor_may_throw>);
    STATIC_EXPECT_FALSE(any_constructs_in_place<destructor_may_throw>);
    STATIC_EXPECT_FALSE(any_emplaces<destructor_may_throw>);
}

TEST(AnyTest, ATypeTheArgumentsCannotBuildIsRefused)
{
    // An lvalue of a move-only type: the trait must answer before the call, not after a hard
    // error inside the library.
    STATIC_EXPECT_FALSE(any_converts_from<::std::unique_ptr<int> &>);
    STATIC_EXPECT_TRUE(any_converts_from<::std::unique_ptr<int>>);
    STATIC_EXPECT_FALSE(any_assigns_lvalue<::std::unique_ptr<int>>);
    STATIC_EXPECT_TRUE(any_assigns_value<::std::unique_ptr<int>>);

    STATIC_EXPECT_FALSE(any_constructs_in_place_from_an_int<::std::unique_ptr<int>>);
    STATIC_EXPECT_FALSE(any_emplaces_from_an_int<::std::unique_ptr<int>>);
    STATIC_EXPECT_TRUE(any_constructs_in_place_from_an_int<int>);
    STATIC_EXPECT_TRUE(any_emplaces_from_an_int<int>);
}

TEST(AnyTest, OnlyAStringLiteralIsStoredAsAPointerToItsElements)
{
    // An array reaches the any as a pointer, and a pointer owns nothing. The type of a
    // string literal is the one array admitted, its elements outliving any use of them.
    STATIC_EXPECT_TRUE(any_stores<char const[6]>);
    STATIC_EXPECT_TRUE(any_stores<char16_t const[6]>);

    STATIC_EXPECT_FALSE(any_stores<char[6]>);
    STATIC_EXPECT_FALSE(any_stores<int[3]>);
    STATIC_EXPECT_FALSE(any_stores<int const[3]>);

    STATIC_EXPECT_TRUE(any_takes<char const[6]>);
    STATIC_EXPECT_FALSE(any_takes<int[3]>);
    STATIC_EXPECT_FALSE(any_takes<int const[3]>);

    // A literal has one extent, so an array of arrays is refused with the rest.
    STATIC_EXPECT_FALSE(any_stores<char const[2][3]>);
    STATIC_EXPECT_FALSE(any_takes<char const[2][3]>);

    // Naming the type instead of deducing it changes nothing.
    STATIC_EXPECT_TRUE(any_constructs_in_place<char const[6]>);
    STATIC_EXPECT_FALSE(any_constructs_in_place<int[3]>);
    STATIC_EXPECT_FALSE(any_constructs_in_place<char[3]>);
    STATIC_EXPECT_FALSE(any_constructs_in_place<char const[2][3]>);
    STATIC_EXPECT_FALSE(any_emplaces<int[3]>);
    STATIC_EXPECT_FALSE(any_makes<int[3]>);

    // A reference to an array is that array, so spelling one changes nothing either.
    STATIC_EXPECT_FALSE(any_constructs_in_place<int (&)[3]>);
    STATIC_EXPECT_FALSE(any_emplaces<int (&)[3]>);
    STATIC_EXPECT_FALSE(any_makes<int (&)[3]>);
    STATIC_EXPECT_TRUE(any_constructs_in_place<char const(&)[6]>);

    // A reference to anything else still names what it refers to.
    STATIC_EXPECT_TRUE(any_emplaces<int &>);

    // make_any answers for the same type the in-place form does, and refuses what it refuses.
    STATIC_EXPECT_TRUE((any_makes<int, int>));
    STATIC_EXPECT_FALSE(any_makes<destructor_may_throw>);
    STATIC_EXPECT_FALSE((any_makes<int, char const *>));

    // The elements themselves are stored by naming a type that owns them.
    ::scl::any const owned{
        ::std::array<int, 3>{1, 2, 3}
    };
    EXPECT_TRUE((owned.type_key() == ::scl::type_key_of<::std::array<int, 3>>()));
}

TEST(AnyTest, ACastReachesTheStoredObjectWithTheOwnersOwnConstness)
{
    ::scl::any value{42};
    ::scl::any const frozen{42};

    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(::scl::any_cast<int>(&value)), int *>));
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(::scl::any_cast<int>(&frozen)), int const *>));

    ASSERT_NE(::scl::any_cast<int>(&value), nullptr);
    EXPECT_EQ(*::scl::any_cast<int>(&value), 42);
    ASSERT_NE(::scl::any_cast<int>(&frozen), nullptr);
    EXPECT_EQ(*::scl::any_cast<int>(&frozen), 42);
}

TEST(AnyTest, ACastOverAnOwnerAnswersNullForAnotherType)
{
    ::scl::any const value{42};

    EXPECT_EQ(::scl::any_cast<::std::string>(&value), nullptr);
}

TEST(AnyTest, ACastOverAnEmptyOwnerAnswersNull)
{
    ::scl::any const value;

    EXPECT_EQ(::scl::any_cast<int>(&value), nullptr);
}

TEST(AnyTest, TheBufferCapacityIsAtLeastAPointerWide)
{
    STATIC_EXPECT_EQ(::scl::any::buffer_capacity, sizeof(void *));
    STATIC_EXPECT_EQ((::scl::basic_any<::std::allocator<::std::byte>, 1>::buffer_capacity), sizeof(void *));
    STATIC_EXPECT_EQ((::scl::basic_any<::std::allocator<::std::byte>, 48>::buffer_capacity), 48U);
}

TEST(AnyTest, ACastOverAnOwnerIsOfferedForAnObjectTypeAndFoldsAtCompileTime)
{
    STATIC_EXPECT_TRUE(any_casts<int>);
    STATIC_EXPECT_FALSE(any_casts<int &>);
    STATIC_EXPECT_FALSE(any_casts<void>);
    STATIC_EXPECT_EQ(read_during_constant_evaluation(), 42);
}

TEST(AnyTest, AnyCostsNoMoreThanTwoPointers)
{
    STATIC_EXPECT_EQ(sizeof(::scl::any), 2 * sizeof(void *));
}

TEST(AnyTest, AnInPlaceObjectIsOwnedWithoutTheAllocator)
{
    trace log;
    allocation_counter::reset();

    {
        counted_any const value{traced{log}};

        EXPECT_EQ(allocation_counter::allocations, 0);
    }

    EXPECT_EQ(log.destructions, 1);
}

TEST(AnyTest, ConstructedAnyCarriesItsValueAndItsType)
{
    ::scl::any value{42};

    EXPECT_TRUE(value.has_value());
    EXPECT_TRUE(value.type_key() == ::scl::type_key_of<int>());
    EXPECT_EQ(*::scl::any_cast<int>(&value), 42);
}

TEST(AnyTest, AnyIsMoveOnly)
{
    STATIC_EXPECT_FALSE(::std::copy_constructible<::scl::any>);
    STATIC_EXPECT_TRUE(::std::move_constructible<::scl::any>);
}

TEST(AnyTest, HandingAnObjectOverAndGivingItUpThrowNothing)
{
    // A move takes over an allocated object by its pointer and relocates a buffer-held one,
    // which only a nothrow-movable type ever is, so neither step has anything to throw.
    ::scl::any value{42};
    ::scl::any other{::std::string{"Hello Any!"}};

    STATIC_EXPECT_TRUE(::std::is_nothrow_move_constructible_v<::scl::any>);
    STATIC_EXPECT_TRUE(::std::is_nothrow_move_assignable_v<::scl::any>);
    STATIC_EXPECT_TRUE(::std::is_nothrow_destructible_v<::scl::any>);
    STATIC_EXPECT_TRUE(noexcept(value.swap(other)));
    STATIC_EXPECT_TRUE(noexcept(swap(value, other)));
    STATIC_EXPECT_TRUE(noexcept(value.reset()));
}

TEST(AnyTest, MovingAnInPlaceObjectRelocatesItAndEmptiesTheSource)
{
    trace log;
    counted_any source{traced{log}};

    counted_any const target{::std::move(source)};

    // NOLINTNEXTLINE(bugprone-use-after-move): emptiness after a move is the contract
    EXPECT_FALSE(source.has_value());
    EXPECT_EQ(log.copies, 0);
    EXPECT_EQ(::scl::any_cast<traced>(&target)->log, &log);
}

TEST(AnyTest, MovingAnAllocatedObjectHandsOverThePointer)
{
    allocation_counter::reset();
    counted_any source{::std::string{"Hello Any!"}};

    counted_any const target{::std::move(source)};

    EXPECT_EQ(allocation_counter::allocations, 1);
    // NOLINTNEXTLINE(bugprone-use-after-move): emptiness after a move is the contract
    EXPECT_FALSE(source.has_value());
    EXPECT_EQ(*::scl::any_cast<::std::string>(&target), "Hello Any!");
}

TEST(AnyTest, MoveAssignmentEndsTheObjectItReplaces)
{
    trace log;
    counted_any target{traced{log}};
    counted_any source{42};

    target = ::std::move(source);

    EXPECT_EQ(log.destructions, 1);
    EXPECT_EQ(*::scl::any_cast<int>(&target), 42);
}

TEST(AnyTest, ResetEndsTheObjectAndEmptiesTheAny)
{
    trace log;
    counted_any value{traced{log}};

    value.reset();

    EXPECT_FALSE(value.has_value());
    EXPECT_EQ(log.destructions, 1);
}

TEST(AnyTest, EmplaceBuildsFromArgumentsAndReplacesWhatWasHeld)
{
    trace log;
    counted_any value{traced{log}};

    auto & built = value.emplace<::std::string>(3U, 'x');

    EXPECT_EQ(log.destructions, 1);
    EXPECT_EQ(built, "xxx");
    EXPECT_EQ(*::scl::any_cast<::std::string>(&value), "xxx");
}

TEST(AnyTest, InPlaceTypeConstructsWithoutACopy)
{
    trace log;

    counted_any const value{::std::in_place_type<traced>, log};

    EXPECT_EQ(log.copies, 0);
    EXPECT_EQ(::scl::any_cast<traced>(&value)->log, &log);
}

TEST(AnyTest, AStringLiteralIsStoredAsAPointer)
{
    ::scl::any const value{"Hello Any!"};

    EXPECT_TRUE(value.type_key() == ::scl::type_key_of<char const *>());
    EXPECT_STREQ(*::scl::any_cast<char const *>(&value), "Hello Any!");
}

TEST(AnyTest, AFunctionIsStoredAsAPointer)
{
    ::scl::any const value{doubled};

    EXPECT_TRUE(value.type_key() == ::scl::type_key_of<int (*)(int)>());
    EXPECT_EQ((*::scl::any_cast<int (*)(int)>(&value))(21), 42);
}

TEST(AnyTest, AConstructionTagIsNotStoredAsAValue)
{
    STATIC_EXPECT_FALSE(any_converts_from<::std::in_place_type_t<int>>);
    STATIC_EXPECT_FALSE(any_converts_from<::std::allocator_arg_t>);
    STATIC_EXPECT_FALSE(any_assigns_value<::std::in_place_type_t<int>>);
    STATIC_EXPECT_FALSE(any_assigns_value<::std::allocator_arg_t>);
}

TEST(AnyTest, AnOverAlignedObjectLandsOnStorageThatSuitsIt)
{
    ::scl::any const value{wide_aligned{.value = 42}};

    auto const * const stored = ::scl::any_cast<wide_aligned>(&value);

    ASSERT_NE(stored, nullptr);
    EXPECT_EQ(reinterpret_cast<::std::uintptr_t>(stored) % alignof(wide_aligned), 0U);
    EXPECT_EQ(stored->value, 42);
}

TEST(AnyTest, AnAlignmentStricterThanTheBufferIsServedByTheAllocator)
{
    STATIC_EXPECT_TRUE(any_converts_from<hyper_aligned>);
    STATIC_EXPECT_TRUE(any_assigns_value<hyper_aligned>);
    STATIC_EXPECT_TRUE(any_constructs_in_place<hyper_aligned>);
    STATIC_EXPECT_TRUE(any_emplaces<hyper_aligned>);

    allocation_counter::reset();
    counted_any value{hyper_aligned{}};
    auto const * const stored = ::scl::any_cast<hyper_aligned>(&value);

    ASSERT_NE(stored, nullptr);
    EXPECT_EQ(allocation_counter::allocations, 1);
    EXPECT_EQ(reinterpret_cast<::std::uintptr_t>(stored) % alignof(hyper_aligned), 0U);
}

TEST(AnyTest, AnImmovableObjectIsHeldAndTravelsByItsPointer)
{
    ::scl::any source{::std::in_place_type<immovable>, 42};
    auto const * const placed = ::scl::any_cast<immovable>(&source);

    ::scl::any const moved = ::std::move(source);

    EXPECT_EQ(::scl::any_cast<immovable>(&moved), placed);
    EXPECT_EQ(::scl::any_cast<immovable>(&moved)->value, 42);
}

TEST(AnyTest, MoveAssigningTheAnyToItselfKeepsTheValue)
{
    ::scl::any value{42};
    ::scl::any & same = value;

    value = ::std::move(same);

    EXPECT_EQ(*::scl::any_cast<int>(&value), 42);
}

TEST(AnyTest, AssigningTheStoredObjectToItsOwnAnyKeepsTheValue)
{
    ::scl::any value{::std::string(64, 'x')};

    value = *::scl::any_cast<::std::string>(&value);

    EXPECT_EQ(*::scl::any_cast<::std::string>(&value), ::std::string(64, 'x'));
}

TEST(AnyTest, AssigningTheStoredObjectToItsOwnAnyLeavesItAlone)
{
    trace log;
    counted_any value{traced{log}};
    allocation_counter::reset();
    void const * const before = ::scl::any_cast<traced>(&value);

    value = *::scl::any_cast<traced>(&value);

    EXPECT_EQ(log.copies, 0);
    EXPECT_EQ(log.destructions, 0);
    EXPECT_EQ(allocation_counter::allocations, 0);
    EXPECT_EQ(::scl::any_cast<traced>(&value), before);
}

TEST(AnyTest, AssigningAnAllocatedObjectToItsOwnAnyLeavesItAlone)
{
    trace log;
    counted_any value{::std::in_place_type<traced_pair>, log};
    allocation_counter::reset();
    void const * const before = ::scl::any_cast<traced_pair>(&value);

    value = *::scl::any_cast<traced_pair>(&value);

    EXPECT_EQ(log.copies, 0);
    EXPECT_EQ(log.destructions, 0);
    EXPECT_EQ(allocation_counter::allocations, 0);
    EXPECT_EQ(allocation_counter::deallocations, 0);
    EXPECT_EQ(::scl::any_cast<traced_pair>(&value), before);
}

TEST(AnyTest, AnAggregateIsBuiltInPlaceFromItsMembers)
{
    ::scl::any value{::std::in_place_type<aggregate>, 1, 2.5};

    auto const * stored = ::scl::any_cast<aggregate>(&value);
    ASSERT_NE(stored, nullptr);
    EXPECT_EQ(stored->first, 1);
    EXPECT_EQ(stored->second, 2.5);

    auto & replaced = value.emplace<aggregate>(3, 4.5);

    EXPECT_EQ(replaced.first, 3);
    EXPECT_EQ(replaced.second, 4.5);
}

TEST(AnyTest, ACapacityBelowAPointerStillHoldsAPointerSizedObjectInPlace)
{
    using cramped_any = ::scl::basic_any<counting_allocator<::std::byte>, 1U>;
    allocation_counter::reset();

    // A pointer, not an int: rounding one byte up to four would hold an int in place too.
    int number = 42;
    cramped_any const value{static_cast<void *>(&number)};

    EXPECT_EQ(allocation_counter::allocations, 0);
    EXPECT_EQ(*::scl::any_cast<void *>(&value), &number);
}

TEST(AnyTest, ASmallAggregateIsBuiltInTheBuffer)
{
    allocation_counter::reset();

    counted_any const value{::std::in_place_type<small_aggregate>, 'x'};

    EXPECT_EQ(allocation_counter::allocations, 0);
    EXPECT_EQ(::scl::any_cast<small_aggregate>(&value)->letter, 'x');
}

TEST(AnyTest, AssigningAValueReplacesWhatWasHeld)
{
    ::scl::any value{42};

    value = ::std::string{"Hello Any!"};

    EXPECT_EQ(*::scl::any_cast<::std::string>(&value), "Hello Any!");
}

TEST(AnyTest, SwappingAnAnyWithItselfLeavesItAlone)
{
    ::scl::any value{::std::make_unique<int>(42)};
    int const * const held = ::scl::any_cast<::std::unique_ptr<int>>(&value)->get();

    value.swap(value);

    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(::scl::any_cast<::std::unique_ptr<int>>(&value)->get(), held);
}

TEST(AnyTest, SwapExchangesWhatTwoAnysHold)
{
    ::scl::any left{42};
    ::scl::any right{::std::string{"Hello Any!"}};

    left.swap(right);

    EXPECT_EQ(*::scl::any_cast<::std::string>(&left), "Hello Any!");
    EXPECT_EQ(*::scl::any_cast<int>(&right), 42);
}

TEST(AnyTest, FreeSwapExchangesWhatTwoAnysHold)
{
    ::scl::any left{42};
    ::scl::any right{::std::string{"Hello Any!"}};

    swap(left, right);

    EXPECT_EQ(*::scl::any_cast<::std::string>(&left), "Hello Any!");
    EXPECT_EQ(*::scl::any_cast<int>(&right), 42);
}

#if SCL_HAS_EXCEPTIONS
TEST(AnyTest, CastFromAMovedAnyMovesTheStoredObject)
{
    trace log;
    ::scl::any value{traced{log}};

    traced const taken = ::scl::any_cast<traced>(::std::move(value));

    EXPECT_EQ(log.copies, 0);
    EXPECT_EQ(taken.log, &log);
}
#endif

TEST(AnyTest, MakeAnyBuildsTheRequestedTypeFromItsArguments)
{
    ::scl::any const value = ::scl::make_any<::std::string>(3U, 'x');

    EXPECT_EQ(*::scl::any_cast<::std::string>(&value), "xxx");
}

TEST(AnyTest, CastToAnotherTypeAnswersNull)
{
    ::scl::any value{42};

    EXPECT_EQ(::scl::any_cast<double>(&value), nullptr);
    EXPECT_EQ(::scl::any_cast<int>(static_cast<::scl::any *>(nullptr)), nullptr);
}

TEST(AnyTest, CastThroughAConstAnyYieldsAPointerToConst)
{
    ::scl::any const value{42};

    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(::scl::any_cast<int>(&value)), int const *>));
    EXPECT_EQ(*::scl::any_cast<int>(&value), 42);
}

TEST(AnyTest, LifecycleOfAnAllocatedObjectRunsDuringConstantEvaluation)
{
    STATIC_EXPECT_EQ(constant_allocated_lifecycle(), ::std::string_view{"Hello Any!"}.size());
}

TEST(AnyTest, ReplacingAnObjectRunsDuringConstantEvaluation)
{
    STATIC_EXPECT_EQ(constant_replacement(), 3U);
}

#if SCL_HAS_EXCEPTIONS
TEST(AnyTest, ThrowingCastAnswersTheValueOrRefuses)
{
    ::scl::any const value{42};

    EXPECT_EQ(::scl::any_cast<int>(value), 42);
    EXPECT_THROW(static_cast<void>(::scl::any_cast<double>(value)), ::scl::bad_any_cast);
}

TEST(AnyTest, EmplaceThatThrowsLeavesTheAnyEmpty)
{
    ::scl::any value{42};

    EXPECT_THROW(value.emplace<throwing>(), construction_failure);

    EXPECT_FALSE(value.has_value());
}
#endif

TEST(AnyTest, AssigningTheStoredObjectItsOwnValueLeavesItAloneAtCompileTimeToo)
{
    STATIC_EXPECT_TRUE(self_assignment_keeps_the_object());
}

TEST(AnyTest, RoomIsAskedForOnlyForATypeAContainerCouldHold)
{
    // A block is laid out for a type, so a type no holder can be built for is refused where
    // the room is asked for rather than answered with room nothing can ever use.
    STATIC_EXPECT_TRUE(any_reserves_space_for<::std::string>);
    STATIC_EXPECT_TRUE(any_answers_space_for<::std::string>);

    STATIC_EXPECT_FALSE(any_reserves_space_for<printable>);
    STATIC_EXPECT_FALSE(any_answers_space_for<printable>);
    STATIC_EXPECT_FALSE(any_reserves_space_for<void>);
    STATIC_EXPECT_FALSE(any_answers_space_for<void>);
    STATIC_EXPECT_FALSE(any_reserves_space_for<int[3]>);
    STATIC_EXPECT_FALSE(any_answers_space_for<int[3]>);
}
