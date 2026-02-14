#include <gtest/gtest.h>

#include <vector>

#include <scl/utility/meta/type.h>

/**
 * @brief Simple structure for testing type extraction.
 */
struct SimpleStruct
{};

/**
 * @brief Simple class for testing type extraction and prefix cleanup.
 */
class SimpleClass
{};

/**
 * @brief Namespace for testing nested type names.
 */
namespace Namespace
{
    struct Struct
    {};

    template <typename T>
    struct TemplateStruct
    {};

    template <typename T>
    class TemplateClass
    {};

} // namespace Namespace

/**
 * @test Verify fundamental types extraction.
 */
TEST(TypeNameTest, FundamentalTypes)
{
    EXPECT_EQ(::scl::type_name<int>(), "int");
    EXPECT_EQ(::scl::type_name<float>(), "float");
    EXPECT_EQ(::scl::type_name<double>(), "double");
    EXPECT_EQ(::scl::type_name<void>(), "void");
}

/**
 * @test Verify user-defined types (struct/class).
 * @note On MSVC, type names include struct/class keywords.
 */
TEST(TypeNameTest, UserDefinedTypes)
{
#ifdef _MSC_VER
    EXPECT_EQ(::scl::type_name<SimpleStruct>(), "struct SimpleStruct");
    EXPECT_EQ(::scl::type_name<SimpleClass>(), "class SimpleClass");
#else
    EXPECT_EQ(::scl::type_name<SimpleStruct>(), "SimpleStruct");
    EXPECT_EQ(::scl::type_name<SimpleClass>(), "SimpleClass");
#endif

    static constexpr auto simple_struct_name = ::scl::type_short_name<SimpleStruct>();
    static constexpr auto simple_class_name = ::scl::type_short_name<SimpleClass>();

    EXPECT_EQ(simple_struct_name, "SimpleStruct");
    EXPECT_EQ(simple_class_name, "SimpleClass");
}

/**
 * @test Verify types within custom namespaces.
 */
TEST(TypeNameTest, NamespacedTypes)
{
#ifdef _MSC_VER
    EXPECT_EQ(::scl::type_name<Namespace::Struct>(), "struct Namespace::Struct");
#else
    EXPECT_EQ(::scl::type_name<Namespace::Struct>(), "Namespace::Struct");
#endif

    EXPECT_EQ(::scl::type_short_name<Namespace::Struct>(), "Struct");
}

/**
 * @test Verify template types extraction.
 * @note On MSVC, template arguments include struct/class keywords.
 */
TEST(TypeNameTest, TemplateTypes)
{
    using T = Namespace::TemplateStruct<Namespace::Struct>;
    using TT = Namespace::TemplateClass<T>;

#ifdef _MSC_VER
    EXPECT_EQ(::scl::type_name<T>(), "struct Namespace::TemplateStruct<struct Namespace::Struct>");
    // MSVC adds space before > in nested templates: "> >" instead of ">>"
    EXPECT_EQ(::scl::type_name<TT>(),
        "class Namespace::TemplateClass<struct Namespace::TemplateStruct<struct Namespace::Struct> >");
#else
    EXPECT_EQ(::scl::type_name<T>(), "Namespace::TemplateStruct<Namespace::Struct>");
    EXPECT_EQ(::scl::type_name<TT>(), "Namespace::TemplateClass<Namespace::TemplateStruct<Namespace::Struct> >");
#endif

    EXPECT_EQ(::scl::type_short_name<T>(), "TemplateStruct");
    EXPECT_EQ(::scl::type_short_name<TT>(), "TemplateClass");
}

/**
 * @test Verify standard library types.
 * @note STL implementations may vary in how they display default allocators.
 */
TEST(TypeNameTest, StandardLibraryTypes)
{
    static constexpr ::std::string_view name = ::scl::type_name<::std::vector<int>>();
    // We check for substring because different compilers/STL versions
    // may include or omit default allocator details.
    EXPECT_TRUE(name.find("vector<int") != ::std::string_view::npos);
    EXPECT_EQ(::scl::type_short_name<::std::vector<int>>(), "vector");
}

/**
 * @test Verify that qualifiers like const and references are preserved or handled.
 */
TEST(TypeNameTest, Qualifiers)
{
    // Current implementation preserves qualifiers as they are part of type T.
    // We use find/contains logic because exact formatting of 'const' (before or after type)
    // might differ between compilers.
    auto const_name = ::scl::type_name<int const>();
    EXPECT_TRUE(const_name.find("int") != ::std::string_view::npos);
    EXPECT_TRUE(const_name.find("const") != ::std::string_view::npos);
}
