#include <gtest_utils.h>

#include <scl/utility/concepts/type_relation.h>

using namespace scl::concepts;

namespace
{
    struct Base
    {};

    struct Plain : Base
    {};

    struct PrivatePlain : private Base
    {};

    struct Virtual : virtual Base
    {};

    struct PrivateVirtual : private virtual Base
    {};

    struct Indirect : Virtual
    {};

    struct Left : Base
    {};

    struct Right : Base
    {};

    struct Twice
        : Left
        , Right
    {};

    struct Unrelated
    {};

    union Union
    {
        int i;
    };

    template <typename Type>
        requires class_type<Type>
    constexpr int base_of_over_class_type_of_base()
    {
        return 1;
    }

    template <typename Type>
        requires base_of<Type, Plain>
    constexpr int base_of_over_class_type_of_base()
    {
        return 2;
    }

    template <typename Type>
        requires class_type<Type>
    constexpr int base_of_over_class_type_of_derived()
    {
        return 1;
    }

    template <typename Type>
        requires base_of<Base, Type>
    constexpr int base_of_over_class_type_of_derived()
    {
        return 2;
    }

    template <typename Type>
        requires non_reference<Type>
    constexpr int base_of_over_non_reference()
    {
        return 1;
    }

    template <typename Type>
        requires base_of<Type, Plain>
    constexpr int base_of_over_non_reference()
    {
        return 2;
    }

    template <typename Type>
        requires object<Type>
    constexpr int base_of_over_object_of_base()
    {
        return 1;
    }

    template <typename Type>
        requires base_of<Type, Plain>
    constexpr int base_of_over_object_of_base()
    {
        return 2;
    }

    template <typename Type>
        requires compound<Type>
    constexpr int base_of_over_compound_of_base()
    {
        return 1;
    }

    template <typename Type>
        requires base_of<Type, Plain>
    constexpr int base_of_over_compound_of_base()
    {
        return 2;
    }

    template <typename Type>
        requires object<Type>
    constexpr int base_of_over_object_of_derived()
    {
        return 1;
    }

    template <typename Type>
        requires base_of<Base, Type>
    constexpr int base_of_over_object_of_derived()
    {
        return 2;
    }

    template <typename Type>
        requires compound<Type>
    constexpr int base_of_over_compound_of_derived()
    {
        return 1;
    }

    template <typename Type>
        requires base_of<Base, Type>
    constexpr int base_of_over_compound_of_derived()
    {
        return 2;
    }

    template <typename Type>
        requires non_reference<Type>
    constexpr int base_of_over_non_reference_of_derived()
    {
        return 1;
    }

    template <typename Type>
        requires base_of<Base, Type>
    constexpr int base_of_over_non_reference_of_derived()
    {
        return 2;
    }

    template <base_of<Plain> Type>
    constexpr bool base_of_plain()
    {
        return true;
    }
} // namespace

TEST(TypeRelation, base_of)
{
    STATIC_EXPECT_TRUE((base_of<Base, Plain>));
    STATIC_EXPECT_TRUE((base_of<Base, Base>));
    STATIC_EXPECT_TRUE((base_of<Base, PrivatePlain>));
    STATIC_EXPECT_TRUE((base_of<Base, Virtual>));
    STATIC_EXPECT_TRUE((base_of<Base, Indirect>));
    STATIC_EXPECT_TRUE((base_of<Base, Twice>));
    STATIC_EXPECT_TRUE((base_of<Base const, Plain volatile>));
    STATIC_EXPECT_FALSE((base_of<Plain, Base>));
    STATIC_EXPECT_FALSE((base_of<Unrelated, Plain>));
    STATIC_EXPECT_FALSE((base_of<Base &, Plain>));
    STATIC_EXPECT_FALSE((base_of<Base, Plain &>));
    STATIC_EXPECT_FALSE((base_of<Union, Union>));
    STATIC_EXPECT_FALSE((base_of<int, int>));
}

TEST(TypeRelation, base_of_constrains_a_template_parameter)
{
    STATIC_EXPECT_TRUE(base_of_plain<Base>());
}

TEST(TypeRelation, base_of_subsumes_class_type)
{
    STATIC_EXPECT_EQ(base_of_over_class_type_of_base<Base>(), 2);
    STATIC_EXPECT_EQ(base_of_over_class_type_of_base<Unrelated>(), 1);
    STATIC_EXPECT_EQ(base_of_over_class_type_of_derived<Plain>(), 2);
    STATIC_EXPECT_EQ(base_of_over_class_type_of_derived<Unrelated>(), 1);
}

TEST(TypeRelation, base_of_subsumes_non_reference)
{
    STATIC_EXPECT_EQ(base_of_over_non_reference<Base>(), 2);
    STATIC_EXPECT_EQ(base_of_over_non_reference<int>(), 1);
    STATIC_EXPECT_EQ(base_of_over_non_reference_of_derived<Plain>(), 2);
    STATIC_EXPECT_EQ(base_of_over_non_reference_of_derived<int>(), 1);
}

TEST(TypeRelation, base_of_subsumes_object_and_compound)
{
    STATIC_EXPECT_EQ(base_of_over_object_of_base<Base>(), 2);
    STATIC_EXPECT_EQ(base_of_over_object_of_base<int>(), 1);
    STATIC_EXPECT_EQ(base_of_over_compound_of_base<Base>(), 2);
    STATIC_EXPECT_EQ(base_of_over_compound_of_base<Unrelated>(), 1);
    STATIC_EXPECT_EQ(base_of_over_object_of_derived<Plain>(), 2);
    STATIC_EXPECT_EQ(base_of_over_object_of_derived<int>(), 1);
    STATIC_EXPECT_EQ(base_of_over_compound_of_derived<Plain>(), 2);
    STATIC_EXPECT_EQ(base_of_over_compound_of_derived<Unrelated>(), 1);
}

