#include <gtest_utils.h>

#include <scl/utility/concepts/type_category.h>

#include <concepts>
#include <cstddef>

namespace
{
    enum unscoped_enum
    {
        unscoped_value
    };

    enum unscoped_sized_enum : unsigned char
    {
        unscoped_sized_value
    };

    enum class scoped_class_enum
    {
        scoped_class_value
    };

    enum struct scoped_struct_enum
    {
        scoped_struct_value
    };

    enum class scoped_sized_enum : unsigned char
    {
        scoped_sized_value
    };

    struct plain_class
    {
        int member;

        void method();
    };

    union plain_union
    {
        int integer;
        float real;
    };

    using closure = decltype([] {});

    template <typename Type>
    using with_const = Type const;

    template <typename Type>
    using with_volatile = Type volatile;

    template <typename Type>
    using with_const_volatile = Type const volatile;

    template <typename Type>
    using lvalue_reference_to = Type &;

    template <typename Type>
    using rvalue_reference_to = Type &&;

    template <::scl::concepts::enum_type auto Value>
    constexpr bool enum_type_constant()
    {
        return true;
    }

    template <auto Value>
    constexpr bool accepted_as_enum_type_constant = requires { enum_type_constant<Value>(); };

    template <::scl::concepts::non_reference Type>
    constexpr int object_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::object Type>
    constexpr int object_over_non_reference()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int fundamental_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::fundamental Type>
    constexpr int fundamental_over_non_reference()
    {
        return 2;
    }

    template <::scl::concepts::object Type>
    constexpr int scalar_over_object()
    {
        return 1;
    }

    template <::scl::concepts::scalar Type>
    constexpr int scalar_over_object()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int scalar_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::scalar Type>
    constexpr int scalar_over_non_reference()
    {
        return 2;
    }

    template <::scl::concepts::trivially_copyable Type>
    constexpr int scalar_over_trivially_copyable()
    {
        return 1;
    }

    template <::scl::concepts::scalar Type>
    constexpr int scalar_over_trivially_copyable()
    {
        return 2;
    }

    template <::scl::concepts::standard_layout Type>
    constexpr int scalar_over_standard_layout()
    {
        return 1;
    }

    template <::scl::concepts::scalar Type>
    constexpr int scalar_over_standard_layout()
    {
        return 2;
    }

    template <::scl::concepts::fundamental Type>
    constexpr int arithmetic_over_fundamental()
    {
        return 1;
    }

    template <::scl::concepts::arithmetic Type>
    constexpr int arithmetic_over_fundamental()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int arithmetic_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::arithmetic Type>
    constexpr int arithmetic_over_non_reference()
    {
        return 2;
    }

    template <::scl::concepts::scalar Type>
    constexpr int arithmetic_over_scalar()
    {
        return 1;
    }

    template <::scl::concepts::arithmetic Type>
    constexpr int arithmetic_over_scalar()
    {
        return 2;
    }

    template <::scl::concepts::object Type>
    constexpr int arithmetic_over_object()
    {
        return 1;
    }

    template <::scl::concepts::arithmetic Type>
    constexpr int arithmetic_over_object()
    {
        return 2;
    }

    template <::scl::concepts::trivially_copyable Type>
    constexpr int arithmetic_over_trivially_copyable()
    {
        return 1;
    }

    template <::scl::concepts::arithmetic Type>
    constexpr int arithmetic_over_trivially_copyable()
    {
        return 2;
    }

    template <::scl::concepts::standard_layout Type>
    constexpr int arithmetic_over_standard_layout()
    {
        return 1;
    }

    template <::scl::concepts::arithmetic Type>
    constexpr int arithmetic_over_standard_layout()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int void_type_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::void_type Type>
    constexpr int void_type_over_non_reference()
    {
        return 2;
    }

    template <::scl::concepts::fundamental Type>
    constexpr int void_type_over_fundamental()
    {
        return 1;
    }

    template <::scl::concepts::void_type Type>
    constexpr int void_type_over_fundamental()
    {
        return 2;
    }

    template <::scl::concepts::fundamental Type>
    constexpr int null_pointer_over_fundamental()
    {
        return 1;
    }

    template <::scl::concepts::null_pointer Type>
    constexpr int null_pointer_over_fundamental()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int null_pointer_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::null_pointer Type>
    constexpr int null_pointer_over_non_reference()
    {
        return 2;
    }

    template <::scl::concepts::scalar Type>
    constexpr int null_pointer_over_scalar()
    {
        return 1;
    }

    template <::scl::concepts::null_pointer Type>
    constexpr int null_pointer_over_scalar()
    {
        return 2;
    }

    template <::scl::concepts::object Type>
    constexpr int null_pointer_over_object()
    {
        return 1;
    }

    template <::scl::concepts::null_pointer Type>
    constexpr int null_pointer_over_object()
    {
        return 2;
    }

    template <::scl::concepts::trivially_copyable Type>
    constexpr int null_pointer_over_trivially_copyable()
    {
        return 1;
    }

    template <::scl::concepts::null_pointer Type>
    constexpr int null_pointer_over_trivially_copyable()
    {
        return 2;
    }

    template <::scl::concepts::standard_layout Type>
    constexpr int null_pointer_over_standard_layout()
    {
        return 1;
    }

    template <::scl::concepts::null_pointer Type>
    constexpr int null_pointer_over_standard_layout()
    {
        return 2;
    }

    template <::scl::concepts::scalar Type>
    constexpr int enum_type_over_scalar()
    {
        return 1;
    }

    template <::scl::concepts::enum_type Type>
    constexpr int enum_type_over_scalar()
    {
        return 2;
    }

    template <::scl::concepts::object Type>
    constexpr int enum_type_over_object()
    {
        return 1;
    }

    template <::scl::concepts::enum_type Type>
    constexpr int enum_type_over_object()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int enum_type_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::enum_type Type>
    constexpr int enum_type_over_non_reference()
    {
        return 2;
    }

    template <::scl::concepts::trivially_copyable Type>
    constexpr int enum_type_over_trivially_copyable()
    {
        return 1;
    }

    template <::scl::concepts::enum_type Type>
    constexpr int enum_type_over_trivially_copyable()
    {
        return 2;
    }

    template <::scl::concepts::standard_layout Type>
    constexpr int enum_type_over_standard_layout()
    {
        return 1;
    }

    template <::scl::concepts::enum_type Type>
    constexpr int enum_type_over_standard_layout()
    {
        return 2;
    }

    template <::scl::concepts::compound Type>
    constexpr int enum_type_over_compound()
    {
        return 1;
    }

    template <::scl::concepts::enum_type Type>
    constexpr int enum_type_over_compound()
    {
        return 2;
    }

    template <::scl::concepts::enum_type Type>
    constexpr int scoped_enum_over_enum_type()
    {
        return 1;
    }

    template <::scl::concepts::scoped_enum Type>
    constexpr int scoped_enum_over_enum_type()
    {
        return 2;
    }

    template <::scl::concepts::scalar Type>
    constexpr int scoped_enum_over_scalar()
    {
        return 1;
    }

    template <::scl::concepts::scoped_enum Type>
    constexpr int scoped_enum_over_scalar()
    {
        return 2;
    }

    template <::scl::concepts::object Type>
    constexpr int scoped_enum_over_object()
    {
        return 1;
    }

    template <::scl::concepts::scoped_enum Type>
    constexpr int scoped_enum_over_object()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int scoped_enum_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::scoped_enum Type>
    constexpr int scoped_enum_over_non_reference()
    {
        return 2;
    }

    template <::scl::concepts::trivially_copyable Type>
    constexpr int scoped_enum_over_trivially_copyable()
    {
        return 1;
    }

    template <::scl::concepts::scoped_enum Type>
    constexpr int scoped_enum_over_trivially_copyable()
    {
        return 2;
    }

    template <::scl::concepts::standard_layout Type>
    constexpr int scoped_enum_over_standard_layout()
    {
        return 1;
    }

    template <::scl::concepts::scoped_enum Type>
    constexpr int scoped_enum_over_standard_layout()
    {
        return 2;
    }

    template <::scl::concepts::compound Type>
    constexpr int scoped_enum_over_compound()
    {
        return 1;
    }

    template <::scl::concepts::scoped_enum Type>
    constexpr int scoped_enum_over_compound()
    {
        return 2;
    }

    template <::scl::concepts::scalar Type>
    constexpr int pointer_over_scalar()
    {
        return 1;
    }

    template <::scl::concepts::pointer Type>
    constexpr int pointer_over_scalar()
    {
        return 2;
    }

    template <::scl::concepts::object Type>
    constexpr int pointer_over_object()
    {
        return 1;
    }

    template <::scl::concepts::pointer Type>
    constexpr int pointer_over_object()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int pointer_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::pointer Type>
    constexpr int pointer_over_non_reference()
    {
        return 2;
    }

    template <::scl::concepts::trivially_copyable Type>
    constexpr int pointer_over_trivially_copyable()
    {
        return 1;
    }

    template <::scl::concepts::pointer Type>
    constexpr int pointer_over_trivially_copyable()
    {
        return 2;
    }

    template <::scl::concepts::standard_layout Type>
    constexpr int pointer_over_standard_layout()
    {
        return 1;
    }

    template <::scl::concepts::pointer Type>
    constexpr int pointer_over_standard_layout()
    {
        return 2;
    }

    template <::scl::concepts::compound Type>
    constexpr int pointer_over_compound()
    {
        return 1;
    }

    template <::scl::concepts::pointer Type>
    constexpr int pointer_over_compound()
    {
        return 2;
    }

    template <::scl::concepts::scalar Type>
    constexpr int member_pointer_over_scalar()
    {
        return 1;
    }

    template <::scl::concepts::member_pointer Type>
    constexpr int member_pointer_over_scalar()
    {
        return 2;
    }

    template <::scl::concepts::object Type>
    constexpr int member_pointer_over_object()
    {
        return 1;
    }

