#include <gtest_utils.h>

#include <scl/utility/concepts/type_category.h>

#include <concepts>
#include <cstddef>

using namespace scl::concepts;

namespace
{
    enum Plain
    {
        a,
        b
    };

    union U
    {
        int i;
        float f;
    };

    struct S
    {};

    struct NonAggregate
    {
        explicit NonAggregate(int) {}
    };

    enum class Scoped
    {
        x,
        y
    };

    enum class ScopedSized : unsigned char
    {
        x
    };

    enum PlainSized : unsigned char
    {
        c
    };

    // A scoped_enum argument picks the second: both definitions of the concept conjoin
    // enum_type, so scoped_enum is the more constrained of the two.
    template <enum_type Enum>
    constexpr int more_constrained()
    {
        return 1;
    }

    template <scoped_enum Enum>
    constexpr int more_constrained()
    {
        return 2;
    }
} // namespace

TEST(TypeCategory, enum_type)
{
    STATIC_EXPECT_TRUE(enum_type<Plain>);
    STATIC_EXPECT_FALSE(enum_type<int>);
    STATIC_EXPECT_FALSE(enum_type<S>);
}

TEST(TypeCategory, scoped_enum)
{
    STATIC_EXPECT_TRUE(scoped_enum<Scoped>);
    STATIC_EXPECT_TRUE(scoped_enum<ScopedSized>);
    STATIC_EXPECT_FALSE(scoped_enum<Plain>);
    STATIC_EXPECT_FALSE(scoped_enum<PlainSized>);
    STATIC_EXPECT_FALSE(scoped_enum<int>);
    STATIC_EXPECT_FALSE(scoped_enum<S>);
    STATIC_EXPECT_TRUE(scoped_enum<Scoped const>);
    STATIC_EXPECT_FALSE(scoped_enum<Scoped &>);
}

TEST(TypeCategory, scoped_enum_subsumes_enum_type)
{
    STATIC_EXPECT_EQ(more_constrained<Scoped>(), 2);
    STATIC_EXPECT_EQ(more_constrained<Plain>(), 1);
}

TEST(TypeCategory, enum_value)
{
    STATIC_EXPECT_TRUE(enum_value<Plain::a>);
    STATIC_EXPECT_TRUE(enum_value<Plain::b>);
    STATIC_EXPECT_FALSE(enum_value<0>);
    STATIC_EXPECT_FALSE(enum_value<nullptr>);
}

TEST(TypeCategory, void_type)
{
    STATIC_EXPECT_TRUE(void_type<void>);
    STATIC_EXPECT_FALSE(void_type<int>);
    STATIC_EXPECT_FALSE(void_type<void *>);
}

TEST(TypeCategory, pointer)
{
    STATIC_EXPECT_TRUE(pointer<int *>);
    STATIC_EXPECT_TRUE(pointer<void *>);
    STATIC_EXPECT_TRUE(pointer<int const *>);
    STATIC_EXPECT_FALSE(pointer<int>);
    STATIC_EXPECT_FALSE(pointer<int &>);
    STATIC_EXPECT_FALSE(pointer<::std::nullptr_t>);
}

TEST(TypeCategory, null_pointer)
{
    STATIC_EXPECT_TRUE(null_pointer<::std::nullptr_t>);
    STATIC_EXPECT_FALSE(null_pointer<int *>);
    STATIC_EXPECT_FALSE(null_pointer<int>);
}

TEST(TypeCategory, member_pointer)
{
    STATIC_EXPECT_TRUE(member_pointer<int S::*>);
    STATIC_EXPECT_TRUE(member_pointer<int (S::*)()>);
    STATIC_EXPECT_FALSE(member_pointer<int *>);
    STATIC_EXPECT_FALSE(member_pointer<int>);
}

TEST(TypeCategory, member_object_pointer)
{
    STATIC_EXPECT_TRUE(member_object_pointer<int S::*>);
    STATIC_EXPECT_FALSE(member_object_pointer<int (S::*)()>);
    STATIC_EXPECT_FALSE(member_object_pointer<int *>);
}

TEST(TypeCategory, member_function_pointer)
{
    STATIC_EXPECT_TRUE(member_function_pointer<int (S::*)()>);
    STATIC_EXPECT_FALSE(member_function_pointer<int S::*>);
    STATIC_EXPECT_FALSE(member_function_pointer<int *>);
}

TEST(TypeCategory, array)
{
    STATIC_EXPECT_TRUE(array<int[3]>);
    STATIC_EXPECT_TRUE(array<int[]>);
    STATIC_EXPECT_TRUE(array<NonAggregate[2]>);
    STATIC_EXPECT_TRUE(array<NonAggregate const[]>);
    STATIC_EXPECT_FALSE(array<int>);
    STATIC_EXPECT_FALSE(array<int *>);
}

TEST(TypeCategory, bounded_array)
{
    STATIC_EXPECT_TRUE(bounded_array<int[3]>);
    STATIC_EXPECT_FALSE(bounded_array<int[]>);
    STATIC_EXPECT_FALSE(bounded_array<int>);
}

TEST(TypeCategory, unbounded_array)
{
    STATIC_EXPECT_TRUE(unbounded_array<int[]>);
    STATIC_EXPECT_FALSE(unbounded_array<int[3]>);
    STATIC_EXPECT_FALSE(unbounded_array<int>);
}

TEST(TypeCategory, function)
{
    STATIC_EXPECT_TRUE(function<void()>);
    STATIC_EXPECT_TRUE(function<int(int, double)>);
    STATIC_EXPECT_FALSE(function<int>);
    STATIC_EXPECT_FALSE(function<void (*)()>);
}

TEST(TypeCategory, class_type)
{
    STATIC_EXPECT_TRUE(class_type<S>);
    STATIC_EXPECT_FALSE(class_type<int>);
    STATIC_EXPECT_FALSE(class_type<U>);
    STATIC_EXPECT_FALSE(class_type<Plain>);
}

TEST(TypeCategory, union_type)
{
    STATIC_EXPECT_TRUE(union_type<U>);
    STATIC_EXPECT_FALSE(union_type<S>);
    STATIC_EXPECT_FALSE(union_type<int>);
}

