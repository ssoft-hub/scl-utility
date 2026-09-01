#include <gtest_utils.h>

#include <scl/utility/any.h>
#include <scl/utility/preprocessor/exceptions.h>
#include <scl/utility/preprocessor/rtti.h>

#if SCL_HAS_RTTI
#include <any>
#endif
#include <concepts>
#include <memory>
#include <string>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <vector>

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
    concept mutable_ref_castable = requires(::scl::any_view view) { ::scl::any_cast<Type &>(view); };

    template <typename Type>
    concept view_castable = requires(::scl::any_view view) { ::scl::any_cast<Type>(view); };

    template <typename Type>
    concept view_ptr_castable =
        requires(::scl::any_view const * view) { ::scl::any_cast<Type>(view); };

    template <typename Type>
    concept view_from_lvalue = requires(Type & object) { ::scl::any_view{object}; };

    template <typename Type>
    concept view_from_rvalue = requires { ::scl::any_view{Type{}}; };

    void bound_to_nothing();

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

    struct printer : printable
    {
        int printed() const override { return 42; }
    };

    template <typename Type>
    concept view_over = requires(Type & object) { ::scl::any_view{object}; };

    template <typename Type>
    concept writing_view_binds = requires(Type & object) { ::scl::any_mutable_view{object}; };

    template <typename Type>
    concept argument_over = requires(Type & object) { ::scl::any_arg{object}; };

    [[nodiscard]]
    ::scl::any_mutable_view writing_view_over(int & value)
    {
        return ::scl::any_mutable_view{value};
    }
} // namespace

TEST(AnyViewTest, CompileTimeGuards)
{
#if SCL_HAS_EXCEPTIONS
    STATIC_EXPECT_FALSE(mutable_ref_castable<int>);
    STATIC_EXPECT_TRUE(mutable_ref_castable<int const>);
#endif

    // A view never hands out an rvalue reference: nothing may look movable-from.
    STATIC_EXPECT_FALSE(view_castable<int &&>);
    STATIC_EXPECT_FALSE(view_castable<int const &&>);
    STATIC_EXPECT_FALSE(view_castable<void>);
    STATIC_EXPECT_FALSE(view_ptr_castable<void>);
    STATIC_EXPECT_FALSE(view_ptr_castable<int()>);

    // A view over a temporary would dangle; lvalues only.
    STATIC_EXPECT_TRUE(view_from_lvalue<counted>);
    STATIC_EXPECT_FALSE(view_from_rvalue<counted>);
    STATIC_EXPECT_FALSE(view_from_rvalue<int>);
    STATIC_EXPECT_FALSE(view_from_rvalue<counted const>);
    STATIC_EXPECT_FALSE(view_from_rvalue<int const>);
    STATIC_EXPECT_FALSE(view_from_rvalue<counted volatile>);

#if SCL_HAS_RTTI
    // std::any is an ordinary type to a view, bound and refused as any other object is.
    STATIC_EXPECT_FALSE(view_from_rvalue<::std::any>);
    STATIC_EXPECT_FALSE(view_from_rvalue<::std::any const>);
    STATIC_EXPECT_TRUE(view_from_lvalue<::std::any>);
    STATIC_EXPECT_TRUE(view_from_lvalue<::std::any const>);
    STATIC_EXPECT_TRUE(view_from_lvalue<::std::any volatile>);
#endif

    // Copying a view — even a const temporary one — is not affected by the guard.
    STATIC_EXPECT_TRUE(view_from_rvalue<::scl::any_view>);
    STATIC_EXPECT_TRUE(view_from_rvalue<::scl::any_view const>);

    // An argument may not be stored, so a view is never made out of one.
    STATIC_EXPECT_FALSE(view_from_lvalue<::scl::any_argument>);

    // Two pointers wide and trivially copyable, as documented.
    STATIC_EXPECT_TRUE(sizeof(::scl::any_view) == 2 * sizeof(void *));
    STATIC_EXPECT_TRUE(::std::is_trivially_copyable_v<::scl::any_view>);
}

