/**
 * @example flags_common_example.cpp
 * @brief Demonstrates scl::flags — a type-safe bitmask over a scoped enum.
 *
 * scl::flags<Enum> stores one bit per enumerator (bit index == enumerator
 * ordinal). It combines flags through bitwise operators without casting to the
 * underlying integer, answers subset/intersection questions, and iterates the
 * set flags as a bidirectional range. Everything is constexpr-capable.
 */

#include <scl/utility/flags.h>

#include <iostream>

enum class permission
{
    read,    // bit 0
    write,   // bit 1
    execute, // bit 2
    remove,  // bit 3
};

using permissions = ::scl::flags<permission>;

// Compile-time construction and combination — no runtime cost.
constexpr permissions read_write{permission::read, permission::write};
constexpr permissions all_but_remove = read_write | permission::execute;

static_assert(read_write.all_of(permission::read, permission::write));
static_assert(all_but_remove.any_of(permission::execute));
static_assert(all_but_remove.none_of(permission::remove));
static_assert(read_write.size() == 2);

// ============================================================================
// Pattern 1 — combine and query flags
// ============================================================================

static void show_combine()
{
    permissions granted{permission::read};
    granted |= permission::write; // add a flag
    granted |= permissions{permission::execute};

    ::std::cout << "read?    " << granted[permission::read] << '\n';                           // 1
    ::std::cout << "remove?  " << granted[permission::remove] << '\n';                         // 0
    ::std::cout << "any rw?  " << granted.any_of(permission::read, permission::write) << '\n'; // 1
    ::std::cout << "all rw?  " << granted.all_of(permission::read, permission::write) << '\n'; // 1
    ::std::cout << "count    " << granted.size() << '\n';                                      // 3
}

// ============================================================================
// Pattern 2 — set algebra between two masks
// ============================================================================

static void show_set_algebra()
{
    constexpr permissions required{permission::read, permission::write};
    permissions const granted{permission::read, permission::write, permission::execute};

    ::std::cout << "granted covers required? " << granted.all_of(required) << '\n'; // 1 (subset)
    ::std::cout << "overlap?                 " << granted.any_of(required) << '\n'; // 1 (intersection)
    ::std::cout << "missing (required - granted) empty? " << (required - granted).none() << '\n'; // 1
}

// ============================================================================
// Pattern 3 — iterate the set flags (bidirectional range)
// ============================================================================

static void show_iteration()
{
    permissions const granted{permission::read, permission::execute};

    ::std::cout << "set ordinals:";
    for (permission const flag : granted)
        ::std::cout << ' ' << static_cast<int>(flag); // 0 2 (ascending)
    ::std::cout << '\n';

    ::std::cout << "reversed:    ";
    for (auto it = granted.rbegin(); it != granted.rend(); ++it)
        ::std::cout << ' ' << static_cast<int>(*it); // 2 0
    ::std::cout << '\n';
}

int main(int, char **)
{
    ::std::cout << "=== Combine and query ===\n";
    show_combine();

    ::std::cout << "\n=== Set algebra ===\n";
    show_set_algebra();

    ::std::cout << "\n=== Iteration ===\n";
    show_iteration();

    return {};
}