    template <::scl::concepts::member_pointer Type>
    constexpr int member_pointer_over_object()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int member_pointer_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::member_pointer Type>
    constexpr int member_pointer_over_non_reference()
    {
        return 2;
    }

    template <::scl::concepts::trivially_copyable Type>
    constexpr int member_pointer_over_trivially_copyable()
    {
        return 1;
    }

    template <::scl::concepts::member_pointer Type>
    constexpr int member_pointer_over_trivially_copyable()
    {
        return 2;
    }

    template <::scl::concepts::standard_layout Type>
    constexpr int member_pointer_over_standard_layout()
    {
        return 1;
    }

    template <::scl::concepts::member_pointer Type>
    constexpr int member_pointer_over_standard_layout()
    {
        return 2;
    }

    template <::scl::concepts::compound Type>
    constexpr int member_pointer_over_compound()
    {
        return 1;
    }

    template <::scl::concepts::member_pointer Type>
    constexpr int member_pointer_over_compound()
    {
        return 2;
    }

    template <::scl::concepts::member_pointer Type>
    constexpr int member_object_pointer_over_member_pointer()
    {
        return 1;
    }

    template <::scl::concepts::member_object_pointer Type>
    constexpr int member_object_pointer_over_member_pointer()
    {
        return 2;
    }

    template <::scl::concepts::scalar Type>
    constexpr int member_object_pointer_over_scalar()
    {
        return 1;
    }

    template <::scl::concepts::member_object_pointer Type>
    constexpr int member_object_pointer_over_scalar()
    {
        return 2;
    }

    template <::scl::concepts::object Type>
    constexpr int member_object_pointer_over_object()
    {
        return 1;
    }

    template <::scl::concepts::member_object_pointer Type>
    constexpr int member_object_pointer_over_object()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int member_object_pointer_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::member_object_pointer Type>
    constexpr int member_object_pointer_over_non_reference()
    {
        return 2;
    }

    template <::scl::concepts::trivially_copyable Type>
    constexpr int member_object_pointer_over_trivially_copyable()
    {
        return 1;
    }

    template <::scl::concepts::member_object_pointer Type>
    constexpr int member_object_pointer_over_trivially_copyable()
    {
        return 2;
    }

    template <::scl::concepts::standard_layout Type>
    constexpr int member_object_pointer_over_standard_layout()
    {
        return 1;
    }

    template <::scl::concepts::member_object_pointer Type>
    constexpr int member_object_pointer_over_standard_layout()
    {
        return 2;
    }

    template <::scl::concepts::compound Type>
    constexpr int member_object_pointer_over_compound()
    {
        return 1;
    }

    template <::scl::concepts::member_object_pointer Type>
    constexpr int member_object_pointer_over_compound()
    {
        return 2;
    }

    template <::scl::concepts::member_pointer Type>
    constexpr int member_function_pointer_over_member_pointer()
    {
        return 1;
    }

    template <::scl::concepts::member_function_pointer Type>
    constexpr int member_function_pointer_over_member_pointer()
    {
        return 2;
    }

    template <::scl::concepts::scalar Type>
    constexpr int member_function_pointer_over_scalar()
    {
        return 1;
    }

    template <::scl::concepts::member_function_pointer Type>
    constexpr int member_function_pointer_over_scalar()
    {
        return 2;
    }

    template <::scl::concepts::object Type>
    constexpr int member_function_pointer_over_object()
    {
        return 1;
    }

    template <::scl::concepts::member_function_pointer Type>
    constexpr int member_function_pointer_over_object()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int member_function_pointer_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::member_function_pointer Type>
    constexpr int member_function_pointer_over_non_reference()
    {
        return 2;
    }

    template <::scl::concepts::trivially_copyable Type>
    constexpr int member_function_pointer_over_trivially_copyable()
    {
        return 1;
    }

    template <::scl::concepts::member_function_pointer Type>
    constexpr int member_function_pointer_over_trivially_copyable()
    {
        return 2;
    }

    template <::scl::concepts::standard_layout Type>
    constexpr int member_function_pointer_over_standard_layout()
    {
        return 1;
    }

    template <::scl::concepts::member_function_pointer Type>
    constexpr int member_function_pointer_over_standard_layout()
    {
        return 2;
    }

    template <::scl::concepts::compound Type>
    constexpr int member_function_pointer_over_compound()
    {
        return 1;
    }

    template <::scl::concepts::member_function_pointer Type>
    constexpr int member_function_pointer_over_compound()
    {
        return 2;
    }

    template <::scl::concepts::object Type>
    constexpr int array_over_object()
    {
        return 1;
    }

    template <::scl::concepts::array Type>
    constexpr int array_over_object()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int array_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::array Type>
    constexpr int array_over_non_reference()
    {
        return 2;
    }

    template <::scl::concepts::compound Type>
    constexpr int array_over_compound()
    {
        return 1;
    }

    template <::scl::concepts::array Type>
    constexpr int array_over_compound()
    {
        return 2;
    }

    template <::scl::concepts::aggregate Type>
    constexpr int array_over_aggregate()
    {
        return 1;
    }

    template <::scl::concepts::array Type>
    constexpr int array_over_aggregate()
    {
        return 2;
    }

    template <::scl::concepts::array Type>
    constexpr int bounded_array_over_array()
    {
        return 1;
    }

    template <::scl::concepts::bounded_array Type>
    constexpr int bounded_array_over_array()
    {
        return 2;
    }

    template <::scl::concepts::object Type>
    constexpr int bounded_array_over_object()
    {
        return 1;
    }

    template <::scl::concepts::bounded_array Type>
    constexpr int bounded_array_over_object()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int bounded_array_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::bounded_array Type>
    constexpr int bounded_array_over_non_reference()
    {
        return 2;
    }

    template <::scl::concepts::compound Type>
    constexpr int bounded_array_over_compound()
    {
        return 1;
    }

    template <::scl::concepts::bounded_array Type>
    constexpr int bounded_array_over_compound()
    {
        return 2;
    }

    template <::scl::concepts::aggregate Type>
    constexpr int bounded_array_over_aggregate()
    {
        return 1;
    }

    template <::scl::concepts::bounded_array Type>
    constexpr int bounded_array_over_aggregate()
    {
        return 2;
    }

    template <::scl::concepts::array Type>
    constexpr int unbounded_array_over_array()
    {
        return 1;
    }

    template <::scl::concepts::unbounded_array Type>
    constexpr int unbounded_array_over_array()
    {
        return 2;
    }

    template <::scl::concepts::object Type>
    constexpr int unbounded_array_over_object()
    {
        return 1;
    }

    template <::scl::concepts::unbounded_array Type>
    constexpr int unbounded_array_over_object()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int unbounded_array_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::unbounded_array Type>
    constexpr int unbounded_array_over_non_reference()
    {
        return 2;
    }

    template <::scl::concepts::compound Type>
    constexpr int unbounded_array_over_compound()
    {
        return 1;
    }

    template <::scl::concepts::unbounded_array Type>
    constexpr int unbounded_array_over_compound()
    {
        return 2;
    }

    template <::scl::concepts::aggregate Type>
    constexpr int unbounded_array_over_aggregate()
    {
        return 1;
    }

    template <::scl::concepts::unbounded_array Type>
    constexpr int unbounded_array_over_aggregate()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int function_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::function Type>
    constexpr int function_over_non_reference()
    {
        return 2;
    }

    template <::scl::concepts::compound Type>
    constexpr int function_over_compound()
    {
        return 1;
    }

    template <::scl::concepts::function Type>
    constexpr int function_over_compound()
    {
        return 2;
    }

    template <::scl::concepts::object Type>
    constexpr int class_type_over_object()
    {
        return 1;
    }

    template <::scl::concepts::class_type Type>
    constexpr int class_type_over_object()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int class_type_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::class_type Type>
    constexpr int class_type_over_non_reference()
    {
        return 2;
    }

    template <::scl::concepts::compound Type>
    constexpr int class_type_over_compound()
    {
        return 1;
    }

    template <::scl::concepts::class_type Type>
    constexpr int class_type_over_compound()
    {
        return 2;
    }

    template <::scl::concepts::object Type>
    constexpr int union_type_over_object()
    {
        return 1;
    }

    template <::scl::concepts::union_type Type>
    constexpr int union_type_over_object()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int union_type_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::union_type Type>
    constexpr int union_type_over_non_reference()
    {
        return 2;
    }

    template <::scl::concepts::compound Type>
    constexpr int union_type_over_compound()
    {
        return 1;
    }

    template <::scl::concepts::union_type Type>
    constexpr int union_type_over_compound()
    {
        return 2;
    }

    template <::scl::concepts::object Type>
    constexpr int object_or_compound()
    {
        return 1;
    }

    template <::scl::concepts::compound Type>
    constexpr int object_or_compound()
    {
        return 2;
    }

    template <typename Type>
    constexpr bool object_or_compound_resolves = requires { object_or_compound<Type>(); };

    template <::std::integral Type>
    constexpr int integral_or_arithmetic()
    {
        return 1;
    }

    template <::scl::concepts::arithmetic Type>
    constexpr int integral_or_arithmetic()
    {
        return 2;
    }

    template <typename Type>
    constexpr bool integral_or_arithmetic_resolves = requires { integral_or_arithmetic<Type>(); };
} // namespace

TEST(ConceptsEnumType, HoldsForAnUnscopedEnumeration)
{
    STATIC_EXPECT_TRUE(::scl::concepts::enum_type<unscoped_enum>);
    STATIC_EXPECT_TRUE(::scl::concepts::enum_type<unscoped_sized_enum>);
}

TEST(ConceptsEnumType, HoldsForAScopedEnumeration)
{
    STATIC_EXPECT_TRUE(::scl::concepts::enum_type<scoped_class_enum>);
    STATIC_EXPECT_TRUE(::scl::concepts::enum_type<scoped_struct_enum>);
}

