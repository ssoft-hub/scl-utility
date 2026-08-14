#include <gtest_utils.h>

#include <scl/utility/any/any.h>
#include <scl/utility/any/any_view.h>
#include <scl/utility/preprocessor/exceptions.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <memory_resource>
#include <string>

namespace
{
    using pmr_allocator = ::std::pmr::polymorphic_allocator<::std::byte>;
    using pmr_any = ::scl::basic_any<pmr_allocator>;

    class counting_resource : public ::std::pmr::memory_resource
    {
    public:
        int allocations = 0;
        int deallocations = 0;

    private:
        void * do_allocate(::std::size_t bytes, ::std::size_t alignment) override
        {
            ++allocations;
            return ::std::pmr::new_delete_resource()->allocate(bytes, alignment);
        }

        void do_deallocate(void * pointer, ::std::size_t bytes, ::std::size_t alignment) override
        {
            ++deallocations;
            ::std::pmr::new_delete_resource()->deallocate(pointer, bytes, alignment);
        }

        [[nodiscard]]
        bool do_is_equal(memory_resource const & other) const noexcept override
        {
            return this == &other;
        }
    };

    // Copyable, yet neither copy_constructible nor movable: the predicate behind a copy has
    // to be the one a copy actually needs.
    struct explicitly_copyable
    {
        int value = 0;

        constexpr explicitly_copyable() = default;

        constexpr explicit explicitly_copyable(int initial) noexcept
            : value{initial}
        {}

        constexpr explicit explicitly_copyable(explicitly_copyable const & other) noexcept
            : value{other.value}
        {}

        explicitly_copyable(explicitly_copyable &&) = delete;
        explicitly_copyable & operator=(explicitly_copyable const &) = delete;
        explicitly_copyable & operator=(explicitly_copyable &&) = delete;
        constexpr ~explicitly_copyable() = default;
    };

    [[nodiscard]]
    constexpr int constant_explicit_copy()
    {
        ::scl::any const source{::std::in_place_type<explicitly_copyable>, 42};
        ::scl::any const copy = source.try_copy();

        return ::scl::any_cast<explicitly_copyable>(&copy)->value;
    }

    [[nodiscard]]
    constexpr ::std::size_t constant_copy()
    {
        ::scl::any const source{::std::string{"Hello Any!"}};
        ::scl::any const copy = source.try_copy();

        return ::scl::any_cast<::std::string>(&copy)->size();
    }

    struct three_doubles
    {
        double first = 0.0;
        double second = 0.0;
        double third = 0.0;
    };

    struct same_shape_as_three_doubles
    {
        double first = 0.0;
        double second = 0.0;
        double marker = 0.0;
    };

    static_assert(sizeof(same_shape_as_three_doubles) == sizeof(three_doubles) &&
            alignof(same_shape_as_three_doubles) == alignof(three_doubles),
        "the block-reusing tests prove nothing unless the two types share one shape");

    struct wider_than_three_doubles
    {
        double values[5] = {};
    };

    // Wider than the copy assignment takes aside to reuse a block.
    struct too_wide_to_take_aside
    {
        double values[64] = {};
    };

    // A pointer's shape, yet allocated: the throwing move keeps it out of the buffer.
    struct throwing_move
    {
        ::std::uintptr_t tag = 0U;

        throwing_move() = default;

        explicit throwing_move(::std::uintptr_t value) noexcept
            : tag{value}
        {}

        throwing_move(throwing_move const &) = default;

        // NOLINTNEXTLINE(performance-noexcept-move-constructor): the throwing move is the point
        throwing_move(throwing_move && other)
            : tag{other.tag}
        {}

        throwing_move & operator=(throwing_move const &) = default;
        throwing_move & operator=(throwing_move &&) = default;
        ~throwing_move() = default;
    };

    struct alignas(4 * alignof(void *)) over_aligned_pair
    {
        double first = 0.0;
        double second = 0.0;
    };

    // Marks itself dead, so a copy taken from a destroyed one is caught without reading
    // whatever the allocator wrote over it.
    struct payload
    {
        inline static bool copied_from_dead = false;

        bool alive = true;
        double first = 0.0;
        double second = 0.0;

        payload() = default;

        payload(payload const & other) noexcept
            : alive{other.alive}
            , first{other.first}
            , second{other.second}
        {
            if (!other.alive)
                copied_from_dead = true;
        }

        payload & operator=(payload const &) = default;
        ~payload() { alive = false; }
    };

