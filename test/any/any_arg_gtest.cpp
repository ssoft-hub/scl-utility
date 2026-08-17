#include <gtest_utils.h>

#include <scl/utility/any.h>

#include <any>
#include <string>
#include <type_traits>
#include <typeinfo>

namespace
{
    struct counted
    {
        inline static int copies = 0;

        int id = 0;

        counted() = default;
        explicit counted(int value) noexcept
            : id{value}
        {}
        counted(counted const & other) noexcept
            : id{other.id}
        {
            ++copies;
        }
        counted(counted &&) noexcept = default;
        counted & operator=(counted const &) = default;
        counted & operator=(counted &&) = default;
        ~counted() = default;
    };

    template <typename Type>
    concept mutable_ref_castable = requires(::scl::any_arg arg) { ::scl::any_cast<Type &>(arg); };

    template <typename Type>
    concept arg_castable = requires(::scl::any_arg arg) { ::scl::any_cast<Type>(arg); };

    template <typename Type>
    concept arg_ptr_castable = requires(::scl::any_arg const * arg) { ::scl::any_cast<Type>(arg); };

    template <typename Type>
    concept arg_from_lvalue = requires(Type & object) { ::scl::any_arg{object}; };

    template <typename Type>
    concept arg_from_rvalue = requires { ::scl::any_arg{Type{}}; };

    // Reaching a mutable reference through a read-only view, which the implicit
    // any_view -> any_arg conversion would otherwise allow.
    template <typename Type>
    concept view_mutable_ref_castable =
        requires(::scl::any_view view) { ::scl::any_cast<Type &>(view); };

    // The constexpr casts below are spelled as calls because that is where an argument's
    // anchor lives: it is created by the caller and lasts for the whole call.
    constexpr int read_argument(::scl::any_arg argument) noexcept
    {
        auto const * reached = ::scl::any_cast<int const>(&argument);
        return (reached != nullptr) ? *reached : -1;
    }

    constexpr int read_argument_by_reference(::scl::any_arg argument)
    {
        return ::scl::any_cast<int const &>(argument);
    }

    constexpr void increment_argument(::scl::any_arg argument) noexcept
    {
        if (auto * reached = ::scl::any_cast<int>(&argument))
            ++*reached;
    }

    constexpr void increment_argument_by_reference(::scl::any_arg argument)
    {
        ++::scl::any_cast<int &>(argument);
    }

    constexpr bool argument_refuses(::scl::any_arg argument) noexcept
    {
        return ::scl::any_cast<double const>(&argument) == nullptr;
    }

    constexpr int read_lvalue_argument() noexcept
    {
        int value = 42;
        return read_argument(value);
    }

    constexpr int incremented_lvalue() noexcept
    {
        int value = 41;
        increment_argument(value);
        return value;
    }

    constexpr int incremented_lvalue_by_reference()
    {
        int value = 41;
        increment_argument_by_reference(value);
        return value;
    }

    constexpr int copy_argument_out(::scl::any_arg argument)
    {
        return ::scl::any_cast<int>(argument);
    }

    // The anchor must not outrank the coverage rule: a write over a const referent has to
    // answer null, not hand out the pointer the anchor holds.
    constexpr bool const_referent_refuses_write() noexcept
    {
        int const value = 42;
        return [](::scl::any_arg argument) noexcept {
            return ::scl::any_cast<int>(&argument) == nullptr && ::scl::any_cast<int const>(&argument) != nullptr;
        }(value);
    }

    constexpr bool volatile_referent_needs_volatile() noexcept
    {
        int volatile value = 42;
        return [](::scl::any_arg argument) noexcept {
            return ::scl::any_cast<int const>(&argument) == nullptr &&
                ::scl::any_cast<int const volatile>(&argument) != nullptr;
        }(value);
    }

    // A const handle cannot escalate to a write either, anchor or no anchor.
    constexpr bool const_handle_refuses_write() noexcept
    {
        int value = 42;
        return [](::scl::any_arg const argument) noexcept {
            return ::scl::any_cast<int>(&argument) == nullptr && ::scl::any_cast<int const>(&argument) != nullptr;
        }(value);
    }

