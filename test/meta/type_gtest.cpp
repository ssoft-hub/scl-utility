#include <gtest_utils.h>

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
 * @brief TU-local type for testing the anonymous-namespace rendering.
 */
namespace
{
    struct AnonStruct
    {};
} // namespace

/**
 * @brief Enumeration for testing the MSVC 'enum ' prefix.
 */
enum class Color
{
    red,
};

/**
 * @test Verify fundamental types extraction.
 */
TEST(MetaTypeTest, FundamentalTypes)
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
TEST(MetaTypeTest, UserDefinedTypes)
{
#if defined(_MSC_VER) && !defined(__clang__)
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
TEST(MetaTypeTest, NamespacedTypes)
{
#if defined(_MSC_VER) && !defined(__clang__)
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
TEST(MetaTypeTest, TemplateTypes)
{
    using T = Namespace::TemplateStruct<Namespace::Struct>;
    using TT = Namespace::TemplateClass<T>;

#if defined(_MSC_VER) && !defined(__clang__)
    EXPECT_EQ(::scl::type_name<T>(), "struct Namespace::TemplateStruct<struct Namespace::Struct>");
    // MSVC adds space before > in nested templates: "> >" instead of ">>"
    EXPECT_EQ(::scl::type_name<TT>(),
        "class Namespace::TemplateClass<struct Namespace::TemplateStruct<struct Namespace::Struct> >");
#elif defined(__clang__)
    EXPECT_EQ(::scl::type_name<T>(), "Namespace::TemplateStruct<Namespace::Struct>");
    EXPECT_EQ(::scl::type_name<TT>(), "Namespace::TemplateClass<Namespace::TemplateStruct<Namespace::Struct>>");
#else
    // GCC adds space before > in nested templates
    EXPECT_EQ(::scl::type_name<T>(), "Namespace::TemplateStruct<Namespace::Struct>");
    EXPECT_EQ(::scl::type_name<TT>(), "Namespace::TemplateClass<Namespace::TemplateStruct<Namespace::Struct> >");
#endif

    EXPECT_EQ(::scl::type_short_name<T>(), "TemplateStruct");
    EXPECT_EQ(::scl::type_short_name<TT>(), "TemplateClass");
}

/**
 * @test Verify the rendering contract the documentation states.
 * @note The MSVC prefix reaches template arguments as well, so a rendered template name has
 * no spelling common to all compilers; the documented examples match on a substring.
 */
TEST(MetaTypeTest, DocumentedRenderingContract)
{
    constexpr auto npos = ::std::string_view::npos;

    static constexpr auto vector_name = ::scl::type_name<::std::vector<SimpleStruct>>();
    static constexpr auto enum_vector_name = ::scl::type_name<::std::vector<Color>>();
    static constexpr auto nested_name = ::scl::type_name<Namespace::Struct>();
    static constexpr auto anon_name = ::scl::type_name<AnonStruct>();

    STATIC_EXPECT_NE(vector_name.find("SimpleStruct"), npos);
    STATIC_EXPECT_NE(enum_vector_name.find("Color"), npos);
    STATIC_EXPECT_NE(nested_name.find("Namespace::Struct"), npos);

    // The anonymous-namespace marker is spelled differently by each compiler, so only the
    // identifier and the MSVC prefix are portable claims about the rendering.
    STATIC_EXPECT_NE(anon_name.find("AnonStruct"), npos);

    STATIC_EXPECT_EQ(::scl::type_short_name<Color>(), "Color");
    STATIC_EXPECT_EQ(::scl::type_short_name<AnonStruct>(), "AnonStruct");
    STATIC_EXPECT_EQ(::scl::type_short_name<::std::vector<SimpleStruct>>(), "vector");

#if defined(_MSC_VER) && !defined(__clang__)
    STATIC_EXPECT_EQ(::scl::type_name<SimpleStruct>(), "struct SimpleStruct");
    STATIC_EXPECT_EQ(::scl::type_name<Color>(), "enum Color");
    STATIC_EXPECT_NE(vector_name.find("struct SimpleStruct"), npos);
    STATIC_EXPECT_NE(enum_vector_name.find("enum Color"), npos);
    STATIC_EXPECT_TRUE(nested_name.starts_with("struct "));
    STATIC_EXPECT_TRUE(anon_name.starts_with("struct "));
#else
    STATIC_EXPECT_EQ(::scl::type_name<SimpleStruct>(), "SimpleStruct");
    STATIC_EXPECT_EQ(::scl::type_name<Color>(), "Color");
    STATIC_EXPECT_EQ(vector_name.find("struct "), npos);
    STATIC_EXPECT_EQ(enum_vector_name.find("enum "), npos);
    STATIC_EXPECT_EQ(anon_name.find("struct "), npos);
#endif
}

/**
 * @test Verify standard library types.
 * @note STL implementations may vary in how they display default allocators.
 */
TEST(MetaTypeTest, StandardLibraryTypes)
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
TEST(MetaTypeTest, Qualifiers)
{
    // Current implementation preserves qualifiers as they are part of type T.
    // We use find/contains logic because exact formatting of 'const' (before or after type)
    // might differ between compilers.
    auto const_name = ::scl::type_name<int const>();
    EXPECT_TRUE(const_name.find("int") != ::std::string_view::npos);
    EXPECT_TRUE(const_name.find("const") != ::std::string_view::npos);
}
