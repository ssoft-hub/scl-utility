/**
 * @example concepts_reference_example.cpp
 * @brief Tells a reference type from a type that is not a reference.
 *
 * The concept scl::concepts::reference is satisfied by an lvalue and an rvalue reference, and
 * scl::concepts::non_reference by every other type. The type deduced for a forwarding reference
 * satisfies scl::concepts::lvalue_reference only for an lvalue argument.
 */

#include <scl/utility/concepts/reference.h>

#include <iostream>
#include <string>
#include <string_view>

//! [reference]
static_assert(scl::concepts::reference<int &>);
static_assert(scl::concepts::reference<int const &&>);
static_assert(!scl::concepts::reference<int>);
static_assert(!scl::concepts::reference<int *>);
//! [reference]

//! [lvalue_reference]
static_assert(scl::concepts::lvalue_reference<int &>);
static_assert(scl::concepts::lvalue_reference<int const &>);
static_assert(!scl::concepts::lvalue_reference<int &&>);
static_assert(!scl::concepts::lvalue_reference<int>);
//! [lvalue_reference]

//! [rvalue_reference]
static_assert(scl::concepts::rvalue_reference<int &&>);
static_assert(scl::concepts::rvalue_reference<int const &&>);
static_assert(!scl::concepts::rvalue_reference<int &>);
static_assert(!scl::concepts::rvalue_reference<int>);
//! [rvalue_reference]

//! [non_reference]
static_assert(scl::concepts::non_reference<int>);
static_assert(scl::concepts::non_reference<int const>);
static_assert(scl::concepts::non_reference<int *>);
static_assert(!scl::concepts::non_reference<int &>);
static_assert(!scl::concepts::non_reference<int &&>);
//! [non_reference]

//! [forwarding]
template <typename Argument>
std::string_view category(Argument &&)
{
    if constexpr (scl::concepts::lvalue_reference<Argument>)
        return "lvalue";
    else
        return "rvalue";
}
//! [forwarding]

//! [box]
template <scl::concepts::non_reference Value>
struct box
{
    Value value;
};
//! [box]

int main()
{
    //! [use]
    std::string text{"text"};
    std::cout << category(text) << '\n';                     // lvalue
    std::cout << category(std::string{"temporary"}) << '\n'; // rvalue

    box<std::string> const label{text};
    std::cout << label.value << '\n'; // text
    //! [use]
}