    // Delegation: the copy keeps the caller's anchor, which is still alive.
    constexpr int delegated_read(::scl::any_arg argument) noexcept
    {
        return read_argument(argument);
    }

    constexpr bool identity_holds(::scl::any_arg const argument) noexcept
    {
        return argument.has_value() && argument.type_name() == ::scl::type_name<int>() &&
            argument.type_key() == &::scl::type_key_of<int>();
    }

    // At run time the descriptor is the static one, so identity holds for an argument that
    // is not a parameter too. Not constexpr: before P2738 (C++26) the descriptor seen during
    // constant evaluation is the caller's anchor, which a local outlives.
    bool identity_holds_on_local() noexcept
    {
        int const probe = 42;
        ::scl::any_arg const argument{probe};

        return argument.has_value() && argument.type_name() == ::scl::type_name<int>() &&
            argument.type_key() == &::scl::type_key_of<int>();
    }

// The same condition any_arg.h branches on to decide whether an anchor is needed at all.
#if SCL_DETAIL_ANY_HAS_CONSTEXPR_VOID_CAST
    // Each of these is refused before P2738 (C++26), and each is a case the anchor could
    // not reach: an argument that is not a parameter has none, and one adopted from a view
    // carries the view's static descriptor instead.
    constexpr int read_local_argument() noexcept
    {
        int const probe = 21;
        ::scl::any_arg const argument{probe};
        auto const * reached = ::scl::any_cast<int const>(&argument);

        return (reached != nullptr) ? *reached * 2 : -1;
    }

    constexpr int read_adopted_argument(::scl::any_view const view) noexcept
    {
        ::scl::any_arg const argument{view};
        auto const * reached = ::scl::any_cast<int const>(&argument);

        return (reached != nullptr) ? *reached * 2 : -1;
    }

    constexpr int read_through_view() noexcept
    {
        int const probe = 21;

        return read_adopted_argument(::scl::any_view{probe});
    }

    constexpr int write_through_local_argument() noexcept
    {
        int value = 21;
        ::scl::any_arg argument{value};
        *::scl::any_cast<int>(&argument) = 42;

        return value;
    }
#endif
} // namespace

