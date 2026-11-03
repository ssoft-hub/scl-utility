/**
 * @example type_traits_forward_like_example.cpp
 * @brief Passes a member on with the value category and the cv-qualifiers of its object.
 *
 * scl::forward_like<Owner>(member) is an lvalue where Owner is an lvalue reference and an rvalue
 * otherwise, and adds the const and volatile of Owner to those of the member.
 */

#include <scl/utility/type_traits/forward_like.h>

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

//! [type]
static_assert(std::is_same_v<scl::forward_like_t<int const &, double>, double const &>);
static_assert(std::is_same_v<scl::forward_like_t<int &&, double const>, double const &&>);
static_assert(std::is_same_v<scl::forward_like_t<int, double &>, double &&>);
static_assert(std::is_same_v<scl::forward_like_t<int volatile &, double>, double volatile &>);
//! [type]

//! [member]
struct label
{
    std::string text;
};

template <typename Owner>
decltype(auto) text_of(Owner && owner)
{
    return scl::forward_like<Owner>(owner.text);
}

static_assert(std::is_same_v<decltype(text_of(std::declval<label &>())), std::string &>);
static_assert(std::is_same_v<decltype(text_of(std::declval<label const &>())), std::string const &>);
static_assert(std::is_same_v<decltype(text_of(std::declval<label>())), std::string &&>);
//! [member]

int main()
{
    //! [move]
    label first{"first"};
    std::string const copied = text_of(first);
    std::string const moved = text_of(std::move(first));
    std::cout << copied << ' ' << moved << '\n'; // first first
    //! [move]
}
