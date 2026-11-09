/**
 * @example concepts_type_category_example.cpp
 * @brief Checks the category of a type, such as enumeration, pointer, array and class.
 *
 * Each concept is satisfied exactly where its standard type trait is true. The concept
 * scl::concepts::scoped_enum subsumes scl::concepts::enum_type, so an overload constrained by it is
 * chosen for a scoped enumeration.
 */

#include <scl/utility/concepts/type_category.h>

#include <cstddef>
#include <iostream>
#include <string_view>
#include <type_traits>

//! [types]
enum plain_color
{
    red,
    green
};

enum class direction : unsigned char
{
    north,
    south
};

struct point
{
    int x;
    int y;
};

union number
{
    int integer;
    float real;
};
//! [types]

//! [enum_type]
static_assert(scl::concepts::enum_type<plain_color>);
static_assert(scl::concepts::enum_type<direction const>);
static_assert(!scl::concepts::enum_type<int>);
static_assert(!scl::concepts::enum_type<direction &>);
//! [enum_type]

//! [enum_value]
static_assert(scl::concepts::enum_value<direction::north>);
static_assert(scl::concepts::enum_value<red>);
static_assert(!scl::concepts::enum_value<42>);
//! [enum_value]

//! [scoped_enum]
static_assert(scl::concepts::scoped_enum<direction>);
static_assert(scl::concepts::scoped_enum<direction const>);
static_assert(!scl::concepts::scoped_enum<plain_color>);
static_assert(!scl::concepts::scoped_enum<int>);
//! [scoped_enum]

//! [void_type]
static_assert(scl::concepts::void_type<void>);
static_assert(scl::concepts::void_type<void const>);
static_assert(!scl::concepts::void_type<void *>);
static_assert(!scl::concepts::void_type<int>);
//! [void_type]

//! [pointer]
static_assert(scl::concepts::pointer<int *>);
static_assert(scl::concepts::pointer<int const * const>);
static_assert(scl::concepts::pointer<void *>);
static_assert(scl::concepts::pointer<void (*)(int)>);
static_assert(!scl::concepts::pointer<std::nullptr_t>);
static_assert(!scl::concepts::pointer<int point::*>);
static_assert(!scl::concepts::pointer<int *&>);
//! [pointer]

//! [null_pointer]
static_assert(scl::concepts::null_pointer<std::nullptr_t>);
static_assert(scl::concepts::null_pointer<std::nullptr_t const>);
static_assert(!scl::concepts::null_pointer<void *>);
//! [null_pointer]

//! [member_pointer]
static_assert(scl::concepts::member_pointer<int point::*>);
static_assert(scl::concepts::member_pointer<int (point::*)() const noexcept>);
static_assert(!scl::concepts::member_pointer<int *>);
//! [member_pointer]

//! [member_object_pointer]
static_assert(scl::concepts::member_object_pointer<int point::*>);
static_assert(!scl::concepts::member_object_pointer<int (point::*)() const noexcept>);
//! [member_object_pointer]

//! [member_function_pointer]
static_assert(scl::concepts::member_function_pointer<int (point::*)() const noexcept>);
static_assert(!scl::concepts::member_function_pointer<int point::*>);
static_assert(!scl::concepts::member_function_pointer<void (*)()>);
//! [member_function_pointer]

//! [array]
static_assert(scl::concepts::array<int[3]>);
static_assert(scl::concepts::array<int[]>);
static_assert(!scl::concepts::array<int *>);
//! [array]

//! [bounded_array]
static_assert(scl::concepts::bounded_array<int[3]>);
static_assert(scl::concepts::bounded_array<int[2][3]>);
static_assert(!scl::concepts::bounded_array<int[]>);
//! [bounded_array]

//! [unbounded_array]
static_assert(scl::concepts::unbounded_array<int[]>);
static_assert(scl::concepts::unbounded_array<int[][3]>);
static_assert(!scl::concepts::unbounded_array<int[3]>);
//! [unbounded_array]

//! [function]
static_assert(scl::concepts::function<void(int)>);
static_assert(scl::concepts::function<int(int, double) const noexcept>);
static_assert(!scl::concepts::function<void (*)(int)>);
static_assert(!scl::concepts::function<void (&)(int)>);
//! [function]

//! [class_type]
static_assert(scl::concepts::class_type<point>);
static_assert(!scl::concepts::class_type<number>);
static_assert(!scl::concepts::class_type<plain_color>);
//! [class_type]

//! [union_type]
static_assert(scl::concepts::union_type<number>);
static_assert(!scl::concepts::union_type<point>);
//! [union_type]

//! [fundamental]
static_assert(scl::concepts::fundamental<int>);
static_assert(scl::concepts::fundamental<void>);
static_assert(scl::concepts::fundamental<std::nullptr_t>);
static_assert(!scl::concepts::fundamental<int *>);
static_assert(!scl::concepts::fundamental<plain_color>);
//! [fundamental]

//! [arithmetic]
static_assert(scl::concepts::arithmetic<int>);
static_assert(scl::concepts::arithmetic<bool>);
static_assert(scl::concepts::arithmetic<double const>);
static_assert(!scl::concepts::arithmetic<plain_color>);
static_assert(!scl::concepts::arithmetic<int *>);
//! [arithmetic]

//! [scalar]
static_assert(scl::concepts::scalar<int>);
static_assert(scl::concepts::scalar<plain_color>);
static_assert(scl::concepts::scalar<int *>);
static_assert(scl::concepts::scalar<int point::*>);
static_assert(scl::concepts::scalar<std::nullptr_t>);
static_assert(!scl::concepts::scalar<point>);
static_assert(!scl::concepts::scalar<int[3]>);
//! [scalar]

//! [object]
static_assert(scl::concepts::object<int>);
static_assert(scl::concepts::object<int[3]>);
static_assert(scl::concepts::object<point>);
static_assert(!scl::concepts::object<void>);
static_assert(!scl::concepts::object<int &>);
static_assert(!scl::concepts::object<void(int)>);
//! [object]

//! [compound]
static_assert(scl::concepts::compound<int *>);
static_assert(scl::concepts::compound<int &>);
static_assert(scl::concepts::compound<point>);
static_assert(scl::concepts::compound<plain_color>);
static_assert(scl::concepts::compound<void(int)>);
static_assert(!scl::concepts::compound<int>);
static_assert(!scl::concepts::compound<void>);
//! [compound]

//! [describe]
template <scl::concepts::enum_type Enum>
std::string_view describe(Enum)
{
    return "enumeration";
}

template <scl::concepts::scoped_enum Enum>
std::string_view describe(Enum)
{
    return "scoped enumeration";
}
//! [describe]

//! [underlying]
template <scl::concepts::enum_type Enum>
constexpr std::underlying_type_t<Enum> to_underlying(Enum value) noexcept
{
    return static_cast<std::underlying_type_t<Enum>>(value);
}

template <auto Value>
    requires scl::concepts::enum_value<Value>
constexpr auto underlying_of = to_underlying(Value);

static_assert(underlying_of<direction::south> == 1);
//! [underlying]

int main()
{
    //! [use]
    std::cout << describe(green) << '\n';            // enumeration
    std::cout << describe(direction::north) << '\n'; // scoped enumeration
    //! [use]
}
