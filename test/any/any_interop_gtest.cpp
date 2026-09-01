#include <gtest_utils.h>

#include <scl/utility/any.h>
#include <scl/utility/preprocessor/rtti.h>

#if SCL_HAS_RTTI
#include <any>
#endif
#include <array>
#include <memory>
#include <memory_resource>
#include <string>

namespace
{
    // Aligned far more strictly than the buffer, so only the allocator path can hold it.
    struct alignas(128) hyper_aligned
    {
        char letter = 0;
    };

    struct trace
    {
        int copies = 0;
        int destructions = 0;
    };

    struct traced_small
    {
        trace * log = nullptr;

        explicit traced_small(trace & shared) noexcept
            : log{&shared}
        {}

        traced_small(traced_small const & other) noexcept
            : log{other.log}
        {
            ++log->copies;
        }

        traced_small(traced_small && other) noexcept
            : log{other.log}
        {
            other.log = nullptr;
        }

        traced_small & operator=(traced_small const &) = default;
        traced_small & operator=(traced_small &&) = default;

        ~traced_small()
        {
            if (log != nullptr)
                ++log->destructions;
        }
    };

    // A factory type: nothing outside may destroy it, and a view destroys nothing.
    class privately_destructible
    {
    public:
        int value = 0;

        static privately_destructible & instance()
        {
            static privately_destructible one;
            return one;
        }

    private:
        privately_destructible() = default;
        ~privately_destructible() = default;
    };

    template <typename Type>
    concept view_from_rvalue = requires { ::scl::any_view{Type{}}; };

    template <typename Type>
    concept any_from = requires(Type & handle) { ::scl::any::try_copy(handle); };

    template <typename Type>
    concept any_converts_from = requires(Type & handle) { ::scl::any{handle}; };

    template <typename Type>
    concept any_assignable_from = requires(::scl::any & value,
        Type & handle) { value = ::scl::any::try_copy(handle); };

    template <typename Type>
    concept any_assignable_from_a_bare_handle = requires(::scl::any & value,
        Type & handle) { value = handle; };

    using roomy_any = ::scl::basic_any<::std::allocator<::std::byte>, 32U>;
    using pmr_any = ::scl::basic_any<::std::pmr::polymorphic_allocator<::std::byte>>;

    template <typename Target, typename Source>
    concept built_from = requires(Source & source) { Target{::std::move(source)}; };

    template <typename Target, typename Source>
    concept assigned_from = requires(Target & target,
        Source & source) { target = ::std::move(source); };

    template <typename Target, typename Source>
    concept swapped_with = requires(Target & target, Source & source) { target.swap(source); };

    constexpr int read_through_a_view_over_an_any()
    {
        ::scl::any const value{42};
        ::scl::any_view const view{value};
        auto const * reached = ::scl::any_cast<int>(&view);

        return (reached != nullptr) ? *reached : -1;
    }

    constexpr int read_through_an_argument_over_an_any()
    {
        ::scl::any const value{21};
        ::scl::any_argument const subject{value};
        auto const * reached = ::scl::any_cast<int const>(&subject);

        return (reached != nullptr) ? *reached * 2 : -1;
    }

    constexpr int take_a_referent_from_a_view_over_an_any()
    {
        ::scl::any const source{42};
        ::scl::any_view const view{source};
        ::scl::any const copy = ::scl::any::try_copy(view);
        auto const * reached = ::scl::any_cast<int>(&copy);

        return (reached != nullptr) ? *reached : -1;
    }

    constexpr int take_a_referent_from_an_argument_over_an_any()
    {
        ::scl::any source{21};
        ::scl::any_argument const subject{source};
        ::scl::any const copy = ::scl::any::try_copy(subject);
        auto const * reached = ::scl::any_cast<int>(&copy);

        return (reached != nullptr) ? *reached * 2 : -1;
    }

    constexpr int assign_a_referent_from_a_view_over_an_any()
    {
        ::scl::any const source{42};
        ::scl::any_view const view{source};
        ::scl::any taker{7};
        taker = ::scl::any::try_copy(view);
        auto const * reached = ::scl::any_cast<int>(&taker);

        return (reached != nullptr) ? *reached : -1;
    }

