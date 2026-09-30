#pragma once

/**
 * @file
 * @brief Name of the type of an object, read through RTTI.
 * @ingroup scl_utility_runtime
 */

#include <scl/utility/preprocessor/rtti.h>
#include <scl/utility/preprocessor/threads.h>

#if SCL_HAS_RTTI || defined(DOXYGEN)

#include <string>
#include <string_view>
#include <typeinfo>

#include <scl/utility/meta/type.h>

#ifdef __has_include
#if __has_include(<cxxabi.h>)
#include <array>
#include <cstddef>
#include <cstdlib>
#include <cxxabi.h>
#include <functional>
#include <iterator>
#include <memory>
#if SCL_HAS_THREADS
#include <mutex>
#endif
#include <unordered_map>
#include <version>
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

#ifdef SCL_DETAIL_HAS_CXXABI
    // Names are kept by the mangled name, since a library loaded after another is unloaded may
    // reuse the address of a type_info.
    struct mangled_name_hash
    {
        using is_transparent = void;

        [[nodiscard]]
        ::std::size_t operator()(::std::string_view mangled) const noexcept
        {
            return ::std::hash<::std::string_view>{}(mangled);
        }
    };

    inline ::std::string_view cached_demangled_name(char const * mangled)
    {
        // Never destroyed, so a destructor that runs after main returns still reads the names.
        static auto & names = *new ::std::unordered_map<::std::string, ::std::string,
            mangled_name_hash, ::std::equal_to<>>;
#if SCL_HAS_THREADS
        static auto & mutex = *new ::std::mutex;
        ::std::scoped_lock const lock{mutex};
#endif
#ifdef __cpp_lib_generic_unordered_lookup
        auto found = names.find(::std::string_view{mangled});
#else
        auto found = names.find(::std::string{mangled});
#endif
        if (found == names.end())
            found = names.try_emplace(mangled, ::scl::detail::demangle(mangled)).first;
        return found->second;
    }

    struct recent_type_name
    {
        ::std::type_info const * type;
        char const * mangled;
        ::std::string_view name;
    };
#endif

    inline ::std::string_view demangled_type_name(::std::type_info const & info)
    {
#ifdef SCL_DETAIL_HAS_CXXABI
        constexpr ::std::size_t slot_count = 8;
#if SCL_HAS_THREADS
        // The slots have no destructor, so a destructor that runs as the thread ends reads them.
        thread_local constinit ::std::array<::scl::detail::recent_type_name, slot_count> recent{};
#else
        static constinit ::std::array<::scl::detail::recent_type_name, slot_count> recent{};
#endif
        auto const address = ::std::hash<::std::type_info const *>{}(&info) / alignof(::std::type_info);
        // Folds the higher bits in, since the type_info objects of classes often lie two alignments apart.
        auto const index = (address ^ address >> 3U) % slot_count;
        auto & slot = *::std::next(recent.begin(), static_cast<::std::ptrdiff_t>(index));
        if (slot.type != &info || slot.mangled != info.name())
            slot = {.type = &info,
                .mangled = info.name(),
                .name = ::scl::detail::cached_demangled_name(info.name())};
        return slot.name;
#else
        return info.name();
#endif
    }

} // namespace scl::detail

namespace scl
{
    template <typename T>
    [[nodiscard]]
    ::std::string type_name(T const & obj)
    {
        return ::std::string{::scl::detail::demangled_type_name(typeid(obj))};
    }

    template <typename T>
    [[nodiscard]]
    ::std::string type_short_name(T const & obj)
    {
        return ::std::string{
            ::scl::detail::short_name_from(::scl::detail::demangled_type_name(typeid(obj)))};
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
 * unchanged. Where the name is demangled, each type is demangled once, and its name is kept until
 * the program ends, so a destructor that runs after `main` returns can call it as well.
 *
 * @snippet runtime/type_name/runtime_type_name_example.cpp dynamic
 *
 * @tparam T Static type of the object, deduced.
 * @param obj Object whose type is named.
 * @return The name of the type described above.
 *
 * @warning The result is for display, not for identity: two distinct types, such as classes of one
 * name in the unnamed namespaces of two translation units, can have one name. Its spelling differs
 * between compilers and standard libraries and may change in a later version of the module, so it
 * must not be compared against a literal, parsed, or persisted. `std::type_index(typeid(obj))`
 * identifies the type the function template names.
 *
 * @note Declared only where the macro `SCL_HAS_RTTI` is `1`.
 * @note Where the macro `SCL_HAS_THREADS` is `1`, it may be called from several threads at once.
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
 * @return The unqualified identifier of that type; for a closure type or an unnamed class or
 *         enumeration, the name the compiler generates for it.
 *
 * @warning The result is for display, as the result of ::scl::type_name(obj) is: its spelling
 * differs between compilers and may change in a later version of the module.
 *
 * @note Declared only where the macro `SCL_HAS_RTTI` is `1`.
 * @note Where the macro `SCL_HAS_THREADS` is `1`, it may be called from several threads at once.
 */

#endif // SCL_HAS_RTTI || DOXYGEN
