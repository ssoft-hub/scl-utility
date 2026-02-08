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
} // namespace Namespace

/**
 * @test Verify fundamental types extraction.
 */
TEST(TypeNameTest, FundamentalTypes)
{
    EXPECT_EQ(scl::type_name<int>(), "int");
    EXPECT_EQ(scl::type_name<float>(), "float");
    EXPECT_EQ(scl::type_name<double>(), "double");
    EXPECT_EQ(scl::type_name<void>(), "void");
}

/**
 * @test Verify user-defined types (struct/class) and MSVC prefix removal.
 */
TEST(TypeNameTest, UserDefinedTypes)
{
    EXPECT_EQ(scl::type_name<SimpleStruct>(), "SimpleStruct");
    EXPECT_EQ(scl::type_name<SimpleClass>(), "SimpleClass");
    EXPECT_EQ(scl::type_short_name<SimpleStruct>(), "SimpleStruct");
    EXPECT_EQ(scl::type_short_name<SimpleClass>(), "SimpleClass");
}

/**
 * @test Verify types within custom namespaces.
 */
TEST(TypeNameTest, NamespacedTypes)
{
    EXPECT_EQ(scl::type_name<Namespace::Struct>(), "Namespace::Struct");
    EXPECT_EQ(scl::type_short_name<Namespace::Struct>(), "Struct");
}

/**
 * @test Verify template types extraction.
 */
TEST(TypeNameTest, TemplateTypes)
{
    using T = Namespace::TemplateStruct<SimpleStruct>;
    EXPECT_EQ(scl::type_name<T>(), "Namespace::TemplateStruct<SimpleStruct>");
}

/**
 * @test Verify standard library types.
 * @note STL implementations may vary in how they display default allocators.
 */
TEST(TypeNameTest, StandardLibraryTypes)
{
    static constexpr std::string_view name = scl::type_name<std::vector<int>>();
    // We check for substring because different compilers/STL versions
    // may include or omit default allocator details.
    EXPECT_TRUE(name.find("vector<int") != std::string_view::npos);
}

/**
 * @test Verify that qualifiers like const and references are preserved or handled.
 */
TEST(TypeNameTest, Qualifiers)
{
    // Current implementation preserves qualifiers as they are part of type T.
    // We use find/contains logic because exact formatting of 'const' (before or after type)
    // might differ between compilers.
    auto const_name = scl::type_name<int const>();
    EXPECT_TRUE(const_name.find("int") != std::string_view::npos);
    EXPECT_TRUE(const_name.find("const") != std::string_view::npos);
}