    // The shape of payload, and the owner of one: destroying it frees what a source may
    // still be pointing at.
    struct owns_a_payload
    {
        ::std::unique_ptr<payload> held;
        double first = 0.0;
        double second = 0.0;
    };

#if SCL_HAS_EXCEPTIONS
    struct block_construction_failure
    {};

    // The shape of three_doubles, so a failure lands on the block-reusing path.
    struct throwing_three_doubles
    {
        double first = 0.0;
        double second = 0.0;
        double third = 0.0;

        throwing_three_doubles() { throw block_construction_failure{}; }
    };

    // Fails where assignment builds, which a throwing default constructor never reaches.
    struct throwing_copy_three_doubles
    {
        double first = 0.0;
        double second = 0.0;
        double third = 0.0;

        throwing_copy_three_doubles() = default;

        throwing_copy_three_doubles(throwing_copy_three_doubles const &) /**/
        {
            throw block_construction_failure{};
        }

        throwing_copy_three_doubles & operator=(throwing_copy_three_doubles const &) = default;
        ~throwing_copy_three_doubles() = default;
    };

    // Nothrow-movable, so assignment reuses the block; the copy it takes aside is what throws.
    struct throwing_copy_movable_three_doubles
    {
        double first = 0.0;
        double second = 0.0;
        double third = 0.0;

        throwing_copy_movable_three_doubles() = default;

        throwing_copy_movable_three_doubles(throwing_copy_movable_three_doubles const &) /**/
        {
            throw block_construction_failure{};
        }

        throwing_copy_movable_three_doubles(throwing_copy_movable_three_doubles &&) noexcept = default;
        throwing_copy_movable_three_doubles &
        operator=(throwing_copy_movable_three_doubles const &) = default;
        throwing_copy_movable_three_doubles &
        operator=(throwing_copy_movable_three_doubles &&) noexcept = default;
        ~throwing_copy_movable_three_doubles() = default;
    };
#endif
} // namespace

TEST(AnyAllocatorTest, AnyKeepsTheAllocatorItWasGiven)
{
    ::std::pmr::monotonic_buffer_resource resource;

    pmr_any const value{::std::allocator_arg, pmr_allocator{&resource}};

    EXPECT_EQ(value.get_allocator().resource(), &resource);
}

TEST(AnyAllocatorTest, AnAllocatedObjectComesFromTheGivenResource)
{
    counting_resource resource;
    pmr_any value{::std::allocator_arg, pmr_allocator{&resource}};

    value.emplace<::std::string>(64U, 'x');

    EXPECT_EQ(resource.allocations, 1);
    value.reset();
    EXPECT_EQ(resource.deallocations, 1);
}

TEST(AnyAllocatorTest, AStatefulAllocatorCostsOnlyItsOwnSize)
{
    STATIC_EXPECT_EQ(sizeof(pmr_any), 3 * sizeof(void *));
}

TEST(AnyAllocatorTest, ReplacingAnObjectWithOneOfTheSameTypeKeepsTheBlock)
{
    counting_resource resource;
    pmr_any value{::std::allocator_arg, pmr_allocator{&resource}};
    value.emplace<three_doubles>(1.0, 2.0, 3.0);

    value.emplace<three_doubles>(4.0, 5.0, 6.0);

    EXPECT_EQ(resource.allocations, 1);
    EXPECT_EQ(resource.deallocations, 0);
    EXPECT_EQ(::scl::any_cast<three_doubles>(&value)->third, 6.0);
}

TEST(AnyAllocatorTest, ReplacingAnObjectWithAnotherOfTheSameShapeKeepsTheBlock)
{
    counting_resource resource;
    pmr_any value{::std::allocator_arg, pmr_allocator{&resource}};
    value.emplace<three_doubles>(1.0, 2.0, 3.0);

    value.emplace<same_shape_as_three_doubles>(4.0, 5.0, 6.0);

    EXPECT_EQ(resource.allocations, 1);
    EXPECT_EQ(resource.deallocations, 0);
    EXPECT_EQ(::scl::any_cast<same_shape_as_three_doubles>(&value)->marker, 6.0);
}

TEST(AnyAllocatorTest, ReplacingAnObjectWithAWiderOneTakesANewBlock)
{
    counting_resource resource;
    pmr_any value{::std::allocator_arg, pmr_allocator{&resource}};
    value.emplace<three_doubles>(1.0, 2.0, 3.0);

    value.emplace<wider_than_three_doubles>();

    EXPECT_EQ(resource.allocations, 2);
    EXPECT_EQ(resource.deallocations, 1);
}

