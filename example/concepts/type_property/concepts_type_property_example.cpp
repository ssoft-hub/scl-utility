/**
 * @example concepts_type_property_example.cpp
 * @brief Checks the properties of a type, such as trivial copying and standard layout.
 *
 * Each concept is satisfied exactly where its standard type trait is true. An object of a type that
 * satisfies scl::concepts::trivially_copyable is copied here with std::memcpy. The concept
 * scl::concepts::abstract subsumes scl::concepts::polymorphic, and both subsume
 * scl::concepts::class_type, so the compiler chooses the overload constrained by the narrowest
 * concept a class satisfies.
 */

#include <scl/utility/concepts/type_category.h>

#include <cstring>
#include <iostream>
#include <string>
#include <string_view>

//! [types]
struct sample
{
    int id;
    double value;
};

struct tag
{};

struct shape
{
    virtual ~shape() = default;
    [[nodiscard]]
    virtual double area() const = 0;
};

struct square final : shape
{
    double side = 0.0;

    [[nodiscard]]
    double area() const override
    {
        return side * side;
    }
};

struct counter
{
    counter() = default;
    explicit counter(int start) noexcept
        : count{start}
    {}

    int count = 0;
};
//! [types]

//! [trivially_copyable]
static_assert(scl::concepts::trivially_copyable<int>);
static_assert(scl::concepts::trivially_copyable<sample>);
static_assert(!scl::concepts::trivially_copyable<std::string>);
static_assert(!scl::concepts::trivially_copyable<square>);
//! [trivially_copyable]

//! [standard_layout]
static_assert(scl::concepts::standard_layout<int>);
static_assert(scl::concepts::standard_layout<sample>);
static_assert(!scl::concepts::standard_layout<square>);
//! [standard_layout]

//! [empty]
static_assert(scl::concepts::empty<tag>);
static_assert(!scl::concepts::empty<sample>);
static_assert(!scl::concepts::empty<shape>);
static_assert(!scl::concepts::empty<int>);
//! [empty]

//! [polymorphic]
static_assert(scl::concepts::polymorphic<shape>);
static_assert(scl::concepts::polymorphic<square>);
static_assert(!scl::concepts::polymorphic<sample>);
//! [polymorphic]

//! [abstract]
static_assert(scl::concepts::abstract<shape>);
static_assert(!scl::concepts::abstract<square>);
static_assert(!scl::concepts::abstract<int>);
//! [abstract]

//! [final]
static_assert(scl::concepts::final<square>);
static_assert(!scl::concepts::final<shape>);
static_assert(!scl::concepts::final<int>);
//! [final]

//! [aggregate]
static_assert(scl::concepts::aggregate<sample>);
static_assert(scl::concepts::aggregate<int[3]>);
static_assert(!scl::concepts::aggregate<counter>);
static_assert(!scl::concepts::aggregate<int>);
//! [aggregate]

//! [copy_bytes]
template <scl::concepts::trivially_copyable Value>
void copy_bytes(Value & target, Value const & source) noexcept
{
    std::memcpy(&target, &source, sizeof(Value));
}
//! [copy_bytes]

//! [describe]
template <scl::concepts::class_type Class>
constexpr std::string_view describe()
{
    return "class";
}

template <scl::concepts::polymorphic Class>
constexpr std::string_view describe()
{
    return "polymorphic class";
}

template <scl::concepts::abstract Class>
constexpr std::string_view describe()
{
    return "abstract class";
}
//! [describe]

int main()
{
    //! [use]
    sample const source{7, 2.5};
    sample target{};
    copy_bytes(target, source);
    std::cout << target.id << ' ' << target.value << '\n'; // 7 2.5

    std::cout << describe<sample>() << '\n'; // class
    std::cout << describe<square>() << '\n'; // polymorphic class
    std::cout << describe<shape>() << '\n';  // abstract class
    //! [use]
}
