#include <gtest_utils.h>

// Each namespace declared here before the group is included takes the name of a namespace the
// headers use, so a name the headers write below it without the leading :: finds this namespace
// and breaks the build of this unit: std:: and scl:: anywhere under scl, and detail:: and
// concepts:: inside scl::detail and scl::concepts.
namespace scl
{
    namespace std
    {}

    namespace scl
    {}
} // namespace scl

namespace scl::detail
{
    namespace std
    {}

    namespace scl
    {}

    namespace detail
    {}

    namespace concepts
    {}
} // namespace scl::detail

namespace scl::concepts
{
    namespace std
    {}

    namespace scl
    {}

    namespace detail
    {}

    namespace concepts
    {}
} // namespace scl::concepts

// A namespace named scl, nominated at global scope, makes a name written as scl:: outside the
// namespace scl ambiguous.
namespace own_names
{
    namespace scl
    {}

    namespace detail
    {}

    // A concept here, always false, answers for a name written as concepts:: at global scope.
    namespace concepts
    {
        template <typename Type>
        concept enum_type = false;

        template <typename Type>
        concept class_type = false;

        template <typename Type>
        concept object = false;

        template <typename Type>
        concept compound = false;
    } // namespace concepts
} // namespace own_names

using namespace own_names;

#include <scl/utility/concepts.h>

namespace own_caller
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
} // namespace own_caller

TEST(ConceptsOwnNames, ConstQualifiedAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::const_qualified<own_caller::base const>);
    STATIC_EXPECT_FALSE(::scl::concepts::const_qualified<own_caller::base>);
}

TEST(ConceptsOwnNames, VolatileQualifiedAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::volatile_qualified<own_caller::base volatile>);
    STATIC_EXPECT_FALSE(::scl::concepts::volatile_qualified<own_caller::base>);
}

TEST(ConceptsOwnNames, CvQualifiedAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::cv_qualified<own_caller::base const volatile>);
    STATIC_EXPECT_FALSE(::scl::concepts::cv_qualified<own_caller::base const>);
}

TEST(ConceptsOwnNames, CvQualifiedSubsumesConstQualifiedBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_EQ(own_caller::qualifiers<own_caller::base const volatile>(), 2);
}

TEST(ConceptsOwnNames, ReferenceAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::reference<own_caller::base &>);
    STATIC_EXPECT_FALSE(::scl::concepts::reference<own_caller::base>);
}

TEST(ConceptsOwnNames, LvalueReferenceAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::lvalue_reference<own_caller::base &>);
    STATIC_EXPECT_FALSE(::scl::concepts::lvalue_reference<own_caller::base &&>);
}

TEST(ConceptsOwnNames, RvalueReferenceAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::rvalue_reference<own_caller::base &&>);
    STATIC_EXPECT_FALSE(::scl::concepts::rvalue_reference<own_caller::base &>);
}

TEST(ConceptsOwnNames, NonReferenceAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::non_reference<own_caller::base>);
    STATIC_EXPECT_FALSE(::scl::concepts::non_reference<own_caller::base &>);
}

TEST(ConceptsOwnNames, ObjectAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::object<own_caller::base>);
    STATIC_EXPECT_FALSE(::scl::concepts::object<own_caller::nothing>);
}

TEST(ConceptsOwnNames, CompoundAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::compound<own_caller::base *>);
    STATIC_EXPECT_FALSE(::scl::concepts::compound<own_caller::count>);
}

TEST(ConceptsOwnNames, FundamentalAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::fundamental<own_caller::nothing>);
    STATIC_EXPECT_FALSE(::scl::concepts::fundamental<own_caller::base *>);
}

TEST(ConceptsOwnNames, ScalarAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::scalar<own_caller::colour>);
    STATIC_EXPECT_FALSE(::scl::concepts::scalar<own_caller::base>);
}

TEST(ConceptsOwnNames, ArithmeticAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::arithmetic<own_caller::count>);
    STATIC_EXPECT_FALSE(::scl::concepts::arithmetic<own_caller::colour>);
}

TEST(ConceptsOwnNames, VoidTypeAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::void_type<own_caller::nothing>);
    STATIC_EXPECT_FALSE(::scl::concepts::void_type<own_caller::count>);
}

TEST(ConceptsOwnNames, NullPointerAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::null_pointer<own_caller::null>);
    STATIC_EXPECT_FALSE(::scl::concepts::null_pointer<own_caller::base *>);
}

TEST(ConceptsOwnNames, EnumTypeAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::enum_type<own_caller::colour>);
    STATIC_EXPECT_FALSE(::scl::concepts::enum_type<own_caller::count>);
}

TEST(ConceptsOwnNames, EnumValueAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::enum_value<own_caller::colour::red>);
    STATIC_EXPECT_FALSE(::scl::concepts::enum_value<own_caller::zero>);
}

