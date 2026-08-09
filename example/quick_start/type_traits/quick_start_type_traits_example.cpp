/**
 * @example quick_start_type_traits_example.cpp
 * @brief Quick start: picking an overload and carrying a value category over.
 */

//! [quick_start]
#include <scl/utility/type_traits/forward_like.h>
#include <scl/utility/type_traits/overload_cast.h>

#include <iostream>
#include <type_traits>

struct widget
{
    void update(int) { ::std::cout << "update(int)\n"; }
    void update(double) { ::std::cout << "update(double)\n"; }
};

// The value category and cv-qualification of the owner, applied to a member type.
static_assert(::std::is_same_v<::scl::forward_like_t<widget const &, int>, int const &>);
static_assert(::std::is_same_v<::scl::forward_like_t<widget &&, int>, int &&>);

int main()
{
    auto const update_int = ::scl::overload_cast<int>(&widget::update);

    widget instance;
    (instance.*update_int)(1); // update(int)
}
//! [quick_start]
