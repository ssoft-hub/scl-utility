#pragma once

/**
 * @file
 * @brief Runtime type name extraction via RTTI (C++20).
 * @ingroup scl_utility_runtime
 * @details
 * - ::scl::type_name(obj):
 *     Returns the demangled fully qualified name of the dynamic type of obj.
 *     On GCC/Clang the mangled name from @c typeid is demangled via @c abi::__cxa_demangle.
 *     On MSVC @c typeid().name() is already human-readable and is returned as-is.
 * - ::scl::type_short_name(obj):
 *     Returns only the unqualified identifier of the dynamic type.
 *     Strips all leading namespace and class-scope qualifiers and template arguments.
 *
 * Both functions are only available when RTTI is enabled.
 */

#if defined(_CPPRTTI) || defined(__GXX_RTTI)
#define SCL_DETAIL_HAS_RTTI 1
#elif defined(__has_feature)
#if __has_feature(cxx_rtti)
#define SCL_DETAIL_HAS_RTTI 1
#endif
#endif

#if defined(SCL_DETAIL_HAS_RTTI) || defined(DOXYGEN)

#include <memory>
#include <string>
#include <string_view>
#include <typeinfo>

#include <scl/utility/meta/type.h>

#if defined(__has_include) && __has_include(<cxxabi.h>)
#include <cstdlib>
#include <cxxabi.h>
#define SCL_DETAIL_HAS_CXXABI 1
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
    /**
     * @brief Returns the demangled fully qualified name of the dynamic type of @p obj.
     * @ingroup scl_utility_runtime
     * @tparam T Static type of the object (deduced automatically).
     * @param  obj Object to inspect. If @p T is polymorphic, the dynamic type is returned.
     * @return Demangled type name as a @c std::string.
     *
     * @note Only available when RTTI is enabled.
     *
     * @code{.cpp}
     * struct Base { virtual ~Base() = default; };
     * struct Derived : Base {};
     *
     * Base* p = new Derived{};
     * ::scl::type_name(*p); // "Derived" (GCC/Clang), "struct Derived" (MSVC)
     * @endcode
     */
    template <typename T>
    ::std::string type_name(T const & obj)
    {
        return detail::demangle(typeid(obj).name());
    }

    /**
     * @brief Returns the short (unqualified) name of the dynamic type of @p obj.
     * @ingroup scl_utility_runtime
     * @tparam T Static type of the object (deduced automatically).
     * @param  obj Object to inspect. If @p T is polymorphic, the dynamic type is returned.
     * @return Unqualified type identifier without namespaces or template arguments.
     *
     * @note Only available when RTTI is enabled.
     *
     * @code{.cpp}
     * namespace app { template <typename T> struct Task : Base {}; }
     *
     * Base* p = new app::Task<int>{};
     * ::scl::type_short_name(*p); // "Task"
     * @endcode
     */
    template <typename T>
    ::std::string type_short_name(T const & obj)
    {
        ::std::string const full = detail::demangle(typeid(obj).name());
        return ::std::string{detail::short_name_from(full)};
    }

} // namespace scl

#endif // SCL_DETAIL_HAS_RTTI || DOXYGEN
