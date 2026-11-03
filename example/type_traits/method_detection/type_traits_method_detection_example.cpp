/**
 * @example type_traits_method_detection_example.cpp
 * @brief Tells whether a member function of a given name accepts given arguments.
 *
 * SCL_METHOD_DETECTION(name) declares, where it is expanded, the traits has_name_method_v,
 * has_name_method_exact_v, has_name_static_method_v and has_name_static_method_exact_v with
 * their operations and result types.
 */

#include <scl/utility/type_traits/detection/method.h>

#include <iostream>
#include <type_traits>
#include <utility>

//! [declare]
class counter
{
private:
    int m_value{};

public:
    int value() const noexcept { return m_value; }
    void add(int step) & { m_value += step; }
    static counter zero() { return {}; }
};

namespace counter_traits
{
    SCL_METHOD_DETECTION(value)
    SCL_METHOD_DETECTION(add)
    SCL_METHOD_DETECTION(zero)
} // namespace counter_traits
//! [declare]

//! [call]
static_assert(counter_traits::has_value_method_v<counter>);
static_assert(counter_traits::has_value_method_v<counter const &>);
static_assert(std::is_same_v<counter_traits::value_method_t<counter>, int>);
static_assert(counter_traits::has_add_method_v<counter &, int>);
static_assert(counter_traits::has_add_method_v<counter &, short>);
static_assert(!counter_traits::has_add_method_v<counter, int>);
//! [call]

//! [exact]
static_assert(counter_traits::has_value_method_exact_v<counter const>);
static_assert(!counter_traits::has_value_method_exact_v<counter>);
static_assert(counter_traits::has_add_method_exact_v<counter &, int>);
static_assert(!counter_traits::has_add_method_exact_v<counter &, short>);
//! [exact]

//! [properties]
static_assert(std::is_same_v<counter_traits::value_method_exact_t<counter const>, int>);
static_assert(noexcept(std::declval<counter const &>().value()));
//! [properties]

//! [static]
static_assert(counter_traits::has_zero_static_method_v<counter>);
static_assert(counter_traits::has_zero_static_method_exact_v<counter>);
static_assert(!counter_traits::has_value_static_method_v<counter>);
//! [static]

int main()
{
    counter total = counter::zero();
    total.add(5);
    std::cout << total.value() << '\n'; // 5
}
