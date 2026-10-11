/**
 * @example meta_type_name_example.cpp
 * @brief Names a type at compile time, in full and in short.
 *
 * scl::type_name<T>() spells a type the way the compiler spells it, so the program checks only
 * the part every compiler agrees on. scl::type_short_name<T>() keeps the bare identifier.
 */

#include <scl/utility/meta/type.h>

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

//! [types]
namespace app
{
    struct Item
    {};

    template <typename T>
    struct Task
    {};
} // namespace app
//! [types]

//! [type_name]
// Every compiler spells int alike.
static_assert(scl::type_name<int>() == "int");

// MSVC writes the class key before the name of a class: app::Item, or struct app::Item.
static_assert(scl::type_name<app::Item>().ends_with("app::Item"));

// GCC spells a type of the standard library with an inline namespace of its library, such as
// std::__cxx11::basic_string<char>, and Clang leaves that namespace out, as in
// std::basic_string<char>.
static_assert(scl::type_name<std::string>().find("string") != std::string_view::npos);
//! [type_name]

//! [type_short_name]
static_assert(scl::type_short_name<app::Task<int>>() == "Task");
static_assert(scl::type_short_name<app::Item const &>() == "Item");
static_assert(scl::type_short_name<std::vector<app::Item>>() == "vector");
//! [type_short_name]

int main()
{
    std::cout << scl::type_name<std::vector<app::Item>>() << '\n';
    std::cout << scl::type_short_name<std::vector<app::Item>>() << '\n';
}
