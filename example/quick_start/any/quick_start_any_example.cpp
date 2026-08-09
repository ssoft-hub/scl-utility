/**
 * @example quick_start_any_example.cpp
 * @brief Quick start: one function accepting any argument type, without templates.
 */

//! [quick_start]
#include <scl/utility/any.h>

#include <iostream>
#include <string>

using ::std::string;

string from_int(int number) { return "int " + ::std::to_string(number); }
string from_string(string const & text) { return "string " + text; }
string from_other(::scl::any_arg value) { return "other " + string{value.type_name()}; }

// The chain holds no subject: build it once, apply it to whatever turns up.
auto const describe =
    ::scl::any_switch<string>() //
        .in_case<int>(from_int)
        .in_case<string const &>(from_string)
        .or_else(from_other);

void print(::scl::any_arg value) { ::std::cout << *describe.apply(value) << '\n'; }

int main()
{
    print(string{"text"}); // string text
    print(42);             // int 42
    print(2.5);            // other double
}
//! [quick_start]