TEST(TypeRelation, scl_has_virtual_base_of_concept)
{
#if defined(__cpp_lib_is_virtual_base_of) && __cpp_lib_is_virtual_base_of >= 202406L
    STATIC_EXPECT_EQ(SCL_HAS_VIRTUAL_BASE_OF_CONCEPT, 1);
#elif defined(__has_builtin)
#if __has_builtin(__builtin_is_virtual_base_of)
    STATIC_EXPECT_EQ(SCL_HAS_VIRTUAL_BASE_OF_CONCEPT, 1);
#else
    STATIC_EXPECT_EQ(SCL_HAS_VIRTUAL_BASE_OF_CONCEPT, 0);
#endif
#else
    STATIC_EXPECT_EQ(SCL_HAS_VIRTUAL_BASE_OF_CONCEPT, 0);
#endif
}

#if SCL_HAS_VIRTUAL_BASE_OF_CONCEPT
namespace
{
    struct Mixed
        : Virtual
        , Plain
    {};

    template <typename Type>
        requires base_of<Base, Type>
    constexpr int virtual_base_of_over_base_of()
    {
        return 1;
    }

    template <typename Type>
        requires virtual_base_of<Base, Type>
    constexpr int virtual_base_of_over_base_of()
    {
        return 2;
    }

    template <typename Type>
        requires class_type<Type>
    constexpr int virtual_base_of_over_class_type()
    {
        return 1;
    }

    template <typename Type>
        requires virtual_base_of<Base, Type>
    constexpr int virtual_base_of_over_class_type()
    {
        return 2;
    }

    template <typename Type>
        requires class_type<Type>
    constexpr int virtual_base_of_over_class_type_of_base()
    {
        return 1;
    }

    template <typename Type>
        requires virtual_base_of<Type, Virtual>
    constexpr int virtual_base_of_over_class_type_of_base()
    {
        return 2;
    }

    template <typename Type>
        requires object<Type>
    constexpr int virtual_base_of_over_object()
    {
        return 1;
    }

    template <typename Type>
        requires virtual_base_of<Type, Virtual>
    constexpr int virtual_base_of_over_object()
    {
        return 2;
    }

    template <typename Type>
        requires compound<Type>
    constexpr int virtual_base_of_over_compound()
    {
        return 1;
    }

    template <typename Type>
        requires virtual_base_of<Base, Type>
    constexpr int virtual_base_of_over_compound()
    {
        return 2;
    }

    template <virtual_base_of<Virtual> Type>
    constexpr bool virtual_base_of_virtual()
    {
        return true;
    }
} // namespace

TEST(TypeRelation, virtual_base_of)
{
    STATIC_EXPECT_TRUE((virtual_base_of<Base, Virtual>));
    STATIC_EXPECT_TRUE((virtual_base_of<Base const, Virtual volatile>));
    STATIC_EXPECT_TRUE((virtual_base_of<Base, PrivateVirtual>));
    STATIC_EXPECT_TRUE((virtual_base_of<Base, Indirect>));
    STATIC_EXPECT_TRUE((virtual_base_of<Base, Mixed>));
    STATIC_EXPECT_FALSE((virtual_base_of<Base, Plain>));
    STATIC_EXPECT_FALSE((virtual_base_of<Base, PrivatePlain>));
    STATIC_EXPECT_FALSE((virtual_base_of<Base, Base>));
    STATIC_EXPECT_FALSE((virtual_base_of<Virtual, Base>));
    STATIC_EXPECT_FALSE((virtual_base_of<Base &, Virtual>));
    STATIC_EXPECT_FALSE((virtual_base_of<Base, Virtual &>));
    STATIC_EXPECT_FALSE((virtual_base_of<Union, Union>));
    STATIC_EXPECT_FALSE((virtual_base_of<int, int>));
}

TEST(TypeRelation, virtual_base_of_constrains_a_template_parameter)
{
    STATIC_EXPECT_TRUE(virtual_base_of_virtual<Base>());
}

TEST(TypeRelation, virtual_base_of_subsumes_base_of)
{
    STATIC_EXPECT_EQ(virtual_base_of_over_base_of<Virtual>(), 2);
    STATIC_EXPECT_EQ(virtual_base_of_over_base_of<Plain>(), 1);
}

TEST(TypeRelation, virtual_base_of_subsumes_class_type)
{
    STATIC_EXPECT_EQ(virtual_base_of_over_class_type<Virtual>(), 2);
    STATIC_EXPECT_EQ(virtual_base_of_over_class_type<Plain>(), 1);
    STATIC_EXPECT_EQ(virtual_base_of_over_class_type_of_base<Base>(), 2);
    STATIC_EXPECT_EQ(virtual_base_of_over_class_type_of_base<Unrelated>(), 1);
}

TEST(TypeRelation, virtual_base_of_subsumes_object_and_compound)
{
    STATIC_EXPECT_EQ(virtual_base_of_over_object<Base>(), 2);
    STATIC_EXPECT_EQ(virtual_base_of_over_object<int>(), 1);
    STATIC_EXPECT_EQ(virtual_base_of_over_compound<Virtual>(), 2);
    STATIC_EXPECT_EQ(virtual_base_of_over_compound<Plain>(), 1);
}
#endif