TEST(AnyViewTest, ConstexprIdentityOverATypedLvalue)
{
    static constexpr int probe = 42;
    constexpr ::scl::any_view view{probe};

    STATIC_EXPECT_TRUE(view.has_value());
    STATIC_EXPECT_FALSE(::scl::any_view{}.has_value());
    STATIC_EXPECT_TRUE(view.type_name() == ::scl::type_name<int>());
    STATIC_EXPECT_TRUE(view.type_key() == ::scl::type_key_of<int>());
    STATIC_EXPECT_TRUE(::scl::any_view{}.type_key() == ::scl::type_key{});
}

TEST(AnyViewTest, QualifiersDoNotAffectIdentity)
{
    counted mutable_value{1};
    counted const const_value{2};
    counted volatile volatile_value{3};

    ::scl::any_view const over_mutable{mutable_value};
    ::scl::any_view const over_const{const_value};
    ::scl::any_view const over_volatile{volatile_value};

    EXPECT_EQ(over_mutable.type_name(), ::scl::type_name<counted>());
    EXPECT_EQ(over_const.type_name(), over_mutable.type_name());
    EXPECT_EQ(over_volatile.type_name(), over_mutable.type_name());

    EXPECT_TRUE(over_const.type_key() == over_mutable.type_key());
    EXPECT_TRUE(over_volatile.type_key() == over_mutable.type_key());

    ASSERT_NE(::scl::any_cast<counted>(&over_mutable), nullptr);
    ASSERT_NE(::scl::any_cast<counted>(&over_const), nullptr);
    EXPECT_EQ(::scl::any_cast<counted>(&over_const)->id, 2);
}

// A view only ever reads, so the request may name any qualification but the result always
// gains const; what the request decides is whether the referent is reachable at all.
TEST(AnyViewTest, PointerCastAlwaysYieldsConst)
{
    counted value{1};
    ::scl::any_view const view{value};

    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(::scl::any_cast<counted>(&view)), counted const *>));
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(::scl::any_cast<counted const>(&view)), counted const *>));

    EXPECT_NE(::scl::any_cast<counted>(&view), nullptr);
    EXPECT_NE(::scl::any_cast<counted const>(&view), nullptr);
}

#if SCL_HAS_EXCEPTIONS
TEST(AnyViewTest, RequestMustCoverReferentQualifiers)
{
    counted volatile volatile_value{5};
    ::scl::any_view const over_volatile{volatile_value};

    EXPECT_EQ(::scl::any_cast<counted>(&over_volatile), nullptr);
    EXPECT_THROW(::std::ignore = ::scl::any_cast<counted const &>(over_volatile), ::scl::bad_any_cast);

    auto const * reached = ::scl::any_cast<counted volatile>(&over_volatile);
    ASSERT_NE(reached, nullptr);
    EXPECT_EQ(reached->id, 5);
    EXPECT_EQ(::scl::any_cast<counted const volatile &>(over_volatile).id, 5);

    // The request may add qualification, so a plain referent is reachable either way.
    counted plain_value{6};
    ::scl::any_view const over_plain{plain_value};
    EXPECT_NE(::scl::any_cast<counted>(&over_plain), nullptr);
    EXPECT_NE(::scl::any_cast<counted volatile>(&over_plain), nullptr);
}

TEST(AnyViewTest, CastOverATypedLvalue)
{
    counted::copies = 0;
    counted value{7};
    ::scl::any_view view{value};

    EXPECT_TRUE(view.has_value());
    EXPECT_EQ(view.type_name(), ::scl::type_name<counted>());

    ASSERT_NE(::scl::any_cast<counted>(&view), nullptr);
    EXPECT_EQ(::scl::any_cast<counted>(&view)->id, 7);
    EXPECT_EQ(::scl::any_cast<double>(&view), nullptr);

    EXPECT_EQ(&::scl::any_cast<counted const &>(view), &value); // reference: binds to the original
    EXPECT_EQ(counted::copies, 0);                              // no copy so far

    EXPECT_EQ(::scl::any_cast<counted>(view).id, 7); // value form: exactly one copy
    EXPECT_EQ(counted::copies, 1);
}

TEST(AnyViewTest, AMismatchOverATypedLvalueThrows)
{
    counted value{1};
    ::scl::any_view view{value};

    EXPECT_THROW(::std::ignore = ::scl::any_cast<double>(view), ::scl::bad_any_cast);
    EXPECT_THROW(::std::ignore = ::scl::any_cast<double>(view), ::std::bad_cast);
}

