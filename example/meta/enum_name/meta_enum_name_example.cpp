/**
 * @example meta_enum_name_example.cpp
 * @brief Names a constant of an enumeration at compile time, in full and in short.
 *
 * scl::enum_name<V>() spells the constant with its namespaces and classes, and for a scoped
 * enumeration with its type, and scl::enum_short_name<V>() keeps the name of the constant alone.
 * A value no constant of its type has gets an empty name.
 */

#include <scl/utility/meta/enum.h>

#include <iostream>

//! [types]
enum class Color
{
    Red,
    Green,
};

enum Status
{
    Active,
};
//! [types]

//! [enum_name]
static_assert(scl::enum_name<Color::Red>() == "Color::Red");
static_assert(scl::enum_name<Active>() == "Active");
//! [enum_name]

//! [enum_short_name]
static_assert(scl::enum_short_name<Color::Green>() == "Green");
static_assert(scl::enum_short_name<Active>() == "Active");
//! [enum_short_name]

//! [no_constant]
// No constant of Color has the value 42.
static_assert(scl::enum_name<Color{42}>().empty());
static_assert(scl::enum_short_name<Color{42}>().empty());
//! [no_constant]

int main()
{
    std::cout << scl::enum_name<Color::Green>() << '\n';
    std::cout << scl::enum_short_name<Color::Green>() << '\n';
}