TEST(AnyAllocatorTest, RepeatedAssignmentOfOneShapeStopsConsumingTheResource)
{
    counting_resource resource;
    pmr_any value{::std::allocator_arg, pmr_allocator{&resource}};
    value.emplace<three_doubles>(0.0, 0.0, 0.0);

    for (int round = 0; round < 16; ++round)
        value.emplace<three_doubles>(static_cast<double>(round), 0.0, 0.0);

    EXPECT_EQ(resource.allocations, 1);
    EXPECT_EQ(::scl::any_cast<three_doubles>(&value)->first, 15.0);
}

TEST(AnyAllocatorTest, ReplacingAnAllocatedObjectWithOneThatBelongsInTheBufferReleasesTheBlock)
{
    counting_resource resource;
    pmr_any value{::std::allocator_arg, pmr_allocator{&resource}};
    value.emplace<throwing_move>(::std::uintptr_t{7U});
    ASSERT_EQ(resource.allocations, 1);

    value.emplace<::std::uintptr_t>(0xABCDEF01U);

    EXPECT_EQ(resource.deallocations, 1);
    ASSERT_NE(::scl::any_cast<::std::uintptr_t>(&value), nullptr);
    EXPECT_EQ(*::scl::any_cast<::std::uintptr_t>(&value), 0xABCDEF01U);
}

TEST(AnyAllocatorTest, AssigningAnAllocatedObjectAValueThatBelongsInTheBufferReleasesTheBlock)
{
    counting_resource resource;
    pmr_any value{::std::allocator_arg, pmr_allocator{&resource}};
    value.emplace<throwing_move>(::std::uintptr_t{7U});

    value = ::std::uintptr_t{0xABCDEF01U};

    EXPECT_EQ(resource.deallocations, 1);
    ASSERT_NE(::scl::any_cast<::std::uintptr_t>(&value), nullptr);
    EXPECT_EQ(*::scl::any_cast<::std::uintptr_t>(&value), 0xABCDEF01U);
}

TEST(AnyAllocatorTest, ReplacingAnOverAlignedObjectKeepsItsBlockAndItsAlignment)
{
    counting_resource resource;
    pmr_any value{::std::allocator_arg, pmr_allocator{&resource}};
    value.emplace<over_aligned_pair>(1.0, 2.0);

    value.emplace<over_aligned_pair>(3.0, 4.0);

    EXPECT_EQ(resource.allocations, 1);
    auto const * const stored = ::scl::any_cast<over_aligned_pair>(&value);
    ASSERT_NE(stored, nullptr);
    EXPECT_EQ(reinterpret_cast<::std::uintptr_t>(stored) % alignof(over_aligned_pair), 0U);
    EXPECT_EQ(stored->second, 4.0);
}

#if SCL_HAS_EXCEPTIONS
TEST(AnyAllocatorTest, AFailedEmplaceReleasesTheBlockItWasReusing)
{
    counting_resource resource;
    pmr_any value{::std::allocator_arg, pmr_allocator{&resource}};
    value.emplace<three_doubles>(1.0, 2.0, 3.0);
    throwing_copy_three_doubles const source;

    EXPECT_THROW(value.emplace<throwing_copy_three_doubles>(source), block_construction_failure);

    EXPECT_FALSE(value.has_value());
    EXPECT_EQ(resource.deallocations, 1);
}
#endif

TEST(AnyAllocatorTest, AssigningAValueOfTheSameShapeKeepsTheBlock)
{
    counting_resource resource;
    pmr_any value{::std::allocator_arg, pmr_allocator{&resource}};
    value.emplace<three_doubles>(1.0, 2.0, 3.0);

    value = same_shape_as_three_doubles{4.0, 5.0, 6.0};

    EXPECT_EQ(resource.allocations, 1);
    EXPECT_EQ(resource.deallocations, 0);
    EXPECT_EQ(::scl::any_cast<same_shape_as_three_doubles>(&value)->marker, 6.0);
}

TEST(AnyAllocatorTest, RepeatedValueAssignmentOfOneShapeStopsConsumingTheResource)
{
    counting_resource resource;
    pmr_any value{::std::allocator_arg, pmr_allocator{&resource}};
    value = three_doubles{0.0, 0.0, 0.0};

    for (int round = 0; round < 16; ++round)
        value = three_doubles{static_cast<double>(round), 0.0, 0.0};

    EXPECT_EQ(resource.allocations, 1);
    EXPECT_EQ(::scl::any_cast<three_doubles>(&value)->first, 15.0);
}

