#pragma once

#include <scl/utility/meta/tu_local.h>
#include <scl/utility/meta/type.h>

#include <cstddef>
#include <string_view>

/**
 * @file
 * @brief RTTI-free type identity key distinguishing TU-local types (C++20).
 * @ingroup scl_utility_meta
 * @details
 * - ::scl::type_key:
 *     Equality-comparable identity key of a type: the pair of its
 *     ::scl::type_name<T>() string and a per-translation-unit discriminator.
 * - ::scl::type_key_of<T>:
 *     Returns a reference to the per-type ::scl::type_key of T.
 */

namespace scl
{
    class type_key;

} // namespace scl

namespace scl::detail
{
    template <typename T>
    constexpr type_key make_type_key() noexcept;

} // namespace scl::detail

namespace scl
{
    class type_key
    {
    private:
        ::std::string_view m_name;
        void const * m_tu = nullptr;

    public:
        constexpr type_key() = default;
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
        constexpr type_key(::std::string_view name, void const * tu) noexcept
            : m_name{name}
            , m_tu{tu}
        {}

        [[nodiscard]]
        friend constexpr bool operator==(type_key const & left, type_key const & right) noexcept
        {
            if (&left == &right)
                return true;
            return left.m_tu == right.m_tu && left.m_name == right.m_name;
        }

        template <typename T>
        friend constexpr type_key detail::make_type_key() noexcept;
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

    template <typename T>
    constexpr type_key make_type_key() noexcept
    {
        void const * tu = nullptr;
        if constexpr (is_tu_local_v<T>)
            tu = &tu_anchor;
        return type_key{type_name<T>(), tu};
    }