TEST(TypeCategory, fundamental)
{
    STATIC_EXPECT_TRUE(fundamental<int>);
    STATIC_EXPECT_TRUE(fundamental<double>);
    STATIC_EXPECT_TRUE(fundamental<void>);
    STATIC_EXPECT_TRUE(fundamental<::std::nullptr_t>);
    STATIC_EXPECT_FALSE(fundamental<S>);
    STATIC_EXPECT_FALSE(fundamental<int *>);
}

TEST(TypeCategory, arithmetic)
{
    STATIC_EXPECT_TRUE(arithmetic<int>);
    STATIC_EXPECT_TRUE(arithmetic<double>);
    STATIC_EXPECT_TRUE(arithmetic<bool>);
    STATIC_EXPECT_FALSE(arithmetic<void>);
    STATIC_EXPECT_FALSE(arithmetic<int *>);
    STATIC_EXPECT_FALSE(arithmetic<S>);
}

TEST(TypeCategory, scalar)
{
    STATIC_EXPECT_TRUE(scalar<int>);
    STATIC_EXPECT_TRUE(scalar<int *>);
    STATIC_EXPECT_TRUE(scalar<Plain>);
    STATIC_EXPECT_TRUE(scalar<::std::nullptr_t>);
    STATIC_EXPECT_TRUE(scalar<int volatile>);
    STATIC_EXPECT_TRUE(scalar<int const volatile>);
    STATIC_EXPECT_TRUE(scalar<int * volatile>);
    STATIC_EXPECT_TRUE(scalar<Plain volatile>);
    STATIC_EXPECT_TRUE(scalar<int S::* volatile>);
    STATIC_EXPECT_FALSE(scalar<S>);
    STATIC_EXPECT_FALSE(scalar<void>);
    STATIC_EXPECT_FALSE(scalar<void()>);
}

TEST(TypeCategory, object)
{
    STATIC_EXPECT_TRUE(object<int>);
    STATIC_EXPECT_TRUE(object<S>);
    STATIC_EXPECT_TRUE(object<int *>);
    STATIC_EXPECT_TRUE(object<int[3]>);
    STATIC_EXPECT_FALSE(object<void>);
    STATIC_EXPECT_FALSE(object<void()>);
    STATIC_EXPECT_FALSE(object<int &>);
}

TEST(TypeCategory, compound)
{
    STATIC_EXPECT_TRUE(compound<int *>);
    STATIC_EXPECT_TRUE(compound<S>);
    STATIC_EXPECT_TRUE(compound<int[3]>);
    STATIC_EXPECT_TRUE(compound<void()>);
    STATIC_EXPECT_FALSE(compound<int>);
    STATIC_EXPECT_FALSE(compound<void>);
    STATIC_EXPECT_FALSE(compound<double>);
}

namespace
{
    template <object Type>
    constexpr int scalar_over_object()
    {
        return 1;
    }

    template <scalar Type>
    constexpr int scalar_over_object()
    {
        return 2;
    }

    template <fundamental Type>
    constexpr int arithmetic_over_fundamental()
    {
        return 1;
    }

    template <arithmetic Type>
    constexpr int arithmetic_over_fundamental()
    {
        return 2;
    }

    template <scalar Type>
    constexpr int arithmetic_over_scalar()
    {
        return 1;
    }

    template <arithmetic Type>
    constexpr int arithmetic_over_scalar()
    {
        return 2;
    }

    template <fundamental Type>
    constexpr int null_pointer_over_fundamental()
    {
        return 1;
    }

    template <null_pointer Type>
    constexpr int null_pointer_over_fundamental()
    {
        return 2;
    }

    template <scalar Type>
    constexpr int null_pointer_over_scalar()
    {
        return 1;
    }

    template <null_pointer Type>
    constexpr int null_pointer_over_scalar()
    {
        return 2;
    }

    template <fundamental Type>
    constexpr int void_type_over_fundamental()
    {
        return 1;
    }

    template <void_type Type>
    constexpr int void_type_over_fundamental()
    {
        return 2;
    }

    template <scalar Type>
    constexpr int enum_type_over_scalar()
    {
        return 1;
    }

    template <enum_type Type>
    constexpr int enum_type_over_scalar()
    {
        return 2;
    }

    template <compound Type>
    constexpr int enum_type_over_compound()
    {
        return 1;
    }

    template <enum_type Type>
    constexpr int enum_type_over_compound()
    {
        return 2;
    }

    template <scalar Type>
    constexpr int pointer_over_scalar()
    {
        return 1;
    }

    template <pointer Type>
    constexpr int pointer_over_scalar()
    {
        return 2;
    }

    template <compound Type>
    constexpr int pointer_over_compound()
    {
        return 1;
    }

    template <pointer Type>
    constexpr int pointer_over_compound()
    {
        return 2;
    }

    template <scalar Type>
    constexpr int member_pointer_over_scalar()
    {
        return 1;
    }

    template <member_pointer Type>
    constexpr int member_pointer_over_scalar()
    {
        return 2;
    }

    template <compound Type>
    constexpr int member_pointer_over_compound()
    {
        return 1;
    }

    template <member_pointer Type>
    constexpr int member_pointer_over_compound()
    {
        return 2;
    }

    template <member_pointer Type>
    constexpr int member_object_pointer_over_member_pointer()
    {
        return 1;
    }

    template <member_object_pointer Type>
    constexpr int member_object_pointer_over_member_pointer()
    {
        return 2;
    }

    template <member_pointer Type>
    constexpr int member_function_pointer_over_member_pointer()
    {
        return 1;
    }

    template <member_function_pointer Type>
    constexpr int member_function_pointer_over_member_pointer()
    {
        return 2;
    }

    template <object Type>
    constexpr int array_over_object()
    {
        return 1;
    }

    template <array Type>
    constexpr int array_over_object()
    {
        return 2;
    }

    template <compound Type>
    constexpr int array_over_compound()
    {
        return 1;
    }

    template <array Type>
    constexpr int array_over_compound()
    {
        return 2;
    }