TEST(AnyViewTest, AnAbstractReferentIsBoundAndReachedByReference)
{
    // No holder can declare an abstract class as a member, and a handle needs none: it stands
    // for the object where it is, so the reference forms reach it and only a copy is refused.
    printer object;
    printable & referent = object;
    ::scl::any_view const view{referent};

    EXPECT_TRUE(view.has_value());
    EXPECT_EQ(view.type_key(), ::scl::type_key_of<printable>());
    EXPECT_EQ(::scl::any_cast<printable>(&view), &object);
    EXPECT_EQ(::scl::any_cast<printable const &>(view).printed(), 42);
    EXPECT_THROW(::std::ignore = ::scl::any_cast<int const &>(view), ::scl::bad_any_cast);
}
#endif

TEST(AnyViewTest, AHandleStandsForAnObjectAndForNothingElse)
{
    // A holder over a function declares a member function rather than a member, so a function
    // lvalue is turned away where the handle is written instead of inside the group.
    STATIC_EXPECT_FALSE(view_over<decltype(bound_to_nothing)>);
    STATIC_EXPECT_FALSE(writing_view_binds<decltype(bound_to_nothing)>);
    STATIC_EXPECT_FALSE(argument_over<decltype(bound_to_nothing)>);

    // A `volatile` container answers none of the members a handle reads.
    STATIC_EXPECT_FALSE(view_over<::scl::any volatile>);
    STATIC_EXPECT_FALSE(argument_over<::scl::any volatile>);
    STATIC_EXPECT_FALSE(writing_view_binds<::scl::any volatile>);

    STATIC_EXPECT_TRUE(view_over<int>);
    STATIC_EXPECT_TRUE(view_over<int const>);
    STATIC_EXPECT_TRUE(writing_view_binds<int>);
    STATIC_EXPECT_TRUE(argument_over<int>);
    STATIC_EXPECT_TRUE(view_over<::scl::any>);
    STATIC_EXPECT_TRUE(view_over<::scl::any const>);
}

TEST(AnyViewTest, AWritingViewNarrowsHoweverItIsSpelled)
{
    int value = 42;
    ::scl::any_mutable_view writing{value};

    // A temporary object is refused because the view would outlive it. A handle is not such
    // an object, so narrowing one to a reading view stands whichever initialisation is written.
    STATIC_EXPECT_TRUE((::std::is_constructible_v<::scl::any_view, ::scl::any_mutable_view>));
    STATIC_EXPECT_TRUE((::std::convertible_to<::scl::any_mutable_view, ::scl::any_view>));
    STATIC_EXPECT_FALSE((::std::is_constructible_v<::scl::any_view, ::std::string>));

    ::scl::any_view const copied = writing;
    ::scl::any_view const direct{writing_view_over(value)};
    ::std::vector<::scl::any_view> kept;
    kept.emplace_back(writing_view_over(value));

    ASSERT_TRUE(direct.has_value());
    EXPECT_EQ(::scl::any_cast<int>(&copied), ::std::addressof(value));
    EXPECT_EQ(::scl::any_cast<int>(&direct), ::std::addressof(value));
    EXPECT_EQ(::scl::any_cast<int>(&kept.front()), ::std::addressof(value));
}

#if SCL_HAS_EXCEPTIONS
TEST(AnyViewTest, EmptyView)
{
    ::scl::any_view view{};

    EXPECT_FALSE(view.has_value());
    EXPECT_TRUE(view.type_name().empty());
    EXPECT_EQ(::scl::any_cast<int>(&view), nullptr);
    EXPECT_EQ(::scl::any_cast<int>(static_cast<::scl::any_view const *>(nullptr)), nullptr);
    EXPECT_THROW(::std::ignore = ::scl::any_cast<int>(view), ::scl::bad_any_cast);
}
#endif

#if SCL_HAS_RTTI

TEST(AnyViewTest, StdAnyIsNamedRatherThanWhatItHolds)
{
    ::std::string text{"hello"};
    ::std::any boxed{text};
    ::scl::any_view view{boxed};

    EXPECT_TRUE(view.has_value());
    EXPECT_EQ(view.type_name(), ::scl::type_name<::std::any>()); // names the box, not what it holds
}