TEST(ConceptsEnumType, FailsForATypeThatIsNotAnEnumeration)
{
    STATIC_EXPECT_FALSE(::scl::concepts::enum_type<int>);
    STATIC_EXPECT_FALSE(::scl::concepts::enum_type<plain_class>);
    STATIC_EXPECT_FALSE(::scl::concepts::enum_type<plain_union>);
}

TEST(ConceptsEnumType, HoldsForACvQualifiedTypeOfTheCategory)
{
    STATIC_EXPECT_TRUE(::scl::concepts::enum_type<with_const<unscoped_enum>>);
    STATIC_EXPECT_TRUE(::scl::concepts::enum_type<with_volatile<unscoped_enum>>);
    STATIC_EXPECT_TRUE(::scl::concepts::enum_type<with_const_volatile<unscoped_enum>>);
}

TEST(ConceptsEnumType, FailsForACvQualifiedTypeOfAnotherCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::enum_type<with_const<int>>);
    STATIC_EXPECT_FALSE(::scl::concepts::enum_type<with_volatile<int>>);
    STATIC_EXPECT_FALSE(::scl::concepts::enum_type<with_const_volatile<int>>);
}

TEST(ConceptsEnumType, FailsForAReferenceToATypeOfTheCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::enum_type<lvalue_reference_to<unscoped_enum>>);
    STATIC_EXPECT_FALSE(::scl::concepts::enum_type<rvalue_reference_to<unscoped_enum>>);
}

TEST(ConceptsEnumType, IsSatisfiedAsAConstantParameterTypeByTheConstantsOfEnumValue)
{
    STATIC_EXPECT_TRUE(accepted_as_enum_type_constant<unscoped_value>);
    STATIC_EXPECT_TRUE(accepted_as_enum_type_constant<scoped_class_enum::scoped_class_value>);
    STATIC_EXPECT_FALSE(accepted_as_enum_type_constant<0>);
    STATIC_EXPECT_FALSE(accepted_as_enum_type_constant<'a'>);
    STATIC_EXPECT_FALSE(accepted_as_enum_type_constant<nullptr>);
}

TEST(ConceptsEnumType, SubsumesScalar)
{
    STATIC_EXPECT_EQ(enum_type_over_scalar<unscoped_enum>(), 2);
}

TEST(ConceptsEnumType, SubsumesObject)
{
    STATIC_EXPECT_EQ(enum_type_over_object<unscoped_enum>(), 2);
}

TEST(ConceptsEnumType, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(enum_type_over_non_reference<unscoped_enum>(), 2);
}

TEST(ConceptsEnumType, SubsumesTriviallyCopyable)
{
    STATIC_EXPECT_EQ(enum_type_over_trivially_copyable<unscoped_enum>(), 2);
}

TEST(ConceptsEnumType, SubsumesStandardLayout)
{
    STATIC_EXPECT_EQ(enum_type_over_standard_layout<unscoped_enum>(), 2);
}

TEST(ConceptsEnumType, SubsumesCompound)
{
    STATIC_EXPECT_EQ(enum_type_over_compound<unscoped_enum>(), 2);
}

TEST(ConceptsEnumValue, HoldsForAConstantOfAnUnscopedEnumeration)
{
    STATIC_EXPECT_TRUE(::scl::concepts::enum_value<unscoped_value>);
}

TEST(ConceptsEnumValue, HoldsForAConstantOfAScopedEnumeration)
{
    STATIC_EXPECT_TRUE(::scl::concepts::enum_value<scoped_class_enum::scoped_class_value>);
}

TEST(ConceptsEnumValue, FailsForAConstantOfAnotherType)
{
    STATIC_EXPECT_FALSE(::scl::concepts::enum_value<0>);
    STATIC_EXPECT_FALSE(::scl::concepts::enum_value<'a'>);
    STATIC_EXPECT_FALSE(::scl::concepts::enum_value<nullptr>);
}

TEST(ConceptsScopedEnum, HoldsForAnEnumerationDeclaredEnumClass)
{
    STATIC_EXPECT_TRUE(::scl::concepts::scoped_enum<scoped_class_enum>);
    STATIC_EXPECT_TRUE(::scl::concepts::scoped_enum<scoped_sized_enum>);
}

TEST(ConceptsScopedEnum, HoldsForAnEnumerationDeclaredEnumStruct)
{
    STATIC_EXPECT_TRUE(::scl::concepts::scoped_enum<scoped_struct_enum>);
}

TEST(ConceptsScopedEnum, FailsForAnUnscopedEnumeration)
{
    STATIC_EXPECT_FALSE(::scl::concepts::scoped_enum<unscoped_enum>);
    STATIC_EXPECT_FALSE(::scl::concepts::scoped_enum<unscoped_sized_enum>);
}

TEST(ConceptsScopedEnum, FailsForATypeThatIsNotAnEnumeration)
{
    STATIC_EXPECT_FALSE(::scl::concepts::scoped_enum<int>);
    STATIC_EXPECT_FALSE(::scl::concepts::scoped_enum<plain_class>);
}

TEST(ConceptsScopedEnum, HoldsForACvQualifiedTypeOfTheCategory)
{
    STATIC_EXPECT_TRUE(::scl::concepts::scoped_enum<with_const<scoped_class_enum>>);
    STATIC_EXPECT_TRUE(::scl::concepts::scoped_enum<with_volatile<scoped_class_enum>>);
    STATIC_EXPECT_TRUE(::scl::concepts::scoped_enum<with_const_volatile<scoped_class_enum>>);
}

TEST(ConceptsScopedEnum, FailsForACvQualifiedTypeOfAnotherCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::scoped_enum<with_const<unscoped_enum>>);
    STATIC_EXPECT_FALSE(::scl::concepts::scoped_enum<with_volatile<unscoped_enum>>);
    STATIC_EXPECT_FALSE(::scl::concepts::scoped_enum<with_const_volatile<unscoped_enum>>);
}

TEST(ConceptsScopedEnum, FailsForAReferenceToATypeOfTheCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::scoped_enum<lvalue_reference_to<scoped_class_enum>>);
    STATIC_EXPECT_FALSE(::scl::concepts::scoped_enum<rvalue_reference_to<scoped_class_enum>>);
}

TEST(ConceptsScopedEnum, SubsumesEnumType)
{
    STATIC_EXPECT_EQ(scoped_enum_over_enum_type<scoped_class_enum>(), 2);
}

TEST(ConceptsScopedEnum, SubsumesScalar)
{
    STATIC_EXPECT_EQ(scoped_enum_over_scalar<scoped_class_enum>(), 2);
}

TEST(ConceptsScopedEnum, SubsumesObject)
{
    STATIC_EXPECT_EQ(scoped_enum_over_object<scoped_class_enum>(), 2);
}

TEST(ConceptsScopedEnum, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(scoped_enum_over_non_reference<scoped_class_enum>(), 2);
}

TEST(ConceptsScopedEnum, SubsumesTriviallyCopyable)
{
    STATIC_EXPECT_EQ(scoped_enum_over_trivially_copyable<scoped_class_enum>(), 2);
}

TEST(ConceptsScopedEnum, SubsumesStandardLayout)
{
    STATIC_EXPECT_EQ(scoped_enum_over_standard_layout<scoped_class_enum>(), 2);
}

TEST(ConceptsScopedEnum, SubsumesCompound)
{
    STATIC_EXPECT_EQ(scoped_enum_over_compound<scoped_class_enum>(), 2);
}

TEST(ConceptsVoidType, HoldsForVoid) { STATIC_EXPECT_TRUE(::scl::concepts::void_type<void>); }

TEST(ConceptsVoidType, FailsForATypeOtherThanVoid)
{
    STATIC_EXPECT_FALSE(::scl::concepts::void_type<void *>);
    STATIC_EXPECT_FALSE(::scl::concepts::void_type<int>);
    STATIC_EXPECT_FALSE(::scl::concepts::void_type<::std::nullptr_t>);
}

TEST(ConceptsVoidType, HoldsForACvQualifiedTypeOfTheCategory)
{
    STATIC_EXPECT_TRUE(::scl::concepts::void_type<with_const<void>>);
    STATIC_EXPECT_TRUE(::scl::concepts::void_type<with_volatile<void>>);
    STATIC_EXPECT_TRUE(::scl::concepts::void_type<with_const_volatile<void>>);
}

TEST(ConceptsVoidType, FailsForACvQualifiedTypeOfAnotherCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::void_type<with_const<int>>);
    STATIC_EXPECT_FALSE(::scl::concepts::void_type<with_volatile<int>>);
    STATIC_EXPECT_FALSE(::scl::concepts::void_type<with_const_volatile<int>>);
}

TEST(ConceptsVoidType, FailsForAReferenceType)
{
    STATIC_EXPECT_FALSE(::scl::concepts::void_type<lvalue_reference_to<int>>);
    STATIC_EXPECT_FALSE(::scl::concepts::void_type<rvalue_reference_to<int>>);
}

TEST(ConceptsVoidType, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(void_type_over_non_reference<void>(), 2);
}

TEST(ConceptsVoidType, SubsumesFundamental)
{
    STATIC_EXPECT_EQ(void_type_over_fundamental<void>(), 2);
}

TEST(ConceptsPointer, HoldsForAPointerToAnObject)
{
    STATIC_EXPECT_TRUE(::scl::concepts::pointer<int *>);
    STATIC_EXPECT_TRUE(::scl::concepts::pointer<int const *>);
    STATIC_EXPECT_TRUE(::scl::concepts::pointer<plain_class *>);
}

TEST(ConceptsPointer, HoldsForAPointerToVoid)
{
    STATIC_EXPECT_TRUE(::scl::concepts::pointer<void *>);
}

TEST(ConceptsPointer, HoldsForAPointerToAFunction)
{
    STATIC_EXPECT_TRUE(::scl::concepts::pointer<void (*)(int)>);
}

TEST(ConceptsPointer, FailsForAPointerToAMember)
{
    STATIC_EXPECT_FALSE(::scl::concepts::pointer<int plain_class::*>);
    STATIC_EXPECT_FALSE(::scl::concepts::pointer<void (plain_class::*)()>);
}

