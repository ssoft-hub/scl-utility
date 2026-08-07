/**
 * @example hash.cpp
 * @brief Quick start: a string as a `switch` label and as a template parameter.
 */

//! [quick_start]
#include <scl/utility/hash/key.h>

#include <iostream>
#include <string_view>

using ::scl::hash::key;
using namespace ::std::literals;

// A key hashes the range it is given, and a string literal carries its
// terminating zero: keep every key in the comparison built from a view.
int code_of(::std::string_view command)
{
    switch (key<>{command})
    {
    case key<>{"start"sv}:
        return 1;
    case key<>{"stop"sv}:
        return 2;
    default:
        return 0;
    }
}

template <key<> Command>
constexpr bool is_start = (Command == key<>{"start"sv});

static_assert(is_start<key<>{"start"sv}>);

int main()
{
    ::std::cout << code_of("start") << code_of("stop") << code_of("pause") << '\n'; // 120
}
//! [quick_start]
