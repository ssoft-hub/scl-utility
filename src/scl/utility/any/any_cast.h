#pragma once

/**
 * @file any_cast.h
 * @brief The single entry point of an ScL Any cast, and the trait a source specialises.
 * @ingroup scl_utility_any
 */

#include <scl/utility/any/bad_any_cast.h>
#include <scl/utility/attribute/hotcold.h>
#include <scl/utility/attribute/lifetimebound.h>
#include <scl/utility/concepts/type_category.h>
#include <scl/utility/preprocessor/exceptions.h>

#include <concepts>
#include <memory>
#include <type_traits>

namespace scl
{
    template <typename Source>
    struct any_cast_traits;
} // namespace scl

namespace scl::detail
{
    template <typename Source>
    using any_traits_of = ::scl::any_cast_traits<::std::remove_cv_t<Source>>;

    template <typename Target, typename Source>
    [[nodiscard]]
    constexpr auto any_access(Source * source)                                     /**/
        noexcept(noexcept(any_traits_of<Source>::template access<Target>(source))) /**/
        -> decltype(any_traits_of<Source>::template access<Target>(source))
    {
        return any_traits_of<Source>::template access<Target>(source);
    }

    template <typename Pointer, typename Target>
    concept any_pointer_to = ::scl::concepts::pointer<Pointer> &&
        ::std::same_as<::std::remove_cv_t<::std::remove_pointer_t<Pointer>>, ::std::remove_cv_t<Target>>;

    template <typename Source, typename Target>
    concept any_accessible =
        requires(Source * source) {
            {
                ::scl::detail::any_access<Target>(source)
            } -> any_pointer_to<Target>;
        };
} // namespace scl::detail

namespace scl
{
    template <::scl::concepts::object_type Target, detail::any_accessible<Target> Source>
    [[nodiscard]]
    SCL_HOT constexpr auto any_cast(Source * source)           /**/
        noexcept(noexcept(detail::any_access<Target>(source))) /**/
        -> decltype(detail::any_access<Target>(source))
    {
        if (source == nullptr) [[unlikely]]
            return nullptr;
        return detail::any_access<Target>(source);
    }

#if SCL_HAS_EXCEPTIONS || defined(DOXYGEN)
} // namespace scl

namespace scl::detail
{
    template <typename Target>
    using any_spelled_request_t =
        ::std::conditional_t<::std::is_reference_v<Target>, ::std::remove_reference_t<Target>, Target const>;

    // Stated by a name the trait declares, not read off the pointer it answers.
    template <typename Source>
    inline constexpr bool any_allows_move_v =
        requires { typename any_traits_of<Source>::movable_tag; };

    // A source with no name at the call site keeps nothing, so one that allows it moves out.
    template <typename Source, typename Target>
    inline constexpr bool any_hands_over_v = (!::std::is_lvalue_reference_v<Source>) &&
        (!::std::is_reference_v<Target>) && any_allows_move_v<::std::remove_cvref_t<Source>>;

    template <typename Source, typename Target>
    inline constexpr bool any_hands_over_as_v = any_hands_over_v<Source, Target> &&
        requires(Source && bound) {
            static_cast<Target>(
                ::std::move(*::scl::any_cast<::std::remove_cv_t<Target>>(::std::addressof(bound))));
        };

    // A source that owns what it hands out but cannot move it is read by copying, as any other.
    template <typename Source, typename Target>
    inline constexpr bool any_copies_as_v = (!any_hands_over_as_v<Source, Target>) &&
        ::std::is_constructible_v<Target, Target const> &&
        requires(Source && bound) {
            static_cast<Target>(*::scl::any_cast<any_spelled_request_t<Target>>(::std::addressof(bound)));
        };

    template <typename Source, typename Target>
    concept any_produces = any_hands_over_as_v<Source, Target> || any_copies_as_v<Source, Target>;
} // namespace scl::detail

