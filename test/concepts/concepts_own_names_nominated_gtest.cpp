#include <gtest_utils.h>

// This unit declares no namespace std inside scl, since such a declaration is found first and
// hides the nominated one.

// A namespace holding std, nominated inside the namespace scl, makes a name written as std::
// there ambiguous.
namespace own_names
{
    namespace std
    {}

    namespace scl
    {}

    namespace detail
    {}

    namespace concepts
    {}
} // namespace own_names

namespace scl
{
    using namespace ::own_names;
} // namespace scl

#include <scl/utility/concepts.h>

namespace nominating_caller
{
    enum class colour
    {
        red
    };

    using count = unsigned short;

    using nothing = void;

    using null = decltype(nullptr);

    constexpr int zero = 0;

    struct base
    {};

    using action = void(base &);

    struct derived : virtual base
    {};

    union variant
    {
        int integer;
    };

    struct abstract_class
    {
        virtual ~abstract_class() = default;

        virtual void act() = 0;
    };

    struct final_class final
    {};

    template <::scl::concepts::const_qualified Type>
    constexpr int qualifiers()
    {
        return 1;
    }

    template <::scl::concepts::cv_qualified Type>
    constexpr int qualifiers()
    {
        return 2;
    }
} // namespace nominating_caller

TEST(ConceptsOwnNamesNominatedInScl, ConstQualifiedAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::const_qualified<nominating_caller::base const>);
    STATIC_EXPECT_FALSE(::scl::concepts::const_qualified<nominating_caller::base>);
}

TEST(ConceptsOwnNamesNominatedInScl, VolatileQualifiedAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::volatile_qualified<nominating_caller::base volatile>);
    STATIC_EXPECT_FALSE(::scl::concepts::volatile_qualified<nominating_caller::base>);
}

TEST(ConceptsOwnNamesNominatedInScl, CvQualifiedAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::cv_qualified<nominating_caller::base const volatile>);
    STATIC_EXPECT_FALSE(::scl::concepts::cv_qualified<nominating_caller::base const>);
}

TEST(ConceptsOwnNamesNominatedInScl, CvQualifiedSubsumesConstQualifiedBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_EQ(nominating_caller::qualifiers<nominating_caller::base const volatile>(), 2);
}

TEST(ConceptsOwnNamesNominatedInScl, ReferenceAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::reference<nominating_caller::base &>);
    STATIC_EXPECT_FALSE(::scl::concepts::reference<nominating_caller::base>);
}

TEST(ConceptsOwnNamesNominatedInScl, LvalueReferenceAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::lvalue_reference<nominating_caller::base &>);
    STATIC_EXPECT_FALSE(::scl::concepts::lvalue_reference<nominating_caller::base &&>);
}

TEST(ConceptsOwnNamesNominatedInScl, RvalueReferenceAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::rvalue_reference<nominating_caller::base &&>);
    STATIC_EXPECT_FALSE(::scl::concepts::rvalue_reference<nominating_caller::base &>);
}

TEST(ConceptsOwnNamesNominatedInScl, NonReferenceAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::non_reference<nominating_caller::base>);
    STATIC_EXPECT_FALSE(::scl::concepts::non_reference<nominating_caller::base &>);
}

TEST(ConceptsOwnNamesNominatedInScl, ObjectAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::object<nominating_caller::base>);
    STATIC_EXPECT_FALSE(::scl::concepts::object<nominating_caller::nothing>);
}

TEST(ConceptsOwnNamesNominatedInScl, CompoundAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::compound<nominating_caller::base *>);
    STATIC_EXPECT_FALSE(::scl::concepts::compound<nominating_caller::count>);
}

TEST(ConceptsOwnNamesNominatedInScl, FundamentalAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::fundamental<nominating_caller::nothing>);
    STATIC_EXPECT_FALSE(::scl::concepts::fundamental<nominating_caller::base *>);
}

TEST(ConceptsOwnNamesNominatedInScl, ScalarAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::scalar<nominating_caller::colour>);
    STATIC_EXPECT_FALSE(::scl::concepts::scalar<nominating_caller::base>);
}

TEST(ConceptsOwnNamesNominatedInScl, ArithmeticAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::arithmetic<nominating_caller::count>);
    STATIC_EXPECT_FALSE(::scl::concepts::arithmetic<nominating_caller::colour>);
}

TEST(ConceptsOwnNamesNominatedInScl, VoidTypeAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::void_type<nominating_caller::nothing>);
    STATIC_EXPECT_FALSE(::scl::concepts::void_type<nominating_caller::count>);
}

TEST(ConceptsOwnNamesNominatedInScl, NullPointerAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::null_pointer<nominating_caller::null>);
    STATIC_EXPECT_FALSE(::scl::concepts::null_pointer<nominating_caller::base *>);
}

TEST(ConceptsOwnNamesNominatedInScl, EnumTypeAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::enum_type<nominating_caller::colour>);
    STATIC_EXPECT_FALSE(::scl::concepts::enum_type<nominating_caller::count>);
}

TEST(ConceptsOwnNamesNominatedInScl, EnumValueAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::enum_value<nominating_caller::colour::red>);
    STATIC_EXPECT_FALSE(::scl::concepts::enum_value<nominating_caller::zero>);
}

