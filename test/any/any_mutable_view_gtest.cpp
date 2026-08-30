#include <gtest_utils.h>

#include <scl/utility/any.h>
#include <scl/utility/preprocessor/rtti.h>

#if SCL_HAS_RTTI
#include <any>
#endif
#include <string>
#include <tuple>
#include <type_traits>

namespace
{
    template <typename Type>
    concept view_from_lvalue = requires(Type & object) { ::scl::any_mutable_view{object}; };

    template <typename Type>
    concept view_from_const_lvalue =
        requires(Type const & object) { ::scl::any_mutable_view{object}; };

    template <typename Type>
    concept view_from_rvalue = requires { ::scl::any_mutable_view{Type{}}; };

    template <typename Type>
    concept view_ptr_castable =
        requires(::scl::any_mutable_view const * view) { ::scl::any_cast<Type>(view); };

    void write_through(::scl::any_arg argument) noexcept
    {
        if (auto * const reached = ::scl::any_cast<int>(&argument))
            *reached = 11;
    }

    // A storable view carries no anchor of its own, so the caller declares one.
    constexpr int incremented_through_anchor() noexcept
    {
        int value = 41;
        ::scl::any_anchor const anchor{value};
        ::scl::any_mutable_view const view{anchor};

        if (auto * const reached = ::scl::any_cast<int>(&view))
            ++*reached;
        return value;
    }

    constexpr int incremented_through_anchor_by_reference()
    {
        int value = 41;
        ::scl::any_anchor const anchor{value};
        ::scl::any_mutable_view const view{anchor};

        ++::scl::any_cast<int &>(view);
        return value;
    }

    constexpr int incremented_in_owner()
    {
        ::scl::any owner{41};
        ::scl::any_mutable_view const view{owner};

        if (auto * const reached = ::scl::any_cast<int>(&view))
            ++*reached;
        return *::scl::any_cast<int const>(&owner);
    }

    // No anchor: before P2738 the recovery from `void const *` is not a constant expression.
    constexpr int incremented_through_plain_binding() noexcept
    {
        int value = 41;
        ::scl::any_mutable_view const view{value};

        if (auto * const reached = ::scl::any_cast<int>(&view))
            ++*reached;
        return value;
    }

    constexpr bool anchored_view_refuses_a_mismatch() noexcept
    {
        int value = 41;
        ::scl::any_anchor const anchor{value};
        ::scl::any_mutable_view const view{anchor};

        return ::scl::any_cast<double>(&view) == nullptr;
    }
} // namespace

TEST(AnyMutableViewTest, LayoutMatchesAReadOnlyView)
{
    STATIC_EXPECT_TRUE(sizeof(::scl::any_mutable_view) == 2 * sizeof(void *));
    STATIC_EXPECT_TRUE(::std::is_trivially_copyable_v<::scl::any_mutable_view>);
}

TEST(AnyMutableViewTest, BindsANonConstLvalue)
{
    STATIC_EXPECT_TRUE(view_from_lvalue<int>);

    int number = 7;
    ::scl::any_mutable_view const view{number};

    EXPECT_TRUE(view.has_value());
    EXPECT_EQ(view.type_key(), ::scl::type_key_of<int>());
    EXPECT_EQ(view.type_name(), ::scl::type_name<int>());
}

TEST(AnyMutableViewTest, RefusesAConstLvalueAndATemporary)
{
    STATIC_EXPECT_FALSE(view_from_const_lvalue<int>);
    STATIC_EXPECT_FALSE(view_from_rvalue<int>);
    STATIC_EXPECT_FALSE(view_from_lvalue<int const volatile>);

    // A temporary of every shape the type has a constructor for, an anchor included: an
    // rvalue also binds a const lvalue reference, so the anchor constructor would take one.
    STATIC_EXPECT_FALSE(view_from_rvalue<::scl::any_anchor<int>>);
    STATIC_EXPECT_FALSE(view_from_rvalue<::scl::any>);
    STATIC_EXPECT_FALSE((::std::is_constructible_v<::scl::any_mutable_view, ::scl::any_anchor<int const> &>));
#if SCL_HAS_RTTI
    STATIC_EXPECT_FALSE(view_from_rvalue<::std::any>);
#endif
}

