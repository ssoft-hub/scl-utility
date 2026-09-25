/**
 * @example runtime_enum_string_example.cpp
 * @brief Spells an enumeration value as its type and its number, beside the compile-time name.
 *
 * scl::enum_name<V>() names an enumerator known at compile time; scl::enum_string(v) spells
 * any value as "Type::N", one no enumerator names included, and needs no RTTI. A function
 * object given as a second argument spells the number in any other form.
 */

#include <scl/utility/meta/enum.h>
#include <scl/utility/runtime/enum.h>

#include <bitset>
#include <climits>
#include <iostream>
#include <string>

//! [types]
enum class Color : int
{
    Red = 1,
    Blue = -3,
};

enum class Grade : char
{
    Pass = 'P',
};

namespace net
{
    enum class Status : unsigned
    {
        Err = 42,
    };
} // namespace net
//! [types]

//! [concept]
constexpr auto wide = [](unsigned long long number) { return std::to_string(number); };
constexpr auto narrow = [](unsigned char number) { return std::to_string(number); };

static_assert(scl::concepts::enum_string_format<decltype(wide), net::Status>);
static_assert(!scl::concepts::enum_string_format<decltype(narrow), net::Status>);
//! [concept]

static void show_named()
{
    //! [named]
    constexpr auto name = scl::enum_name<Color::Red>(); // Color::Red
    auto const red = scl::enum_string(Color::Red);      // Color::1
    auto const blue = scl::enum_string(Color::Blue);    // Color::-3
    //! [named]

    std::cout << "enum_name  : " << name << '\n';
    std::cout << "enum_string : " << red << '\n';
    std::cout << "enum_string : " << blue << '\n';
}

static void show_unnamed()
{
    //! [unnamed]
    auto const unnamed = scl::enum_string(Color{42}); // Color::42
    //! [unnamed]

    std::cout << "enum_string : " << unnamed << '\n';
}

static void show_underlying()
{
    //! [underlying]
    auto const grade = scl::enum_string(Grade::Pass);       // Grade::80
    auto const status = scl::enum_string(net::Status::Err); // Status::42
    //! [underlying]

    std::cout << "enum_string : " << grade << '\n';
    std::cout << "enum_string : " << status << '\n';
}

static void show_formatted()
{
    //! [formatted]
    auto const binary = [](auto number) {
        return std::bitset<sizeof(number) * CHAR_BIT>(number).to_string();
    };
    auto const grade = scl::enum_string(Grade::Pass, binary); // Grade::01010000
    //! [formatted]

    std::cout << "enum_string : " << grade << '\n';
}

int main(int, char **)
{
    std::cout << "=== Named enumerator ===\n";
    show_named();

    std::cout << "\n=== Value no enumerator names ===\n";
    show_unnamed();

    std::cout << "\n=== Character underlying type, namespaced enumeration ===\n";
    show_underlying();

    std::cout << "\n=== Number in another base ===\n";
    show_formatted();

    return {};
}
