/**
 * @example type_traits_member_like_example.cpp
 * @brief Builds the type of a pointer to a member from the type of the object it is used on.
 *
 * scl::member_function_like_t copies the cv-qualifiers and the reference of the object type onto
 * the member function, scl::member_object_like_t copies the cv-qualifiers onto the data member,
 * and scl::member_like_t chooses between the two by the member type. The pointer type picks one
 * overload out of a set that differs only in its qualifiers.
 */

#include <scl/utility/type_traits/member_like.h>

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

//! [types]
struct gauge
{
    int level;
    std::string unit;

    std::string label() const & { return unit; }
    std::string label() && { return std::move(unit); }
    void set(int value) noexcept { level = value; }
};
//! [types]

//! [function]
static_assert(std::is_same_v<
    scl::member_function_like_t<gauge const &, std::string()>, std::string (gauge::*)() const &>);
static_assert(
    std::is_same_v<scl::member_function_like_t<gauge &&, std::string()>, std::string (gauge::*)() &&>);
static_assert(
    std::is_same_v<scl::member_function_like_t<gauge, void(int) noexcept>, void (gauge::*)(int) noexcept>);
//! [function]

//! [object]
static_assert(std::is_same_v<scl::member_object_like_t<gauge, int>, int gauge::*>);
static_assert(std::is_same_v<scl::member_object_like_t<gauge const &, int>, int const gauge::*>);
static_assert(
    std::is_same_v<scl::member_object_like_t<gauge volatile, int const>, int const volatile gauge::*>);
//! [object]

//! [member_like]
static_assert(std::is_same_v<scl::member_like_t<gauge, void(int) noexcept>, decltype(&gauge::set)>);
static_assert(std::is_same_v<scl::member_like_t<gauge, int>, decltype(&gauge::level)>);
//! [member_like]

//! [overload]
template <typename Object>
std::string label_of(Object && object)
{
    auto const label = static_cast<scl::member_like_t<Object &&, std::string()>>(&gauge::label);
    return (std::forward<Object>(object).*label)();
}
//! [overload]

int main()
{
    //! [use]
    gauge tank{7, "litre"};
    gauge const & view = tank;

    std::string const copied = label_of(view);
    std::string const moved = label_of(std::move(tank));
    std::cout << copied << ' ' << moved << '\n'; // litre litre
    //! [use]
}