    // A handle bound to nothing carries no holder, so the assignment must not reach for one.
    constexpr bool assign_a_handle_bound_to_nothing_empties_the_any()
    {
        ::scl::any value{42};
        ::scl::any_view const nothing{};

        value = ::scl::any::try_copy(nothing);
        return !value.has_value();
    }

    constexpr int assign_a_view_of_the_any_itself()
    {
        ::scl::any value{42};
        ::scl::any_view const view{value};
        value = ::scl::any::try_copy(view);
        auto const * reached = ::scl::any_cast<int>(&value);

        return (reached != nullptr) ? *reached : -1;
    }

    // Literal, unlike std::unique_ptr before C++23: the refusal has to be observable in a
    // constant expression too.
    struct movable_only
    {
        int value = 0;

        constexpr explicit movable_only(int number) noexcept
            : value{number}
        {}

        movable_only(movable_only const &) = delete;
        constexpr movable_only(movable_only &&) noexcept = default;
        movable_only & operator=(movable_only const &) = delete;
        constexpr movable_only & operator=(movable_only &&) noexcept = default;
        constexpr ~movable_only() = default;
    };

    constexpr bool taking_an_uncopyable_referent_answers_an_empty_any()
    {
        ::scl::any source{movable_only{42}};
        ::scl::any_view const view{source};
        ::scl::any const copy = ::scl::any::try_copy(view);

        return !copy.has_value();
    }

    constexpr bool view_over_an_any_names_the_stored_type()
    {
        ::scl::any const value{42};
        ::scl::any_view const view{value};

        return view.has_value() && view.type_key() == ::scl::type_key_of<int>() &&
            view.type_name() == ::scl::type_name<int>();
    }
    // An owner has no way to end such an object, so taking one through a handle leaves it
    // empty rather than storing what it cannot destroy.
    struct destructor_may_throw
    {
        destructor_may_throw() = default;
        destructor_may_throw(destructor_may_throw const &) = default;
        destructor_may_throw(destructor_may_throw &&) = default;
        destructor_may_throw & operator=(destructor_may_throw const &) = default;
        destructor_may_throw & operator=(destructor_may_throw &&) = default;
        ~destructor_may_throw() noexcept(false) {}
    };

} // namespace

TEST(AnyInteropTest, ViewOverAnAnyReadsItsContent)
{
    ::scl::any value{::std::string{"Hello Any!"}};

    ::scl::any_view const view = value;

    EXPECT_EQ(view.type_name(), ::scl::type_name<::std::string>());
    EXPECT_EQ(*::scl::any_cast<::std::string>(&view), "Hello Any!");
}

TEST(AnyInteropTest, ViewOverAnAnyTemporaryIsRefused)
{
    STATIC_EXPECT_FALSE(view_from_rvalue<::scl::any>);
}

TEST(AnyInteropTest, ViewOverAnAnyNamesTheStoredTypeDuringConstantEvaluation)
{
    STATIC_EXPECT_TRUE(view_over_an_any_names_the_stored_type());
}

TEST(AnyInteropTest, ViewOverAnAnyReadsItsContentDuringConstantEvaluation)
{
    STATIC_EXPECT_TRUE(read_through_a_view_over_an_any() == 42);
}

TEST(AnyInteropTest, ArgumentOverAnAnyReadsItsContentDuringConstantEvaluation)
{
    STATIC_EXPECT_TRUE(read_through_an_argument_over_an_any() == 42);
}

TEST(AnyInteropTest, AnAnyTakesAReferentFromAViewDuringConstantEvaluation)
{
    STATIC_EXPECT_TRUE(take_a_referent_from_a_view_over_an_any() == 42);
}

TEST(AnyInteropTest, AnAnyTakesAReferentFromAnArgumentDuringConstantEvaluation)
{
    STATIC_EXPECT_TRUE(take_a_referent_from_an_argument_over_an_any() == 42);
}

TEST(AnyInteropTest, AnAnyIsAssignedFromAViewDuringConstantEvaluation)
{
    STATIC_EXPECT_TRUE(assign_a_referent_from_a_view_over_an_any() == 42);
    STATIC_EXPECT_TRUE(assign_a_handle_bound_to_nothing_empties_the_any());
}

TEST(AnyInteropTest, AssigningAViewOfTheAnyItselfKeepsTheValueDuringConstantEvaluation)
{
    STATIC_EXPECT_TRUE(assign_a_view_of_the_any_itself() == 42);
}