TEST(AnyArgTest, CompileTimeGuards)
{
    // Parameter-only view: binds lvalues and rvalues of any constness.
    STATIC_EXPECT_TRUE(arg_from_lvalue<counted>);
    STATIC_EXPECT_TRUE(arg_from_lvalue<counted const>);
    STATIC_EXPECT_TRUE(arg_from_rvalue<counted>);
    STATIC_EXPECT_TRUE(arg_from_rvalue<counted const>);
    STATIC_EXPECT_TRUE(arg_from_rvalue<int>);
    STATIC_EXPECT_TRUE(arg_from_lvalue<::std::any>);
    STATIC_EXPECT_TRUE(arg_from_rvalue<::std::any>);
    STATIC_EXPECT_TRUE(arg_from_lvalue<::scl::any_view>);
    STATIC_EXPECT_TRUE(arg_from_rvalue<::scl::any_view>);

#if SCL_HAS_RTTI
    // std::any has no volatile-qualified members, so a volatile std::any cannot
    // be bound at all — not even as an lvalue. Without RTTI the library cannot
    // name std::any to exclude it (see the @warning on the std::any constructor).
    STATIC_EXPECT_FALSE(arg_from_lvalue<::std::any volatile>);
#endif

    // Storage is discouraged at the API level: no default state, no rebinding.
    STATIC_EXPECT_FALSE(::std::is_default_constructible_v<::scl::any_arg>);
    STATIC_EXPECT_FALSE(::std::is_copy_assignable_v<::scl::any_arg>);
    STATIC_EXPECT_FALSE(::std::is_move_assignable_v<::scl::any_arg>);
    STATIC_EXPECT_TRUE(::std::is_copy_constructible_v<::scl::any_arg>);
    STATIC_EXPECT_TRUE(::std::is_trivially_copyable_v<::scl::any_arg>);
    // Two pointers, as a view: what makes a cast constant-evaluable is a descriptor too,
    // so it rides in the pointer the view already spends on one.
    STATIC_EXPECT_TRUE(sizeof(::scl::any_arg) == 2 * sizeof(void *));
    STATIC_EXPECT_TRUE(sizeof(::scl::any_view) == 2 * sizeof(void *));

    // Unlike any_view, an argument grants write access — and only an argument does.
    STATIC_EXPECT_TRUE(mutable_ref_castable<int>);
    STATIC_EXPECT_TRUE(mutable_ref_castable<int const>);
    STATIC_EXPECT_FALSE(view_mutable_ref_castable<int>);

    // Rvalue references never come out: nothing may look movable-from.
    STATIC_EXPECT_FALSE(arg_castable<int &&>);
    STATIC_EXPECT_FALSE(arg_castable<int const &&>);
    STATIC_EXPECT_FALSE(arg_ptr_castable<void>);
    STATIC_EXPECT_FALSE(arg_ptr_castable<int()>);

    // No conversion to a view in any qualification: a view may be stored and an argument
    // may not, so one is never obtained from the other.
    STATIC_EXPECT_FALSE((::std::is_convertible_v<::scl::any_arg, ::scl::any_view>));
    STATIC_EXPECT_FALSE((::std::is_convertible_v<::scl::any_arg &, ::scl::any_view>));
    STATIC_EXPECT_FALSE((::std::is_convertible_v<::scl::any_arg const &, ::scl::any_view>));
    STATIC_EXPECT_FALSE((::std::is_constructible_v<::scl::any_view, ::scl::any_arg &>));

    // The other direction stays: a view hands its referent to an argument.
    STATIC_EXPECT_TRUE((::std::is_convertible_v<::scl::any_view &, ::scl::any_arg>));
}

TEST(AnyArgTest, ConstexprIdentityOnRawBacking)
{
    STATIC_EXPECT_TRUE(identity_holds(42));

    // An argument that is not a parameter keeps its identity at run time.
    EXPECT_TRUE(identity_holds_on_local());
}

TEST(AnyArgTest, ConstexprCastThroughParameter)
{
    // An rvalue argument: the referent and the anchor are both the caller's temporaries.
    STATIC_EXPECT_TRUE(read_argument(42) == 42);
    STATIC_EXPECT_TRUE(read_argument_by_reference(42) == 42);
    STATIC_EXPECT_TRUE(read_lvalue_argument() == 42);

    // A mismatch answers null rather than failing the evaluation.
    STATIC_EXPECT_TRUE(argument_refuses(42));

    // The value form copies out; delegation carries the caller's anchor onward.
    STATIC_EXPECT_TRUE(copy_argument_out(42) == 42);
    STATIC_EXPECT_TRUE(delegated_read(42) == 42);
}

TEST(AnyArgTest, ConstexprCastObeysQualifierCoverage)
{
    // The anchor answers only what accepts() has already granted — it is reached after
    // the coverage check, never instead of it.
    STATIC_EXPECT_TRUE(const_referent_refuses_write());
    STATIC_EXPECT_TRUE(volatile_referent_needs_volatile());
    STATIC_EXPECT_TRUE(const_handle_refuses_write());
}

TEST(AnyArgTest, ConstexprWriteReachesTheCallersObject)
{
    STATIC_EXPECT_TRUE(incremented_lvalue() == 42);
    STATIC_EXPECT_TRUE(incremented_lvalue_by_reference() == 42);
}

TEST(AnyArgTest, ConstexprCastReachesAnyPositionOnceTheVoidCastFolds)
{
#if SCL_DETAIL_ANY_HAS_CONSTEXPR_VOID_CAST
    // The parameter-only bound is the anchor's, not the type's: where the recovery folds
    // on its own, an argument answers wherever it sits.
    STATIC_EXPECT_TRUE(read_local_argument() == 42);
    STATIC_EXPECT_TRUE(read_through_view() == 42);
    STATIC_EXPECT_TRUE(write_through_local_argument() == 42);
#else
    GTEST_SKIP() << "recovering a typed pointer from void const * needs P2738 (C++26)";
#endif
}