    template <array Type>
    constexpr int bounded_array_over_array()
    {
        return 1;
    }

    template <bounded_array Type>
    constexpr int bounded_array_over_array()
    {
        return 2;
    }

    template <array Type>
    constexpr int unbounded_array_over_array()
    {
        return 1;
    }

    template <unbounded_array Type>
    constexpr int unbounded_array_over_array()
    {
        return 2;
    }

    template <compound Type>
    constexpr int function_over_compound()
    {
        return 1;
    }

    template <function Type>
    constexpr int function_over_compound()
    {
        return 2;
    }

    template <object Type>
    constexpr int class_type_over_object()
    {
        return 1;
    }

    template <class_type Type>
    constexpr int class_type_over_object()
    {
        return 2;
    }

    template <compound Type>
    constexpr int class_type_over_compound()
    {
        return 1;
    }

    template <class_type Type>
    constexpr int class_type_over_compound()
    {
        return 2;
    }

    template <object Type>
    constexpr int union_type_over_object()
    {
        return 1;
    }

    template <union_type Type>
    constexpr int union_type_over_object()
    {
        return 2;
    }

    template <compound Type>
    constexpr int union_type_over_compound()
    {
        return 1;
    }

    template <union_type Type>
    constexpr int union_type_over_compound()
    {
        return 2;
    }
} // namespace

TEST(TypeCategory, scalar_subsumes_object)
{
    STATIC_EXPECT_EQ(scalar_over_object<int>(), 2);
    STATIC_EXPECT_EQ(scalar_over_object<S>(), 1);
}

TEST(TypeCategory, arithmetic_subsumes_fundamental)
{
    STATIC_EXPECT_EQ(arithmetic_over_fundamental<int>(), 2);
    STATIC_EXPECT_EQ(arithmetic_over_fundamental<void>(), 1);
}

TEST(TypeCategory, arithmetic_subsumes_scalar)
{
    STATIC_EXPECT_EQ(arithmetic_over_scalar<int>(), 2);
    STATIC_EXPECT_EQ(arithmetic_over_scalar<int *>(), 1);
}

TEST(TypeCategory, null_pointer_subsumes_fundamental)
{
    STATIC_EXPECT_EQ(null_pointer_over_fundamental<::std::nullptr_t>(), 2);
    STATIC_EXPECT_EQ(null_pointer_over_fundamental<int>(), 1);
}

TEST(TypeCategory, null_pointer_subsumes_scalar)
{
    STATIC_EXPECT_EQ(null_pointer_over_scalar<::std::nullptr_t>(), 2);
    STATIC_EXPECT_EQ(null_pointer_over_scalar<int>(), 1);
}

TEST(TypeCategory, void_type_subsumes_fundamental)
{
    STATIC_EXPECT_EQ(void_type_over_fundamental<void>(), 2);
    STATIC_EXPECT_EQ(void_type_over_fundamental<int>(), 1);
}

TEST(TypeCategory, enum_type_subsumes_scalar)
{
    STATIC_EXPECT_EQ(enum_type_over_scalar<Plain>(), 2);
    STATIC_EXPECT_EQ(enum_type_over_scalar<int>(), 1);
}

TEST(TypeCategory, enum_type_subsumes_compound)
{
    STATIC_EXPECT_EQ(enum_type_over_compound<Plain>(), 2);
    STATIC_EXPECT_EQ(enum_type_over_compound<S>(), 1);
}

TEST(TypeCategory, pointer_subsumes_scalar)
{
    STATIC_EXPECT_EQ(pointer_over_scalar<int *>(), 2);
    STATIC_EXPECT_EQ(pointer_over_scalar<int>(), 1);
}

TEST(TypeCategory, pointer_subsumes_compound)
{
    STATIC_EXPECT_EQ(pointer_over_compound<int *>(), 2);
    STATIC_EXPECT_EQ(pointer_over_compound<S>(), 1);
}

TEST(TypeCategory, member_pointer_subsumes_scalar)
{
    STATIC_EXPECT_EQ(member_pointer_over_scalar<int S::*>(), 2);
    STATIC_EXPECT_EQ(member_pointer_over_scalar<int>(), 1);
}

TEST(TypeCategory, member_pointer_subsumes_compound)
{
    STATIC_EXPECT_EQ(member_pointer_over_compound<int S::*>(), 2);
    STATIC_EXPECT_EQ(member_pointer_over_compound<S>(), 1);
}

TEST(TypeCategory, member_object_pointer_subsumes_member_pointer)
{
    STATIC_EXPECT_EQ(member_object_pointer_over_member_pointer<int S::*>(), 2);
    STATIC_EXPECT_EQ(member_object_pointer_over_member_pointer<int (S::*)()>(), 1);
}

TEST(TypeCategory, member_function_pointer_subsumes_member_pointer)
{
    STATIC_EXPECT_EQ(member_function_pointer_over_member_pointer<int (S::*)()>(), 2);
    STATIC_EXPECT_EQ(member_function_pointer_over_member_pointer<int S::*>(), 1);
}

TEST(TypeCategory, array_subsumes_object)
{
    STATIC_EXPECT_EQ(array_over_object<int[3]>(), 2);
    STATIC_EXPECT_EQ(array_over_object<int>(), 1);
}

TEST(TypeCategory, array_subsumes_compound)
{
    STATIC_EXPECT_EQ(array_over_compound<int[3]>(), 2);
    STATIC_EXPECT_EQ(array_over_compound<S>(), 1);
}

TEST(TypeCategory, bounded_array_subsumes_array)
{
    STATIC_EXPECT_EQ(bounded_array_over_array<int[3]>(), 2);
    STATIC_EXPECT_EQ(bounded_array_over_array<int[]>(), 1);
}

TEST(TypeCategory, unbounded_array_subsumes_array)
{
    STATIC_EXPECT_EQ(unbounded_array_over_array<int[]>(), 2);
    STATIC_EXPECT_EQ(unbounded_array_over_array<int[3]>(), 1);
}