TEST(AnyInteropTest, TakingAnUncopyableReferentAnswersAnEmptyAnyDuringConstantEvaluation)
{
    STATIC_EXPECT_TRUE(taking_an_uncopyable_referent_answers_an_empty_any());
}

TEST(AnyInteropTest, AnAnyBuiltFromAnArgumentTakesTheValue)
{
    ::scl::any const value = ::scl::any::try_copy(::scl::any_arg{"Hello Any!"});

    // The referent is an array, and an array cannot be copied, so it decays on the way in
    // exactly as it would reaching an any directly.
    EXPECT_TRUE(value.type_key() == ::scl::type_key_of<char const *>());
    EXPECT_STREQ(*::scl::any_cast<char const *>(&value), "Hello Any!");
}

TEST(AnyInteropTest, AnArrayReferentLeavesTheAnyEmpty)
{
    int numbers[3] = {1, 2, 3};
    int const frozen[3] = {1, 2, 3};

    // As a value an array is refused where it is written; through a handle its type is not named
    // at the call, so the copy answers an empty any instead.
    ::scl::any const from_plain = ::scl::any::try_copy(::scl::any_view{numbers});
    ::scl::any const from_frozen = ::scl::any::try_copy(::scl::any_view{frozen});

    EXPECT_FALSE(from_plain.has_value());
    EXPECT_FALSE(from_frozen.has_value());

    // The type of a string literal is the one array kept, and kept as the pointer it decays to.
    ::scl::any const from_literal = ::scl::any::try_copy(::scl::any_arg{"abc"});

    ASSERT_TRUE(from_literal.has_value());
    EXPECT_TRUE(from_literal.type_key() == ::scl::type_key_of<char const *>());
}

TEST(AnyInteropTest, AnAnyBuiltFromAViewCopiesTheReferent)
{
    ::std::string text{"Hello Any!"};
    ::scl::any_view const view{text};

    ::scl::any const value = ::scl::any::try_copy(view);

    EXPECT_EQ(value.type_name(), ::scl::type_name<::std::string>());
    EXPECT_NE(::scl::any_cast<::std::string>(&value), ::std::addressof(text));

    text = "changed";
    EXPECT_EQ(*::scl::any_cast<::std::string>(&value), "Hello Any!");
}

TEST(AnyInteropTest, AnAnyBuiltFromAWritingViewCopiesTheReferent)
{
    ::std::string text{"Hello Any!"};
    ::scl::any_mutable_view const view{text};

    ::scl::any const value = ::scl::any::try_copy(view);

    EXPECT_EQ(value.type_name(), ::scl::type_name<::std::string>());
    EXPECT_NE(::scl::any_cast<::std::string>(&value), ::std::addressof(text));

    text = "changed";
    EXPECT_EQ(*::scl::any_cast<::std::string>(&value), "Hello Any!");
}

TEST(AnyInteropTest, AssigningAWritingViewTakesTheValueToo)
{
    ::std::string text{"Hello Any!"};
    ::scl::any value{42};

    value = ::scl::any::try_copy(::scl::any_mutable_view{text});

    EXPECT_NE(::scl::any_cast<::std::string>(&value), ::std::addressof(text));
    text = "changed";
    EXPECT_EQ(*::scl::any_cast<::std::string>(&value), "Hello Any!");
}

TEST(AnyInteropTest, ASmallReferentIsTakenAsACopyIntoTheBuffer)
{
    int number = 42;
    ::scl::any_view const view{number};

    ::scl::any const value = ::scl::any::try_copy(view);

    // A copy, not a handle: the any answers int, its object sits elsewhere, and it does not
    // follow the source when the source changes.
    EXPECT_TRUE(value.type_key() == ::scl::type_key_of<int>());
    EXPECT_NE(::scl::any_cast<int>(&value), ::std::addressof(number));

    number = 7;
    EXPECT_EQ(*::scl::any_cast<int>(&value), 42);
}

TEST(AnyInteropTest, AssigningAViewTakesTheValueToo)
{
    ::std::string text{"Hello Any!"};
    ::scl::any value{42};

    value = ::scl::any::try_copy(::scl::any_view{text});

    EXPECT_NE(::scl::any_cast<::std::string>(&value), ::std::addressof(text));
    text = "changed";
    EXPECT_EQ(*::scl::any_cast<::std::string>(&value), "Hello Any!");
}

