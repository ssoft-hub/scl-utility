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
 * @brief Whether `<cxxabi.h>` exists, so a name from `typeid` can go through
 *        `abi::__cxa_demangle`.
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
 * @brief Returns the name of the dynamic type of @p obj if its class is polymorphic, and of
 *        its static type otherwise.
 *
 * `typeid` resolves the type, so an object reached through a pointer or a reference to a
 * polymorphic base answers the class it was created as; through a base with no virtual
 * function it answers that base. Where `<cxxabi.h>` exists, as with GCC and with Clang on
 * Linux, macOS and MinGW, the name goes through `abi::__cxa_demangle`; elsewhere, as with
 * MSVC and with Clang on the MSVC library, `typeid().name()` is returned as it is, with a
 * prefix such as `class` or `struct` where the type has one.
 *
 * @snippet runtime/type_name/runtime_type_name_example.cpp dynamic
 *
 * @tparam T Static type of the object, deduced.
 * @param obj Object whose type is named.
 * @return The name of the type described above.
 *
 * @note Declared only where `SCL_HAS_RTTI` is `1`.
 */

/**
 * @fn scl::type_short_name(T const & obj)
 * @ingroup scl_utility_runtime
 * @brief Returns the identifier of the type ::scl::type_name(obj) names, with no qualifiers.
 *
 * The name ::scl::type_name(obj) answers, less its namespace and class qualifiers and its
 * template arguments: `app::Task<int>` answers `Task`.
 *
 * @snippet runtime/type_name/runtime_type_name_example.cpp short
 *
 * @tparam T Static type of the object, deduced.
 * @param obj Object whose type is named.
 * @return The unqualified identifier of that type.
 *
 * @note Declared only where `SCL_HAS_RTTI` is `1`.
 */

#endif // SCL_HAS_RTTI || DOXYGEN
