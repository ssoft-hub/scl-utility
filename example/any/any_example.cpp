/// @file any_view_example.cpp
/// @brief Demonstrates scl::any_view — a non-owning, read-only view over a
///        std::any or a typed lvalue.
///
/// scl::any_view is to std::any what std::string_view is to std::string: it
/// refers to an existing value without copying it, so one function can accept a
/// heterogeneous read-only argument at no allocation cost. It is two pointers
/// wide and trivially copyable. A typed lvalue forms the "raw" backing (no RTTI,
/// carries a compile-time type name); a std::any forms the "std::any" backing
/// (RTTI builds only). scl::any_cast recovers the value by pointer (nullptr on
/// mismatch, never throws), by copy, or by const reference (zero-copy).

#include <scl/utility/any.h>
#include <scl/utility/preprocessor/rtti.h>

#if SCL_HAS_RTTI
#include <any>
#endif
#include <iostream>
#include <string>

// ============================================================================
// Pattern 1 — one function reads either backing without copying the payload
// ============================================================================

static void bar(::scl::any_view value)
{
    // Pointer form: yields a const pointer into the viewed object, or nullptr on
    // a type mismatch. No copy, never throws.
    if (auto const * text = ::scl::any_cast<::std::string>(&value))
        ::std::cout << "  sees std::string: \"" << *text << "\" (no copy)\n";
    else
        ::std::cout << "  not a std::string (type_name=\"" << value.type_name() << "\")\n";
}

// ============================================================================
// Pattern 2 — reference-form any_cast: value copy vs zero-copy const reference
// ============================================================================

static void show_reference_cast()
{
    ::std::string const text{"Hello Any!"};

    // Bind through a NAMED view: casting `T const &` through a temporary view
    // would trip GCC's -Wdangling-reference, even though the payload outlives it.
    ::scl::any_view const view{text};

    auto const copy = ::scl::any_cast<::std::string>(view);          // independent copy
    auto const & ref = ::scl::any_cast<::std::string const &>(view); // zero-copy reference

    ::std::cout << "copy == \"" << copy << "\" (own object)\n";
    ::std::cout << "ref  == \"" << ref << "\" bound to source? " << (&ref == &text) << '\n'; // 1
}

// ============================================================================
// Pattern 3 — identity queries across both backings and the empty view
// ============================================================================

static void show_identity()
{
    ::std::string const text{"payload"};

    ::scl::any_view const raw{text}; // raw backing — carries a compile-time type name
    ::scl::any_view const empty{};   // views nothing

    ::std::cout << "raw   has_value=" << raw.has_value() << " type_name=\"" << raw.type_name() << "\"\n";
#if SCL_HAS_RTTI
    ::std::any const boxed{text};
    ::scl::any_view const anyv{boxed}; // std::any backing — names the backing, not the boxed type
    ::std::cout << "any   has_value=" << anyv.has_value() << " type_name=\"" << anyv.type_name() << "\"\n";
#endif
    ::std::cout << "empty has_value=" << empty.has_value() << '\n'; // 0
}

// ============================================================================
// main
// ============================================================================

int main(int, char **)
{
    ::std::cout << "=== One function, two backings ===\n";

    ::std::string greeting{"Hello Any!"};
    ::std::cout << "raw std::string lvalue:\n";
    bar(greeting); // raw backing, no copy

#if SCL_HAS_RTTI
    ::std::any boxed{greeting};
    ::std::cout << "std::any:\n";
    bar(boxed); // std::any backing, no copy
#endif

    int number = 42;
    ::std::cout << "int lvalue (mismatch):\n";
    bar(number); // pointer form returns nullptr

    ::std::cout << "\n=== Reference-form cast ===\n";
    show_reference_cast();

    ::std::cout << "\n=== Identity queries ===\n";
    show_identity();

    return {};
}
