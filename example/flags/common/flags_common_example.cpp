/**
 * @example flags_common_example.cpp
 * @brief Demonstrates scl::flags — a set of enum values with one bit per enumerator.
 *
 * scl::flags<Enum> stores one bit per enumerator (bit index == enumerator
 * ordinal). It combines sets through operators without casting to the
 * underlying integer, answers subset/intersection questions, and iterates the
 * set flags as a bidirectional range. Everything is constexpr-capable.
 */

#include <scl/utility/flags.h>

#include <iostream>

//! [declare]
enum class permission
{
    read,    // bit 0
    write,   // bit 1
    execute, // bit 2
    remove,  // bit 3
};

using permissions = ::scl::flags<permission>;

// The set the others are measured against. A flags knows its storage width, not which
// values an enumeration declares, so a caller who needs the whole set names it.
constexpr permissions all_permissions{
    permission::read, permission::write, permission::execute, permission::remove};
//! [declare]

//! [membership]
constexpr permissions granted{permission::read, permission::write};

static_assert(granted[permission::read]);
static_assert(granted.all_of(permission::read, permission::write));
static_assert(granted.any_of(permission::write, permission::execute));
static_assert(granted.none_of(permission::remove));

static_assert(granted.all_of(permissions{permission::read}));    // subset
static_assert(granted.any_of(permissions{permission::write}));   // intersection
static_assert(granted.none_of(permissions{permission::remove})); // disjoint

static_assert(granted.any());
static_assert(!granted.none());
static_assert(!granted.all_of(all_permissions)); // two flags of the four
static_assert(granted.size() == 2);
//! [membership]

//! [algebra]
constexpr permissions required{permission::read, permission::execute};

static_assert((granted | required) == permissions{permission::read, permission::write, permission::execute});
static_assert((granted & required) == permissions{permission::read});
static_assert((granted ^ required) == permissions{permission::write, permission::execute});
static_assert((granted - required) == permissions{permission::write});
static_assert((granted | permission::remove).size() == 3); // flags-Enum form

// A complement is taken against a named set, never against the storage width.
static_assert((all_permissions - granted) == permissions{permission::execute, permission::remove});

// The compound forms mutate in place, so they need an object rather than a constant.
constexpr permissions effective = [] {
    permissions result{permission::read};
    result |= permission::write;
    result -= permission::read;
    return result;
}();
static_assert(effective == permissions{permission::write});
//! [algebra]

//! [iteration]
// Iterating yields the set flags as Enum values, ascending by ordinal; the reverse
// iterators walk the same flags back. size() counts them, capacity is the bit width.
static void print_flags(permissions const & flags)
{
    for (permission const flag : flags)
        ::std::cout << ' ' << static_cast<int>(flag);
    ::std::cout << " |";

    for (auto it = flags.rbegin(); it != flags.rend(); ++it)
        ::std::cout << ' ' << static_cast<int>(*it);

    ::std::cout << " (" << flags.size() << " of " << permissions::capacity << ")\n";
}
//! [iteration]

int main(int, char **)
{
    ::std::cout << "granted:  ";
    print_flags(granted); // 0 1 | 1 0 (2 of 32)

    ::std::cout << "required: ";
    print_flags(required); // 0 2 | 2 0 (2 of 32)

    ::std::cout << "missing:  ";
    print_flags(required - granted); // 2 | 2 (1 of 32)

    ::std::cout << "unheld:   ";
    print_flags(all_permissions - granted); // 2 3 | 3 2 (2 of 32)

    return {};
}
