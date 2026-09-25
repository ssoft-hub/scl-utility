#pragma once

/**
 * @file
 * @brief Name of the type of an object, read through RTTI.
 * @ingroup scl_utility_runtime
 */

#include <scl/utility/preprocessor/rtti.h>

#if SCL_HAS_RTTI || defined(DOXYGEN)

#include <string>
#include <typeinfo>

#include <scl/utility/meta/type.h>

#ifdef __has_include
#if __has_include(<cxxabi.h>)
#include <cstdlib>
#include <cxxabi.h>
#include <memory>
/**
 * @internal
 * @def SCL_DETAIL_HAS_CXXABI
 * @ingroup scl_utility_runtime
 * @brief Whether `<cxxabi.h>` exists, so `abi::__cxa_demangle` can demangle a `typeid` name.
 */
#define SCL_DETAIL_HAS_CXXABI 1
#endif
#endif

namespace scl::detail
{
    inline ::std::string demangle(char const * mangled)
    {
#ifdef SCL_DETAIL_HAS_CXXABI
        int status = 0;
        ::std::unique_ptr<char, decltype(&::std::free)> result{
            ::abi::__cxa_demangle(mangled, nullptr, nullptr, &status), ::std::free};
        if (status == 0 && result != nullptr)
            return ::std::string{result.get()};
        return ::std::string{mangled};
#else
        return ::std::string{mangled};
#endif
    }

} // namespace scl::detail

namespace scl
{
    template <typename T>
    [[nodiscard]]
    ::std::string type_name(T const & obj)
    {
        return ::scl::detail::demangle(typeid(obj).name());
    }

    template <typename T>
    [[nodiscard]]
    ::std::string type_short_name(T const & obj)
    {
        ::std::string const full = ::scl::detail::demangle(typeid(obj).name());
        return ::std::string{::scl::detail::short_name_from(full)};
    }

} // namespace scl

// -----------------------------------------------------------------------------
// Documentation
// -----------------------------------------------------------------------------

/**
 * @fn scl::type_name(T const & obj)
 * @ingroup scl_utility_runtime
 * @brief Returns the name of the dynamic type of a polymorphic @p obj, or of its static type.
 *
 * The type is determined with the operator `typeid`. For an object reached through a pointer or a
 * reference to a polymorphic base class the function template therefore returns the name of the
 * class the object was created as, and through a pointer or a reference to a base class with no
 * virtual function it returns the name of that base class.
 *
 * Where the header `<cxxabi.h>` exists, as with GCC and with Clang on Linux, macOS and MinGW, the
 * name is demangled with the function `abi::__cxa_demangle`. Elsewhere, as with MSVC and with Clang
 * on the MSVC library, the result of `typeid().name()` is returned unchanged, with a prefix such as
 * `class` or `struct` where that name carries one. Where demangling fails, the name is returned
 * unchanged.
 *
 * @snippet runtime/type_name/runtime_type_name_example.cpp dynamic
 *
 * @tparam T Static type of the object, deduced.
 * @param obj Object whose type is named.
 * @return The name of the type described above.
 *
 * @note Declared only where the macro `SCL_HAS_RTTI` is `1`.
 */

/**
 * @fn scl::type_short_name(T const & obj)
 * @ingroup scl_utility_runtime
 * @brief Returns ::scl::type_name(obj) without qualifiers, template arguments or a class prefix.
 *
 * For the type `app::Task<int>` the result is `Task`.
 *
 * @snippet runtime/type_name/runtime_type_name_example.cpp short
 *
 * @tparam T Static type of the object, deduced.
 * @param obj Object whose type is named.
 * @return The unqualified identifier of that type.
 *
 * @note Declared only where the macro `SCL_HAS_RTTI` is `1`.
 */

#endif // SCL_HAS_RTTI || DOXYGEN
