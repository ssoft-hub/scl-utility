#include <gtest_utils.h>

#include <scl/utility/any.h>
#include <scl/utility/preprocessor/exceptions.h>

#include <memory>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace
{
    struct boxed_int
    {
        int value = 0;
    };

    struct empty_source
    {};

    struct throwing_source
    {
        int referent = 42;
    };

    struct payload_base
    {};

    struct payload : payload_base
    {};

    struct sliced_source
    {
        payload object;
    };

    struct non_const_copy
    {
        int payload = 0;

        non_const_copy() = default;
        explicit non_const_copy(int value) noexcept
            : payload{value}
        {}
        non_const_copy(non_const_copy & other) noexcept
            : payload{other.payload}
        {}
        non_const_copy & operator=(non_const_copy const &) = default;
        non_const_copy & operator=(non_const_copy &&) = default;
        ~non_const_copy() = default;
    };

    // Copyable and immovable: an unnamed owner cannot hand it over, so the cast copies it.
    struct copy_only
    {
        int id = 0;

        copy_only() = default;

        explicit copy_only(int value) noexcept
            : id{value}
        {}

        copy_only(copy_only const &) = default;
        copy_only(copy_only &&) = delete;
        copy_only & operator=(copy_only const &) = default;
        copy_only & operator=(copy_only &&) = delete;
        ~copy_only() = default;
    };

    // Constructible from anything, so `initializer_list<swallowing>` is viable for a container
    // of them: a hand-over spelled with braces would take that container whole as one element.
    struct swallowing
    {
        template <typename Type>
        // NOLINTNEXTLINE(*-explicit-*): taking anything is what the fixture is for
        constexpr swallowing(Type && /*source*/) noexcept
        {}
    };

    // The cast hands the object to its caller and never ends it, so how it is destroyed is no
    // part of what the cast asks for.
    struct destructor_may_throw
    {
        int payload = 0;

        destructor_may_throw() = default;

        explicit destructor_may_throw(int value) noexcept
            : payload{value}
        {}

        destructor_may_throw(destructor_may_throw const &) = default;
        destructor_may_throw(destructor_may_throw &&) = default;
        destructor_may_throw & operator=(destructor_may_throw const &) = default;
        destructor_may_throw & operator=(destructor_may_throw &&) = default;
        ~destructor_may_throw() noexcept(false) {}
    };

    struct writable_source
    {
        non_const_copy object;
    };

    struct copy_counted
    {
        inline static int copies = 0;
        inline static int moves = 0;

        int payload = 0;

        copy_counted() = default;
        explicit copy_counted(int value) noexcept
            : payload{value}
        {}
        copy_counted(copy_counted const & other) noexcept
            : payload{other.payload}
        {
            ++copies;
        }
        copy_counted(copy_counted && other) noexcept
            : payload{other.payload}
        {
            ++moves;
        }
        copy_counted & operator=(copy_counted const &) = default;
        copy_counted & operator=(copy_counted &&) = default;
        ~copy_counted() = default;
    };

    // Owns nothing, yet its answer carries the source's constness, as an owner's does.
    struct borrowing_source
    {
        copy_counted * referent;
    };

    struct explicit_copy
    {
        int payload = 0;

        explicit_copy() = default;
        explicit explicit_copy(int value) noexcept
            : payload{value}
        {}
        explicit explicit_copy(explicit_copy const &) = default;
        explicit_copy(explicit_copy &&) = default;
        explicit_copy & operator=(explicit_copy const &) = default;
        explicit_copy & operator=(explicit_copy &&) = default;
        ~explicit_copy() = default;
    };
} // namespace

namespace scl
{
    template <>
    struct any_cast_traits<boxed_int>
    {
        template <typename Target, typename Source>
        [[nodiscard]]
        static constexpr auto access(Source * box) noexcept /**/
            -> ::std::conditional_t<::std::is_const_v<Source>, Target const, Target> *
        {
            if constexpr (::std::is_same_v<::std::remove_cv_t<Target>, int>)
                return ::std::addressof(box->value);
            else
                return nullptr;
        }
    };

    template <>
    struct any_cast_traits<sliced_source>
    {
        template <typename Target, typename Source>
        [[nodiscard]]
        static constexpr payload * access(Source * source) noexcept
        {
            return ::std::addressof(source->object);
        }
    };

    template <>
    struct any_cast_traits<writable_source>
    {
        template <typename Target, typename Source>
        [[nodiscard]]
        static constexpr auto access(Source * source) noexcept /**/
            -> ::std::conditional_t<::std::is_const_v<Source>, non_const_copy const, non_const_copy> *
        {
            return ::std::addressof(source->object);
        }
    };

