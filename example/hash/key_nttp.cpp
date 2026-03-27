/// @file key_nttp.cpp
/// @brief Demonstrates scl::hash::key<> as a non-type template parameter (NTTP).
///
/// In C++20 a class type may be used as an NTTP if it is a *structural type*:
/// all non-static data members must be public and of structural types.
/// scl::hash::key<> stores a single integral value, so it qualifies.
///
/// This enables three useful compile-time patterns:
///   1. Distinct types tagged by a string literal.
///   2. Template specialisation selected by a string value.
///   3. Generic dispatch functions keyed on compile-time strings.

#include <scl/utility/hash/key.h>

#include <iostream>
#include <type_traits>

using ::scl::hash::key;

// ============================================================================
// Pattern 1 — type tag from a compile-time string
//
// Two instantiations with different string keys are different types;
// two instantiations with the same string key are the same type.
// ============================================================================

template <key<> Tag>
struct event_tag
{
    static constexpr key<> id = Tag;
};

static_assert(!std::is_same_v<event_tag<key<>{"start"}>, event_tag<key<>{"stop"}>>);
static_assert(std::is_same_v<event_tag<key<>{"start"}>, event_tag<key<>{"start"}>>);

// ============================================================================
// Pattern 2 — template specialisation by string key
//
// Define a primary template and specialise it for each known command string.
// The compiler selects the correct specialisation entirely at compile time.
// ============================================================================

template <key<> Cmd>
struct command_handler
{
    static void execute() { std::cout << "(unknown command)\n"; }
};

template <>
struct command_handler<key<>{"start"}>
{
    static void execute() { std::cout << "Starting...\n"; }
};

template <>
struct command_handler<key<>{"stop"}>
{
    static void execute() { std::cout << "Stopping...\n"; }
};

template <>
struct command_handler<key<>{"status"}>
{
    static void execute() { std::cout << "Status: all systems nominal.\n"; }
};

// ============================================================================
// Pattern 3 — generic dispatch function keyed on a compile-time string
// ============================================================================

template <key<> Cmd>
void dispatch()
{
    command_handler<Cmd>::execute();
}

// ============================================================================
// main
// ============================================================================

int main()
{
    // Pattern 1: types are distinct for different keys.
    using start_event = event_tag<key<>{"start"}>;
    using stop_event = event_tag<key<>{"stop"}>;
    static_assert(!std::is_same_v<start_event, stop_event>);
    static_assert(start_event::id == key<>{"start"});

    // Pattern 2 & 3: specialisation selected by compile-time string.
    dispatch<key<>{"start"}>();  // Starting...
    dispatch<key<>{"stop"}>();   // Stopping...
    dispatch<key<>{"status"}>(); // Status: all systems nominal.
    dispatch<key<>{"reboot"}>(); // (unknown command)
}
