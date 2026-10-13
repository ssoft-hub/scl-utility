/**
 * @example meta_is_tu_local_example.cpp
 * @brief Tells a type local to the translation unit from a type with external linkage.
 *
 * A type declared in an unnamed namespace is local to its translation unit, and so is
 * a pointer to it or a specialization over it.
 */

#include <scl/utility/meta/tu_local.h>

#include <iostream>
#include <vector>

//! [types]
namespace
{
    struct duck
    {};
} // namespace

struct goose
{};
//! [types]

//! [is_tu_local]
static_assert(scl::is_tu_local_v<duck>);
static_assert(scl::is_tu_local_v<duck const *>);
static_assert(scl::is_tu_local_v<std::vector<duck>>);
static_assert(!scl::is_tu_local_v<goose>);
static_assert(!scl::is_tu_local_v<int>);
//! [is_tu_local]

int main()
{
    std::cout << std::boolalpha << scl::is_tu_local<duck>::value << '\n';
    std::cout << std::boolalpha << scl::is_tu_local<goose>::value << '\n';
}