TEST(AnyMutableViewTest, PointerCastWritesThroughToTheCallersObject)
{
    int number = 7;
    ::scl::any_mutable_view const view{number};

    auto * const reached = ::scl::any_cast<int>(&view);

    ASSERT_NE(reached, nullptr);
    *reached = 11;
    EXPECT_EQ(number, 11);
}

TEST(AnyMutableViewTest, PointerCastTakesTheRequestAsWritten)
{
    int number = 7;
    ::scl::any_mutable_view const view{number};

    STATIC_EXPECT_TRUE(view_ptr_castable<int>);
    STATIC_EXPECT_TRUE(view_ptr_castable<int const>);

    EXPECT_NE(::scl::any_cast<int const>(&view), nullptr);
    EXPECT_EQ(::scl::any_cast<long>(&view), nullptr);
}

TEST(AnyMutableViewTest, ConvertsTowardsTheOtherHandlesAndNotBack)
{
    STATIC_EXPECT_TRUE((::std::is_convertible_v<::scl::any_mutable_view, ::scl::any_view>));
    STATIC_EXPECT_TRUE((::std::is_convertible_v<::scl::any_mutable_view, ::scl::any_arg>));
    STATIC_EXPECT_FALSE((::std::is_convertible_v<::scl::any_view, ::scl::any_mutable_view>));
    STATIC_EXPECT_FALSE((::std::is_convertible_v<::scl::any_argument, ::scl::any_mutable_view>));

    // is_convertible_v alone would let an explicit constructor added later slip through.
    STATIC_EXPECT_FALSE((::std::is_constructible_v<::scl::any_mutable_view, ::scl::any_view &>));
    STATIC_EXPECT_FALSE((::std::is_constructible_v<::scl::any_mutable_view, ::scl::any_argument &>));
}

TEST(AnyMutableViewTest, ConvertedViewReadsTheSameObject)
{
    int number = 7;
    ::scl::any_mutable_view const view{number};

    ::scl::any_view const reading = view;

    ASSERT_NE(::scl::any_cast<int>(&reading), nullptr);
    EXPECT_EQ(*::scl::any_cast<int>(&reading), 7);
}

TEST(AnyMutableViewTest, ConvertedArgumentKeepsWriteAccess)
{
    int number = 7;
    ::scl::any_mutable_view const view{number};

    write_through(view);

    EXPECT_EQ(number, 11);
}

TEST(AnyMutableViewTest, ConstViewStillWrites)
{
    int number = 7;
    ::scl::any_mutable_view const view{number};

    *::scl::any_cast<int>(&view) = 11;

    EXPECT_EQ(number, 11);
}

TEST(AnyMutableViewTest, VolatileViewLeavesTheRequestAlone)
{
    // A qualifier on the view governs the view, not the object it refers to.
    int number = 7;
    ::scl::any_mutable_view volatile view{number};

    ASSERT_NE(::scl::any_cast<int>(&view), nullptr);
    *::scl::any_cast<int>(&view) = 11;

    EXPECT_EQ(number, 11);
}

TEST(AnyMutableViewTest, VolatileReferentRequiresAVolatileRequest)
{
    int volatile number = 7;
    ::scl::any_mutable_view const view{number};

    EXPECT_EQ(::scl::any_cast<int>(&view), nullptr);
    ASSERT_NE(::scl::any_cast<int volatile>(&view), nullptr);

    *::scl::any_cast<int volatile>(&view) = 11;
    EXPECT_EQ(number, 11);
}

TEST(AnyMutableViewTest, ReferenceFormsReachAVolatileReferent)
{
    int volatile number = 7;
    ::scl::any_mutable_view const view{number};

    ::scl::any_cast<int volatile &>(view) = 11;
    EXPECT_EQ(number, 11);

    int const read = ::scl::any_cast<int const volatile &>(view);
    EXPECT_EQ(read, 11);

    EXPECT_THROW(::std::ignore = ::scl::any_cast<int &>(view), ::scl::bad_any_cast);
}

