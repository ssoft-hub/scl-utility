/**
 * @example meta_symbol_name_example.cpp
 * @brief Names a function, a member function and a data member at compile time.
 *
 * scl::symbol_name<S>() spells the entity a pointer or a pointer to member names, with its
 * namespaces and classes, and scl::symbol_short_name<S>() keeps its name alone.
 */

#include <scl/utility/meta/symbol.h>

#include <iostream>

//! [types]
void start() {}

namespace app
{
    struct Clock
    {
        int ticks = 0;

        void reset() {}
    };
} // namespace app
//! [types]

//! [symbol_name]
static_assert(scl::symbol_name<start>() == "start");
static_assert(scl::symbol_name<&app::Clock::reset>() == "app::Clock::reset");
static_assert(scl::symbol_name<&app::Clock::ticks>() == "app::Clock::ticks");
//! [symbol_name]

//! [symbol_short_name]
static_assert(scl::symbol_short_name<&app::Clock::reset>() == "reset");
static_assert(scl::symbol_short_name<&app::Clock::ticks>() == "ticks");
//! [symbol_short_name]

int main()
{
    std::cout << scl::symbol_name<&app::Clock::reset>() << '\n';
    std::cout << scl::symbol_short_name<&app::Clock::reset>() << '\n';
}