TEST(TypeCategory, function_subsumes_compound)
{
    STATIC_EXPECT_EQ(function_over_compound<void()>(), 2);
    STATIC_EXPECT_EQ(function_over_compound<S>(), 1);
}

TEST(TypeCategory, class_type_subsumes_object)
{
    STATIC_EXPECT_EQ(class_type_over_object<S>(), 2);
    STATIC_EXPECT_EQ(class_type_over_object<int>(), 1);
}

TEST(TypeCategory, class_type_subsumes_compound)
{
    STATIC_EXPECT_EQ(class_type_over_compound<S>(), 2);
    STATIC_EXPECT_EQ(class_type_over_compound<int *>(), 1);
}

TEST(TypeCategory, union_type_subsumes_object)
{
    STATIC_EXPECT_EQ(union_type_over_object<U>(), 2);
    STATIC_EXPECT_EQ(union_type_over_object<int>(), 1);
}

TEST(TypeCategory, union_type_subsumes_compound)
{
    STATIC_EXPECT_EQ(union_type_over_compound<U>(), 2);
    STATIC_EXPECT_EQ(union_type_over_compound<S>(), 1);
}

namespace
{
    template <non_reference Type>
    constexpr int object_over_non_reference()
    {
        return 1;
    }

    template <object Type>
    constexpr int object_over_non_reference()
    {
        return 2;
    }

    template <non_reference Type>
    constexpr int scalar_over_non_reference()
    {
        return 1;
    }

    template <scalar Type>
    constexpr int scalar_over_non_reference()
    {
        return 2;
    }

    template <object Type>
    constexpr int arithmetic_over_object()
    {
        return 1;
    }

    template <arithmetic Type>
    constexpr int arithmetic_over_object()
    {
        return 2;
    }

    template <non_reference Type>
    constexpr int arithmetic_over_non_reference()
    {
        return 1;
    }

    template <arithmetic Type>
    constexpr int arithmetic_over_non_reference()
    {
        return 2;
    }

    template <non_reference Type>
    constexpr int void_type_over_non_reference()
    {
        return 1;
    }

    template <void_type Type>
    constexpr int void_type_over_non_reference()
    {
        return 2;
    }

    template <object Type>
    constexpr int null_pointer_over_object()
    {
        return 1;
    }

    template <null_pointer Type>
    constexpr int null_pointer_over_object()
    {
        return 2;
    }

    template <non_reference Type>
    constexpr int null_pointer_over_non_reference()
    {
        return 1;
    }

    template <null_pointer Type>
    constexpr int null_pointer_over_non_reference()
    {
        return 2;
    }

    template <object Type>
    constexpr int enum_type_over_object()
    {
        return 1;
    }

    template <enum_type Type>
    constexpr int enum_type_over_object()
    {
        return 2;
    }

    template <non_reference Type>
    constexpr int enum_type_over_non_reference()
    {
        return 1;
    }

    template <enum_type Type>
    constexpr int enum_type_over_non_reference()
    {
        return 2;
    }

    template <scalar Type>
    constexpr int scoped_enum_over_scalar()
    {
        return 1;
    }

    template <scoped_enum Type>
    constexpr int scoped_enum_over_scalar()
    {
        return 2;
    }

    template <object Type>
    constexpr int scoped_enum_over_object()
    {
        return 1;
    }

    template <scoped_enum Type>
    constexpr int scoped_enum_over_object()
    {
        return 2;
    }

    template <non_reference Type>
    constexpr int scoped_enum_over_non_reference()
    {
        return 1;
    }

    template <scoped_enum Type>
    constexpr int scoped_enum_over_non_reference()
    {
        return 2;
    }

    template <compound Type>
    constexpr int scoped_enum_over_compound()
    {
        return 1;
    }

    template <scoped_enum Type>
    constexpr int scoped_enum_over_compound()
    {
        return 2;
    }

    template <object Type>
    constexpr int pointer_over_object()
    {
        return 1;
    }

    template <pointer Type>
    constexpr int pointer_over_object()
    {
        return 2;
    }

    template <non_reference Type>
    constexpr int pointer_over_non_reference()
    {
        return 1;
    }

    template <pointer Type>
    constexpr int pointer_over_non_reference()
    {
        return 2;
    }

    template <object Type>
    constexpr int member_pointer_over_object()
    {
        return 1;
    }

    template <member_pointer Type>
    constexpr int member_pointer_over_object()
    {
        return 2;
    }

    template <non_reference Type>
    constexpr int member_pointer_over_non_reference()
    {
        return 1;
    }

    template <member_pointer Type>
    constexpr int member_pointer_over_non_reference()
    {
        return 2;
    }

    template <scalar Type>
    constexpr int member_object_pointer_over_scalar()
    {
        return 1;
    }

    template <member_object_pointer Type>
    constexpr int member_object_pointer_over_scalar()
    {
        return 2;
    }

    template <object Type>
    constexpr int member_object_pointer_over_object()
    {
        return 1;
    }

    template <member_object_pointer Type>
    constexpr int member_object_pointer_over_object()
    {
        return 2;
    }

    template <non_reference Type>
    constexpr int member_object_pointer_over_non_reference()
    {
        return 1;
    }

    template <member_object_pointer Type>
    constexpr int member_object_pointer_over_non_reference()
    {
        return 2;
    }

    template <compound Type>
    constexpr int member_object_pointer_over_compound()
    {
        return 1;
    }

    template <member_object_pointer Type>
    constexpr int member_object_pointer_over_compound()
    {
        return 2;
    }

    template <scalar Type>
    constexpr int member_function_pointer_over_scalar()
    {
        return 1;
    }

    template <member_function_pointer Type>
    constexpr int member_function_pointer_over_scalar()
    {
        return 2;
    }

    template <object Type>
    constexpr int member_function_pointer_over_object()
    {
        return 1;
    }

    template <member_function_pointer Type>
    constexpr int member_function_pointer_over_object()
    {
        return 2;
    }

    template <non_reference Type>
    constexpr int member_function_pointer_over_non_reference()
    {
        return 1;
    }

    template <member_function_pointer Type>
    constexpr int member_function_pointer_over_non_reference()
    {
        return 2;
    }

