#include <gtest_utils.h>

#include <scl/utility/any/detail/holder.h>
#include <scl/utility/meta/type_key.h>
#include <scl/utility/preprocessor/exceptions.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace
{
    using default_allocator = ::std::allocator<::std::byte>;

    template <typename Type>
    inline constexpr auto const & described = ::scl::detail::any_type_descriptor_of<Type &>;

    struct immovable
    {
        int value = 0;

        immovable() = default;
        immovable(immovable const &) = delete;
        immovable(immovable &&) = delete;
        immovable & operator=(immovable const &) = delete;
        immovable & operator=(immovable &&) = delete;
        ~immovable() = default;
    };

    struct alignas(2 * alignof(void *)) over_aligned
    {
        char value = 0;
    };

    // Wider than one pointer, aligned like one: what a roomy buffer is meant to take.
    struct two_words
    {
        void * first = nullptr;
        void * second = nullptr;
    };

    struct allocation_counter
    {
        int allocations = 0;
        int deallocations = 0;
        ::std::vector<void *> live;
    };

    template <typename Type>
    struct counting_allocator
    {
        using value_type = Type;

        allocation_counter * counter = nullptr;

        counting_allocator() = default;

        explicit counting_allocator(allocation_counter & shared) noexcept
            : counter{&shared}
        {}

        template <typename Other>
        // cppcheck-suppress noExplicitConstructor
        counting_allocator(counting_allocator<Other> const & other) noexcept // NOLINT(*-explicit-*)
            : counter{other.counter}
        {}

        [[nodiscard]]
        Type * allocate(::std::size_t count)
        {
            ++counter->allocations;
            Type * const acquired = ::std::allocator<Type>{}.allocate(count);
            counter->live.push_back(acquired);
            return acquired;
        }

        // Storage this allocator never handed out is counted and left alone, so a test can
        // watch a wrong release without the undefined behaviour of performing it.
        void deallocate(Type * pointer, ::std::size_t count) noexcept
        {
            ++counter->deallocations;

            auto const released = ::std::ranges::find(counter->live, static_cast<void *>(pointer));
            if (released == counter->live.end())
                return;

            counter->live.erase(released);
            ::std::allocator<Type>{}.deallocate(pointer, count);
        }

        friend bool operator==(counting_allocator const &, counting_allocator const &) = default;
    };

    struct trace
    {
        int moves = 0;
        int destructions = 0;
    };

    struct traced
    {
        trace * log = nullptr;

        explicit traced(trace & shared) noexcept
            : log{&shared}
        {}

        traced(traced const &) = default;

        traced(traced && other) noexcept
            : log{other.log}
        {
            ++log->moves;
        }

        traced & operator=(traced const &) = default;
        traced & operator=(traced &&) = default;

        ~traced() { ++log->destructions; }
    };

    [[nodiscard]]
    constexpr ::std::size_t constant_round_trip()
    {
        default_allocator allocator;
        ::scl::detail::any_holder_base * const held =
            ::scl::detail::any_make_holder<::std::string>(allocator, "Hello Any!");

        ::scl::detail::any_holder_base * const copy =
            ::scl::detail::any_duplicate_holder<::std::string>(held, allocator);
        ::std::size_t const length = ::scl::detail::any_holder_object<::std::string>(copy)->size();

        ::scl::detail::any_destroy_holder<::std::string>(copy, allocator);
        ::scl::detail::any_destroy_holder<::std::string>(held, allocator);
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

TEST(AnyHolderTest, ErasedBaseReachesStoredValue)
{
    ::scl::detail::any_holder<int> held{42};

    ::scl::detail::any_holder_base * const erased = &held;

    EXPECT_EQ(*::scl::detail::any_holder_object<int>(erased), 42);
}

TEST(AnyHolderTest, DescriptorCarriesTypeIdentity)
{
    STATIC_EXPECT_TRUE(*described<::std::string>.type == ::scl::type_key_of<::std::string>());
    EXPECT_EQ(described<::std::string>.type->name(), ::scl::type_name<::std::string>());
}

TEST(AnyHolderTest, DescriptorReportsTheRoomTheObjectNeeds)
{
    STATIC_EXPECT_EQ(described<int>.size, sizeof(int));
    STATIC_EXPECT_EQ(described<int>.alignment, alignof(int));
    STATIC_EXPECT_EQ(described<over_aligned>.alignment, alignof(over_aligned));
}

TEST(AnyHolderTest, ATypeThatFitsTheBufferIsRecognised)
{
    STATIC_EXPECT_TRUE(::scl::detail::any_fits_in_buffer(described<int>, sizeof(void *)));
    STATIC_EXPECT_FALSE(::scl::detail::any_fits_in_buffer(described<::std::string>, sizeof(void *)));
}

TEST(AnyHolderTest, ATypeWithNoNothrowMoveIsKeptOutOfTheBuffer)
{
    STATIC_EXPECT_FALSE(::scl::detail::any_fits_in_buffer(described<immovable>, sizeof(void *)));
    STATIC_EXPECT_TRUE(described<immovable>.move == nullptr);
}

TEST(AnyHolderTest, AnOverAlignedTypeIsKeptOutOfTheBuffer)
{
    constexpr ::std::size_t roomy = 4 * sizeof(void *);

    STATIC_EXPECT_TRUE(::scl::detail::any_fits_in_buffer(described<two_words>, roomy));
    STATIC_EXPECT_FALSE(::scl::detail::any_fits_in_buffer(described<over_aligned>, roomy));
}

TEST(AnyHolderTest, ErasedCopyReproducesTheObjectAtAGivenAddress)
{
    ::std::string source{"Hello Any!"};
    alignas(::std::string)::std::byte storage[sizeof(::std::string)]{};

    ::scl::detail::any_holder_base * const copy = described<::std::string>.place(storage,
        ::std::addressof(source));

    EXPECT_EQ(*::scl::detail::any_holder_object<::std::string>(copy), "Hello Any!");
    described<::std::string>.erase(copy);
}

TEST(AnyHolderTest, ErasedCopyIsAbsentForAMoveOnlyType)
{
    STATIC_EXPECT_TRUE(described<::std::unique_ptr<int>>.place == nullptr);
}

TEST(AnyHolderTest, ErasedMoveRelocatesTheObjectAndEndsTheSource)
{
    trace log;
    alignas(traced)::std::byte source_storage[sizeof(traced)]{};
    alignas(traced)::std::byte target_storage[sizeof(traced)]{};
    traced source{log};
    ::scl::detail::any_holder_base * const held =
        ::scl::detail::any_make_holder_in_place<traced>(source_storage, source);

    ::scl::detail::any_holder_base * const target = described<traced>.move(target_storage, held);

    EXPECT_EQ(::scl::detail::any_holder_object<traced>(target)->log, &log);
    EXPECT_EQ(log.moves, 1);
    EXPECT_EQ(log.destructions, 1);
    described<traced>.erase(target);
}

TEST(AnyHolderTest, ErasedRecoveryFindsTheHolderInRawStorage)
{
    alignas(void *)::std::byte storage[sizeof(void *)]{};
    static_cast<void>(::scl::detail::any_make_holder_in_place<int>(storage, 42));

    ::scl::detail::any_holder_base * const held = described<int>.reach(storage);

    EXPECT_EQ(*::scl::detail::any_holder_object<int>(held), 42);
    described<int>.erase(held);
}

TEST(AnyHolderTest, ErasedObjectAddressSkipsTheHolderAroundIt)
{
    ::scl::detail::any_holder<::std::string> held{"Hello Any!"};

    void const * const object = described<::std::string>.object(&held);

    EXPECT_EQ(object, ::std::addressof(held.value));
}

TEST(AnyHolderTest, AcquiredStorageMatchesTheAlignmentAsked)
{
    allocation_counter counter;
    counting_allocator<::std::byte> allocator{counter};
    ::scl::detail::any_extent const room{.size = sizeof(over_aligned), .alignment = alignof(over_aligned)};

    void * const storage = ::scl::detail::any_acquire(allocator, room);

    EXPECT_EQ(reinterpret_cast<::std::uintptr_t>(storage) % alignof(over_aligned), 0U);
    EXPECT_EQ(counter.allocations, 1);
    ::scl::detail::any_release(allocator, storage, room);
    EXPECT_EQ(counter.deallocations, 1);
}

TEST(AnyHolderTest, AllocatedLifecycleRunsDuringConstantEvaluation)
{
    STATIC_EXPECT_EQ(constant_round_trip(), ::std::string_view{"Hello Any!"}.size());
}

#if SCL_HAS_EXCEPTIONS
TEST(AnyHolderTest, AllocationIsReleasedWhenConstructionThrows)
{
    allocation_counter counter;
    counting_allocator<::std::byte> allocator{counter};

    EXPECT_THROW(static_cast<void>(::scl::detail::any_make_holder<throwing>(allocator)), construction_failure);

    EXPECT_EQ(counter.allocations, 1);
    EXPECT_EQ(counter.deallocations, 1);
}
#endif