TEST(ConceptsPointer, FailsForTheTypeOfNullptr)
{
    STATIC_EXPECT_FALSE(::scl::concepts::pointer<::std::nullptr_t>);
}

TEST(ConceptsPointer, FailsForATypeThatIsNotAPointer)
{
    STATIC_EXPECT_FALSE(::scl::concepts::pointer<int>);
    STATIC_EXPECT_FALSE(::scl::concepts::pointer<int[3]>);
}

TEST(ConceptsPointer, HoldsForACvQualifiedTypeOfTheCategory)
{
    STATIC_EXPECT_TRUE(::scl::concepts::pointer<with_const<int *>>);
    STATIC_EXPECT_TRUE(::scl::concepts::pointer<with_volatile<int *>>);
    STATIC_EXPECT_TRUE(::scl::concepts::pointer<with_const_volatile<int *>>);
}

TEST(ConceptsPointer, FailsForACvQualifiedTypeOfAnotherCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::pointer<with_const<int>>);
    STATIC_EXPECT_FALSE(::scl::concepts::pointer<with_volatile<int>>);
    STATIC_EXPECT_FALSE(::scl::concepts::pointer<with_const_volatile<int>>);
}

TEST(ConceptsPointer, FailsForAReferenceToATypeOfTheCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::pointer<lvalue_reference_to<int *>>);
    STATIC_EXPECT_FALSE(::scl::concepts::pointer<rvalue_reference_to<int *>>);
}

TEST(ConceptsPointer, SubsumesScalar) { STATIC_EXPECT_EQ(pointer_over_scalar<int *>(), 2); }

TEST(ConceptsPointer, SubsumesObject) { STATIC_EXPECT_EQ(pointer_over_object<int *>(), 2); }

TEST(ConceptsPointer, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(pointer_over_non_reference<int *>(), 2);
}

TEST(ConceptsPointer, SubsumesTriviallyCopyable)
{
    STATIC_EXPECT_EQ(pointer_over_trivially_copyable<int *>(), 2);
}

TEST(ConceptsPointer, SubsumesStandardLayout)
{
    STATIC_EXPECT_EQ(pointer_over_standard_layout<int *>(), 2);
}

TEST(ConceptsPointer, SubsumesCompound) { STATIC_EXPECT_EQ(pointer_over_compound<int *>(), 2); }

TEST(ConceptsNullPointer, HoldsForTheTypeOfNullptr)
{
    STATIC_EXPECT_TRUE(::scl::concepts::null_pointer<::std::nullptr_t>);
}

TEST(ConceptsNullPointer, FailsForAPointer)
{
    STATIC_EXPECT_FALSE(::scl::concepts::null_pointer<int *>);
    STATIC_EXPECT_FALSE(::scl::concepts::null_pointer<void *>);
}

TEST(ConceptsNullPointer, FailsForAnArithmeticType)
{
    STATIC_EXPECT_FALSE(::scl::concepts::null_pointer<int>);
}

TEST(ConceptsNullPointer, HoldsForACvQualifiedTypeOfTheCategory)
{
    STATIC_EXPECT_TRUE(::scl::concepts::null_pointer<with_const<::std::nullptr_t>>);
    STATIC_EXPECT_TRUE(::scl::concepts::null_pointer<with_volatile<::std::nullptr_t>>);
    STATIC_EXPECT_TRUE(::scl::concepts::null_pointer<with_const_volatile<::std::nullptr_t>>);
}

TEST(ConceptsNullPointer, FailsForACvQualifiedTypeOfAnotherCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::null_pointer<with_const<int *>>);
    STATIC_EXPECT_FALSE(::scl::concepts::null_pointer<with_volatile<int *>>);
    STATIC_EXPECT_FALSE(::scl::concepts::null_pointer<with_const_volatile<int *>>);
}

TEST(ConceptsNullPointer, FailsForAReferenceToATypeOfTheCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::null_pointer<lvalue_reference_to<::std::nullptr_t>>);
    STATIC_EXPECT_FALSE(::scl::concepts::null_pointer<rvalue_reference_to<::std::nullptr_t>>);
}

TEST(ConceptsNullPointer, SubsumesFundamental)
{
    STATIC_EXPECT_EQ(null_pointer_over_fundamental<::std::nullptr_t>(), 2);
}

TEST(ConceptsNullPointer, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(null_pointer_over_non_reference<::std::nullptr_t>(), 2);
}

TEST(ConceptsNullPointer, SubsumesScalar)
{
    STATIC_EXPECT_EQ(null_pointer_over_scalar<::std::nullptr_t>(), 2);
}

TEST(ConceptsNullPointer, SubsumesObject)
{
    STATIC_EXPECT_EQ(null_pointer_over_object<::std::nullptr_t>(), 2);
}

TEST(ConceptsNullPointer, SubsumesTriviallyCopyable)
{
    STATIC_EXPECT_EQ(null_pointer_over_trivially_copyable<::std::nullptr_t>(), 2);
}

TEST(ConceptsNullPointer, SubsumesStandardLayout)
{
    STATIC_EXPECT_EQ(null_pointer_over_standard_layout<::std::nullptr_t>(), 2);
}

TEST(ConceptsMemberPointer, HoldsForAPointerToADataMember)
{
    STATIC_EXPECT_TRUE(::scl::concepts::member_pointer<int plain_class::*>);
}

TEST(ConceptsMemberPointer, HoldsForAPointerToAMemberFunction)
{
    STATIC_EXPECT_TRUE(::scl::concepts::member_pointer<void (plain_class::*)()>);
}

TEST(ConceptsMemberPointer, FailsForAPointer)
{
    STATIC_EXPECT_FALSE(::scl::concepts::member_pointer<int *>);
    STATIC_EXPECT_FALSE(::scl::concepts::member_pointer<void (*)()>);
}

TEST(ConceptsMemberPointer, HoldsForACvQualifiedTypeOfTheCategory)
{
    STATIC_EXPECT_TRUE(::scl::concepts::member_pointer<with_const<int plain_class::*>>);
    STATIC_EXPECT_TRUE(::scl::concepts::member_pointer<with_volatile<int plain_class::*>>);
    STATIC_EXPECT_TRUE(::scl::concepts::member_pointer<with_const_volatile<int plain_class::*>>);
}

TEST(ConceptsMemberPointer, FailsForACvQualifiedTypeOfAnotherCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::member_pointer<with_const<int *>>);
    STATIC_EXPECT_FALSE(::scl::concepts::member_pointer<with_volatile<int *>>);
    STATIC_EXPECT_FALSE(::scl::concepts::member_pointer<with_const_volatile<int *>>);
}

TEST(ConceptsMemberPointer, FailsForAReferenceToATypeOfTheCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::member_pointer<lvalue_reference_to<int plain_class::*>>);
    STATIC_EXPECT_FALSE(::scl::concepts::member_pointer<rvalue_reference_to<int plain_class::*>>);
}

TEST(ConceptsMemberPointer, SubsumesScalar)
{
    STATIC_EXPECT_EQ(member_pointer_over_scalar<int plain_class::*>(), 2);
}

TEST(ConceptsMemberPointer, SubsumesObject)
{
    STATIC_EXPECT_EQ(member_pointer_over_object<int plain_class::*>(), 2);
}

TEST(ConceptsMemberPointer, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(member_pointer_over_non_reference<int plain_class::*>(), 2);
}

TEST(ConceptsMemberPointer, SubsumesTriviallyCopyable)
{
    STATIC_EXPECT_EQ(member_pointer_over_trivially_copyable<int plain_class::*>(), 2);
}

TEST(ConceptsMemberPointer, SubsumesStandardLayout)
{
    STATIC_EXPECT_EQ(member_pointer_over_standard_layout<int plain_class::*>(), 2);
}

TEST(ConceptsMemberPointer, SubsumesCompound)
{
    STATIC_EXPECT_EQ(member_pointer_over_compound<int plain_class::*>(), 2);
}

TEST(ConceptsMemberObjectPointer, HoldsForAPointerToADataMember)
{
    STATIC_EXPECT_TRUE(::scl::concepts::member_object_pointer<int plain_class::*>);
    STATIC_EXPECT_TRUE(::scl::concepts::member_object_pointer<int const plain_class::*>);
}

TEST(ConceptsMemberObjectPointer, FailsForAPointerToAMemberFunction)
{
    STATIC_EXPECT_FALSE(::scl::concepts::member_object_pointer<void (plain_class::*)()>);
}

TEST(ConceptsMemberObjectPointer, FailsForAPointer)
{
    STATIC_EXPECT_FALSE(::scl::concepts::member_object_pointer<int *>);
}

TEST(ConceptsMemberObjectPointer, HoldsForACvQualifiedTypeOfTheCategory)
{
    STATIC_EXPECT_TRUE(::scl::concepts::member_object_pointer<with_const<int plain_class::*>>);
    STATIC_EXPECT_TRUE(::scl::concepts::member_object_pointer<with_volatile<int plain_class::*>>);
    STATIC_EXPECT_TRUE(::scl::concepts::member_object_pointer<with_const_volatile<int plain_class::*>>);
}

TEST(ConceptsMemberObjectPointer, FailsForACvQualifiedTypeOfAnotherCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::member_object_pointer<with_const<void (plain_class::*)()>>);
    STATIC_EXPECT_FALSE(::scl::concepts::member_object_pointer<with_volatile<void (plain_class::*)()>>);
    STATIC_EXPECT_FALSE(
        ::scl::concepts::member_object_pointer<with_const_volatile<void (plain_class::*)()>>);
}

TEST(ConceptsMemberObjectPointer, FailsForAReferenceToATypeOfTheCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::member_object_pointer<lvalue_reference_to<int plain_class::*>>);
    STATIC_EXPECT_FALSE(::scl::concepts::member_object_pointer<rvalue_reference_to<int plain_class::*>>);
}

