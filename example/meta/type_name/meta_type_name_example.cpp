/**
 * @example meta_type_name_example.cpp
 * @brief Demonstrates scl::type_name and scl::type_short_name, and the part of a
 * rendered name that depends on the compiler.
 *
 * The rendering is taken from the compiler's own signature macro and is not
 * normalized: MSVC keeps the 'class'/'struct'/'union' prefix where GCC and Clang
 * omit it, and the standard library decides how its own types spell themselves.
 * A name is therefore something to show, not something to compare - scl::type_key
 * is what compares, and scl::type_short_name is the bare identifier.
 *
 * The assertions below are how this program checks itself on every compiler, not a
 * recommended way to use the API: each one states the little that is true everywhere,
 * with the diverging renderings named in a comment beside it.
 */

#include <iostream>

//! [type_name]
#include <scl/utility/meta/type.h>

#include <string>
#include <vector>

struct MyStruct
{};

constexpr auto npos = ::std::string_view::npos;

// Fundamental types render identically on every compiler.
static_assert(::scl::type_name<int>() == "int");

// A standard library type spells itself the way the toolchain does. std::string:
// GCC:   "std::__cxx11::basic_string<char>"
// Clang: "std::basic_string<char>"
// MSVC:  "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >"
static_assert(::scl::type_name<::std::string>().find("basic_string") != npos);

// A user-defined type carries the MSVC prefix.
// GCC/Clang: "MyStruct"  |  MSVC: "struct MyStruct"
static_assert(::scl::type_name<MyStruct>().find("MyStruct") != npos);

// The prefix reaches template arguments as well, so a rendered template name has no
// spelling common to all three compilers.
// GCC/Clang: "std::vector<MyStruct>"
// MSVC:      "class std::vector<struct MyStruct,class std::allocator<struct MyStruct> >"
static_assert(::scl::type_name<::std::vector<MyStruct>>().find("MyStruct") != npos);
//! [type_name]

//! [type_short_name]
namespace app::core
{
    struct Task
    {};
} // namespace app::core

// The short name is the bare identifier on every compiler: namespace qualifiers, the
// MSVC prefix and the template arguments are all stripped.
static_assert(::scl::type_short_name<app::core::Task>() == "Task");
static_assert(::scl::type_short_name<::std::vector<MyStruct>>() == "vector");
//! [type_short_name]

int main()
{
    ::std::cout << ::scl::type_name<::std::vector<MyStruct>>() << '\n';
    ::std::cout << ::scl::type_short_name<::std::vector<MyStruct>>() << '\n';
}
