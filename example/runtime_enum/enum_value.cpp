/// @file enum_value.cpp
/// @brief Demonstrates scl::enum_value for runtime enum representation.
///
/// Compile-time:  scl::enum_name<V>()    — returns the member identifier as a string_view
///                                         (e.g. "Red"); requires a valid named enumerator as
///                                         a non-type template parameter.
/// Runtime:       scl::enum_value(v)     — returns "TypeName::N" where N is the underlying
///                                         numeric value; works for any value including
///                                         out-of-range ones that have no named enumerator.

#include <scl/utility/meta/enum.h>
#include <scl/utility/runtime/enum.h>

#include <iostream>

enum class Color : int
{
    Red   = 1,
    Green = 2,
    Blue  = -3,
};

enum class Flags : unsigned
{
    None = 0,
    A    = 1,
    B    = 2,
};

namespace app
{
    enum class Status : int
    {
        Ok  = 0,
        Err = 42,
    };
} // namespace app

// ============================================================================
// Pattern 1 — named enumerator: compile-time gives the identifier, runtime gives "Type::N"
// ============================================================================

static void show_named()
{
    // Compile-time: non-type template parameter, result is constexpr string_view.
    constexpr auto ct_name  = ::scl::enum_name<Color::Red>();
    constexpr auto ct_short = ::scl::enum_short_name<Color::Red>();

    ::std::cout << "[compile-time] enum_name       : " << ct_name << '\n';  // Color::Red
    ::std::cout << "[compile-time] enum_short_name : " << ct_short << '\n'; // Red

    // Runtime: accepts any value; returns "TypeName::N".
    ::std::cout << "[runtime     ] enum_value       : " << ::scl::enum_value(Color::Red) << '\n';   // Color::1
    ::std::cout << "[runtime     ] enum_value       : " << ::scl::enum_value(Color::Blue) << '\n';  // Color::-3
}

// ============================================================================
// Pattern 2 — out-of-range value: only runtime can represent it
// ============================================================================

static void show_out_of_range()
{
    // Compile-time enum_name requires a valid enumerator — no way to instantiate Color{42}.
    // Runtime enum_value handles any underlying value.
    ::std::cout << "[runtime     ] enum_value(Color{42}) : " << ::scl::enum_value(Color{42}) << '\n'; // Color::42
}

// ============================================================================
// Pattern 3 — unsigned underlying type and namespaced enum
// ============================================================================

static void show_unsigned_and_namespaced()
{
    ::std::cout << "[runtime     ] Flags::B          : " << ::scl::enum_value(Flags::B) << '\n';          // Flags::2
    ::std::cout << "[runtime     ] app::Status::Err  : " << ::scl::enum_value(app::Status::Err) << '\n';  // Status::42
}

// ============================================================================
// main
// ============================================================================

int main(int, char **)
{
    ::std::cout << "=== Named enumerator ===\n";
    show_named();

    ::std::cout << "\n=== Out-of-range value ===\n";
    show_out_of_range();

    ::std::cout << "\n=== Unsigned underlying / namespaced enum ===\n";
    show_unsigned_and_namespaced();

    return {};
}
