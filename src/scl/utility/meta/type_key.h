#pragma once

#include <scl/utility/meta/tu_local.h>
#include <scl/utility/meta/type.h>

#include <cstddef>
#include <string_view>

/**
 * @file
 * @brief Identity of a type without RTTI.
 * @ingroup scl_utility_meta
 */

namespace scl
{
    class type_key;

} // namespace scl

namespace scl::detail
{
    template <typename T, ::std::byte const * Unit>
    constexpr auto make_type_key() noexcept -> ::scl::type_key;

} // namespace scl::detail

namespace scl
{
    class type_key
    {
    private:
        ::std::string_view m_name;
        void const * m_anchor = nullptr;

    public:
        constexpr type_key() noexcept = default;
        constexpr type_key(type_key const &) = default;
        constexpr type_key(type_key &&) = default;
        constexpr type_key & operator=(type_key const &) = default;
        constexpr type_key & operator=(type_key &&) = default;
        constexpr ~type_key() = default;

        [[nodiscard]]
        constexpr ::std::string_view name() const noexcept
        {
            return m_name;
        }

    private:
        constexpr type_key(::std::string_view name, void const * anchor) noexcept
            : m_name{name}
            , m_anchor{anchor}
        {}

        [[nodiscard]]
        friend constexpr bool operator==(type_key const & left, type_key const & right) noexcept
        {
            if (&left == &right)
                return true;
            return left.m_anchor == right.m_anchor && left.m_name == right.m_name;
        }

#ifndef SCL_DOXYGEN
        template <typename T, ::std::byte const * Unit>
        friend constexpr auto ::scl::detail::make_type_key() noexcept -> ::scl::type_key;
#endif
    };

} // namespace scl

namespace scl::detail
{
    // Anonymous namespace on purpose: every including TU gets its own anchor
    // object, so the anchor address discriminates same-named TU-local types.
    namespace
    {
        [[maybe_unused]]
        constexpr ::std::byte tu_anchor{};
    } // namespace

    // GCC and Clang glue operator- and operator<= to a closing >, and Clang glues operator> to it
    // as well, so a name holding such a glued symbol may stand for two types.
    constexpr bool name_reads_two_ways([[maybe_unused]] ::std::string_view name) noexcept
    {
#if defined(_MSC_VER) && !defined(__clang__)
        return false;
#else
        constexpr ::std::string_view keyword = "operator";
        for (auto at = name.find(keyword); at != ::std::string_view::npos; at = name.find(keyword, at + 1))
        {
            bool const whole = at == 0 ||
                !::scl::detail::identifier_char(::scl::detail::char_or_null_at(name, at - 1));
            if (whole && ::scl::detail::glued_angle_symbol(name, at))
                return true;
        }
        return false;
#endif
    }

    template <typename T, ::std::byte const * Unit>
    inline constexpr ::std::byte type_anchor{};

    // Unit is the anchor of the translation unit for a local type and null otherwise.
    template <typename T, ::std::byte const * Unit>
    constexpr auto make_type_key() noexcept -> ::scl::type_key
    {
        void const * anchor = Unit;
        if constexpr (::scl::detail::name_reads_two_ways(::scl::type_name<T>()))
            anchor = &::scl::detail::type_anchor<T, Unit>;
        return ::scl::type_key{::scl::type_name<T>(), anchor};
    }

    template <typename T, ::std::byte const * Unit>
    inline constexpr ::scl::type_key type_key_v = ::scl::detail::make_type_key<T, Unit>();

} // namespace scl::detail

namespace scl
{
    // GCC merges across translation units a specialization whose argument points to a member of
    // a local type, unless another argument names the anchor of the translation unit.
#ifdef SCL_DOXYGEN
    template <typename T, ::std::byte const * Unit = see below>
#else
    template <typename T, ::std::byte const * Unit = ::scl::is_tu_local_v<T> ? &::scl::detail::tu_anchor : nullptr>
#endif
    [[nodiscard]]
    constexpr type_key const & type_key_of() noexcept
    {
        return ::scl::detail::type_key_v<T, Unit>;
    }

} // namespace scl