    template <compound Type>
    constexpr int member_function_pointer_over_compound()
    {
        return 1;
    }

    template <member_function_pointer Type>
    constexpr int member_function_pointer_over_compound()
    {
        return 2;
    }

    template <non_reference Type>
    constexpr int array_over_non_reference()
    {
        return 1;
    }

    template <array Type>
    constexpr int array_over_non_reference()
    {
        return 2;
    }

    template <object Type>
    constexpr int bounded_array_over_object()
    {
        return 1;
    }

    template <bounded_array Type>
    constexpr int bounded_array_over_object()
    {
        return 2;
    }

    template <non_reference Type>
    constexpr int bounded_array_over_non_reference()
    {
        return 1;
    }

    template <bounded_array Type>
    constexpr int bounded_array_over_non_reference()
    {
        return 2;
    }

    template <compound Type>
    constexpr int bounded_array_over_compound()
    {
        return 1;
    }

    template <bounded_array Type>
    constexpr int bounded_array_over_compound()
    {
        return 2;
    }

    template <object Type>
    constexpr int unbounded_array_over_object()
    {
        return 1;
    }

    template <unbounded_array Type>
    constexpr int unbounded_array_over_object()
    {
        return 2;
    }

    template <non_reference Type>
    constexpr int unbounded_array_over_non_reference()
    {
        return 1;
    }

    template <unbounded_array Type>
    constexpr int unbounded_array_over_non_reference()
    {
        return 2;
    }

    template <compound Type>
    constexpr int unbounded_array_over_compound()
    {
        return 1;
    }

    template <unbounded_array Type>
    constexpr int unbounded_array_over_compound()
    {
        return 2;
    }

    template <non_reference Type>
    constexpr int function_over_non_reference()
    {
        return 1;
    }

    template <function Type>
    constexpr int function_over_non_reference()
    {
        return 2;
    }

    template <non_reference Type>
    constexpr int class_type_over_non_reference()
    {
        return 1;
    }

    template <class_type Type>
    constexpr int class_type_over_non_reference()
    {
        return 2;
    }

    template <non_reference Type>
    constexpr int union_type_over_non_reference()
    {
        return 1;
    }

    template <union_type Type>
    constexpr int union_type_over_non_reference()
    {
        return 2;
    }
} // namespace

TEST(TypeCategory, object_subsumes_non_reference)
{
    STATIC_EXPECT_EQ(object_over_non_reference<int>(), 2);
    STATIC_EXPECT_EQ(object_over_non_reference<void>(), 1);
}

TEST(TypeCategory, scalar_subsumes_non_reference)
{
    STATIC_EXPECT_EQ(scalar_over_non_reference<int>(), 2);
    STATIC_EXPECT_EQ(scalar_over_non_reference<void>(), 1);
}

TEST(TypeCategory, arithmetic_subsumes_object)
{
    STATIC_EXPECT_EQ(arithmetic_over_object<int>(), 2);
    STATIC_EXPECT_EQ(arithmetic_over_object<::std::nullptr_t>(), 1);
}

TEST(TypeCategory, arithmetic_subsumes_non_reference)
{
    STATIC_EXPECT_EQ(arithmetic_over_non_reference<int>(), 2);
    STATIC_EXPECT_EQ(arithmetic_over_non_reference<void>(), 1);
}

TEST(TypeCategory, void_type_subsumes_non_reference)
{
    STATIC_EXPECT_EQ(void_type_over_non_reference<void>(), 2);
    STATIC_EXPECT_EQ(void_type_over_non_reference<int>(), 1);
}

TEST(TypeCategory, null_pointer_subsumes_object)
{
    STATIC_EXPECT_EQ(null_pointer_over_object<::std::nullptr_t>(), 2);
    STATIC_EXPECT_EQ(null_pointer_over_object<int>(), 1);
}

TEST(TypeCategory, null_pointer_subsumes_non_reference)
{
    STATIC_EXPECT_EQ(null_pointer_over_non_reference<::std::nullptr_t>(), 2);
    STATIC_EXPECT_EQ(null_pointer_over_non_reference<int>(), 1);
}

TEST(TypeCategory, enum_type_subsumes_object)
{
    STATIC_EXPECT_EQ(enum_type_over_object<Plain>(), 2);
    STATIC_EXPECT_EQ(enum_type_over_object<int>(), 1);
}

TEST(TypeCategory, enum_type_subsumes_non_reference)
{
    STATIC_EXPECT_EQ(enum_type_over_non_reference<Plain>(), 2);
    STATIC_EXPECT_EQ(enum_type_over_non_reference<int>(), 1);
}

TEST(TypeCategory, scoped_enum_subsumes_scalar)
{
    STATIC_EXPECT_EQ(scoped_enum_over_scalar<Scoped>(), 2);
    STATIC_EXPECT_EQ(scoped_enum_over_scalar<int>(), 1);
}

TEST(TypeCategory, scoped_enum_subsumes_object)
{
    STATIC_EXPECT_EQ(scoped_enum_over_object<Scoped>(), 2);
    STATIC_EXPECT_EQ(scoped_enum_over_object<int>(), 1);
}

TEST(TypeCategory, scoped_enum_subsumes_non_reference)
{
    STATIC_EXPECT_EQ(scoped_enum_over_non_reference<Scoped>(), 2);
    STATIC_EXPECT_EQ(scoped_enum_over_non_reference<int>(), 1);
}

TEST(TypeCategory, scoped_enum_subsumes_compound)
{
    STATIC_EXPECT_EQ(scoped_enum_over_compound<Scoped>(), 2);
    STATIC_EXPECT_EQ(scoped_enum_over_compound<Plain>(), 1);
}

TEST(TypeCategory, pointer_subsumes_object)
{
    STATIC_EXPECT_EQ(pointer_over_object<int *>(), 2);
    STATIC_EXPECT_EQ(pointer_over_object<int>(), 1);
}

TEST(TypeCategory, pointer_subsumes_non_reference)
{
    STATIC_EXPECT_EQ(pointer_over_non_reference<int *>(), 2);
    STATIC_EXPECT_EQ(pointer_over_non_reference<int>(), 1);
}