TEST(ConceptsMemberObjectPointer, SubsumesMemberPointer)
{
    STATIC_EXPECT_EQ(member_object_pointer_over_member_pointer<int plain_class::*>(), 2);
}

TEST(ConceptsMemberObjectPointer, SubsumesScalar)
{
    STATIC_EXPECT_EQ(member_object_pointer_over_scalar<int plain_class::*>(), 2);
}

TEST(ConceptsMemberObjectPointer, SubsumesObject)
{
    STATIC_EXPECT_EQ(member_object_pointer_over_object<int plain_class::*>(), 2);
}

TEST(ConceptsMemberObjectPointer, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(member_object_pointer_over_non_reference<int plain_class::*>(), 2);
}

TEST(ConceptsMemberObjectPointer, SubsumesTriviallyCopyable)
{
    STATIC_EXPECT_EQ(member_object_pointer_over_trivially_copyable<int plain_class::*>(), 2);
}

TEST(ConceptsMemberObjectPointer, SubsumesStandardLayout)
{
    STATIC_EXPECT_EQ(member_object_pointer_over_standard_layout<int plain_class::*>(), 2);
}

TEST(ConceptsMemberObjectPointer, SubsumesCompound)
{
    STATIC_EXPECT_EQ(member_object_pointer_over_compound<int plain_class::*>(), 2);
}

TEST(ConceptsMemberFunctionPointer, HoldsForAPointerToAMemberFunction)
{
    STATIC_EXPECT_TRUE(::scl::concepts::member_function_pointer<void (plain_class::*)()>);
}

TEST(ConceptsMemberFunctionPointer, HoldsWhateverTheQualifiersOfTheMemberFunction)
{
    STATIC_EXPECT_TRUE(::scl::concepts::member_function_pointer<void (plain_class::*)() const>);
    STATIC_EXPECT_TRUE(::scl::concepts::member_function_pointer<void (plain_class::*)() volatile>);
    STATIC_EXPECT_TRUE(::scl::concepts::member_function_pointer<void (plain_class::*)() &>);
    STATIC_EXPECT_TRUE(::scl::concepts::member_function_pointer<void (plain_class::*)() const &&>);
    STATIC_EXPECT_TRUE(::scl::concepts::member_function_pointer<void (plain_class::*)() noexcept>);
}

TEST(ConceptsMemberFunctionPointer, FailsForAPointerToADataMember)
{
    STATIC_EXPECT_FALSE(::scl::concepts::member_function_pointer<int plain_class::*>);
}

TEST(ConceptsMemberFunctionPointer, FailsForAPointerToAFunction)
{
    STATIC_EXPECT_FALSE(::scl::concepts::member_function_pointer<void (*)()>);
}

TEST(ConceptsMemberFunctionPointer, HoldsForACvQualifiedTypeOfTheCategory)
{
    STATIC_EXPECT_TRUE(::scl::concepts::member_function_pointer<with_const<void (plain_class::*)()>>);
    STATIC_EXPECT_TRUE(::scl::concepts::member_function_pointer<with_volatile<void (plain_class::*)()>>);
    STATIC_EXPECT_TRUE(
        ::scl::concepts::member_function_pointer<with_const_volatile<void (plain_class::*)()>>);
}

TEST(ConceptsMemberFunctionPointer, FailsForACvQualifiedTypeOfAnotherCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::member_function_pointer<with_const<int plain_class::*>>);
    STATIC_EXPECT_FALSE(::scl::concepts::member_function_pointer<with_volatile<int plain_class::*>>);
    STATIC_EXPECT_FALSE(::scl::concepts::member_function_pointer<with_const_volatile<int plain_class::*>>);
}

TEST(ConceptsMemberFunctionPointer, FailsForAReferenceToATypeOfTheCategory)
{
    STATIC_EXPECT_FALSE(
        ::scl::concepts::member_function_pointer<lvalue_reference_to<void (plain_class::*)()>>);
    STATIC_EXPECT_FALSE(
        ::scl::concepts::member_function_pointer<rvalue_reference_to<void (plain_class::*)()>>);
}

TEST(ConceptsMemberFunctionPointer, SubsumesMemberPointer)
{
    STATIC_EXPECT_EQ(member_function_pointer_over_member_pointer<void (plain_class::*)()>(), 2);
}

TEST(ConceptsMemberFunctionPointer, SubsumesScalar)
{
    STATIC_EXPECT_EQ(member_function_pointer_over_scalar<void (plain_class::*)()>(), 2);
}

TEST(ConceptsMemberFunctionPointer, SubsumesObject)
{
    STATIC_EXPECT_EQ(member_function_pointer_over_object<void (plain_class::*)()>(), 2);
}

TEST(ConceptsMemberFunctionPointer, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(member_function_pointer_over_non_reference<void (plain_class::*)()>(), 2);
}

TEST(ConceptsMemberFunctionPointer, SubsumesTriviallyCopyable)
{
    STATIC_EXPECT_EQ(member_function_pointer_over_trivially_copyable<void (plain_class::*)()>(), 2);
}

TEST(ConceptsMemberFunctionPointer, SubsumesStandardLayout)
{
    STATIC_EXPECT_EQ(member_function_pointer_over_standard_layout<void (plain_class::*)()>(), 2);
}

TEST(ConceptsMemberFunctionPointer, SubsumesCompound)
{
    STATIC_EXPECT_EQ(member_function_pointer_over_compound<void (plain_class::*)()>(), 2);
}

TEST(ConceptsArray, HoldsForAnArrayWithAKnownBound)
{
    STATIC_EXPECT_TRUE(::scl::concepts::array<int[3]>);
    STATIC_EXPECT_TRUE(::scl::concepts::array<int[2][3]>);
}

TEST(ConceptsArray, HoldsForAnArrayWithAnUnknownBound)
{
    STATIC_EXPECT_TRUE(::scl::concepts::array<int[]>);
    STATIC_EXPECT_TRUE(::scl::concepts::array<int[][3]>);
}

TEST(ConceptsArray, FailsForAPointerToAnElement)
{
    STATIC_EXPECT_FALSE(::scl::concepts::array<int *>);
}

TEST(ConceptsArray, FailsForATypeThatIsNotAnArray)
{
    STATIC_EXPECT_FALSE(::scl::concepts::array<int>);
    STATIC_EXPECT_FALSE(::scl::concepts::array<plain_class>);
}

TEST(ConceptsArray, HoldsForACvQualifiedTypeOfTheCategory)
{
    STATIC_EXPECT_TRUE(::scl::concepts::array<with_const<int[3]>>);
    STATIC_EXPECT_TRUE(::scl::concepts::array<with_volatile<int[3]>>);
    STATIC_EXPECT_TRUE(::scl::concepts::array<with_const_volatile<int[3]>>);
}

TEST(ConceptsArray, FailsForACvQualifiedTypeOfAnotherCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::array<with_const<int *>>);
    STATIC_EXPECT_FALSE(::scl::concepts::array<with_volatile<int *>>);
    STATIC_EXPECT_FALSE(::scl::concepts::array<with_const_volatile<int *>>);
}

TEST(ConceptsArray, FailsForAReferenceToATypeOfTheCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::array<lvalue_reference_to<int[3]>>);
    STATIC_EXPECT_FALSE(::scl::concepts::array<rvalue_reference_to<int[3]>>);
}

TEST(ConceptsArray, SubsumesObject) { STATIC_EXPECT_EQ(array_over_object<int[3]>(), 2); }

TEST(ConceptsArray, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(array_over_non_reference<int[3]>(), 2);
}

TEST(ConceptsArray, SubsumesCompound) { STATIC_EXPECT_EQ(array_over_compound<int[3]>(), 2); }

TEST(ConceptsArray, SubsumesAggregate) { STATIC_EXPECT_EQ(array_over_aggregate<int[3]>(), 2); }

TEST(ConceptsBoundedArray, HoldsForAnArrayWithAKnownBound)
{
    STATIC_EXPECT_TRUE(::scl::concepts::bounded_array<int[3]>);
    STATIC_EXPECT_TRUE(::scl::concepts::bounded_array<plain_class[1]>);
}

TEST(ConceptsBoundedArray, HoldsForABoundedArrayOfBoundedArrays)
{
    STATIC_EXPECT_TRUE(::scl::concepts::bounded_array<int[2][3]>);
}

TEST(ConceptsBoundedArray, FailsForAnArrayWithAnUnknownBound)
{
    STATIC_EXPECT_FALSE(::scl::concepts::bounded_array<int[]>);
    STATIC_EXPECT_FALSE(::scl::concepts::bounded_array<int[][3]>);
}

TEST(ConceptsBoundedArray, FailsForAPointerToAnElement)
{
    STATIC_EXPECT_FALSE(::scl::concepts::bounded_array<int *>);
}

TEST(ConceptsBoundedArray, HoldsForACvQualifiedTypeOfTheCategory)
{
    STATIC_EXPECT_TRUE(::scl::concepts::bounded_array<with_const<int[3]>>);
    STATIC_EXPECT_TRUE(::scl::concepts::bounded_array<with_volatile<int[3]>>);
    STATIC_EXPECT_TRUE(::scl::concepts::bounded_array<with_const_volatile<int[3]>>);
}

TEST(ConceptsBoundedArray, FailsForACvQualifiedTypeOfAnotherCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::bounded_array<with_const<int[]>>);
    STATIC_EXPECT_FALSE(::scl::concepts::bounded_array<with_volatile<int[]>>);
    STATIC_EXPECT_FALSE(::scl::concepts::bounded_array<with_const_volatile<int[]>>);
}

TEST(ConceptsBoundedArray, FailsForAReferenceToATypeOfTheCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::bounded_array<lvalue_reference_to<int[3]>>);
    STATIC_EXPECT_FALSE(::scl::concepts::bounded_array<rvalue_reference_to<int[3]>>);
}

TEST(ConceptsBoundedArray, SubsumesArray)
{
    STATIC_EXPECT_EQ(bounded_array_over_array<int[3]>(), 2);
}