    template <>
    struct any_cast_traits<borrowing_source>
    {
        template <typename Target, typename Source>
        [[nodiscard]]
        static constexpr auto access(Source * source) noexcept /**/
            -> ::std::conditional_t<::std::is_const_v<Source>, copy_counted const, copy_counted> *
        {
            return source->referent;
        }
    };

    template <>
    struct any_cast_traits<throwing_source>
    {
        template <typename Target, typename Source>
        [[nodiscard]]
        static constexpr Target * access(Source * source)
        {
            return ::std::addressof(source->referent);
        }
    };

    template <>
    struct any_cast_traits<empty_source>
    {
        template <typename Target, typename Source>
        [[nodiscard]]
        static constexpr Target const * access(Source *) noexcept
        {
            return nullptr;
        }
    };
} // namespace scl

namespace
{
    template <typename Source, typename Type>
    concept castable_to = requires(Source * source) { ::scl::any_cast<Type>(source); };

    template <typename Source>
    concept pointer_castable = castable_to<Source, int>;

    template <typename Source, typename Type>
    concept lvalue_castable_to = requires(Source & source) { ::scl::any_cast<Type>(source); };

    template <typename Source>
    concept value_castable = lvalue_castable_to<Source, int>;

    template <typename Source>
    concept bound_castable = lvalue_castable_to<Source, int const &>;

    template <typename Source>
    concept writing_castable = lvalue_castable_to<Source, int &>;

    template <typename Source>
    concept rvalue_castable = requires(Source source) { ::scl::any_cast<int>(::std::move(source)); };
} // namespace

TEST(AnyCastTest, OnlyASourceWhoseTraitAnswersIsCastable)
{
    STATIC_EXPECT_TRUE(pointer_castable<::scl::any_view>);
    STATIC_EXPECT_TRUE(pointer_castable<::scl::any_mutable_view>);
    STATIC_EXPECT_TRUE(pointer_castable<::scl::any_argument>);
    STATIC_EXPECT_TRUE(pointer_castable<::scl::any>);
    STATIC_EXPECT_TRUE(pointer_castable<boxed_int>);

    STATIC_EXPECT_FALSE(pointer_castable<int>);
    STATIC_EXPECT_FALSE(pointer_castable<::std::string>);
}

TEST(AnyCastTest, AQualifiedHandleIsCastableAsItsUnqualifiedSelf)
{
    STATIC_EXPECT_TRUE(pointer_castable<::scl::any_view const>);
    STATIC_EXPECT_TRUE(pointer_castable<::scl::any_view volatile>);
    STATIC_EXPECT_TRUE(pointer_castable<::scl::any_view const volatile>);
}

TEST(AnyCastTest, ATraitOutsideTheLibraryJoinsTheCast)
{
    boxed_int box{42};

    ASSERT_NE(::scl::any_cast<int>(&box), nullptr);
    EXPECT_EQ(*::scl::any_cast<int>(&box), 42);

    *::scl::any_cast<int>(&box) = 11; // the trait grants the write, so the request may ask for it
    EXPECT_EQ(box.value, 11);

    EXPECT_EQ(::scl::any_cast<double>(&box), nullptr);
}

TEST(AnyCastTest, ATraitDecidesWhatTheRequestCarries)
{
    boxed_int box{42};
    boxed_int const frozen{42};

    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(::scl::any_cast<int>(&box)), int *>));
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(::scl::any_cast<int>(&frozen)), int const *>));
}

TEST(AnyCastTest, ATraitAnsweringAnotherTypeIsOfferedOnlyForTheTypeItAnswers)
{
    STATIC_EXPECT_TRUE((castable_to<boxed_int, int>));
    STATIC_EXPECT_TRUE((castable_to<sliced_source, payload>));
    STATIC_EXPECT_FALSE((castable_to<sliced_source, payload_base>));

    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::any_cast<payload>(static_cast<sliced_source *>(nullptr))), payload *>));
}

#if SCL_HAS_EXCEPTIONS
TEST(AnyCastTest, ASourceThatReachesNothingAnswersNothing)
{
    empty_source nothing;

    EXPECT_EQ(::scl::any_cast<int>(&nothing), nullptr);
    EXPECT_THROW(::std::ignore = ::scl::any_cast<int>(nothing), ::scl::bad_any_cast);
}
#endif

