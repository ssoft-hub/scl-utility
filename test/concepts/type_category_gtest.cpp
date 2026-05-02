#include <gtest_utils.h>

#include <scl/utility/concepts/type_category.h>

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
} // namespace

TEST(TypeCategory, enum_type)
{
    STATIC_EXPECT_TRUE(enum_type<Plain>);
    STATIC_EXPECT_FALSE(enum_type<int>);
    STATIC_EXPECT_FALSE(enum_type<S>);
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

TEST(TypeCategory, array_type)
{
    STATIC_EXPECT_TRUE(array_type<int[3]>);
    STATIC_EXPECT_TRUE(array_type<int[]>);
    STATIC_EXPECT_FALSE(array_type<int>);
    STATIC_EXPECT_FALSE(array_type<int *>);
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

TEST(TypeCategory, function_type)
{
    STATIC_EXPECT_TRUE(function_type<void()>);
    STATIC_EXPECT_TRUE(function_type<int(int, double)>);
    STATIC_EXPECT_FALSE(function_type<int>);
    STATIC_EXPECT_FALSE(function_type<void (*)()>);
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
    STATIC_EXPECT_FALSE(scalar<S>);
    STATIC_EXPECT_FALSE(scalar<void>);
    STATIC_EXPECT_FALSE(scalar<void()>);
}

TEST(TypeCategory, object_type)
{
    STATIC_EXPECT_TRUE(object_type<int>);
    STATIC_EXPECT_TRUE(object_type<S>);
    STATIC_EXPECT_TRUE(object_type<int *>);
    STATIC_EXPECT_TRUE(object_type<int[3]>);
    STATIC_EXPECT_FALSE(object_type<void>);
    STATIC_EXPECT_FALSE(object_type<void()>);
    STATIC_EXPECT_FALSE(object_type<int &>);
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