TEST(AnyAllocatorTest, AssigningAValueTooWideToTakeAsideTakesANewBlock)
{
    counting_resource resource;
    pmr_any value{::std::allocator_arg, pmr_allocator{&resource}};
    value.emplace<too_wide_to_take_aside>();

    value = too_wide_to_take_aside{};

    EXPECT_EQ(resource.allocations, 2);
    EXPECT_EQ(resource.deallocations, 1);
}

TEST(AnyAllocatorTest, TakingAValueFromAViewReplacesTheObject)
{
    counting_resource resource;
    pmr_any value{::std::allocator_arg, pmr_allocator{&resource}};
    value.emplace<three_doubles>(1.0, 2.0, 3.0);
    three_doubles const source{4.0, 5.0, 6.0};

    value = ::scl::any_view{source};

    EXPECT_EQ(::scl::any_cast<three_doubles>(&value)->third, 6.0);
}

TEST(AnyAllocatorTest, TakingAValueOfTheSameShapeFromAViewKeepsTheBlock)
{
    counting_resource resource;
    pmr_any value{::std::allocator_arg, pmr_allocator{&resource}};
    value.emplace<three_doubles>(1.0, 2.0, 3.0);
    same_shape_as_three_doubles const source{4.0, 5.0, 6.0};

    value = ::scl::any_view{source};

    EXPECT_EQ(resource.allocations, 1);
    EXPECT_EQ(resource.deallocations, 0);
    EXPECT_EQ(::scl::any_cast<same_shape_as_three_doubles>(&value)->marker, 6.0);
}

TEST(AnyAllocatorTest, TakingAViewOfSomethingTheStoredObjectOwnsReadsALiveObject)
{
    payload::copied_from_dead = false;
    ::scl::any value{
        owns_a_payload{::std::make_unique<payload>(), 0.0, 0.0}
    };
    ::scl::any_cast<owns_a_payload>(&value)->held->first = 42.0;

    value = ::scl::any_view{*::scl::any_cast<owns_a_payload>(&value)->held};

    EXPECT_FALSE(payload::copied_from_dead);
    ASSERT_NE(::scl::any_cast<payload>(&value), nullptr);
    EXPECT_EQ(::scl::any_cast<payload>(&value)->first, 42.0);
}

TEST(AnyAllocatorTest, TakingAValueOfAnotherShapeFromAViewTakesANewBlock)
{
    counting_resource resource;
    pmr_any value{::std::allocator_arg, pmr_allocator{&resource}};
    value.emplace<three_doubles>(1.0, 2.0, 3.0);
    wider_than_three_doubles const source;

    value = ::scl::any_view{source};

    EXPECT_EQ(resource.allocations, 2);
    EXPECT_EQ(resource.deallocations, 1);
    EXPECT_NE(::scl::any_cast<wider_than_three_doubles>(&value), nullptr);
}

TEST(AnyAllocatorTest, AssigningAValueTheStoredObjectOwnsIndirectlyReadsALiveObject)
{
    payload::copied_from_dead = false;
    ::scl::any value{
        owns_a_payload{::std::make_unique<payload>(), 0.0, 0.0}
    };
    ::scl::any_cast<owns_a_payload>(&value)->held->first = 42.0;

    value = *::scl::any_cast<owns_a_payload>(&value)->held;

    EXPECT_FALSE(payload::copied_from_dead);
    ASSERT_NE(::scl::any_cast<payload>(&value), nullptr);
    EXPECT_EQ(::scl::any_cast<payload>(&value)->first, 42.0);
}

#if SCL_HAS_EXCEPTIONS
TEST(AnyAllocatorTest, AFailedTakeFromAViewKeepsTheObjectAndItsBlock)
{
    counting_resource resource;
    pmr_any value{::std::allocator_arg, pmr_allocator{&resource}};
    value.emplace<three_doubles>(1.0, 2.0, 3.0);
    throwing_copy_movable_three_doubles const source;

    EXPECT_THROW(value = ::scl::any_view{source}, block_construction_failure);

    ASSERT_NE(::scl::any_cast<three_doubles>(&value), nullptr);
    EXPECT_EQ(::scl::any_cast<three_doubles>(&value)->third, 3.0);
    EXPECT_EQ(resource.allocations, 1);
}

TEST(AnyAllocatorTest, AFailedAssignmentOfAReusableShapeKeepsTheObjectAndItsBlock)
{
    counting_resource resource;
    pmr_any value{::std::allocator_arg, pmr_allocator{&resource}};
    value.emplace<three_doubles>(1.0, 2.0, 3.0);
    throwing_copy_movable_three_doubles const source;

    EXPECT_THROW(value = source, block_construction_failure);

    ASSERT_NE(::scl::any_cast<three_doubles>(&value), nullptr);
    EXPECT_EQ(::scl::any_cast<three_doubles>(&value)->third, 3.0);
    EXPECT_EQ(resource.allocations, 1);
}

