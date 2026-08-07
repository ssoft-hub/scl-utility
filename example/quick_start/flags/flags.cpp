/**
 * @example flags.cpp
 * @brief Quick start: a type-safe bitmask over a scoped enum.
 */

//! [quick_start]
#include <scl/utility/flags.h>

#include <iostream>

enum class permission
{
    read,
    write,
    execute,
};

using permissions = ::scl::flags<permission>;

constexpr permissions read_write{permission::read, permission::write};

static_assert(read_write.all_of(permission::read, permission::write));
static_assert(read_write.none_of(permission::execute));

int main()
{
    permissions granted = read_write;
    granted |= permission::execute;

    ::std::cout
        << granted[permission::read]    // 1
        << granted[permission::execute] // 1
        << granted.size() << '\n';      // 3
}
//! [quick_start]
