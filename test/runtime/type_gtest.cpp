#include <gtest_utils.h>

#include <memory>

#include <scl/utility/runtime/type.h>

#if defined(SCL_DETAIL_HAS_RTTI)

struct SimpleStruct
{};

namespace ns
{
    struct NamespacedType
    {};

    template <typename T>
    struct TemplateType
    {};
} // namespace ns

struct PolymorphicBase
{
    virtual ~PolymorphicBase() = default;
};

struct PolymorphicDerived : PolymorphicBase
{};

struct FurtherDerived : PolymorphicDerived
{};

/**
 * @test Verify that type_name returns a name containing the expected identifier for fundamental types.
 */
TEST(TypeNameTest, FundamentalType)
{
    int i = 0;
    double d = 0.0;
    EXPECT_NE(::scl::type_name(i).find("int"), ::std::string::npos);
    EXPECT_NE(::scl::type_name(d).find("double"), ::std::string::npos);
}

/**
 * @test Verify that type_name returns a name containing the type identifier for user-defined types.
 */
TEST(TypeNameTest, UserDefinedType)
{
    SimpleStruct s;
    EXPECT_NE(::scl::type_name(s).find("SimpleStruct"), ::std::string::npos);
}

/**
 * @test Verify that type_name returns the dynamic type name when called through a base pointer.
 */
TEST(TypeNameTest, Polymorphism)
{
    ::std::unique_ptr<PolymorphicBase> p = ::std::make_unique<PolymorphicDerived>();
    EXPECT_NE(::scl::type_name(*p).find("PolymorphicDerived"), ::std::string::npos);
}

/**
 * @test Verify that type_name includes both the type name and template argument for template types.
 */
TEST(TypeNameTest, TemplateType)
{
    ns::TemplateType<int> t;
    EXPECT_NE(::scl::type_name(t).find("TemplateType"), ::std::string::npos);
    EXPECT_NE(::scl::type_name(t).find("int"), ::std::string::npos);
}

/**
 * @test Verify that type_short_name strips namespace qualifiers.
 */
TEST(TypeShortNameTest, Namespaced)
{
    ns::NamespacedType t;
    EXPECT_EQ(::scl::type_short_name(t), "NamespacedType");
}

/**
 * @test Verify that type_short_name strips both namespace qualifiers and template arguments.
 */
TEST(TypeShortNameTest, Template)
{
    ns::TemplateType<int> t;
    EXPECT_EQ(::scl::type_short_name(t), "TemplateType");
}

/**
 * @test Verify that type_short_name returns the dynamic short name when called through a base pointer.
 */
TEST(TypeShortNameTest, Polymorphism)
{
    ::std::unique_ptr<PolymorphicBase> p = ::std::make_unique<FurtherDerived>();
    EXPECT_EQ(::scl::type_short_name(*p), "FurtherDerived");
}

#else

TEST(RuntimeTypeTest, SkippedWithoutRtti)
{
    GTEST_SKIP() << "RTTI disabled — scl::type_name and scl::type_short_name are not available";
}

#endif // SCL_DETAIL_HAS_RTTI