TEST(AnyCastTest, AVolatileOwnerIsRefusedRatherThanAnsweredWithoutTheQualifier)
{
    STATIC_EXPECT_TRUE(pointer_castable<::scl::any>);
    STATIC_EXPECT_TRUE(pointer_castable<::scl::any const>);
    STATIC_EXPECT_FALSE(pointer_castable<::scl::any volatile>);
    STATIC_EXPECT_FALSE(pointer_castable<::scl::any const volatile>);

    STATIC_EXPECT_FALSE(value_castable<::scl::any volatile>);
    STATIC_EXPECT_FALSE(bound_castable<::scl::any volatile>);
    STATIC_EXPECT_FALSE(writing_castable<::scl::any volatile>);
}

#if SCL_HAS_EXCEPTIONS
TEST(AnyCastTest, ATraitDecidesWhichReferenceFormsAreOffered)
{
    STATIC_EXPECT_TRUE(writing_castable<boxed_int>);
    STATIC_EXPECT_TRUE(bound_castable<boxed_int>);
    STATIC_EXPECT_FALSE(writing_castable<empty_source>);
    STATIC_EXPECT_TRUE(bound_castable<empty_source>);

    STATIC_EXPECT_FALSE(writing_castable<boxed_int const>);
    STATIC_EXPECT_TRUE(bound_castable<boxed_int const>);
}

TEST(AnyCastTest, ASourceWithNoNameIsReadWhateverKindItIs)
{
    STATIC_EXPECT_TRUE(rvalue_castable<::scl::any_view>);
    STATIC_EXPECT_TRUE(rvalue_castable<::scl::any_mutable_view>);
    STATIC_EXPECT_TRUE(rvalue_castable<::scl::any_argument>);
    STATIC_EXPECT_TRUE(rvalue_castable<boxed_int>);
    STATIC_EXPECT_TRUE(rvalue_castable<::scl::any>);
}

TEST(AnyCastTest, AnUnnamedOwnerHandsTheObjectOverInsteadOfCopying)
{
    copy_counted::copies = 0;
    copy_counted::moves = 0;

    auto const taken = ::scl::any_cast<copy_counted>(::scl::any{::std::in_place_type<copy_counted>, 42});

    EXPECT_EQ(copy_counted::copies, 0);
    EXPECT_EQ(copy_counted::moves, 1);
    EXPECT_EQ(taken.payload, 42);
}

TEST(AnyCastTest, ANamedOwnerIsCopiedFromAndKeepsWhatItHolds)
{
    ::scl::any named{::std::in_place_type<copy_counted>, 42};
    copy_counted::copies = 0;
    copy_counted::moves = 0;

    // Only a source with no name at the call hands the object over; a named one is copied.
    auto const taken = ::scl::any_cast<copy_counted>(named);

    EXPECT_EQ(copy_counted::copies, 1);
    EXPECT_EQ(copy_counted::moves, 0);
    EXPECT_EQ(taken.payload, 42);
    ASSERT_NE(::scl::any_cast<copy_counted>(&named), nullptr);
    EXPECT_EQ(::scl::any_cast<copy_counted>(&named)->payload, 42);
}

TEST(AnyCastTest, AWritingReferenceReachesTheObjectAnOwnerHolds)
{
    ::scl::any value{42};

    STATIC_EXPECT_TRUE(writing_castable<::scl::any>);
    ::scl::any_cast<int &>(value) = 9;

    EXPECT_EQ(*::scl::any_cast<int>(&value), 9);
}

TEST(AnyCastTest, AHandleWithNoNameIsReadByCopyingBecauseItsTraitSaysSo)
{
    // A handle owns nothing, so no trait of one declares movable_tag: an unnamed handle is read
    // by copying, and the object it refers to survives the read.
    copy_counted object{42};
    copy_counted::copies = 0;
    copy_counted::moves = 0;

    auto const from_writing = ::scl::any_cast<copy_counted>(::scl::any_mutable_view{object});
    auto const from_argument = ::scl::any_cast<copy_counted>(::scl::any_arg{object});

    EXPECT_EQ(copy_counted::copies, 2);
    EXPECT_EQ(copy_counted::moves, 0);
    EXPECT_EQ(from_writing.payload, 42);
    EXPECT_EQ(from_argument.payload, 42);
    EXPECT_EQ(object.payload, 42);
}
#endif

TEST(AnyCastTest, ThePointerFormThrowsExactlyWhereTheTraitDoes)
{
    // The exception specification of the pointer form follows the trait it forwards to.
    STATIC_EXPECT_TRUE(noexcept(::scl::any_cast<int>(static_cast<::scl::any_view const *>(nullptr))));
    STATIC_EXPECT_TRUE(noexcept(::scl::any_cast<int>(static_cast<::scl::any_argument const *>(nullptr))));
    STATIC_EXPECT_TRUE(noexcept(::scl::any_cast<int>(static_cast<::scl::any *>(nullptr))));
    STATIC_EXPECT_FALSE(noexcept(::scl::any_cast<int>(static_cast<throwing_source *>(nullptr))));
}