namespace scl
{
#ifdef DOXYGEN
    // Doxygen tells apart no overloads differing in their constraints alone, so it sees one.
    template <typename Target, typename Source>
    [[nodiscard]]
    constexpr Target any_cast(Source && source);
#else
    template <typename Target, detail::any_produces<Target> Source>
    [[nodiscard]]
    // NOLINTNEXTLINE(*-missing-std-forward): the value category chooses the overload, nothing is forwarded
    constexpr Target any_cast(Source && source SCL_LIFETIMEBOUND)
        requires ::std::is_lvalue_reference_v<Target>
    {
        auto * const reached = ::scl::any_cast<detail::any_spelled_request_t<Target>>(::std::addressof(source));
        if (reached == nullptr)
            throw bad_any_cast{};

        return static_cast<Target>(*reached);
    }

    // No lifetime attribute: the result is an object of its own, however it was produced.
    template <::scl::concepts::object_type Target, detail::any_produces<Target> Source>
    [[nodiscard]]
    // NOLINTNEXTLINE(*-missing-std-forward): the value category chooses the overload, nothing is forwarded
    constexpr Target any_cast(Source && source)
    {
        if constexpr (detail::any_hands_over_as_v<Source, Target>)
        {
            auto * const owned = ::scl::any_cast<::std::remove_cv_t<Target>>(::std::addressof(source));
            if (owned == nullptr)
                throw bad_any_cast{};

            // Not braces: an initializer-list constructor would take the object whole as one
            // element.
            return static_cast<Target>(::std::move(*owned));
        }
        else
        {
            auto * const reached =
                ::scl::any_cast<detail::any_spelled_request_t<Target>>(::std::addressof(source));
            if (reached == nullptr)
                throw bad_any_cast{};

            return static_cast<Target>(*reached);
        }
    }
#endif

#endif
} // namespace scl

// =============================================================================
// Documentation
// =============================================================================

/**
 * @file any_cast.h
 *
 * One cast serves every source. What differs between them is stated by
 * @ref scl::any_cast_traits — the request a source's access rule makes of a spelling, and
 * how that request reaches the object. A type joins the cast by specialising that trait
 * and needs no overload of its own.
 *
 * Whether a cast is offered for a source is a constraint, and a compiler remembers the
 * answer for a given source. A specialisation therefore has to be visible before the first
 * question about its type: including the header that defines a source before asking is
 * enough, while asking first and including after is ill-formed and a compiler need not say
 * so. Including `<scl/utility/any.h>` names every source the library serves at once.
 */

/**
 * @struct scl::any_cast_traits
 * @ingroup scl_utility_any
 * @brief What a source states about itself so that @ref scl::any_cast serves it
 *
 * Declared and never defined. A type joins the cast by specialising this in namespace
 * `scl`. A specialisation states what its source answers and forwards the work to the
 * source itself; it holds no algorithm of its own and needs no access the source does not
 * already grant.
 *
 * A specialisation carries one name: `access<Target, Source>(Source *)`, which answers the
 * address of the object as a pointer, or `nullptr` where the request is not answered.
 * @p Source is never null there — @ref scl::any_cast checks that once and calls nothing
 * otherwise, so a specialisation states no null case of its own.
 *
 * The result type of `access` is the result type of the cast, and the access rule is stated
 * by it: a source that hands out no write answers `Target const *`, one that does answers
 * `Target *`, and one whose own constness reaches the object carries that constness into the
 * answer. What the answer points at is compared with the spelling up to cv-qualification, so
 * a specialisation answering a pointer to a base class of what its source holds leaves the
 * constraint unsatisfied, and the cast is not offered for that spelling.
 *
 * @ref scl::any_cast is offered exactly where the call it forwards compiles and answers such
 * a pointer. A source that cannot answer therefore needs no condition written out for it: a
 * `Source` whose methods `access` cannot call, or a `Target` it cannot produce, leaves the
 * constraint unsatisfied and the caller reads that there is no such cast. Whether `access`
 * throws decides the exception specification of the pointer form; a specialisation that
 * throws makes that form throwing rather than terminating the program.
 *
 * A specialisation may declare one name more, the type `movable_tag`, which needs no
 * definition. Declaring it states that the source owns what it hands out, so such a source
 * with no name at the call site is read by moving the object out instead of copying it. A
 * specialisation that leaves the name out is read by copying, which is what a source
 * referring to an object a caller still holds needs.
 *
 * `access` is what @ref scl::any_cast calls, not an interface for a caller to use: called
 * directly it skips the null check, and specialising the trait for a type the library
 * already serves replaces the library's own answer for that type and is not supported.
 *
 * @tparam Source  The type served, without cv-qualification.
 *
 * @code
 * struct boxed_int { int value; };
 *
 * namespace scl
 * {
 *     template <>
 *     struct any_cast_traits<boxed_int>
 *     {
 *         template <typename Target, typename Source>
 *         static constexpr auto access(Source * box) noexcept
 *             -> decltype(std::addressof(box->value))
 *         {
 *             return std::addressof(box->value);
 *         }
 *     };
 * }
 * @endcode
 */