TEST(ConceptsBoundedArray, SubsumesObject)
{
    STATIC_EXPECT_EQ(bounded_array_over_object<int[3]>(), 2);
}

TEST(ConceptsBoundedArray, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(bounded_array_over_non_reference<int[3]>(), 2);
}

TEST(ConceptsBoundedArray, SubsumesCompound)
{
    STATIC_EXPECT_EQ(bounded_array_over_compound<int[3]>(), 2);
}

TEST(ConceptsBoundedArray, SubsumesAggregate)
{
    STATIC_EXPECT_EQ(bounded_array_over_aggregate<int[3]>(), 2);
}

TEST(ConceptsUnboundedArray, HoldsForAnArrayWithAnUnknownBound)
{
    STATIC_EXPECT_TRUE(::scl::concepts::unbounded_array<int[]>);
    STATIC_EXPECT_TRUE(::scl::concepts::unbounded_array<plain_class[]>);
}

TEST(ConceptsUnboundedArray, HoldsForAnUnboundedArrayOfBoundedArrays)
{
    STATIC_EXPECT_TRUE(::scl::concepts::unbounded_array<int[][3]>);
}

TEST(ConceptsUnboundedArray, FailsForAnArrayWithAKnownBound)
{
    STATIC_EXPECT_FALSE(::scl::concepts::unbounded_array<int[3]>);
    STATIC_EXPECT_FALSE(::scl::concepts::unbounded_array<int[2][3]>);
}

TEST(ConceptsUnboundedArray, FailsForAPointerToAnElement)
{
    STATIC_EXPECT_FALSE(::scl::concepts::unbounded_array<int *>);
}

TEST(ConceptsUnboundedArray, HoldsForACvQualifiedTypeOfTheCategory)
{
    STATIC_EXPECT_TRUE(::scl::concepts::unbounded_array<with_const<int[]>>);
    STATIC_EXPECT_TRUE(::scl::concepts::unbounded_array<with_volatile<int[]>>);
    STATIC_EXPECT_TRUE(::scl::concepts::unbounded_array<with_const_volatile<int[]>>);
}

TEST(ConceptsUnboundedArray, FailsForACvQualifiedTypeOfAnotherCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::unbounded_array<with_const<int[3]>>);
    STATIC_EXPECT_FALSE(::scl::concepts::unbounded_array<with_volatile<int[3]>>);
    STATIC_EXPECT_FALSE(::scl::concepts::unbounded_array<with_const_volatile<int[3]>>);
}

TEST(ConceptsUnboundedArray, FailsForAReferenceToATypeOfTheCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::unbounded_array<lvalue_reference_to<int[]>>);
    STATIC_EXPECT_FALSE(::scl::concepts::unbounded_array<rvalue_reference_to<int[]>>);
}

TEST(ConceptsUnboundedArray, SubsumesArray)
{
    STATIC_EXPECT_EQ(unbounded_array_over_array<int[]>(), 2);
}

TEST(ConceptsUnboundedArray, SubsumesObject)
{
    STATIC_EXPECT_EQ(unbounded_array_over_object<int[]>(), 2);
}

TEST(ConceptsUnboundedArray, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(unbounded_array_over_non_reference<int[]>(), 2);
}

TEST(ConceptsUnboundedArray, SubsumesCompound)
{
    STATIC_EXPECT_EQ(unbounded_array_over_compound<int[]>(), 2);
}

TEST(ConceptsUnboundedArray, SubsumesAggregate)
{
    STATIC_EXPECT_EQ(unbounded_array_over_aggregate<int[]>(), 2);
}

TEST(ConceptsFunction, HoldsForAFunctionType)
{
    STATIC_EXPECT_TRUE(::scl::concepts::function<void(int)>);
    STATIC_EXPECT_TRUE(::scl::concepts::function<int()>);
}

TEST(ConceptsFunction, HoldsForAFunctionTypeWithQualifiersOrNoexcept)
{
    STATIC_EXPECT_TRUE(::scl::concepts::function<void() const>);
    STATIC_EXPECT_TRUE(::scl::concepts::function<void() volatile>);
    STATIC_EXPECT_TRUE(::scl::concepts::function<void() &>);
    STATIC_EXPECT_TRUE(::scl::concepts::function<void() &&>);
    STATIC_EXPECT_TRUE(::scl::concepts::function<void() noexcept>);
}

TEST(ConceptsFunction, FailsForAPointerToAFunction)
{
    STATIC_EXPECT_FALSE(::scl::concepts::function<void (*)(int)>);
}

TEST(ConceptsFunction, FailsForAReferenceToAFunction)
{
    STATIC_EXPECT_FALSE(::scl::concepts::function<void (&)(int)>);
    STATIC_EXPECT_FALSE(::scl::concepts::function<void (&&)(int)>);
}

TEST(ConceptsFunction, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(function_over_non_reference<void()>(), 2);
}

TEST(ConceptsFunction, SubsumesCompound) { STATIC_EXPECT_EQ(function_over_compound<void()>(), 2); }

TEST(ConceptsClassType, HoldsForAClass)
{
    STATIC_EXPECT_TRUE(::scl::concepts::class_type<plain_class>);
}

TEST(ConceptsClassType, HoldsForTheClosureTypeOfALambda)
{
    STATIC_EXPECT_TRUE(::scl::concepts::class_type<closure>);
}

TEST(ConceptsClassType, FailsForAUnion)
{
    STATIC_EXPECT_FALSE(::scl::concepts::class_type<plain_union>);
}

TEST(ConceptsClassType, FailsForAnEnumeration)
{
    STATIC_EXPECT_FALSE(::scl::concepts::class_type<unscoped_enum>);
    STATIC_EXPECT_FALSE(::scl::concepts::class_type<scoped_class_enum>);
}

TEST(ConceptsClassType, FailsForATypeOfAnotherCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::class_type<int>);
    STATIC_EXPECT_FALSE(::scl::concepts::class_type<plain_class *>);
}

TEST(ConceptsClassType, HoldsForACvQualifiedTypeOfTheCategory)
{
    STATIC_EXPECT_TRUE(::scl::concepts::class_type<with_const<plain_class>>);
    STATIC_EXPECT_TRUE(::scl::concepts::class_type<with_volatile<plain_class>>);
    STATIC_EXPECT_TRUE(::scl::concepts::class_type<with_const_volatile<plain_class>>);
}

TEST(ConceptsClassType, FailsForACvQualifiedTypeOfAnotherCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::class_type<with_const<plain_union>>);
    STATIC_EXPECT_FALSE(::scl::concepts::class_type<with_volatile<plain_union>>);
    STATIC_EXPECT_FALSE(::scl::concepts::class_type<with_const_volatile<plain_union>>);
}

TEST(ConceptsClassType, FailsForAReferenceToATypeOfTheCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::class_type<lvalue_reference_to<plain_class>>);
    STATIC_EXPECT_FALSE(::scl::concepts::class_type<rvalue_reference_to<plain_class>>);
}

TEST(ConceptsClassType, SubsumesObject)
{
    STATIC_EXPECT_EQ(class_type_over_object<plain_class>(), 2);
}

TEST(ConceptsClassType, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(class_type_over_non_reference<plain_class>(), 2);
}

TEST(ConceptsClassType, SubsumesCompound)
{
    STATIC_EXPECT_EQ(class_type_over_compound<plain_class>(), 2);
}

TEST(ConceptsUnionType, HoldsForAUnion)
{
    STATIC_EXPECT_TRUE(::scl::concepts::union_type<plain_union>);
}

TEST(ConceptsUnionType, FailsForAClass)
{
    STATIC_EXPECT_FALSE(::scl::concepts::union_type<plain_class>);
    STATIC_EXPECT_FALSE(::scl::concepts::union_type<closure>);
}

TEST(ConceptsUnionType, FailsForATypeOfAnotherCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::union_type<int>);
    STATIC_EXPECT_FALSE(::scl::concepts::union_type<unscoped_enum>);
}

TEST(ConceptsUnionType, HoldsForACvQualifiedTypeOfTheCategory)
{
    STATIC_EXPECT_TRUE(::scl::concepts::union_type<with_const<plain_union>>);
    STATIC_EXPECT_TRUE(::scl::concepts::union_type<with_volatile<plain_union>>);
    STATIC_EXPECT_TRUE(::scl::concepts::union_type<with_const_volatile<plain_union>>);
}

TEST(ConceptsUnionType, FailsForACvQualifiedTypeOfAnotherCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::union_type<with_const<plain_class>>);
    STATIC_EXPECT_FALSE(::scl::concepts::union_type<with_volatile<plain_class>>);
    STATIC_EXPECT_FALSE(::scl::concepts::union_type<with_const_volatile<plain_class>>);
}

TEST(ConceptsUnionType, FailsForAReferenceToATypeOfTheCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::union_type<lvalue_reference_to<plain_union>>);
    STATIC_EXPECT_FALSE(::scl::concepts::union_type<rvalue_reference_to<plain_union>>);
}

TEST(ConceptsUnionType, SubsumesObject)
{
    STATIC_EXPECT_EQ(union_type_over_object<plain_union>(), 2);
}

TEST(ConceptsUnionType, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(union_type_over_non_reference<plain_union>(), 2);
}

TEST(ConceptsUnionType, SubsumesCompound)
{
    STATIC_EXPECT_EQ(union_type_over_compound<plain_union>(), 2);
}

TEST(ConceptsFundamental, HoldsForAnArithmeticType)
{
    STATIC_EXPECT_TRUE(::scl::concepts::fundamental<int>);
    STATIC_EXPECT_TRUE(::scl::concepts::fundamental<double>);
    STATIC_EXPECT_TRUE(::scl::concepts::fundamental<bool>);
}

TEST(ConceptsFundamental, HoldsForVoid) { STATIC_EXPECT_TRUE(::scl::concepts::fundamental<void>); }