/**
 * @class scl::type_key
 * @ingroup scl_utility_meta
 * @brief The identity of a type, compared without RTTI.
 *
 * A key holds the name ::scl::type_name<T>() gives the type beside a discriminator: a null pointer
 * for a type with external linkage, apart from a type whose name holds one of the symbols described
 * below, and for a type local to its translation unit, as ::scl::is_tu_local tells, the address of
 * an object private to that translation unit. Two keys compare equal when their names and their
 * discriminators are equal. Types of one name in unnamed namespaces of two translation units differ
 * by the discriminator, and a type with external linkage used from an executable and from a shared
 * library compares equal by its name, apart from a type whose name holds one of the symbols
 * described below.
 *
 * Only ::scl::type_key_of produces a key that identifies a type: the data members are private, and
 * no member function changes a key apart from assignment. The one key a caller writes is the empty
 * one, `type_key{}`, which identifies no type.
 *
 * With GCC and Clang one name may stand for two types, as ::scl::type_short_name states: both write
 * a pointer to `operator-` before a closing `>` as `operator->` and a pointer to `operator<=`
 * before it as `operator<=>`, and Clang writes a pointer to `operator>` before it as `operator>>`.
 * GCC also writes a pointer to such a specialization, `Tagged<&P::operator- > *`, as
 * `Tagged<&P::operator->*`. With GCC and Clang, a type whose name holds `operator->`,
 * `operator->*`, `operator<=>` or `operator>>` carries the address of an object of its own as the
 * discriminator. The keys of two such types differ, and the key of such a type from another module
 * may compare unequal to its key from this one.
 *
 * A key is a value: it copies, moves and assigns, and a copy identifies the type its source
 * identifies. Keys carry no order, neither `operator<=>` nor ::std::less: the identity of a local
 * type and of a type whose name holds one of the symbols above rests on an address, whose relative
 * order between unrelated objects the standard leaves unspecified and constant evaluation cannot
 * read. No hash comes with the key.
 *
 * @warning A key points into the module, an executable or a shared library, that produced it:
 * its name into the string of that module, and a discriminator that is not null into the object of
 * that module. A key must not outlive the module that produced it. After `FreeLibrary` or `dlclose`
 * the name dangles, and so does a discriminator that is not null, and a module loaded later at the
 * same address can make a stale key compare equal to the key of another type.
 */

/**
 * @fn scl::type_key::type_key()
 * @brief Constructs the empty key, which identifies no type.
 *
 * A key that identifies a type comes from ::scl::type_key_of alone, so this is the one key
 * a caller writes.
 */

/**
 * @fn scl::type_key::type_key(type_key const &)
 * @brief Copies a key; the copy identifies the type the source identifies.
 */

/**
 * @fn scl::type_key::type_key(type_key &&)
 * @brief Copies a key, as the copy constructor does.
 */

/**
 * @fn scl::type_key::operator=(type_key const &)
 * @brief Makes this key identify the type the other key identifies.
 * @return This key.
 */

/**
 * @fn scl::type_key::operator=(type_key &&)
 * @brief Makes this key identify the type the other key identifies, as copy assignment does.
 * @return This key.
 */

/**
 * @fn scl::type_key::~type_key()
 * @brief Destroys the key, which owns nothing.
 */

/**
 * @fn scl::type_key::name()
 * @brief Returns the name of the identified type, as ::scl::type_name<T>() gives it.
 * @return A view of the name; an empty view for the empty key. The characters lie in the module
 *         that produced the key, as the warning on the class states.
 */

/**
 * @fn scl::type_key::operator==(type_key const &, type_key const &)
 * @brief Compares two keys for the identity of their types.
 *
 * Two references to one key compare equal at once, so comparing a stored reference with the result
 * of ::scl::type_key_of costs one comparison of addresses when the reference is to that key. Other
 * keys compare equal when their names and their discriminators are equal, which is how keys from
 * two modules compare.
 *
 * @return `true` when both keys hold equal names and equal discriminators; `!=` is the negation
 *         of this result.
 */

/**
 * @fn scl::type_key_of()
 * @ingroup scl_utility_meta
 * @brief Returns a reference to the ::scl::type_key that identifies the type T.
 *
 * The key is an `inline constexpr` variable, one per type, so within one module every call for
 * a type with external linkage returns a reference to one object. A caller that stores a key holds
 * either that reference, which compares by address, or a copy, which compares by content.
 *
 * @pre `T` is neither a local class nor a closure type, nor a type built of one. Nothing reports
 * a violation: Clang spells a local class `duck` of a function as `duck`, whatever namespace
 * the function is in, the name of a class `duck` declared in the global namespace, so the keys of
 * the two classes compare equal. A class moved into an unnamed namespace of the same translation
 * unit gets a key of its own.
 *
 * @tparam T The type to identify.
 * @tparam Unit A caller does not pass it. Its default is the anchor of the translation unit for
 *         a local type and a null pointer for any other type, and the anchor keeps the key of
 *         a local type in that translation unit, which GCC would otherwise merge with the key of
 *         a local type of the same name from another translation unit where `T` holds a pointer to
 *         a member of a type in an unnamed namespace. GCC merges in the same way the specialization
 *         of a caller's template that depends on `T` alone, such as a variable template holding
 *         `&type_key_of<T>()`, so for such a type that specialization holds the key of only one
 *         translation unit.
 * @return A reference to the key of `T`.
 *
 * @snippet meta/type_key/meta_type_key_example.cpp type_key_of
 */