#if SCL_HAS_EXCEPTIONS
TEST(AnyCastTest, OwnershipIsStatedByTheTraitRatherThanReadOffItsAnswer)
{
    copy_counted object{42};
    copy_counted::copies = 0;
    copy_counted::moves = 0;

    auto const taken = ::scl::any_cast<copy_counted>(borrowing_source{&object});

    EXPECT_EQ(copy_counted::copies, 1);
    EXPECT_EQ(copy_counted::moves, 0);
    EXPECT_EQ(taken.payload, 42);
}

TEST(AnyCastTest, AMoveOnlyObjectComesOutOfAnUnnamedOwner)
{
    auto const owned = ::scl::any_cast<::std::unique_ptr<int>>(::scl::any{::std::make_unique<int>(7)});

    ASSERT_NE(owned, nullptr);
    EXPECT_EQ(*owned, 7);
}
#endif

#if SCL_HAS_EXCEPTIONS
TEST(AnyCastTest, ACopyComesOutOfAConstBoundReferent)
{
    int const frozen = 42;
    ::scl::any_argument const & arg = frozen;

    EXPECT_EQ(::scl::any_cast<int>(arg), 42);
}

TEST(AnyCastTest, ATypeWithAnExplicitCopyConstructorStillCopiesOut)
{
    explicit_copy const source{42};
    ::scl::any_view const view{source};

    EXPECT_EQ(::scl::any_cast<explicit_copy>(view).payload, 42);
}

TEST(AnyCastTest, OnlyTheCopyingFormAsksForACopyFromAConstObject)
{
    STATIC_EXPECT_TRUE((lvalue_castable_to<::scl::any_view, explicit_copy>));

    STATIC_EXPECT_FALSE((lvalue_castable_to<writable_source, non_const_copy>));
    STATIC_EXPECT_TRUE((lvalue_castable_to<writable_source, non_const_copy &>));
    STATIC_EXPECT_TRUE((lvalue_castable_to<writable_source, non_const_copy const &>));
    STATIC_EXPECT_TRUE((castable_to<writable_source, non_const_copy>));
}

TEST(AnyCastTest, TheWritingReferenceFormReachesTheObjectOutsideTheLibrary)
{
    boxed_int box{7};

    ::scl::any_cast<int &>(box) = 9;

    EXPECT_EQ(box.value, 9);
}
#endif

TEST(AnyCastTest, CoverageComparesEveryQualifierAndNotJustConst)
{
    int const frozen = 42;
    ::scl::any_argument const & arg = frozen;

    EXPECT_EQ(::scl::any_cast<int>(&arg), nullptr);
    EXPECT_EQ(::scl::any_cast<int volatile>(&arg), nullptr);
    EXPECT_NE(::scl::any_cast<int const>(&arg), nullptr);
    EXPECT_NE(::scl::any_cast<int const volatile>(&arg), nullptr);
}

TEST(AnyCastTest, TheRequestSpellingDecidesTheResultTypeAndTheBindingDecidesTheAnswer)
{
    int plain = 42;
    ::scl::any_view const reading{plain};
    ::scl::any_mutable_view const writing{plain};

    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(::scl::any_cast<int>(&reading)), int const *>));
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(::scl::any_cast<int>(&writing)), int *>));
}

#if SCL_HAS_EXCEPTIONS
TEST(AnyCastTest, AnUnnamedOwnerOfAnImmovableObjectIsReadByCopying)
{
    auto const taken = ::scl::any_cast<copy_only>(::scl::any{::std::in_place_type<copy_only>, 5});

    EXPECT_EQ(taken.id, 5);
}

TEST(AnyCastTest, AnUnnamedOwnerHandsOverAContainerWhole)
{
    ::std::vector<swallowing> const source(3U, swallowing{0});

    auto const taken = ::scl::any_cast<::std::vector<swallowing>>(::scl::any{source});

    EXPECT_EQ(taken.size(), source.size());
}

TEST(AnyCastTest, ATypeWhoseDestructorMayThrowIsStillReadOut)
{
    destructor_may_throw object{5};
    ::scl::any_view const view{object};

    STATIC_EXPECT_TRUE((lvalue_castable_to<::scl::any_view, destructor_may_throw>));
    EXPECT_EQ(::scl::any_cast<destructor_may_throw>(view).payload, 5);
}
#endif