    template <typename T>
    inline constexpr type_key type_key_v = make_type_key<T>();

} // namespace scl::detail

namespace scl
{
    template <typename T>
    [[nodiscard]]
    constexpr type_key const & type_key_of() noexcept
    {
        return detail::type_key_v<T>;
    }

} // namespace scl

// -----------------------------------------------------------------------------
// Documentation
// -----------------------------------------------------------------------------

/**
 * @class scl::type_key
 * @ingroup scl_utility_meta
 * @brief Equality-comparable, RTTI-free identity key of a type.
 *
 * Identity is the pair `{name, tu}`:
 * - `name` — the compile-time ::scl::type_name<T>() string;
 * - `tu` — a per-translation-unit discriminator: `nullptr` for external
 *   types, the address of a TU-private anchor for TU-local types
 *   (see ::scl::is_tu_local).
 *
 * For types declared at namespace scope the two components together
 * eliminate both failure modes of simpler schemes:
 * - **No false match:** same-named anonymous-namespace types from different
 *   TUs share the `name` string but carry different anchor addresses.
 * - **No false reject:** an external type used across a shared-library
 *   boundary compares by `name` with `tu == nullptr` on both sides, so
 *   unmerged inline instantiations (Windows DLLs) cannot break equality.
 *
 * Keys are exposed by ::scl::type_key_of<T>() as references to per-type
 * constants and are fully usable in constant expressions. Only `type_key_of`
 * produces a key naming a type: the members are private and there is no
 * mutating API, so a hand-built key pairing one type's name with another
 * type's discriminator cannot exist. The one key a caller can spell is the
 * empty one, `type_key{}`, which names nothing.
 *
 * The key is a value: a `std::string_view` beside a discriminator, copied the
 * way that view is copied, and a handle that has no type to report answers
 * an empty key rather than an absent one. Comparing against
 * `type_key_of<T>()` still short-circuits on address identity where the
 * caller kept a reference to the per-type constant.
 *
 * The key is intentionally not ordered (no `operator<=>`, no `std::less`
 * support): its identity involves object addresses, whose relative order is
 * unspecified between unrelated objects and unusable in constant
 * expressions. Hashing support is likewise deferred until a real consumer
 * exists.
 *
 * @warning A key is a view into the module (executable or shared library)
 * that produced it: `name` points into that module's signature literal and
 * `tu` into that module's anchor. A key must not outlive its producing
 * module — after `FreeLibrary`/`dlclose` the pointers dangle, and a module
 * later mapped at the same base address can even resurrect a stale TU-local
 * key into falsely matching a fresh unrelated one. Drop stored keys when
 * unloading the module that produced them.
 */

/**
 * @fn scl::type_key::type_key()
 * @brief Constructs the key that names no type.
 *
 * A key naming a type comes from @ref scl::type_key_of and from nowhere else,
 * so this is the one key a caller can spell.
 */

/**
 * @fn scl::type_key::type_key(type_key const &)
 * @brief Copies the key; a key is a value, not a handle to one.
 */

/**
 * @fn scl::type_key::type_key(type_key &&)
 * @brief Same as the copy constructor: there is nothing to steal.
 */

/**
 * @fn scl::type_key::operator=(type_key const &)
 * @brief Replaces which type this key names.
 */

/**
 * @fn scl::type_key::operator=(type_key &&)
 * @brief Same as copy assignment.
 */

/**
 * @fn scl::type_key::~type_key()
 * @brief Trivial: a key owns nothing, holding only a view of its module.
 */

/**
 * @fn scl::type_key::name()
 * @brief Fully qualified compile-time type name, as ::scl::type_name<T>()
 *        renders it for the current compiler.
 */

/**
 * @fn scl::type_key::operator==(type_key const &, type_key const &)
 * @brief Compares two keys for type identity.
 *
 * Short-circuits on address identity first: `type_key_of<T>()` returns a
 * reference to a per-type inline variable, unique within a module, so
 * comparing stored references against it costs one pointer comparison.
 * Otherwise falls back to content: `a.name == b.name && a.tu == b.tu` —
 * the path taken by keys that crossed a module boundary.
 * @return `true` when both keys denote the same type; `!=` is synthesised
 *         from this.
 */

/**
 * @fn scl::type_key_of()
 * @ingroup scl_utility_meta
 * @brief Returns a reference to the ::scl::type_key identifying T.
 *
 * The key is a per-type `inline constexpr` variable: every use of
 * `type_key_of<T>()` for an external T inside one module refers to the same
 * object, which makes equality against a stored reference an address
 * comparison. A consumer storing keys long-term may hold either the
 * reference, which keeps that fast path, or a copy of the key, which compares
 * by content.
 *
 * For an external type the key is `{type_name<T>(), nullptr}` — identical in
 * every translation unit and across shared-library boundaries. For a TU-local
 * type (::scl::is_tu_local_v<T> is `true`) the key additionally carries the
 * address of an anchor object private to the including translation unit, so
 * same-named TU-local types from different TUs compare unequal.
 *
 * @pre T must be declared at namespace scope. Local classes and closure
 * types are outside the contract: compilers render them without a reliable
 * TU-locality marker, and no compile-time guard can detect the violation.
 * On Clang a local class renders as its bare name, so its key is
 * byte-identical to the key of a same-named namespace-scope type — two
 * genuinely different types compare equal:
 * @code
 * struct duck {};                     // namespace scope
 * int main() {
 *     struct duck {};                 // local class — outside the contract
 *     // passes on Clang: a false match against the ordinary external type
 *     static_assert(::scl::type_key_of<::duck>() == ::scl::type_key_of<duck>());
 * }
 * @endcode
 * Move such a type into an anonymous namespace of the same translation unit
 * to give it a well-defined key.
 *
 * @note A string non-type template parameter cannot make an external type
 * TU-local: NTTP values render inside string/character literals (or as
 * character codes on MSVC), and ::scl::is_tu_local skips literal contents
 * when searching for the anonymous-namespace marker.
 *
 * @tparam T The type to identify.
 * @return Reference to the per-type ::scl::type_key of T.
 *
 * @code
 * namespace { struct duck {}; }
 * struct goose {};
 *
 * constexpr auto & anon_key = ::scl::type_key_of<duck>();
 * constexpr auto & ext_key  = ::scl::type_key_of<goose>();
 * static_assert(anon_key == ::scl::type_key_of<duck>());
 * static_assert(anon_key != ext_key);
 * @endcode
 */
