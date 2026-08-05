/**
 * @file any_example.cpp
 * @brief Demonstrates the ScL Any views — scl::any_view and scl::any_arg.
 *
 * scl::any_view is to std::any what std::string_view is to std::string: it
 * refers to an existing value without copying it, so one function can accept a
 * heterogeneous read-only argument at no allocation cost. It is two pointers
 * wide and trivially copyable. A typed lvalue forms the "raw" backing (no RTTI,
 * carries a compile-time type name); a std::any forms the "std::any" backing
 * (RTTI builds only). scl::any_cast recovers the value by pointer (nullptr on
 * mismatch, never throws), by copy, or by const reference (zero-copy).
 *
 * scl::any_arg is the parameter-position companion: it also binds rvalues, is
 * valid only for the duration of the call, and — unlike the view — grants write
 * access when the argument was bound to an unqualified object.
 */

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
// Pattern 4 — any_arg: a parameter that also binds temporaries
// ============================================================================

static void describe(::scl::any_arg value)
{
    // `const` asks to read; the unqualified spelling would ask to write instead.
    if (auto const * text = ::scl::any_cast<::std::string const>(&value))
        ::std::cout << "  std::string: \"" << *text << "\" (no copy)\n";
    else
        ::std::cout << "  something else (type_name=\"" << value.type_name() << "\")\n";
}

static void log_view(::scl::any_view value)
{
    ::std::cout << "  delegated, has_value=" << value.has_value() << '\n';
}

static ::std::string keep(::scl::any_arg value)
{
    // The argument may be a temporary that dies once this call returns, so copy the
    // payload out; keeping the any_arg — or an any_view converted from it — would dangle.
    return ::scl::any_cast<::std::string>(value);
}

// ============================================================================
// Pattern 5 — an argument grants write access, a view never does
// ============================================================================

static void bump(::scl::any_arg value)
{
    if (auto * number = ::scl::any_cast<int>(&value))
        ++*number;
    else
        ::std::cout << "  refused: the referent is not a writable int\n";
}

// ============================================================================
// Pattern 6 — an argument's casts fold at compile time
// ============================================================================

// The cast reaches the referent through an anchor the caller creates per binding,
// and a call outlives its own arguments. So this folds where the same code over an
// any_view would wait for P2738 (C++26).
static constexpr int doubled(::scl::any_arg value)
{
    auto const * number = ::scl::any_cast<int const>(&value);
    return (number != nullptr) ? *number * 2 : 0;
}

static void show_constant_evaluation()
{
    // Block scope, not namespace scope: GCC 13.1 rejects comparing the address of a
    // temporary with nullptr inside a constant expression evaluated at namespace scope,
    // and the argument above binds one. Clang and MSVC accept either placement.
    static_assert(doubled(21) == 42, "an argument casts during constant evaluation");

    ::std::cout << "  doubled(21) folded to " << doubled(21) << " at compile time\n";
}

// ============================================================================
// Pattern 7 — any_switch: one chain instead of a cascade of casts
// ============================================================================

// The chain holds no subject, so it is built once and applied to as many values
// as the caller has. Each branch names its type once; the first match runs.
static auto const describe_switch =
    ::scl::any_switch<::std::string>()
        .in_case<void>("nothing at all")
        .in_case<int>([](int number) { return "int " + ::std::to_string(number); })
        .in_case<::std::string const &>([](::std::string const & text) {
    return "string \"" + text + "\"";
}).or_else([](::scl::any_arg other) {
    return "something else (" + ::std::string{other.type_name()} + ")";
});

static void show_switch()
{
    ::std::string text{"Hello Any!"};
    int number = 42;
    double ratio = 1.5;
    ::scl::any_view const nothing{};

    ::std::cout << "  " << *describe_switch.apply(text) << '\n';
    ::std::cout << "  " << *describe_switch.apply(number) << '\n';
    ::std::cout << "  " << *describe_switch.apply(ratio) << '\n';
    ::std::cout << "  " << *describe_switch.apply(nothing) << '\n';

    // Asking without running a branch: an empty result cannot tell a miss from a
    // branch that produced nothing, and a void chain returns no result at all.
    ::std::cout << "  covered? " << describe_switch.has_case(number) << '\n'; // 1
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

    ::std::cout << "\n=== any_arg: temporaries bind ===\n";
    describe(greeting);                   // lvalue
    describe(::std::string{"temporary"}); // rvalue — outlives the call
    describe(42);
#if SCL_HAS_RTTI
    describe(::std::any{greeting}); // temporary std::any
#endif

    ::std::cout << "\n=== Delegation to an any_view API ===\n";
    log_view(::scl::any_arg{::std::string{"delegated"}}); // implicit conversion

    ::std::cout << "\n=== Copying a value out of a temporary ===\n";
    ::std::cout << "  kept == \"" << keep(::std::string{"kept beyond the call"}) << "\"\n";

    ::std::cout << "\n=== Write access through an argument ===\n";
    bump(number);
    ::std::cout << "  number == " << number << '\n'; // 43
    int const frozen = 1;
    bump(frozen); // const referent: refused

    ::std::cout << "\n=== Casting during constant evaluation ===\n";
    show_constant_evaluation();

    ::std::cout << "\n=== One chain instead of a cascade of casts ===\n";
    show_switch();

    return {};
}