TEST(TypeCategory, member_pointer_subsumes_object)
{
    STATIC_EXPECT_EQ(member_pointer_over_object<int S::*>(), 2);
    STATIC_EXPECT_EQ(member_pointer_over_object<int>(), 1);
}

TEST(TypeCategory, member_pointer_subsumes_non_reference)
{
    STATIC_EXPECT_EQ(member_pointer_over_non_reference<int S::*>(), 2);
    STATIC_EXPECT_EQ(member_pointer_over_non_reference<int>(), 1);
}

TEST(TypeCategory, member_object_pointer_subsumes_scalar)
{
    STATIC_EXPECT_EQ(member_object_pointer_over_scalar<int S::*>(), 2);
    STATIC_EXPECT_EQ(member_object_pointer_over_scalar<int>(), 1);
}

TEST(TypeCategory, member_object_pointer_subsumes_object)
{
    STATIC_EXPECT_EQ(member_object_pointer_over_object<int S::*>(), 2);
    STATIC_EXPECT_EQ(member_object_pointer_over_object<int>(), 1);
}

TEST(TypeCategory, member_object_pointer_subsumes_non_reference)
{
    STATIC_EXPECT_EQ(member_object_pointer_over_non_reference<int S::*>(), 2);
    STATIC_EXPECT_EQ(member_object_pointer_over_non_reference<int>(), 1);
}

TEST(TypeCategory, member_object_pointer_subsumes_compound)
{
    STATIC_EXPECT_EQ(member_object_pointer_over_compound<int S::*>(), 2);
    STATIC_EXPECT_EQ(member_object_pointer_over_compound<Plain>(), 1);
}

TEST(TypeCategory, member_function_pointer_subsumes_scalar)
{
    STATIC_EXPECT_EQ(member_function_pointer_over_scalar<int (S::*)()>(), 2);
    STATIC_EXPECT_EQ(member_function_pointer_over_scalar<int>(), 1);
}

TEST(TypeCategory, member_function_pointer_subsumes_object)
{
    STATIC_EXPECT_EQ(member_function_pointer_over_object<int (S::*)()>(), 2);
    STATIC_EXPECT_EQ(member_function_pointer_over_object<int>(), 1);
}

TEST(TypeCategory, member_function_pointer_subsumes_non_reference)
{
    STATIC_EXPECT_EQ(member_function_pointer_over_non_reference<int (S::*)()>(), 2);
    STATIC_EXPECT_EQ(member_function_pointer_over_non_reference<int>(), 1);
}

TEST(TypeCategory, member_function_pointer_subsumes_compound)
{
    STATIC_EXPECT_EQ(member_function_pointer_over_compound<int (S::*)()>(), 2);
    STATIC_EXPECT_EQ(member_function_pointer_over_compound<Plain>(), 1);
}

TEST(TypeCategory, array_subsumes_non_reference)
{
    STATIC_EXPECT_EQ(array_over_non_reference<int[3]>(), 2);
    STATIC_EXPECT_EQ(array_over_non_reference<int>(), 1);
}

TEST(TypeCategory, bounded_array_subsumes_object)
{
    STATIC_EXPECT_EQ(bounded_array_over_object<int[3]>(), 2);
    STATIC_EXPECT_EQ(bounded_array_over_object<int>(), 1);
}

TEST(TypeCategory, bounded_array_subsumes_non_reference)
{
    STATIC_EXPECT_EQ(bounded_array_over_non_reference<int[3]>(), 2);
    STATIC_EXPECT_EQ(bounded_array_over_non_reference<int>(), 1);
}

TEST(TypeCategory, bounded_array_subsumes_compound)
{
    STATIC_EXPECT_EQ(bounded_array_over_compound<int[3]>(), 2);
    STATIC_EXPECT_EQ(bounded_array_over_compound<Plain>(), 1);
}

TEST(TypeCategory, unbounded_array_subsumes_object)
{
    STATIC_EXPECT_EQ(unbounded_array_over_object<int[]>(), 2);
    STATIC_EXPECT_EQ(unbounded_array_over_object<int>(), 1);
}

TEST(TypeCategory, unbounded_array_subsumes_non_reference)
{
    STATIC_EXPECT_EQ(unbounded_array_over_non_reference<int[]>(), 2);
    STATIC_EXPECT_EQ(unbounded_array_over_non_reference<int>(), 1);
}

TEST(TypeCategory, unbounded_array_subsumes_compound)
{
    STATIC_EXPECT_EQ(unbounded_array_over_compound<int[]>(), 2);
    STATIC_EXPECT_EQ(unbounded_array_over_compound<Plain>(), 1);
}

TEST(TypeCategory, function_subsumes_non_reference)
{
    STATIC_EXPECT_EQ(function_over_non_reference<void()>(), 2);
    STATIC_EXPECT_EQ(function_over_non_reference<int>(), 1);
}

TEST(TypeCategory, class_type_subsumes_non_reference)
{
    STATIC_EXPECT_EQ(class_type_over_non_reference<S>(), 2);
    STATIC_EXPECT_EQ(class_type_over_non_reference<int>(), 1);
}

TEST(TypeCategory, union_type_subsumes_non_reference)
{
    STATIC_EXPECT_EQ(union_type_over_non_reference<U>(), 2);
    STATIC_EXPECT_EQ(union_type_over_non_reference<int>(), 1);
}

namespace
{
    template <object Type>
    constexpr int object_or_compound()
    {
        return 1;
    }

    template <compound Type>
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

    template <arithmetic Type>
    constexpr int integral_or_arithmetic()
    {
        return 2;
    }

    template <typename Type>
    constexpr bool integral_or_arithmetic_resolves = requires { integral_or_arithmetic<Type>(); };
} // namespace

TEST(TypeCategory, object_and_compound_are_unordered)
{
    STATIC_EXPECT_FALSE(object_or_compound_resolves<int *>);
    STATIC_EXPECT_TRUE(object_or_compound_resolves<int>);
}

