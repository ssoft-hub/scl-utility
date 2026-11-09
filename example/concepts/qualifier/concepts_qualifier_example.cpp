/**
 * @example concepts_qualifier_example.cpp
 * @brief Checks the top-level cv-qualifiers of a type.
 *
 * The concepts scl::concepts::const_qualified and scl::concepts::volatile_qualified are satisfied
 * by the qualifiers of the type itself, not of the type a reference or a pointer refers to.
 * The concept scl::concepts::cv_qualified is satisfied where both are, and an overload constrained
 * by it is preferred.
 */

#include <scl/utility/concepts/qualifier.h>

#include <iostream>
#include <string_view>

//! [const_qualified]
static_assert(scl::concepts::const_qualified<int const>);
static_assert(scl::concepts::const_qualified<int const volatile>);
static_assert(scl::concepts::const_qualified<int * const>);
static_assert(scl::concepts::const_qualified<int const[3]>);
static_assert(!scl::concepts::const_qualified<int>);
static_assert(!scl::concepts::const_qualified<int const *>);
static_assert(!scl::concepts::const_qualified<int const &>);
//! [const_qualified]

//! [volatile_qualified]
static_assert(scl::concepts::volatile_qualified<int volatile>);
static_assert(scl::concepts::volatile_qualified<int const volatile>);
static_assert(scl::concepts::volatile_qualified<int volatile[3]>);
static_assert(!scl::concepts::volatile_qualified<int>);
static_assert(!scl::concepts::volatile_qualified<int volatile *>);
static_assert(!scl::concepts::volatile_qualified<int volatile &>);
//! [volatile_qualified]

//! [cv_qualified]
static_assert(scl::concepts::cv_qualified<int const volatile>);
static_assert(!scl::concepts::cv_qualified<int const>);
static_assert(!scl::concepts::cv_qualified<int volatile>);
static_assert(!scl::concepts::cv_qualified<int const volatile &>);
//! [cv_qualified]

//! [qualifiers]
template <scl::concepts::const_qualified Value>
std::string_view qualifiers(Value &)
{
    return "const";
}

template <scl::concepts::cv_qualified Value>
std::string_view qualifiers(Value &)
{
    return "const volatile";
}
//! [qualifiers]

//! [reset]
template <typename Value>
void reset(Value & value)
    requires(!scl::concepts::const_qualified<Value>)
{
    value = Value{};
}

template <typename Value>
concept resettable = requires(Value & value) { reset(value); };

static_assert(resettable<int>);
static_assert(!resettable<int const>);
//! [reset]

int main()
{
    //! [use]
    int counter = 5;
    reset(counter);
    std::cout << counter << '\n'; // 0

    int const fixed = 1;
    int const volatile shared = 2;
    std::cout << qualifiers(fixed) << '\n';  // const
    std::cout << qualifiers(shared) << '\n'; // const volatile
    //! [use]
}