TEST(AnyMutableViewTest, EmptyViewReachesNothing)
{
    ::scl::any_mutable_view const view{};

    EXPECT_FALSE(view.has_value());
    EXPECT_EQ(view.type_key(), ::scl::type_key{});
    EXPECT_EQ(::scl::any_cast<int>(&view), nullptr);
}

TEST(AnyMutableViewTest, AnOwningAnyIsWrittenThrough)
{
    ::scl::any owner{7};
    ::scl::any_mutable_view const view{owner};

    ASSERT_NE(::scl::any_cast<int>(&view), nullptr);
    *::scl::any_cast<int>(&view) = 11;

    EXPECT_EQ(*::scl::any_cast<int>(&owner), 11);
}

TEST(AnyMutableViewTest, RefusesAConstOwningAny)
{
    STATIC_EXPECT_FALSE(view_from_const_lvalue<::scl::any>);
}

#if SCL_HAS_RTTI
TEST(AnyMutableViewTest, StdAnyWritesIntoTheBoxedObject)
{
    ::std::any boxed{7};
    ::scl::any_mutable_view const view{boxed};

    EXPECT_EQ(::scl::any_cast<int>(&view), nullptr);

    // The handle hands out the box; the box is what grants access to the object inside.
    auto * box = ::scl::any_cast<::std::any>(&view);
    ASSERT_NE(box, nullptr);
    ::std::any_cast<int &>(*box) = 11;

    EXPECT_EQ(::std::any_cast<int>(boxed), 11);
}

TEST(AnyMutableViewTest, StdAnyHandsOutTheBoxForWriting)
{
    ::std::any boxed{1};
    ::scl::any_mutable_view const view{boxed};

    auto * const box = ::scl::any_cast<::std::any>(&view);
    ASSERT_NE(box, nullptr);
    *box = ::std::string{"replaced"};

    EXPECT_EQ(::std::any_cast<::std::string const &>(boxed), "replaced");
}

TEST(AnyMutableViewTest, RefusesAConstStdAny)
{
    STATIC_EXPECT_FALSE(view_from_const_lvalue<::std::any>);
}
#endif

TEST(AnyMutableViewTest, ReferenceFormWritesThrough)
{
    int number = 7;
    ::scl::any_mutable_view const view{number};

    ::scl::any_cast<int &>(view) = 11;

    EXPECT_EQ(number, 11);
}

TEST(AnyMutableViewTest, ReadingFormsCopyOutAndBind)
{
    int number = 7;
    ::scl::any_mutable_view const view{number};

    EXPECT_EQ(::scl::any_cast<int>(view), 7);
    EXPECT_EQ(&::scl::any_cast<int const &>(view), &number);
}

TEST(AnyMutableViewTest, MismatchThrows)
{
    int number = 7;
    ::scl::any_mutable_view const view{number};

    EXPECT_THROW(::std::ignore = ::scl::any_cast<double>(view), ::scl::bad_any_cast);
    EXPECT_THROW(::std::ignore = ::scl::any_cast<double &>(view), ::scl::bad_any_cast);
}

TEST(AnyMutableViewTest, ConstexprCastThroughAnAnchor)
{
    STATIC_EXPECT_TRUE(incremented_through_anchor() == 42);
    STATIC_EXPECT_TRUE(incremented_through_anchor_by_reference() == 42);
    STATIC_EXPECT_TRUE(anchored_view_refuses_a_mismatch());
}

TEST(AnyMutableViewTest, ConstexprCastOverAnOwningAny)
{
    STATIC_EXPECT_TRUE(incremented_in_owner() == 42);
}

TEST(AnyMutableViewTest, ConstexprCastReachesAPlainBindingOnceTheVoidCastFolds)
{
#if SCL_DETAIL_ANY_HAS_CONSTEXPR_VOID_CAST
    STATIC_EXPECT_TRUE(incremented_through_plain_binding() == 42);
#else
    GTEST_SKIP() << "recovering a typed pointer from void const * needs P2738 (C++26)";
#endif
}