TEST(AnyArgTest, TemporaryArgumentReadWithinCall)
{
    counted::copies = 0;

    auto const id = [](::scl::any_arg value) {
        return ::scl::any_cast<counted const &>(value).id; // zero-copy read of a temporary
    }(counted{7});

    EXPECT_EQ(id, 7);
    EXPECT_EQ(counted::copies, 0);
}

TEST(AnyArgTest, LvalueArgumentCasts)
{
    counted::copies = 0;
    counted value{9};
    ::scl::any_arg arg{value};

    EXPECT_TRUE(arg.has_value());
    EXPECT_EQ(arg.type_name(), ::scl::type_name<counted>());

    ASSERT_NE(::scl::any_cast<counted>(&arg), nullptr); // pointer form
    EXPECT_EQ(::scl::any_cast<counted>(&arg)->id, 9);
    EXPECT_EQ(::scl::any_cast<double>(&arg), nullptr);
    EXPECT_EQ(::scl::any_cast<int>(static_cast<::scl::any_arg const *>(nullptr)), nullptr);

    EXPECT_EQ(&::scl::any_cast<counted const &>(arg), &value); // binds to the original
    EXPECT_EQ(counted::copies, 0);

    EXPECT_EQ(::scl::any_cast<counted>(arg).id, 9); // value form: exactly one copy
    EXPECT_EQ(counted::copies, 1);
}

TEST(AnyArgTest, MismatchThrows)
{
    counted value{1};
    ::scl::any_arg arg{value};

    EXPECT_THROW((void)::scl::any_cast<double>(arg), ::scl::bad_any_cast);
    EXPECT_THROW((void)::scl::any_cast<double>(arg), ::std::bad_cast);
}

TEST(AnyArgTest, ConstructsFromAnyView)
{
    counted value{6};
    ::scl::any_view const view{value};
    ::scl::any_arg const arg{view}; // adopts the referent, not the view object

    EXPECT_EQ(::scl::any_cast<counted const>(&arg), &value);
    EXPECT_EQ(arg.type_name(), ::scl::type_name<counted>());
}

TEST(AnyArgTest, AdoptedAnyViewStaysReadOnly)
{
    counted value{1};
    ::scl::any_view const view{value};

    auto writes = [](::scl::any_arg arg) { return ::scl::any_cast<counted>(&arg) != nullptr; };

    EXPECT_TRUE(writes(value)); // the object itself grants write access
    EXPECT_FALSE(writes(view)); // the same object through a view does not
}

TEST(AnyArgTest, AdoptedFromEmptyAnyView)
{
    constexpr ::scl::any_view empty{};
    constexpr ::scl::any_arg arg{empty};

    STATIC_EXPECT_FALSE(arg.has_value());
    STATIC_EXPECT_TRUE(arg.type_name().empty());
    EXPECT_EQ(::scl::any_cast<int const>(&arg), nullptr);
    EXPECT_THROW((void)::scl::any_cast<int &>(arg), ::scl::bad_any_cast);
}

#if SCL_HAS_RTTI

TEST(AnyArgTest, StdAnyBackingCast)
{
    ::std::any const boxed{::std::string{"hello"}};
    ::scl::any_arg const arg{boxed};

    EXPECT_TRUE(arg.has_value());
    EXPECT_EQ(arg.type_name(), ::scl::type_name<::std::any>()); // names the backing, not the boxed type

    ASSERT_NE(::scl::any_cast<::std::string const>(&arg), nullptr);
    EXPECT_EQ(*::scl::any_cast<::std::string const>(&arg), "hello");
    EXPECT_EQ(::scl::any_cast<int const>(&arg), nullptr);
    EXPECT_EQ(::scl::any_cast<::std::string const &>(arg), "hello");
}