TEST(TypeCategory, std_integral_and_arithmetic_are_unordered)
{
    STATIC_EXPECT_FALSE(integral_or_arithmetic_resolves<int>);
    STATIC_EXPECT_TRUE(integral_or_arithmetic_resolves<double>);
}

namespace
{
    template <trivially_copyable Type>
    constexpr int scalar_over_trivially_copyable()
    {
        return 1;
    }

    template <scalar Type>
    constexpr int scalar_over_trivially_copyable()
    {
        return 2;
    }

    template <standard_layout Type>
    constexpr int scalar_over_standard_layout()
    {
        return 1;
    }

    template <scalar Type>
    constexpr int scalar_over_standard_layout()
    {
        return 2;
    }

    template <trivially_copyable Type>
    constexpr int arithmetic_over_trivially_copyable()
    {
        return 1;
    }

    template <arithmetic Type>
    constexpr int arithmetic_over_trivially_copyable()
    {
        return 2;
    }

    template <standard_layout Type>
    constexpr int arithmetic_over_standard_layout()
    {
        return 1;
    }

    template <arithmetic Type>
    constexpr int arithmetic_over_standard_layout()
    {
        return 2;
    }

    template <trivially_copyable Type>
    constexpr int null_pointer_over_trivially_copyable()
    {
        return 1;
    }

    template <null_pointer Type>
    constexpr int null_pointer_over_trivially_copyable()
    {
        return 2;
    }

    template <standard_layout Type>
    constexpr int null_pointer_over_standard_layout()
    {
        return 1;
    }

    template <null_pointer Type>
    constexpr int null_pointer_over_standard_layout()
    {
        return 2;
    }

    template <trivially_copyable Type>
    constexpr int enum_type_over_trivially_copyable()
    {
        return 1;
    }

    template <enum_type Type>
    constexpr int enum_type_over_trivially_copyable()
    {
        return 2;
    }

    template <standard_layout Type>
    constexpr int enum_type_over_standard_layout()
    {
        return 1;
    }

    template <enum_type Type>
    constexpr int enum_type_over_standard_layout()
    {
        return 2;
    }

    template <trivially_copyable Type>
    constexpr int scoped_enum_over_trivially_copyable()
    {
        return 1;
    }

    template <scoped_enum Type>
    constexpr int scoped_enum_over_trivially_copyable()
    {
        return 2;
    }

    template <standard_layout Type>
    constexpr int scoped_enum_over_standard_layout()
    {
        return 1;
    }

    template <scoped_enum Type>
    constexpr int scoped_enum_over_standard_layout()
    {
        return 2;
    }

    template <trivially_copyable Type>
    constexpr int pointer_over_trivially_copyable()
    {
        return 1;
    }

    template <pointer Type>
    constexpr int pointer_over_trivially_copyable()
    {
        return 2;
    }

    template <standard_layout Type>
    constexpr int pointer_over_standard_layout()
    {
        return 1;
    }

    template <pointer Type>
    constexpr int pointer_over_standard_layout()
    {
        return 2;
    }

    template <trivially_copyable Type>
    constexpr int member_pointer_over_trivially_copyable()
    {
        return 1;
    }

    template <member_pointer Type>
    constexpr int member_pointer_over_trivially_copyable()
    {
        return 2;
    }

    template <standard_layout Type>
    constexpr int member_pointer_over_standard_layout()
    {
        return 1;
    }

    template <member_pointer Type>
    constexpr int member_pointer_over_standard_layout()
    {
        return 2;
    }

    template <trivially_copyable Type>
    constexpr int member_object_pointer_over_trivially_copyable()
    {
        return 1;
    }

    template <member_object_pointer Type>
    constexpr int member_object_pointer_over_trivially_copyable()
    {
        return 2;
    }

    template <standard_layout Type>
    constexpr int member_object_pointer_over_standard_layout()
    {
        return 1;
    }

    template <member_object_pointer Type>
    constexpr int member_object_pointer_over_standard_layout()
    {
        return 2;
    }

    template <trivially_copyable Type>
    constexpr int member_function_pointer_over_trivially_copyable()
    {
        return 1;
    }

    template <member_function_pointer Type>
    constexpr int member_function_pointer_over_trivially_copyable()
    {
        return 2;
    }

    template <standard_layout Type>
    constexpr int member_function_pointer_over_standard_layout()
    {
        return 1;
    }

    template <member_function_pointer Type>
    constexpr int member_function_pointer_over_standard_layout()
    {
        return 2;
    }

    template <aggregate Type>
    constexpr int array_over_aggregate()
    {
        return 1;
    }

    template <array Type>
    constexpr int array_over_aggregate()
    {
        return 2;
    }

    template <aggregate Type>
    constexpr int bounded_array_over_aggregate()
    {
        return 1;
    }

    template <bounded_array Type>
    constexpr int bounded_array_over_aggregate()
    {
        return 2;
    }

    template <aggregate Type>
    constexpr int unbounded_array_over_aggregate()
    {
        return 1;
    }

    template <unbounded_array Type>
    constexpr int unbounded_array_over_aggregate()
    {
        return 2;
    }
} // namespace

TEST(TypeCategory, scalar_subsumes_trivially_copyable)
{
    STATIC_EXPECT_EQ(scalar_over_trivially_copyable<int>(), 2);
    STATIC_EXPECT_EQ(scalar_over_trivially_copyable<int[3]>(), 1);
}

TEST(TypeCategory, scalar_subsumes_standard_layout)
{
    STATIC_EXPECT_EQ(scalar_over_standard_layout<int>(), 2);
    STATIC_EXPECT_EQ(scalar_over_standard_layout<int[3]>(), 1);
}

TEST(TypeCategory, arithmetic_subsumes_trivially_copyable)
{
    STATIC_EXPECT_EQ(arithmetic_over_trivially_copyable<int>(), 2);
    STATIC_EXPECT_EQ(arithmetic_over_trivially_copyable<::std::nullptr_t>(), 1);
}

TEST(TypeCategory, arithmetic_subsumes_standard_layout)
{
    STATIC_EXPECT_EQ(arithmetic_over_standard_layout<int>(), 2);
    STATIC_EXPECT_EQ(arithmetic_over_standard_layout<::std::nullptr_t>(), 1);
}