TEST(ConceptsFundamental, HoldsForTheTypeOfNullptr)
{
    STATIC_EXPECT_TRUE(::scl::concepts::fundamental<::std::nullptr_t>);
}

TEST(ConceptsFundamental, FailsForACompoundType)
{
    STATIC_EXPECT_FALSE(::scl::concepts::fundamental<int *>);
    STATIC_EXPECT_FALSE(::scl::concepts::fundamental<unscoped_enum>);
    STATIC_EXPECT_FALSE(::scl::concepts::fundamental<plain_class>);
    STATIC_EXPECT_FALSE(::scl::concepts::fundamental<int[3]>);
}

TEST(ConceptsFundamental, HoldsForACvQualifiedTypeOfTheCategory)
{
    STATIC_EXPECT_TRUE(::scl::concepts::fundamental<with_const<int>>);
    STATIC_EXPECT_TRUE(::scl::concepts::fundamental<with_volatile<int>>);
    STATIC_EXPECT_TRUE(::scl::concepts::fundamental<with_const_volatile<int>>);
}

TEST(ConceptsFundamental, FailsForACvQualifiedTypeOfAnotherCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::fundamental<with_const<plain_class>>);
    STATIC_EXPECT_FALSE(::scl::concepts::fundamental<with_volatile<plain_class>>);
    STATIC_EXPECT_FALSE(::scl::concepts::fundamental<with_const_volatile<plain_class>>);
}

TEST(ConceptsFundamental, FailsForAReferenceToATypeOfTheCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::fundamental<lvalue_reference_to<int>>);
    STATIC_EXPECT_FALSE(::scl::concepts::fundamental<rvalue_reference_to<int>>);
}

TEST(ConceptsFundamental, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(fundamental_over_non_reference<int>(), 2);
}

TEST(ConceptsArithmetic, HoldsForAnIntegralType)
{
    STATIC_EXPECT_TRUE(::scl::concepts::arithmetic<int>);
    STATIC_EXPECT_TRUE(::scl::concepts::arithmetic<unsigned long long>);
    STATIC_EXPECT_TRUE(::scl::concepts::arithmetic<short>);
}

TEST(ConceptsArithmetic, HoldsForAFloatingPointType)
{
    STATIC_EXPECT_TRUE(::scl::concepts::arithmetic<float>);
    STATIC_EXPECT_TRUE(::scl::concepts::arithmetic<double>);
    STATIC_EXPECT_TRUE(::scl::concepts::arithmetic<long double>);
}

TEST(ConceptsArithmetic, HoldsForBool) { STATIC_EXPECT_TRUE(::scl::concepts::arithmetic<bool>); }

TEST(ConceptsArithmetic, HoldsForACharacterType)
{
    STATIC_EXPECT_TRUE(::scl::concepts::arithmetic<char>);
    STATIC_EXPECT_TRUE(::scl::concepts::arithmetic<wchar_t>);
    STATIC_EXPECT_TRUE(::scl::concepts::arithmetic<char8_t>);
    STATIC_EXPECT_TRUE(::scl::concepts::arithmetic<char16_t>);
    STATIC_EXPECT_TRUE(::scl::concepts::arithmetic<char32_t>);
}

TEST(ConceptsArithmetic, FailsForAnEnumeration)
{
    STATIC_EXPECT_FALSE(::scl::concepts::arithmetic<unscoped_enum>);
    STATIC_EXPECT_FALSE(::scl::concepts::arithmetic<scoped_class_enum>);
}

TEST(ConceptsArithmetic, FailsForATypeOfAnotherCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::arithmetic<int *>);
    STATIC_EXPECT_FALSE(::scl::concepts::arithmetic<::std::nullptr_t>);
    STATIC_EXPECT_FALSE(::scl::concepts::arithmetic<void>);
}

TEST(ConceptsArithmetic, HoldsForACvQualifiedTypeOfTheCategory)
{
    STATIC_EXPECT_TRUE(::scl::concepts::arithmetic<with_const<int>>);
    STATIC_EXPECT_TRUE(::scl::concepts::arithmetic<with_volatile<int>>);
    STATIC_EXPECT_TRUE(::scl::concepts::arithmetic<with_const_volatile<int>>);
}

TEST(ConceptsArithmetic, FailsForACvQualifiedTypeOfAnotherCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::arithmetic<with_const<unscoped_enum>>);
    STATIC_EXPECT_FALSE(::scl::concepts::arithmetic<with_volatile<unscoped_enum>>);
    STATIC_EXPECT_FALSE(::scl::concepts::arithmetic<with_const_volatile<unscoped_enum>>);
}

TEST(ConceptsArithmetic, FailsForAReferenceToATypeOfTheCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::arithmetic<lvalue_reference_to<int>>);
    STATIC_EXPECT_FALSE(::scl::concepts::arithmetic<rvalue_reference_to<int>>);
}

TEST(ConceptsArithmetic, IsUnorderedWithStdIntegral)
{
    STATIC_EXPECT_FALSE(integral_or_arithmetic_resolves<int>);
}

TEST(ConceptsArithmetic, SubsumesFundamental)
{
    STATIC_EXPECT_EQ(arithmetic_over_fundamental<int>(), 2);
}

TEST(ConceptsArithmetic, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(arithmetic_over_non_reference<int>(), 2);
}

TEST(ConceptsArithmetic, SubsumesScalar) { STATIC_EXPECT_EQ(arithmetic_over_scalar<int>(), 2); }

TEST(ConceptsArithmetic, SubsumesObject) { STATIC_EXPECT_EQ(arithmetic_over_object<int>(), 2); }

TEST(ConceptsArithmetic, SubsumesTriviallyCopyable)
{
    STATIC_EXPECT_EQ(arithmetic_over_trivially_copyable<int>(), 2);
}

TEST(ConceptsArithmetic, SubsumesStandardLayout)
{
    STATIC_EXPECT_EQ(arithmetic_over_standard_layout<int>(), 2);
}

TEST(ConceptsScalar, HoldsForAnArithmeticType)
{
    STATIC_EXPECT_TRUE(::scl::concepts::scalar<int>);
    STATIC_EXPECT_TRUE(::scl::concepts::scalar<double>);
}

TEST(ConceptsScalar, HoldsForAnEnumeration)
{
    STATIC_EXPECT_TRUE(::scl::concepts::scalar<unscoped_enum>);
    STATIC_EXPECT_TRUE(::scl::concepts::scalar<scoped_class_enum>);
}

TEST(ConceptsScalar, HoldsForAPointer)
{
    STATIC_EXPECT_TRUE(::scl::concepts::scalar<int *>);
    STATIC_EXPECT_TRUE(::scl::concepts::scalar<void (*)()>);
}

TEST(ConceptsScalar, HoldsForAPointerToAMember)
{
    STATIC_EXPECT_TRUE(::scl::concepts::scalar<int plain_class::*>);
    STATIC_EXPECT_TRUE(::scl::concepts::scalar<void (plain_class::*)()>);
}

TEST(ConceptsScalar, HoldsForTheTypeOfNullptr)
{
    STATIC_EXPECT_TRUE(::scl::concepts::scalar<::std::nullptr_t>);
}

TEST(ConceptsScalar, FailsForATypeOfAnotherCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::scalar<plain_class>);
    STATIC_EXPECT_FALSE(::scl::concepts::scalar<plain_union>);
    STATIC_EXPECT_FALSE(::scl::concepts::scalar<int[3]>);
    STATIC_EXPECT_FALSE(::scl::concepts::scalar<void>);
    STATIC_EXPECT_FALSE(::scl::concepts::scalar<void()>);
}

TEST(ConceptsScalar, HoldsForACvQualifiedTypeOfTheCategory)
{
    STATIC_EXPECT_TRUE(::scl::concepts::scalar<with_const<int>>);
    STATIC_EXPECT_TRUE(::scl::concepts::scalar<with_volatile<int>>);
    STATIC_EXPECT_TRUE(::scl::concepts::scalar<with_const_volatile<int>>);
}

TEST(ConceptsScalar, FailsForACvQualifiedTypeOfAnotherCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::scalar<with_const<plain_class>>);
    STATIC_EXPECT_FALSE(::scl::concepts::scalar<with_volatile<plain_class>>);
    STATIC_EXPECT_FALSE(::scl::concepts::scalar<with_const_volatile<plain_class>>);
}

TEST(ConceptsScalar, FailsForAReferenceToATypeOfTheCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::scalar<lvalue_reference_to<int>>);
    STATIC_EXPECT_FALSE(::scl::concepts::scalar<rvalue_reference_to<int>>);
}

TEST(ConceptsScalar, SubsumesObject) { STATIC_EXPECT_EQ(scalar_over_object<int>(), 2); }

TEST(ConceptsScalar, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(scalar_over_non_reference<int>(), 2);
}

TEST(ConceptsScalar, SubsumesTriviallyCopyable)
{
    STATIC_EXPECT_EQ(scalar_over_trivially_copyable<int>(), 2);
}

TEST(ConceptsScalar, SubsumesStandardLayout)
{
    STATIC_EXPECT_EQ(scalar_over_standard_layout<int>(), 2);
}

TEST(ConceptsObject, HoldsForAScalarType)
{
    STATIC_EXPECT_TRUE(::scl::concepts::object<int>);
    STATIC_EXPECT_TRUE(::scl::concepts::object<int *>);
    STATIC_EXPECT_TRUE(::scl::concepts::object<unscoped_enum>);
}

TEST(ConceptsObject, HoldsForAnArrayType)
{
    STATIC_EXPECT_TRUE(::scl::concepts::object<int[3]>);
    STATIC_EXPECT_TRUE(::scl::concepts::object<int[]>);
}

TEST(ConceptsObject, HoldsForAClassOrAUnion)
{
    STATIC_EXPECT_TRUE(::scl::concepts::object<plain_class>);
    STATIC_EXPECT_TRUE(::scl::concepts::object<plain_union>);
}

TEST(ConceptsObject, FailsForAFunctionType)
{
    STATIC_EXPECT_FALSE(::scl::concepts::object<void()>);
}

