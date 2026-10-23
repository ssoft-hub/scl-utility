/**
 * @example type_traits_detection_example.cpp
 * @brief Tells whether an expression over given types is well-formed, and the type it yields.
 *
 * An operation is an alias template whose definition is the expression. The variable template
 * scl::is_detected_v tells whether it is well-formed for the given types, and the alias template
 * scl::detected_t names the type it yields.
 */

#include <scl/utility/type_traits/detection.h>

#include <array>
#include <cstddef>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

//! [operations]
template <typename Container>
using size_operation = decltype(std::declval<Container const &>().size());

template <typename Container>
using value_type_operation = typename Container::value_type;
//! [operations]

//! [is_detected]
static_assert(scl::is_detected_v<size_operation, std::vector<int>>);
static_assert(!scl::is_detected_v<size_operation, int>);
static_assert(std::is_same_v<scl::is_detected<size_operation, int>, std::false_type>);
//! [is_detected]

//! [detected_t]
static_assert(std::is_same_v<scl::detected_t<value_type_operation, std::vector<long>>, long>);
static_assert(std::is_same_v<scl::detected_t<value_type_operation, int>, scl::nonesuch>);
//! [detected_t]

//! [detected_or]
static_assert(std::is_same_v<scl::detected_or_t<int, value_type_operation, std::string>, char>);
static_assert(std::is_same_v<scl::detected_or_t<int, value_type_operation, double>, int>);
static_assert(scl::detected_or<int, value_type_operation, std::string>::value_t::value);
static_assert(!scl::detected_or<int, value_type_operation, double>::value_t::value);
//! [detected_or]

//! [exact]
static_assert(scl::is_detected_exact_v<std::size_t, size_operation, std::vector<int>>);
static_assert(!scl::is_detected_exact_v<int, size_operation, std::vector<int>>);
static_assert(scl::is_detected_convertible_v<double, size_operation, std::vector<int>>);
static_assert(!scl::is_detected_convertible_v<std::string, size_operation, std::vector<int>>);
//! [exact]

//! [dispatch]
template <typename Container>
std::size_t count_of(Container const & container)
{
    if constexpr (scl::is_detected_v<size_operation, Container>)
        return container.size();
    else
        return 1;
}
//! [dispatch]

int main()
{
    //! [run]
    std::cout << count_of(std::array<int, 3>{1, 2, 3}) << '\n'; // 3
    std::cout << count_of(42) << '\n';                          // 1
    //! [run]
}