TEST(TypeCategory, null_pointer_subsumes_trivially_copyable)
{
    STATIC_EXPECT_EQ(null_pointer_over_trivially_copyable<::std::nullptr_t>(), 2);
    STATIC_EXPECT_EQ(null_pointer_over_trivially_copyable<int>(), 1);
}

TEST(TypeCategory, null_pointer_subsumes_standard_layout)
{
    STATIC_EXPECT_EQ(null_pointer_over_standard_layout<::std::nullptr_t>(), 2);
    STATIC_EXPECT_EQ(null_pointer_over_standard_layout<int>(), 1);
}

TEST(TypeCategory, enum_type_subsumes_trivially_copyable)
{
    STATIC_EXPECT_EQ(enum_type_over_trivially_copyable<Plain>(), 2);
    STATIC_EXPECT_EQ(enum_type_over_trivially_copyable<int>(), 1);
}

TEST(TypeCategory, enum_type_subsumes_standard_layout)
{
    STATIC_EXPECT_EQ(enum_type_over_standard_layout<Plain>(), 2);
    STATIC_EXPECT_EQ(enum_type_over_standard_layout<int>(), 1);
}

TEST(TypeCategory, scoped_enum_subsumes_trivially_copyable)
{
    STATIC_EXPECT_EQ(scoped_enum_over_trivially_copyable<Scoped>(), 2);
    STATIC_EXPECT_EQ(scoped_enum_over_trivially_copyable<int>(), 1);
}

TEST(TypeCategory, scoped_enum_subsumes_standard_layout)
{
    STATIC_EXPECT_EQ(scoped_enum_over_standard_layout<Scoped>(), 2);
    STATIC_EXPECT_EQ(scoped_enum_over_standard_layout<int>(), 1);
}

TEST(TypeCategory, pointer_subsumes_trivially_copyable)
{
    STATIC_EXPECT_EQ(pointer_over_trivially_copyable<int *>(), 2);
    STATIC_EXPECT_EQ(pointer_over_trivially_copyable<int>(), 1);
}

TEST(TypeCategory, pointer_subsumes_standard_layout)
{
    STATIC_EXPECT_EQ(pointer_over_standard_layout<int *>(), 2);
    STATIC_EXPECT_EQ(pointer_over_standard_layout<int>(), 1);
}

TEST(TypeCategory, member_pointer_subsumes_trivially_copyable)
{
    STATIC_EXPECT_EQ(member_pointer_over_trivially_copyable<int S::*>(), 2);
    STATIC_EXPECT_EQ(member_pointer_over_trivially_copyable<int>(), 1);
}

TEST(TypeCategory, member_pointer_subsumes_standard_layout)
{
    STATIC_EXPECT_EQ(member_pointer_over_standard_layout<int S::*>(), 2);
    STATIC_EXPECT_EQ(member_pointer_over_standard_layout<int>(), 1);
}

TEST(TypeCategory, member_object_pointer_subsumes_trivially_copyable)
{
    STATIC_EXPECT_EQ(member_object_pointer_over_trivially_copyable<int S::*>(), 2);
    STATIC_EXPECT_EQ(member_object_pointer_over_trivially_copyable<int>(), 1);
}

TEST(TypeCategory, member_object_pointer_subsumes_standard_layout)
{
    STATIC_EXPECT_EQ(member_object_pointer_over_standard_layout<int S::*>(), 2);
    STATIC_EXPECT_EQ(member_object_pointer_over_standard_layout<int>(), 1);
}

TEST(TypeCategory, member_function_pointer_subsumes_trivially_copyable)
{
    STATIC_EXPECT_EQ(member_function_pointer_over_trivially_copyable<int (S::*)()>(), 2);
    STATIC_EXPECT_EQ(member_function_pointer_over_trivially_copyable<int>(), 1);
}

TEST(TypeCategory, member_function_pointer_subsumes_standard_layout)
{
    STATIC_EXPECT_EQ(member_function_pointer_over_standard_layout<int (S::*)()>(), 2);
    STATIC_EXPECT_EQ(member_function_pointer_over_standard_layout<int>(), 1);
}

TEST(TypeCategory, array_subsumes_aggregate)
{
    STATIC_EXPECT_EQ(array_over_aggregate<int[3]>(), 2);
    STATIC_EXPECT_EQ(array_over_aggregate<S>(), 1);
}

TEST(TypeCategory, bounded_array_subsumes_aggregate)
{
    STATIC_EXPECT_EQ(bounded_array_over_aggregate<int[3]>(), 2);
    STATIC_EXPECT_EQ(bounded_array_over_aggregate<int[]>(), 1);
}

TEST(TypeCategory, unbounded_array_subsumes_aggregate)
{
    STATIC_EXPECT_EQ(unbounded_array_over_aggregate<int[]>(), 2);
    STATIC_EXPECT_EQ(unbounded_array_over_aggregate<int[3]>(), 1);
}

#if SCL_HAS_IMPLICIT_LIFETIME_CONCEPT
namespace
{
    template <implicit_lifetime Type>
    constexpr int scalar_over_implicit_lifetime()
    {
        return 1;
    }

    template <scalar Type>
    constexpr int scalar_over_implicit_lifetime()
    {
        return 2;
    }

    template <implicit_lifetime Type>
    constexpr int array_over_implicit_lifetime()
    {
        return 1;
    }

    template <array Type>
    constexpr int array_over_implicit_lifetime()
    {
        return 2;
    }
} // namespace

TEST(TypeCategory, scalar_subsumes_implicit_lifetime)
{
    STATIC_EXPECT_EQ(scalar_over_implicit_lifetime<int>(), 2);
    STATIC_EXPECT_EQ(scalar_over_implicit_lifetime<S>(), 1);
}

TEST(TypeCategory, array_subsumes_implicit_lifetime)
{
    STATIC_EXPECT_EQ(array_over_implicit_lifetime<NonAggregate[2]>(), 2);
    STATIC_EXPECT_EQ(array_over_implicit_lifetime<S>(), 1);
}
#endif