TEST(AnyAllocatorTest, AFailedAssignmentKeepsTheObjectItWouldHaveReplaced)
{
    ::scl::any value{
        three_doubles{1.0, 2.0, 3.0}
    };
    throwing_copy_three_doubles const source;

    EXPECT_THROW(value = source, block_construction_failure);

    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(::scl::any_cast<three_doubles>(&value)->third, 3.0);
}
#endif

TEST(AnyAllocatorTest, AssigningAPartOfTheStoredObjectKeepsTheValue)
{
    ::scl::any value{::std::array<::std::string, 1>{::std::string(64, 'x')}};

    value = ::scl::any_cast<::std::array<::std::string, 1>>(&value)->front();

    EXPECT_EQ(*::scl::any_cast<::std::string>(&value), ::std::string(64, 'x'));
}

#if SCL_HAS_EXCEPTIONS
TEST(AnyAllocatorTest, AFailedConstructionReleasesTheBlockItWasReusing)
{
    counting_resource resource;
    pmr_any value{::std::allocator_arg, pmr_allocator{&resource}};
    value.emplace<three_doubles>(1.0, 2.0, 3.0);

    EXPECT_THROW(value.emplace<throwing_three_doubles>(), block_construction_failure);

    EXPECT_FALSE(value.has_value());
    EXPECT_EQ(resource.deallocations, 1);
}
#endif

TEST(AnyAllocatorTest, TryCopyOfAnExplicitlyCopyableObjectRunsDuringConstantEvaluation)
{
    STATIC_EXPECT_EQ(constant_explicit_copy(), 42);
}

TEST(AnyAllocatorTest, TryCopyReproducesACopyableObject)
{
    ::scl::any const source{::std::string{"Hello Any!"}};

    ::scl::any const copy = source.try_copy();

    EXPECT_TRUE(source.copyable());
    EXPECT_EQ(*::scl::any_cast<::std::string>(&copy), "Hello Any!");
}

TEST(AnyAllocatorTest, TryCopyOfAMoveOnlyObjectAnswersAnEmptyAny)
{
    ::scl::any const source{::std::make_unique<int>(42)};

    ::scl::any const copy = source.try_copy();

    EXPECT_FALSE(source.copyable());
    EXPECT_FALSE(copy.has_value());
}

TEST(AnyAllocatorTest, AnEmptyAnyIsNotCopyableAndCopiesToAnEmptyAny)
{
    ::scl::any const source;

    ::scl::any const copy = source.try_copy();

    EXPECT_FALSE(source.copyable());
    EXPECT_FALSE(copy.has_value());
}

TEST(AnyAllocatorTest, TheAllocatorTravelsWithTheObject)
{
    counting_resource first;
    counting_resource second;
    pmr_any source{::std::allocator_arg, pmr_allocator{&first}};
    source.emplace<::std::string>(64U, 'x');
    pmr_any target{::std::allocator_arg, pmr_allocator{&second}};

    target = ::std::move(source);

    EXPECT_EQ(target.get_allocator().resource(), &first);
    target.reset();
    EXPECT_EQ(first.deallocations, 1);
    EXPECT_EQ(second.allocations, 0);
}

TEST(AnyAllocatorTest, TryCopyAsksTheAllocatorWhichOneTheCopyGets)
{
    counting_resource resource;
    pmr_any source{::std::allocator_arg, pmr_allocator{&resource}};
    source.emplace<::std::string>(64U, 'x');

    pmr_any const copy = source.try_copy();

    EXPECT_EQ(copy.get_allocator().resource(), ::std::pmr::get_default_resource());
    EXPECT_EQ(resource.allocations, 1);
}

TEST(AnyAllocatorTest, TryCopyRunsDuringConstantEvaluation)
{
    STATIC_EXPECT_EQ(constant_copy(), ::std::string_view{"Hello Any!"}.size());
}

TEST(AnyAllocatorTest, TryCopyOfAnInPlaceObjectStaysInTheBuffer)
{
    counting_resource resource;
    pmr_any const source{::std::allocator_arg, pmr_allocator{&resource}};
    pmr_any const stored = [&resource] {
        pmr_any built{::std::allocator_arg, pmr_allocator{&resource}};
        built.emplace<int>(42);
        return built;
    }();

    pmr_any const copy = stored.try_copy();

    EXPECT_EQ(resource.allocations, 0);
    EXPECT_EQ(*::scl::any_cast<int>(&copy), 42);
}