#if SCL_HAS_EXCEPTIONS
TEST(AnyViewTest, StdAnyIsAnsweredAsTheBoxItself)
{
    ::std::any boxed{42};
    ::scl::any_view const view{boxed};

    EXPECT_EQ(::scl::any_cast<::std::any const>(&view), &boxed);
    EXPECT_EQ(&::scl::any_cast<::std::any const &>(view), &boxed);
    EXPECT_EQ(::scl::any_cast<int const>(&view), nullptr); // and never what it holds
}

TEST(AnyViewTest, StdAnyDoesNotReachTheBoxedObject)
{
    ::std::any boxed{::std::string{"hello"}};
    ::scl::any_view const view{boxed};

    EXPECT_EQ(::scl::any_cast<::std::string>(&view), nullptr);
    EXPECT_THROW(::std::ignore = ::scl::any_cast<::std::string const &>(view), ::scl::bad_any_cast);
}

TEST(AnyViewTest, ATypedLvalueRefusesAStdAnyRequest)
{
    ::std::string text{"hello"};
    ::scl::any_view const view{text};

    EXPECT_EQ(::scl::any_cast<::std::any const>(&view), nullptr);
    EXPECT_THROW(::std::ignore = ::scl::any_cast<::std::any const &>(view), ::scl::bad_any_cast);
}
#endif

TEST(AnyViewTest, ViewOverEmptyStdAny)
{
    ::std::any empty_any{};
    ::scl::any_view view{empty_any};

    EXPECT_TRUE(view.has_value());
    EXPECT_EQ(view.type_name(), ::scl::type_name<::std::any>());
}

TEST(AnyViewTest, IdentityDistinguishesEveryState)
{
    counted value{3};
    ::std::any boxed{value};

    ::scl::any_view const raw{value};
    ::scl::any_view const over_any{boxed};
    ::scl::any_view const empty{};

    EXPECT_EQ(raw.type_name(), ::scl::type_name<counted>());
    EXPECT_EQ(over_any.type_name(), ::scl::type_name<::std::any>());
    EXPECT_TRUE(empty.type_name().empty());

    EXPECT_FALSE(raw.type_key() == over_any.type_key());
    EXPECT_FALSE(over_any.type_key() == empty.type_key());
    EXPECT_FALSE(raw.type_key() == empty.type_key());

    EXPECT_TRUE(raw.type_key() == ::scl::type_key_of<counted>());
    EXPECT_TRUE(over_any.type_key() == ::scl::type_key_of<::std::any>());
    EXPECT_TRUE(empty.type_key() == ::scl::type_key{});

    // The key a non-empty view hands out always names the type the view reports.
    EXPECT_EQ(raw.type_key().name(), raw.type_name());
    EXPECT_EQ(over_any.type_key().name(), over_any.type_name());
}

// A std::any referent is an ordinary object here, so the handle's own qualifiers govern it.
TEST(AnyViewTest, VolatileHandleLeavesTheRequestAloneOverStdAny)
{
    ::std::any boxed{7};
    ::scl::any_view volatile view{boxed};

    EXPECT_EQ(::scl::any_cast<::std::any const>(&view), &boxed);
    EXPECT_EQ(::scl::any_cast<int>(&view), nullptr);
}

#endif // SCL_HAS_RTTI

TEST(AnyViewTest, VolatileHandleLeavesTheRequestAlone)
{
    // The handle's own volatile governs the handle, not the object it refers to, so the
    // request answers to the referent's qualifiers alone - here, none.
    int value = 1;
    ::scl::any_view volatile view{value};

    auto const * reached = ::scl::any_cast<int>(&view);
    ASSERT_NE(reached, nullptr);
    EXPECT_EQ(*reached, 1);

    // A volatile referent still requires the request to carry the qualifier.
    int volatile sensor = 2;
    ::scl::any_view volatile over_sensor{sensor};

    EXPECT_EQ(::scl::any_cast<int>(&over_sensor), nullptr);
    ASSERT_NE(::scl::any_cast<int volatile>(&over_sensor), nullptr);

    // A volatile handle answers the identity queries as any other does.
    EXPECT_TRUE(view.has_value());
    EXPECT_TRUE(view.type_key() == ::scl::type_key_of<int>());
    EXPECT_EQ(view.type_name(), ::scl::type_name<int>());
}