TEST(AnyInteropTest, TakingAnUncopyableReferentAnswersAnEmptyAny)
{
    ::std::unique_ptr<int> owned = ::std::make_unique<int>(42);
    ::scl::any_view const view{owned};

    ::scl::any const value = ::scl::any::try_copy(view);

    EXPECT_FALSE(value.has_value());
}

TEST(AnyInteropTest, TakingAReferentAlignedStricterThanTheBufferAllocatesForIt)
{
    hyper_aligned const object{.letter = 'x'};
    ::scl::any_view const view{object};

    ::scl::any const value = ::scl::any::try_copy(view);

    ASSERT_TRUE(value.has_value());
    auto const * const stored = ::scl::any_cast<hyper_aligned>(&value);
    ASSERT_NE(stored, nullptr);
    EXPECT_EQ(stored->letter, 'x');
    EXPECT_EQ(reinterpret_cast<::std::uintptr_t>(stored) % alignof(hyper_aligned), 0U);
}

TEST(AnyInteropTest, AssigningAViewOfTheAnyItselfKeepsTheValue)
{
    ::scl::any value{::std::string(64, 'x')};

    value = ::scl::any::try_copy(::scl::any_view{value});

    EXPECT_EQ(*::scl::any_cast<::std::string>(&value), ::std::string(64, 'x'));
}

TEST(AnyInteropTest, AssigningAnArgumentOfTheAnyItselfKeepsTheValue)
{
    ::scl::any value{42};

    value = ::scl::any::try_copy(::scl::any_arg{value});

    EXPECT_EQ(*::scl::any_cast<int>(&value), 42);
}

TEST(AnyInteropTest, AssigningAViewOfTheAnyItselfLeavesTheObjectAlone)
{
    trace log;
    ::scl::any value{traced_small{log}};
    void const * const before = ::scl::any_cast<traced_small>(&value);

    value = ::scl::any::try_copy(::scl::any_view{value});

    EXPECT_EQ(log.copies, 0);
    EXPECT_EQ(log.destructions, 0);
    EXPECT_EQ(::scl::any_cast<traced_small>(&value), before);
}

TEST(AnyInteropTest, AssigningAViewOfTheAnyItselfKeepsAnUncopyableValue)
{
    ::scl::any value{::std::make_unique<int>(42)};

    value = ::scl::any::try_copy(::scl::any_view{value});

    ASSERT_TRUE(value.has_value());
    auto const * const owned = ::scl::any_cast<::std::unique_ptr<int>>(&value);
    ASSERT_NE(owned, nullptr);
    EXPECT_EQ(**owned, 42);
}

TEST(AnyInteropTest, AssigningAViewOfAMemberOfTheStoredObjectReplacesIt)
{
    ::scl::any value{::std::array<::std::string, 1>{::std::string(64, 'x')}};

    value = ::scl::any::try_copy(::scl::any_view{
        ::scl::any_cast<::std::array<::std::string, 1>>(&value)->front()});

    EXPECT_EQ(*::scl::any_cast<::std::string>(&value), ::std::string(64, 'x'));
}

TEST(AnyInteropTest, AViewReadsATypeItMayNotDestroy)
{
    privately_destructible & object = privately_destructible::instance();
    object.value = 42;

    ::scl::any_view const view{object};

    EXPECT_EQ(view.type_name(), ::scl::type_name<privately_destructible>());
    EXPECT_EQ(::scl::any_cast<privately_destructible>(&view)->value, 42);
}

TEST(AnyInteropTest, TakingAReferentWhoseDestructorMayThrowAnswersAnEmptyAny)
{
    destructor_may_throw object;

    ::scl::any const taken = ::scl::any::try_copy(::scl::any_view{object});

    EXPECT_FALSE(taken.has_value());
}

TEST(AnyInteropTest, TakingAReferentTheAnyCouldNotDestroyAnswersAnEmptyAny)
{
    privately_destructible & object = privately_destructible::instance();

    ::scl::any const taken = ::scl::any::try_copy(::scl::any_view{object});

    EXPECT_FALSE(taken.has_value());
}

