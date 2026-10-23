/**
 * @example type_traits_overload_cast_example.cpp
 * @brief Takes the address of one overload of a function, chosen by its parameter types.
 *
 * scl::overload_cast<Args...>(&f) returns the pointer to the overload of f whose parameter types
 * are Args..., whatever its result, cv-qualifiers, ref-qualifier and noexcept.
 */

#include <scl/utility/type_traits/overload_cast.h>

#include <iostream>
#include <type_traits>

//! [types]
class canvas
{
private:
    int m_width{};

public:
    void resize(int width) { m_width = width; }
    void resize(int width, int height) noexcept { m_width = width * height; }
    int width() const & { return m_width; }
};

int twice(int value);
double twice(double value) noexcept;
//! [types]

//! [member]
[[maybe_unused]]
auto const resize_one = scl::overload_cast<int>(&canvas::resize);
auto const resize_two = scl::overload_cast<int, int>(&canvas::resize);

static_assert(std::is_same_v<decltype(resize_one), void (canvas::* const)(int)>);
static_assert(std::is_same_v<decltype(resize_two), void (canvas::* const)(int, int) noexcept>);
static_assert(std::is_same_v<decltype(scl::overload_cast<>(&canvas::width)), int (canvas::*)() const &>);
//! [member]

//! [function]
auto const twice_int = scl::overload_cast<int>(&twice);
[[maybe_unused]]
auto const twice_double = scl::overload_cast<double>(&twice);

static_assert(std::is_same_v<decltype(twice_int), int (* const)(int)>);
static_assert(std::is_same_v<decltype(twice_double), double (* const)(double) noexcept>);
//! [function]

int twice(int value) { return value * 2; }

double twice(double value) noexcept { return value * 2; }

int main()
{
    //! [call]
    canvas board;
    (board.*resize_two)(3, 4);
    std::cout << board.width() << ' ' << twice_int(21) << '\n'; // 12 42
    //! [call]
}