TEST(ConceptsOwnNames, ScopedEnumAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::scoped_enum<own_caller::colour>);
    STATIC_EXPECT_FALSE(::scl::concepts::scoped_enum<own_caller::count>);
}

TEST(ConceptsOwnNames, PointerAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::pointer<own_caller::base *>);
    STATIC_EXPECT_FALSE(::scl::concepts::pointer<own_caller::null>);
}

TEST(ConceptsOwnNames, MemberPointerAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::member_pointer<int own_caller::variant::*>);
    STATIC_EXPECT_FALSE(::scl::concepts::member_pointer<own_caller::base *>);
}

TEST(ConceptsOwnNames, MemberObjectPointerAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::member_object_pointer<int own_caller::variant::*>);
    STATIC_EXPECT_FALSE(::scl::concepts::member_object_pointer<void (own_caller::base::*)()>);
}

TEST(ConceptsOwnNames, MemberFunctionPointerAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::member_function_pointer<void (own_caller::base::*)()>);
    STATIC_EXPECT_FALSE(::scl::concepts::member_function_pointer<int own_caller::variant::*>);
}

TEST(ConceptsOwnNames, ArrayAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::array<own_caller::base[]>);
    STATIC_EXPECT_FALSE(::scl::concepts::array<own_caller::base *>);
}

TEST(ConceptsOwnNames, BoundedArrayAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::bounded_array<own_caller::base[2]>);
    STATIC_EXPECT_FALSE(::scl::concepts::bounded_array<own_caller::base[]>);
}

TEST(ConceptsOwnNames, UnboundedArrayAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::unbounded_array<own_caller::base[]>);
    STATIC_EXPECT_FALSE(::scl::concepts::unbounded_array<own_caller::base[2]>);
}

TEST(ConceptsOwnNames, FunctionAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::function<own_caller::action>);
    STATIC_EXPECT_FALSE(::scl::concepts::function<own_caller::action *>);
}

TEST(ConceptsOwnNames, ClassTypeAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::class_type<own_caller::base>);
    STATIC_EXPECT_FALSE(::scl::concepts::class_type<own_caller::variant>);
}

TEST(ConceptsOwnNames, UnionTypeAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::union_type<own_caller::variant>);
    STATIC_EXPECT_FALSE(::scl::concepts::union_type<own_caller::base>);
}

TEST(ConceptsOwnNames, TriviallyCopyableAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::trivially_copyable<own_caller::base>);
    STATIC_EXPECT_FALSE(::scl::concepts::trivially_copyable<own_caller::derived>);
}

TEST(ConceptsOwnNames, StandardLayoutAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::standard_layout<own_caller::base>);
    STATIC_EXPECT_FALSE(::scl::concepts::standard_layout<own_caller::derived>);
}

TEST(ConceptsOwnNames, EmptyAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::empty<own_caller::base>);
    STATIC_EXPECT_FALSE(::scl::concepts::empty<own_caller::variant>);
}

TEST(ConceptsOwnNames, PolymorphicAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::polymorphic<own_caller::abstract_class>);
    STATIC_EXPECT_FALSE(::scl::concepts::polymorphic<own_caller::base>);
}

TEST(ConceptsOwnNames, AbstractAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::abstract<own_caller::abstract_class>);
    STATIC_EXPECT_FALSE(::scl::concepts::abstract<own_caller::derived>);
}

TEST(ConceptsOwnNames, FinalAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::final<own_caller::final_class>);
    STATIC_EXPECT_FALSE(::scl::concepts::final<own_caller::base>);
}

TEST(ConceptsOwnNames, AggregateAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::aggregate<own_caller::base>);
    STATIC_EXPECT_FALSE(::scl::concepts::aggregate<own_caller::derived>);
}

TEST(ConceptsOwnNames, BaseOfAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE((::scl::concepts::base_of<own_caller::base, own_caller::derived>));
    STATIC_EXPECT_FALSE((::scl::concepts::base_of<own_caller::derived, own_caller::base>));
}

#if SCL_HAS_IMPLICIT_LIFETIME_CONCEPT
TEST(ConceptsOwnNames, ImplicitLifetimeAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE(::scl::concepts::implicit_lifetime<own_caller::base>);
    STATIC_EXPECT_FALSE(::scl::concepts::implicit_lifetime<own_caller::nothing>);
}
#endif

#if SCL_HAS_VIRTUAL_BASE_OF_CONCEPT
TEST(ConceptsOwnNames, VirtualBaseOfAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE((::scl::concepts::virtual_base_of<own_caller::base, own_caller::derived>));
    STATIC_EXPECT_FALSE((::scl::concepts::virtual_base_of<own_caller::base, own_caller::base>));
}
#endif