TEST(ConceptsOwnNamesNominatedInScl, ScopedEnumAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::scoped_enum<nominating_caller::colour>);
    STATIC_EXPECT_FALSE(::scl::concepts::scoped_enum<nominating_caller::count>);
}

TEST(ConceptsOwnNamesNominatedInScl, PointerAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::pointer<nominating_caller::base *>);
    STATIC_EXPECT_FALSE(::scl::concepts::pointer<nominating_caller::null>);
}

TEST(ConceptsOwnNamesNominatedInScl, MemberPointerAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::member_pointer<int nominating_caller::variant::*>);
    STATIC_EXPECT_FALSE(::scl::concepts::member_pointer<nominating_caller::base *>);
}

TEST(ConceptsOwnNamesNominatedInScl, MemberObjectPointerAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::member_object_pointer<int nominating_caller::variant::*>);
    STATIC_EXPECT_FALSE(::scl::concepts::member_object_pointer<void (nominating_caller::base::*)()>);
}

TEST(ConceptsOwnNamesNominatedInScl, MemberFunctionPointerAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::member_function_pointer<void (nominating_caller::base::*)()>);
    STATIC_EXPECT_FALSE(::scl::concepts::member_function_pointer<int nominating_caller::variant::*>);
}

TEST(ConceptsOwnNamesNominatedInScl, ArrayAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::array<nominating_caller::base[]>);
    STATIC_EXPECT_FALSE(::scl::concepts::array<nominating_caller::base *>);
}

TEST(ConceptsOwnNamesNominatedInScl, BoundedArrayAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::bounded_array<nominating_caller::base[2]>);
    STATIC_EXPECT_FALSE(::scl::concepts::bounded_array<nominating_caller::base[]>);
}

TEST(ConceptsOwnNamesNominatedInScl, UnboundedArrayAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::unbounded_array<nominating_caller::base[]>);
    STATIC_EXPECT_FALSE(::scl::concepts::unbounded_array<nominating_caller::base[2]>);
}

TEST(ConceptsOwnNamesNominatedInScl, FunctionAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::function<nominating_caller::action>);
    STATIC_EXPECT_FALSE(::scl::concepts::function<nominating_caller::action *>);
}

TEST(ConceptsOwnNamesNominatedInScl, ClassTypeAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::class_type<nominating_caller::base>);
    STATIC_EXPECT_FALSE(::scl::concepts::class_type<nominating_caller::variant>);
}

TEST(ConceptsOwnNamesNominatedInScl, UnionTypeAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::union_type<nominating_caller::variant>);
    STATIC_EXPECT_FALSE(::scl::concepts::union_type<nominating_caller::base>);
}

TEST(ConceptsOwnNamesNominatedInScl, TriviallyCopyableAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::trivially_copyable<nominating_caller::base>);
    STATIC_EXPECT_FALSE(::scl::concepts::trivially_copyable<nominating_caller::derived>);
}

TEST(ConceptsOwnNamesNominatedInScl, StandardLayoutAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::standard_layout<nominating_caller::base>);
    STATIC_EXPECT_FALSE(::scl::concepts::standard_layout<nominating_caller::derived>);
}

TEST(ConceptsOwnNamesNominatedInScl, EmptyAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::empty<nominating_caller::base>);
    STATIC_EXPECT_FALSE(::scl::concepts::empty<nominating_caller::variant>);
}

TEST(ConceptsOwnNamesNominatedInScl, PolymorphicAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::polymorphic<nominating_caller::abstract_class>);
    STATIC_EXPECT_FALSE(::scl::concepts::polymorphic<nominating_caller::base>);
}

TEST(ConceptsOwnNamesNominatedInScl, AbstractAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::abstract<nominating_caller::abstract_class>);
    STATIC_EXPECT_FALSE(::scl::concepts::abstract<nominating_caller::derived>);
}

TEST(ConceptsOwnNamesNominatedInScl, FinalAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::final<nominating_caller::final_class>);
    STATIC_EXPECT_FALSE(::scl::concepts::final<nominating_caller::base>);
}

TEST(ConceptsOwnNamesNominatedInScl, AggregateAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::aggregate<nominating_caller::base>);
    STATIC_EXPECT_FALSE(::scl::concepts::aggregate<nominating_caller::derived>);
}

TEST(ConceptsOwnNamesNominatedInScl, BaseOfAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE((::scl::concepts::base_of<nominating_caller::base, nominating_caller::derived>));
    STATIC_EXPECT_FALSE((::scl::concepts::base_of<nominating_caller::derived, nominating_caller::base>));
}

#if SCL_HAS_IMPLICIT_LIFETIME_CONCEPT
TEST(ConceptsOwnNamesNominatedInScl, ImplicitLifetimeAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::implicit_lifetime<nominating_caller::base>);
    STATIC_EXPECT_FALSE(::scl::concepts::implicit_lifetime<nominating_caller::nothing>);
}
#endif

#if SCL_HAS_VIRTUAL_BASE_OF_CONCEPT
TEST(ConceptsOwnNamesNominatedInScl, VirtualBaseOfAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(
        (::scl::concepts::virtual_base_of<nominating_caller::base, nominating_caller::derived>));
    STATIC_EXPECT_FALSE(
        (::scl::concepts::virtual_base_of<nominating_caller::base, nominating_caller::base>));
}
#endif
