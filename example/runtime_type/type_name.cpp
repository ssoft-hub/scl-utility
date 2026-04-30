/// @file type_name.cpp
/// @brief Compares compile-time and runtime type name extraction.
///
/// Compile-time:  scl::type_name<T>()       — T is a template parameter; no object required;
///                                             result is a constexpr std::string_view.
/// Runtime:       scl::type_name(obj)        — T is deduced from the object; supports polymorphism
///                                             via RTTI; result is a std::string (heap-allocated).
///
/// When T is statically known, both approaches produce the same name.
/// When an object is accessed through a base pointer only the runtime variant
/// resolves to the actual dynamic type.

#include <scl/utility/meta/type.h>
#include <scl/utility/runtime/type.h>

#include <iostream>
#include <memory>

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

// ============================================================================
// Pattern 1 — static type: both approaches agree
// ============================================================================

static void show_static()
{
    app::Derived d;

    // Compile-time: result is constexpr, evaluated at compile time, no allocation.
    constexpr auto ct_full = ::scl::type_name<app::Derived>();
    constexpr auto ct_short = ::scl::type_short_name<app::Derived>();

    ::std::cout << "[compile-time] type_name       : " << ct_full << '\n';
    ::std::cout << "[compile-time] type_short_name : " << ct_short << '\n';

#if defined(SCL_DETAIL_HAS_RTTI)
    // Runtime: deduces T = app::Derived from the argument; same result here.
    ::std::cout << "[runtime     ] type_name       : " << ::scl::type_name(d) << '\n';
    ::std::cout << "[runtime     ] type_short_name : " << ::scl::type_short_name(d) << '\n';
#endif
}

// ============================================================================
// Pattern 2 — polymorphism: compile-time sees the static type, runtime the dynamic one
// ============================================================================

static void show_polymorphic()
{
#if defined(SCL_DETAIL_HAS_RTTI)
    ::std::unique_ptr<app::Base> p = ::std::make_unique<app::Derived>();

    // Compile-time only knows the static type at the pointer declaration.
    constexpr auto ct_full = ::scl::type_name<app::Base>();
    constexpr auto ct_short = ::scl::type_short_name<app::Base>();

    ::std::cout << "[compile-time] type_name       : " << ct_full << '\n';  // app::Base
    ::std::cout << "[compile-time] type_short_name : " << ct_short << '\n'; // Base

    // Runtime resolves the actual dynamic type through the base pointer via typeid.
    ::std::cout << "[runtime     ] type_name       : " << ::scl::type_name(*p) << '\n'; // app::Derived
    ::std::cout << "[runtime     ] type_short_name : " << ::scl::type_short_name(*p) << '\n'; // Derived
#else
    ::std::cout << "(RTTI disabled — runtime functions not available)\n";
#endif
}

// ============================================================================
// Pattern 3 — template type accessed through a base pointer
// ============================================================================

static void show_template()
{
    // Compile-time: full type including template argument.
    constexpr auto ct_full = ::scl::type_name<app::Task<int>>();
    constexpr auto ct_short = ::scl::type_short_name<app::Task<int>>();

    ::std::cout << "[compile-time] type_name       : " << ct_full << '\n';  // app::Task<int>
    ::std::cout << "[compile-time] type_short_name : " << ct_short << '\n'; // Task

#if defined(SCL_DETAIL_HAS_RTTI)
    ::std::unique_ptr<app::Base> p = ::std::make_unique<app::Task<int>>();

    ::std::cout << "[runtime     ] type_name       : " << ::scl::type_name(*p) << '\n'; // app::Task<int>
    ::std::cout << "[runtime     ] type_short_name : " << ::scl::type_short_name(*p) << '\n'; // Task
#endif
}

// ============================================================================
// main
// ============================================================================

int main(int, char **)
{
    ::std::cout << "=== Static type ===\n";
    show_static();

    ::std::cout << "\n=== Polymorphic type (base pointer -> derived object) ===\n";
    show_polymorphic();

    ::std::cout << "\n=== Template type (base pointer -> Task<int>) ===\n";
    show_template();

    return {};
}