TEST(AnyArgTest, MutableAccessThroughNonConstStdAny)
{
    ::std::any boxed{::std::string{"hello"}};
    ::scl::any_arg arg{boxed};

    auto * text = ::scl::any_cast<::std::string>(&arg);
    ASSERT_NE(text, nullptr);
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(text), ::std::string *>));

    *text += ", world";
    EXPECT_EQ(::std::any_cast<::std::string const &>(boxed), "hello, world");

    ::scl::any_cast<::std::string &>(arg) += "!"; // reference form writes too
    EXPECT_EQ(::std::any_cast<::std::string const &>(boxed), "hello, world!");
}

TEST(AnyArgTest, MutableAccessRefusedOnConstStdAny)
{
    ::std::any const boxed{::std::string{"frozen"}};
    ::scl::any_arg const arg{boxed};

    EXPECT_EQ(::scl::any_cast<::std::string>(&arg), nullptr);
    EXPECT_EQ(*::scl::any_cast<::std::string const>(&arg), "frozen");
    EXPECT_THROW((void)::scl::any_cast<::std::string &>(arg), ::scl::bad_any_cast);
}

TEST(AnyArgTest, MutableAccessRefusedOnStdAnyThroughAnyView)
{
    ::std::any boxed{::std::string{"frozen"}};
    ::scl::any_view const view{boxed};
    ::scl::any_arg const arg{view};

    EXPECT_EQ(::scl::any_cast<::std::string>(&arg), nullptr);
    EXPECT_EQ(*::scl::any_cast<::std::string const>(&arg), "frozen");
}

TEST(AnyArgTest, MutableAccessThroughTemporaryStdAny)
{
    auto const doubled = [](::scl::any_arg value) {
        auto * number = ::scl::any_cast<int>(&value);
        return (number != nullptr) ? (*number *= 2) : 0;
    }(::std::any{21});

    EXPECT_EQ(doubled, 42);
}

TEST(AnyArgTest, TemporaryStdAnyReadWithinCall)
{
    auto const text = [](::scl::any_arg value) {
        return ::scl::any_cast<::std::string>(value); // copy out survives the call
    }(::std::any{::std::string{"boxed"}});

    EXPECT_EQ(text, "boxed");
}

TEST(AnyArgTest, ViewOverEmptyStdAny)
{
    ::std::any const empty_any{};
    ::scl::any_arg const arg{empty_any};

    EXPECT_FALSE(arg.has_value());
    EXPECT_EQ(arg.type_name(), ::scl::type_name<::std::any>());
}

#endif // SCL_HAS_RTTI

TEST(AnyArgTest, MutableAccessOnUnqualifiedReferent)
{
    counted value{1};

    auto bump = [](::scl::any_arg arg) {
        if (auto * target = ::scl::any_cast<counted>(&arg))
            target->id = 42;
    };

    bump(value);
    EXPECT_EQ(value.id, 42); // the write reached the caller's object

    ::scl::any_arg arg{value};
    ::scl::any_cast<counted &>(arg).id = 7;
    EXPECT_EQ(value.id, 7);

    // The pointer form yields exactly the requested qualification.
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(::scl::any_cast<counted>(&arg)), counted *>));
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(::scl::any_cast<counted const>(&arg)), counted const *>));
}

TEST(AnyArgTest, MutableAccessRefusedOnQualifiedReferent)
{
    counted const const_value{1};
    counted volatile volatile_value{2};

    ::scl::any_arg over_const{const_value};
    ::scl::any_arg over_volatile{volatile_value};

    EXPECT_EQ(::scl::any_cast<counted>(&over_const), nullptr);
    EXPECT_EQ(::scl::any_cast<counted>(&over_volatile), nullptr);
    EXPECT_THROW((void)::scl::any_cast<counted &>(over_const), ::scl::bad_any_cast);

    // A request that covers the referent's qualifiers still answers.
    ASSERT_NE(::scl::any_cast<counted const>(&over_const), nullptr);
    ASSERT_NE(::scl::any_cast<counted volatile>(&over_volatile), nullptr);
    EXPECT_EQ(::scl::any_cast<counted const &>(over_const).id, 1);
}