/**
 * @fn scl::any_cast(Source *source)
 * @ingroup scl_utility_any
 * @brief Reaches the object @p source refers to
 *
 * The spelling decides the result type wherever the source states one, the binding decides
 * whether anything comes back:
 * a request answers only where it covers every qualifier the object was bound with. So a
 * request naming neither `const` nor `volatile` asks to write, and only an object bound
 * without either is answered.
 *
 * Throws nothing where the trait's `access` throws nothing, which holds for every source the
 * library serves.
 *
 * @tparam Target  The spelling asked for; a reference type is rejected.
 * @tparam Source  Deduced source, possibly cv-qualified.
 * @param  source  The source to read; may be null.
 * @return The object, as the pointer its source's trait answers; `nullptr` for a null
 *         source, a type that does not match, or a qualifier the request does not cover.
 *
 * @warning A match compares @ref scl::type_key values, which stays exact across module
 *          boundaries and tells same-named anonymous-namespace types from different
 *          translation units apart. Two limits of the key carry over: a type declared at
 *          block scope (a local class or a closure type) is outside its contract and may
 *          falsely match a same-named namespace-scope type, and a key must not outlive the
 *          module that produced it — drop a source built inside a shared library before
 *          unloading it.
 */

/**
 * @fn scl::any_cast(Source &&source)
 * @ingroup scl_utility_any
 * @brief Reads the object @p source refers to, throwing on a mismatch
 *
 * The spelling asked for decides what comes back, and each spelling has a declaration of its
 * own. An object spelling asks for a copy, which is an object of its own and outlives
 * @p source, so nothing binds to it. Copying asks to read, so it is offered where the type
 * can be copied from a `const` object, or where the source hands the object over instead: a
 * copy constructor taking a non-`const` reference would otherwise modify what it copies from,
 * through a trait that hands out a write. An lvalue reference binds the object without copying
 * it, so the result must not outlive what @p source refers to, which is what the lifetime
 * attribute on that declaration states. A
 * source that adds `const` to every request offers no mutable reference, and an rvalue
 * reference is refused for every source.
 *
 * A source with no name at the call site is read by moving the object out, where its trait
 * declares `movable_tag` and the spelling asks for an object rather than a reference. Every
 * other source is read by copying, a named one whatever its trait declares, since a name
 * means the caller still holds what it refers to. Moving out is therefore stated by the
 * source rather than asked for at the call, and a type that only moves is read this way alone.
 *
 * @note Declared only where @ref SCL_HAS_EXCEPTIONS is `1`. A translation unit compiled
 *       without exceptions keeps the pointer form, which answers `nullptr` instead.
 *
 * @tparam Target  The spelling asked for: an object type, or a `const` lvalue reference.
 * @tparam Source  Deduced source.
 * @param  source  The source to read.
 * @return The object as @p Target asks: a copy, the object moved out, or a reference bound
 *         to it.
 * @throws scl::bad_any_cast  Where the pointer form would answer `nullptr`.
 */