TEST(ConceptsObject, FailsForVoid) { STATIC_EXPECT_FALSE(::scl::concepts::object<void>); }

TEST(ConceptsObject, HoldsForACvQualifiedTypeOfTheCategory)
{
    STATIC_EXPECT_TRUE(::scl::concepts::object<with_const<int>>);
    STATIC_EXPECT_TRUE(::scl::concepts::object<with_volatile<int>>);
    STATIC_EXPECT_TRUE(::scl::concepts::object<with_const_volatile<int>>);
}

TEST(ConceptsObject, FailsForACvQualifiedTypeOfAnotherCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::object<with_const<void>>);
    STATIC_EXPECT_FALSE(::scl::concepts::object<with_volatile<void>>);
    STATIC_EXPECT_FALSE(::scl::concepts::object<with_const_volatile<void>>);
}

TEST(ConceptsObject, FailsForAReferenceToATypeOfTheCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::object<lvalue_reference_to<int>>);
    STATIC_EXPECT_FALSE(::scl::concepts::object<rvalue_reference_to<int>>);
}

TEST(ConceptsObject, IsUnorderedWithCompound)
{
    STATIC_EXPECT_FALSE(object_or_compound_resolves<int *>);
}

TEST(ConceptsObject, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(object_over_non_reference<int>(), 2);
}

TEST(ConceptsCompound, HoldsForAPointer) { STATIC_EXPECT_TRUE(::scl::concepts::compound<int *>); }

TEST(ConceptsCompound, HoldsForAPointerToAMember)
{
    STATIC_EXPECT_TRUE(::scl::concepts::compound<int plain_class::*>);
    STATIC_EXPECT_TRUE(::scl::concepts::compound<void (plain_class::*)()>);
}

TEST(ConceptsCompound, HoldsForAReference)
{
    STATIC_EXPECT_TRUE(::scl::concepts::compound<int &>);
    STATIC_EXPECT_TRUE(::scl::concepts::compound<int &&>);
}

TEST(ConceptsCompound, HoldsForAnArray)
{
    STATIC_EXPECT_TRUE(::scl::concepts::compound<int[3]>);
    STATIC_EXPECT_TRUE(::scl::concepts::compound<int[]>);
}

TEST(ConceptsCompound, HoldsForAFunction) { STATIC_EXPECT_TRUE(::scl::concepts::compound<void()>); }

TEST(ConceptsCompound, HoldsForAClassOrAUnion)
{
    STATIC_EXPECT_TRUE(::scl::concepts::compound<plain_class>);
    STATIC_EXPECT_TRUE(::scl::concepts::compound<plain_union>);
}

TEST(ConceptsCompound, HoldsForAnEnumeration)
{
    STATIC_EXPECT_TRUE(::scl::concepts::compound<unscoped_enum>);
    STATIC_EXPECT_TRUE(::scl::concepts::compound<scoped_class_enum>);
}

TEST(ConceptsCompound, FailsForAFundamentalType)
{
    STATIC_EXPECT_FALSE(::scl::concepts::compound<int>);
    STATIC_EXPECT_FALSE(::scl::concepts::compound<double>);
    STATIC_EXPECT_FALSE(::scl::concepts::compound<void>);
    STATIC_EXPECT_FALSE(::scl::concepts::compound<::std::nullptr_t>);
}

TEST(ConceptsCompound, HoldsForACvQualifiedTypeOfTheCategory)
{
    STATIC_EXPECT_TRUE(::scl::concepts::compound<with_const<int *>>);
    STATIC_EXPECT_TRUE(::scl::concepts::compound<with_volatile<int *>>);
    STATIC_EXPECT_TRUE(::scl::concepts::compound<with_const_volatile<int *>>);
}

TEST(ConceptsCompound, FailsForACvQualifiedTypeOfAnotherCategory)
{
    STATIC_EXPECT_FALSE(::scl::concepts::compound<with_const<int>>);
    STATIC_EXPECT_FALSE(::scl::concepts::compound<with_volatile<int>>);
    STATIC_EXPECT_FALSE(::scl::concepts::compound<with_const_volatile<int>>);
}

#if SCL_HAS_IMPLICIT_LIFETIME_CONCEPT
namespace
{
    template <::scl::concepts::implicit_lifetime Type>
    constexpr int scalar_over_implicit_lifetime()
    {
        return 1;
    }

    template <::scl::concepts::scalar Type>
    constexpr int scalar_over_implicit_lifetime()
    {
        return 2;
    }

    template <::scl::concepts::implicit_lifetime Type>
    constexpr int arithmetic_over_implicit_lifetime()
    {
        return 1;
    }

    template <::scl::concepts::arithmetic Type>
    constexpr int arithmetic_over_implicit_lifetime()
    {
        return 2;
    }

    template <::scl::concepts::implicit_lifetime Type>
    constexpr int null_pointer_over_implicit_lifetime()
    {
        return 1;
    }

    template <::scl::concepts::null_pointer Type>
    constexpr int null_pointer_over_implicit_lifetime()
    {
        return 2;
    }

    template <::scl::concepts::implicit_lifetime Type>
    constexpr int enum_type_over_implicit_lifetime()
    {
        return 1;
    }

    template <::scl::concepts::enum_type Type>
    constexpr int enum_type_over_implicit_lifetime()
    {
        return 2;
    }

    template <::scl::concepts::implicit_lifetime Type>
    constexpr int scoped_enum_over_implicit_lifetime()
    {
        return 1;
    }

    template <::scl::concepts::scoped_enum Type>
    constexpr int scoped_enum_over_implicit_lifetime()
    {
        return 2;
    }

    template <::scl::concepts::implicit_lifetime Type>
    constexpr int pointer_over_implicit_lifetime()
    {
        return 1;
    }

    template <::scl::concepts::pointer Type>
    constexpr int pointer_over_implicit_lifetime()
    {
        return 2;
    }

    template <::scl::concepts::implicit_lifetime Type>
    constexpr int member_pointer_over_implicit_lifetime()
    {
        return 1;
    }

    template <::scl::concepts::member_pointer Type>
    constexpr int member_pointer_over_implicit_lifetime()
    {
        return 2;
    }

    template <::scl::concepts::implicit_lifetime Type>
    constexpr int member_object_pointer_over_implicit_lifetime()
    {
        return 1;
    }

    template <::scl::concepts::member_object_pointer Type>
    constexpr int member_object_pointer_over_implicit_lifetime()
    {
        return 2;
    }

    template <::scl::concepts::implicit_lifetime Type>
    constexpr int member_function_pointer_over_implicit_lifetime()
    {
        return 1;
    }

    template <::scl::concepts::member_function_pointer Type>
    constexpr int member_function_pointer_over_implicit_lifetime()
    {
        return 2;
    }

    template <::scl::concepts::implicit_lifetime Type>
    constexpr int array_over_implicit_lifetime()
    {
        return 1;
    }

    template <::scl::concepts::array Type>
    constexpr int array_over_implicit_lifetime()
    {
        return 2;
    }

    template <::scl::concepts::implicit_lifetime Type>
    constexpr int bounded_array_over_implicit_lifetime()
    {
        return 1;
    }

    template <::scl::concepts::bounded_array Type>
    constexpr int bounded_array_over_implicit_lifetime()
    {
        return 2;
    }

    template <::scl::concepts::implicit_lifetime Type>
    constexpr int unbounded_array_over_implicit_lifetime()
    {
        return 1;
    }

    template <::scl::concepts::unbounded_array Type>
    constexpr int unbounded_array_over_implicit_lifetime()
    {
        return 2;
    }
} // namespace

TEST(ConceptsScalar, SubsumesImplicitLifetime)
{
    STATIC_EXPECT_EQ(scalar_over_implicit_lifetime<int>(), 2);
}

TEST(ConceptsArithmetic, SubsumesImplicitLifetime)
{
    STATIC_EXPECT_EQ(arithmetic_over_implicit_lifetime<int>(), 2);
}

TEST(ConceptsNullPointer, SubsumesImplicitLifetime)
{
    STATIC_EXPECT_EQ(null_pointer_over_implicit_lifetime<::std::nullptr_t>(), 2);
}

TEST(ConceptsEnumType, SubsumesImplicitLifetime)
{
    STATIC_EXPECT_EQ(enum_type_over_implicit_lifetime<unscoped_enum>(), 2);
}

TEST(ConceptsScopedEnum, SubsumesImplicitLifetime)
{
    STATIC_EXPECT_EQ(scoped_enum_over_implicit_lifetime<scoped_class_enum>(), 2);
}

TEST(ConceptsPointer, SubsumesImplicitLifetime)
{
    STATIC_EXPECT_EQ(pointer_over_implicit_lifetime<int *>(), 2);
}

TEST(ConceptsMemberPointer, SubsumesImplicitLifetime)
{
    STATIC_EXPECT_EQ(member_pointer_over_implicit_lifetime<int plain_class::*>(), 2);
}

TEST(ConceptsMemberObjectPointer, SubsumesImplicitLifetime)
{
    STATIC_EXPECT_EQ(member_object_pointer_over_implicit_lifetime<int plain_class::*>(), 2);
}

TEST(ConceptsMemberFunctionPointer, SubsumesImplicitLifetime)
{
    STATIC_EXPECT_EQ(member_function_pointer_over_implicit_lifetime<void (plain_class::*)()>(), 2);
}

TEST(ConceptsArray, SubsumesImplicitLifetime)
{
    STATIC_EXPECT_EQ(array_over_implicit_lifetime<int[3]>(), 2);
}

TEST(ConceptsBoundedArray, SubsumesImplicitLifetime)
{
    STATIC_EXPECT_EQ(bounded_array_over_implicit_lifetime<int[3]>(), 2);
}

TEST(ConceptsUnboundedArray, SubsumesImplicitLifetime)
{
    STATIC_EXPECT_EQ(unbounded_array_over_implicit_lifetime<int[]>(), 2);
}
#endif
