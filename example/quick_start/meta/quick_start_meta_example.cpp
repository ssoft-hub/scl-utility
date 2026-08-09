/**
 * @example quick_start_meta_example.cpp
 * @brief Quick start: type and enumerator names at compile time, without RTTI.
 */

//! [quick_start]
#include <scl/utility/meta/enum.h>
#include <scl/utility/meta/type.h>

#include <iostream>

enum class color
{
    red,
    green,
};

int main()
{
    static_assert(::scl::type_name<int>() == "int");
    static_assert(::scl::enum_short_name<color::green>() == "green");

    ::std::cout << ::scl::type_name<color>() << '\n';        // color; MSVC: enum color
    ::std::cout << ::scl::enum_name<color::green>() << '\n'; // color::green
}
//! [quick_start]