TEST(AnyInteropTest, HandlesStillConvertAndAssign)
{
    STATIC_EXPECT_TRUE(any_from<::scl::any_view>);
    STATIC_EXPECT_TRUE(any_from<::scl::any_arg>);

    // The copy is asked for by name; no handle converts to an any on its own.
    STATIC_EXPECT_FALSE(any_converts_from<::scl::any_view>);
    STATIC_EXPECT_FALSE(any_converts_from<::scl::any_arg>);
    STATIC_EXPECT_TRUE(any_assignable_from<::scl::any_view>);
    STATIC_EXPECT_TRUE(any_assignable_from<::scl::any_arg>);

    // A handle assigns no more implicitly than it converts.
    STATIC_EXPECT_FALSE(any_assignable_from_a_bare_handle<::scl::any_view>);
    STATIC_EXPECT_FALSE(any_assignable_from_a_bare_handle<::scl::any_arg>);
}

TEST(AnyInteropTest, StoringAViewOnPurposeStaysAvailable)
{
    ::std::string text{"Hello Any!"};
    ::scl::any_view const view{text};

    ::scl::any const stored{::std::in_place_type<::scl::any_view>, view};

    EXPECT_EQ(stored.type_name(), ::scl::type_name<::scl::any_view>());
    EXPECT_EQ(*::scl::any_cast<::std::string>(::scl::any_cast<::scl::any_view>(&stored)), "Hello Any!");
}

TEST(AnyInteropTest, ArgumentOverAnAnyGrantsWriteAccess)
{
    ::scl::any value{42};

    ::scl::any_arg argument = value;

    int * const reached = ::scl::any_cast<int>(&argument);
    ASSERT_NE(reached, nullptr);
    *reached = 7;
    EXPECT_EQ(*::scl::any_cast<int>(&value), 7);
}

TEST(AnyInteropTest, ArgumentOverAConstAnyReadsButDoesNotWrite)
{
    ::scl::any const value{42};

    ::scl::any_arg argument = value;

    EXPECT_EQ(::scl::any_cast<int>(&argument), nullptr);
    EXPECT_EQ(*::scl::any_cast<int const>(&argument), 42);
}

TEST(AnyInteropTest, SwitchSelectsOnAnAny)
{
    ::scl::any const value{42};

    auto const doubled =
        ::scl::any_switch<int>().in_case<int>([](int number) { return number * 2; }).or_else(0).apply(value);

    EXPECT_EQ(doubled, 84);
}

TEST(AnyInteropTest, AnotherSpecializationIsAnotherTypeAndDoesNotTravel)
{
    STATIC_EXPECT_TRUE((built_from<::scl::any, ::scl::any>));

    STATIC_EXPECT_FALSE((built_from<::scl::any, roomy_any>));
    STATIC_EXPECT_FALSE((assigned_from<::scl::any, roomy_any>));
    STATIC_EXPECT_FALSE((swapped_with<::scl::any, roomy_any>));
    STATIC_EXPECT_FALSE((built_from<::scl::any, pmr_any>));
    STATIC_EXPECT_FALSE((assigned_from<::scl::any, pmr_any>));
}

TEST(AnyInteropTest, AValueTravelsBetweenSpecializationsThroughAView)
{
    roomy_any source{::std::string{"Hello Any!"}};

    ::scl::any const taken = ::scl::any::try_copy(::scl::any_view{source});

    EXPECT_EQ(*::scl::any_cast<::std::string>(&taken), "Hello Any!");
    EXPECT_NE(::scl::any_cast<::std::string>(&taken), ::scl::any_cast<::std::string>(&source));
}

TEST(AnyInteropTest, AnAnyIsStoredInsideAnotherOnlyWhenSpelledOut)
{
    ::scl::any const nested{::std::in_place_type<roomy_any>, ::std::string{"inner"}};

    EXPECT_EQ(nested.type_name(), ::scl::type_name<roomy_any>());
    EXPECT_EQ(*::scl::any_cast<::std::string>(::scl::any_cast<roomy_any>(&nested)), "inner");
}

#if SCL_HAS_RTTI
// has_value() looks at the handle, not at what a std::any inside it holds.
TEST(AnyInteropTest, AnEmptyStdAnyStoredInAnAnswersTheSameThroughEveryHandle)
{
    ::scl::any const owner{::std::any{}};
    ::scl::any_view const over_owner{owner};
    ::scl::any_argument const over_owner_arg{owner};

    EXPECT_TRUE(owner.has_value());
    EXPECT_TRUE(over_owner.has_value());
    EXPECT_TRUE(over_owner_arg.has_value());
}
#endif
