/**
 * @example type_traits_overload_cast_example.cpp
 * @brief Takes the address of one overload of a function, chosen by its parameter types.
 *
 * scl::overload_cast<Arguments...>(&f) returns the pointer to the overload of f whose parameter types
 * are Arguments..., whatever its result, cv-qualifiers, ref-qualifier and noexcept, and
 * scl::member_overload_cast<Type, Arguments...>(&Class::f) the one qualified as Type.
 */

#include <scl/utility/type_traits/detection.h>
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

class counter
{
private:
    int m_count{};

public:
    int & count() & { return m_count; }
    int count() const & { return m_count; }
};

class gauge
{
private:
    int m_level{};

public:
    int & level() { return m_level; }
    int level() const { return m_level; }
};
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

//! [qualified]
auto const count_mutable = scl::member_overload_cast<counter &>(&counter::count);
auto const count_const = scl::member_overload_cast<counter const &>(&counter::count);

static_assert(std::is_same_v<decltype(count_mutable), int & (counter::* const)() &>);
static_assert(std::is_same_v<decltype(count_const), int (counter::* const)() const &>);
//! [qualified]

//! [no_ref_qualifier]
template <typename Type>
using level_cast = decltype(scl::member_overload_cast<Type>(&gauge::level));

[[maybe_unused]]
auto const level_const = scl::member_overload_cast<gauge const>(&gauge::level);

static_assert(std::is_same_v<decltype(level_const), int (gauge::* const)() const>);
static_assert(!scl::is_detected_v<level_cast, gauge const &>);
//! [no_ref_qualifier]

int twice(int value) { return value * 2; }

double twice(double value) noexcept { return value * 2; }

int main()
{
    //! [call]
    canvas board;
    (board.*resize_two)(3, 4);
    counter clicks;
    (clicks.*count_mutable)() = 7;
    int const count = (clicks.*count_const)();
    std::cout << board.width() << ' ' << twice_int(21) << ' ' << count << '\n'; // 12 42 7
    //! [call]
}