TEST(AnyArgTest, MutableAccessRejectsTypeMismatch)
{
    counted value{1};
    ::scl::any_arg const arg{value};

    EXPECT_EQ(::scl::any_cast<double>(&arg), nullptr);
    EXPECT_THROW((void)::scl::any_cast<double &>(arg), ::scl::bad_any_cast);
}

TEST(AnyArgTest, VolatileReferentWritableWhenRequestedAsVolatile)
{
    int volatile value = 1;
    ::scl::any_arg arg{value};

    auto * writable = ::scl::any_cast<int volatile>(&arg);
    ASSERT_NE(writable, nullptr);
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(writable), int volatile *>));

    *writable = 42;
    EXPECT_EQ(value, 42);
}

TEST(AnyArgTest, VolatileSurvivesAdoptionFromAnyView)
{
    int volatile value = 5;
    ::scl::any_view const view{value};
    ::scl::any_arg const adopted{view};

    // The const-qualified form keeps volatile: a plain read would drop it.
    EXPECT_EQ(::scl::any_cast<int const>(&adopted), nullptr);
    EXPECT_EQ(::scl::any_cast<int const volatile>(&adopted), &value);
    EXPECT_EQ(::scl::any_cast<int volatile>(&adopted), nullptr); // write stays closed
}

TEST(AnyArgTest, RvalueBoundReferentWritableUnlessConst)
{
    // A temporary stays alive for the whole call, so only its own constness decides.
    auto writes = [](::scl::any_arg arg) { return ::scl::any_cast<counted>(&arg) != nullptr; };

    EXPECT_TRUE(writes(counted{1}));
    EXPECT_FALSE(writes(static_cast<counted const>(counted{1})));
}

TEST(AnyArgTest, ConstHandleRefusesWriteRegardlessOfReferent)
{
    // The handle's own const is a qualifier too: it must be covered by the request just
    // like the referent's, so a const any_arg cannot escalate to a write the handle
    // itself does not carry — even over an otherwise unqualified referent.
    counted value{1};
    ::scl::any_arg const arg{value};

    EXPECT_EQ(::scl::any_cast<counted>(&arg), nullptr);
    ASSERT_NE(::scl::any_cast<counted const>(&arg), nullptr);
    EXPECT_EQ(::scl::any_cast<counted const>(&arg)->id, 1);
    EXPECT_THROW((void)::scl::any_cast<counted &>(arg), ::scl::bad_any_cast);
}

TEST(AnyArgTest, NonConstHandleStillWrites)
{
    // Confirms the const-handle refusal above is specific to const, not a general
    // regression: a non-const handle over the same kind of referent still writes.
    counted value{1};
    ::scl::any_arg arg{value};

    auto * p = ::scl::any_cast<counted>(&arg);
    ASSERT_NE(p, nullptr);
    p->id = 9;
    EXPECT_EQ(value.id, 9);
}

TEST(AnyArgTest, VolatileHandleRequiresVolatileRequest)
{
    // Symmetric with the const case: the handle's own volatile is a qualifier the
    // request must cover too, independent of the referent's own qualifiers.
    counted value{1};
    ::scl::any_arg volatile arg{value};

    EXPECT_EQ(::scl::any_cast<counted>(&arg), nullptr);
    auto * p = ::scl::any_cast<counted volatile>(&arg);
    ASSERT_NE(p, nullptr);
    p->id = 5; // volatile handle, unqualified referent: still writable once covered
    EXPECT_EQ(value.id, 5);
}

TEST(AnyArgTest, ConstVolatileHandleRefusesWrite)
{
    counted value{1};
    ::scl::any_arg const volatile arg{value};

    EXPECT_EQ(::scl::any_cast<counted volatile>(&arg), nullptr); // volatile alone: still const-refused
    ASSERT_NE(::scl::any_cast<counted const volatile>(&arg), nullptr);
    EXPECT_EQ(::scl::any_cast<counted const volatile>(&arg)->id, 1);
}
