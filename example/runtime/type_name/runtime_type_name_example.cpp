/**
 * @example runtime_type_name_example.cpp
 * @brief Names the type of an object through RTTI, beside the compile-time name of its type.
 *
 * scl::type_name<T>() names the type written at the call site, at compile time;
 * scl::type_name(obj) names the type the object was created as, through RTTI. The two agree
 * on a static type and part ways behind a base reference. Without RTTI only the compile-time
 * form exists, and a caller branches on SCL_HAS_RTTI.
 */

#include <scl/utility/meta/type.h>
#include <scl/utility/preprocessor/rtti.h>
#include <scl/utility/runtime/type.h>

#include <iostream>
#include <memory>
#include <string>

//! [types]
namespace app
{
    struct Base
    {
        virtual ~Base() = default;
    };

    struct Derived : Base
    {};

    template <typename T>
    struct Task : Base
    {};
} // namespace app
//! [types]

//! [no_rtti]
template <typename T>
::std::string name_of(T const & object)
{
#if SCL_HAS_RTTI
    return ::scl::type_name(object); // the dynamic type of a polymorphic object
#else
    static_cast<void>(object);
    return ::std::string{::scl::type_name<T>()}; // the static type only
#endif
}
//! [no_rtti]

static void show_static()
{
    ::app::Derived const derived;

    ::std::cout << "compile time: " << ::scl::type_name<::app::Derived>() << '\n';
    ::std::cout << "run time    : " << name_of(derived) << '\n';
}

static void show_dynamic()
{
#if SCL_HAS_RTTI
    //! [dynamic]
    ::std::unique_ptr<::app::Base> const pointer = ::std::make_unique<::app::Derived>();

    auto const static_name = ::scl::type_name<::app::Base>(); // app::Base
    auto const dynamic_name = ::scl::type_name(*pointer); // app::Derived, or struct app::Derived
    //! [dynamic]

    ::std::cout << "compile time: " << static_name << '\n';
    ::std::cout << "run time    : " << dynamic_name << '\n';
#else
    ::std::cout << "run time    : not declared without RTTI\n";
#endif
}

static void show_short()
{
#if SCL_HAS_RTTI
    //! [short]
    ::std::unique_ptr<::app::Base> const pointer = ::std::make_unique<::app::Task<int>>();

    auto const full_name = ::scl::type_name(*pointer); // app::Task<int>, or struct app::Task<int>
    auto const short_name = ::scl::type_short_name(*pointer); // Task
    //! [short]

    ::std::cout << "type_name       : " << full_name << '\n';
    ::std::cout << "type_short_name : " << short_name << '\n';
#else
    ::std::cout << "type_short_name : not declared without RTTI\n";
#endif
}

int main(int, char **)
{
    ::std::cout << "=== Static type ===\n";
    show_static();

    ::std::cout << "\n=== Through a base pointer ===\n";
    show_dynamic();

    ::std::cout << "\n=== Short name ===\n";
    show_short();

    return {};
}
