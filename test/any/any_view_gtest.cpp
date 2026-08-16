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
} // namespace

TEST(AnyViewTest, CompileTimeGuards)
{
    STATIC_EXPECT_FALSE(mutable_ref_castable<int>);
    STATIC_EXPECT_TRUE(mutable_ref_castable<int const>);

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
    STATIC_EXPECT_FALSE(view_from_rvalue<::std::any>);
    STATIC_EXPECT_FALSE(view_from_rvalue<::std::any const>);
    STATIC_EXPECT_FALSE(view_from_rvalue<counted volatile>);

#if SCL_HAS_RTTI
    // std::any has no volatile-qualified members, so a volatile std::any cannot
    // be viewed at all — not even as an lvalue. Without RTTI the library cannot
    // name std::any to exclude it (see the @warning on the std::any constructor).
    STATIC_EXPECT_FALSE(view_from_lvalue<::std::any volatile>);
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

TEST(AnyViewTest, ConstexprIdentityOnRawBacking)
{
    static constexpr int probe = 42;
    constexpr ::scl::any_view view{probe};

    STATIC_EXPECT_TRUE(view.has_value());
    STATIC_EXPECT_FALSE(::scl::any_view{}.has_value());
    STATIC_EXPECT_TRUE(view.type_name() == ::scl::type_name<int>());
    STATIC_EXPECT_TRUE(*view.type_key() == ::scl::type_key_of<int>());
    STATIC_EXPECT_TRUE(::scl::any_view{}.type_key() == nullptr);
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

    EXPECT_EQ(over_const.type_key(), over_mutable.type_key());
    EXPECT_EQ(over_volatile.type_key(), over_mutable.type_key());

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

TEST(AnyViewTest, RequestMustCoverReferentQualifiers)
{
    counted volatile volatile_value{5};
    ::scl::any_view const over_volatile{volatile_value};

    EXPECT_EQ(::scl::any_cast<counted>(&over_volatile), nullptr);
    EXPECT_THROW((void)::scl::any_cast<counted const &>(over_volatile), ::scl::bad_any_cast);

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

TEST(AnyViewTest, RawBackingCast)
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

TEST(AnyViewTest, RawBackingMismatchThrows)
{
    counted value{1};
    ::scl::any_view view{value};

    EXPECT_THROW((void)::scl::any_cast<double>(view), ::scl::bad_any_cast);
    EXPECT_THROW((void)::scl::any_cast<double>(view), ::std::bad_cast);
}

TEST(AnyViewTest, EmptyView)
{
    ::scl::any_view view{};

    EXPECT_FALSE(view.has_value());
    EXPECT_TRUE(view.type_name().empty());
    EXPECT_EQ(::scl::any_cast<int>(&view), nullptr);
    EXPECT_EQ(::scl::any_cast<int>(static_cast<::scl::any_view const *>(nullptr)), nullptr);
    EXPECT_THROW((void)::scl::any_cast<int>(view), ::scl::bad_any_cast);
}

#if SCL_HAS_RTTI

TEST(AnyViewTest, StdAnyBackingCast)
{
    ::std::string text{"hello"};
    ::std::any boxed{text};
    ::scl::any_view view{boxed};

    EXPECT_TRUE(view.has_value());
    EXPECT_EQ(view.type_name(), ::scl::type_name<::std::any>()); // names the backing, not the boxed type

    ASSERT_NE(::scl::any_cast<::std::string>(&view), nullptr);
    EXPECT_EQ(*::scl::any_cast<::std::string>(&view), "hello");
    EXPECT_EQ(::scl::any_cast<int>(&view), nullptr);

    EXPECT_EQ(::scl::any_cast<::std::string const &>(view), "hello"); // reference through a named view
    EXPECT_EQ(::scl::any_cast<::std::string>(boxed), "hello"); // implicit std::any -> any_view

    EXPECT_THROW((void)::scl::any_cast<int>(view), ::scl::bad_any_cast);
}

TEST(AnyViewTest, StdAnyBackingAnswersTheBoxItself)
{
    ::std::any boxed{42};
    ::scl::any_view const view{boxed};

    EXPECT_EQ(::scl::any_cast<::std::any const>(&view), &boxed);
    EXPECT_EQ(&::scl::any_cast<::std::any const &>(view), &boxed);
    EXPECT_EQ(::scl::any_cast<int const>(&view), ::std::any_cast<int>(&boxed)); // and still what it holds
}

TEST(AnyViewTest, RawBackingRefusesAStdAnyRequest)
{
    ::std::string text{"hello"};
    ::scl::any_view const view{text};

    EXPECT_EQ(::scl::any_cast<::std::any const>(&view), nullptr);
    EXPECT_THROW((void)::scl::any_cast<::std::any const &>(view), ::scl::bad_any_cast);
}

TEST(AnyViewTest, StdAnyBackingDoesNotCopyInner)
{
    ::std::any boxed{counted{5}};
    counted::copies = 0; // ignore copies incurred while boxing

    ::scl::any_view view{boxed};
    EXPECT_EQ(counted::copies, 0);

    (void)::scl::any_cast<counted const &>(view);
    EXPECT_EQ(counted::copies, 0);
    EXPECT_EQ(::scl::any_cast<counted const &>(view).id, 5);
}

TEST(AnyViewTest, ViewOverEmptyStdAny)
{
    ::std::any empty_any{};
    ::scl::any_view view{empty_any};

    EXPECT_FALSE(view.has_value());
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

    EXPECT_NE(raw.type_key(), over_any.type_key());
    EXPECT_NE(over_any.type_key(), empty.type_key());
    EXPECT_NE(raw.type_key(), empty.type_key());

    EXPECT_EQ(raw.type_key(), &::scl::type_key_of<counted>());
    EXPECT_EQ(over_any.type_key(), &::scl::type_key_of<::std::any>());
    EXPECT_EQ(empty.type_key(), nullptr);

    // The key a non-empty view hands out always names the type the view reports.
    EXPECT_EQ(raw.type_key()->name(), raw.type_name());
    EXPECT_EQ(over_any.type_key()->name(), over_any.type_name());
}

// The std::any backing delegates to std::any_cast, but it reaches that delegation through
// the same handle the raw backing does, so the handle's own qualifiers gate it identically.
TEST(AnyViewTest, VolatileHandleRequiresVolatileRequestOverStdAny)
{
    ::std::any boxed{7};
    ::scl::any_view volatile view{boxed};

    EXPECT_EQ(::scl::any_cast<int>(&view), nullptr);
    auto const * reached = ::scl::any_cast<int volatile>(&view);
    ASSERT_NE(reached, nullptr);
    EXPECT_EQ(*reached, 7);
}

#endif // SCL_HAS_RTTI

TEST(AnyViewTest, VolatileHandleRequiresVolatileRequest)
{
    // The handle's own volatile is a qualifier the request must cover too, on top of
    // whatever the referent itself was bound with — here, nothing.
    int value = 1;
    ::scl::any_view volatile view{value};

    EXPECT_EQ(::scl::any_cast<int>(&view), nullptr);
    auto const * p = ::scl::any_cast<int volatile>(&view);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(*p, 1);
}
