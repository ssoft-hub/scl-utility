#pragma once

/**
 * @file any_switch.h
 * @brief Branch chain running one branch, chosen by the type an erased value holds.
 * @ingroup scl_utility_any
 */

#include <scl/utility/any/any_arg.h>

#include <cstddef>
#include <optional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace scl::detail
{
    // A case selects the way any_cast does, so it is normalised to the request any_cast
    // takes: a reference case asks for its referent as written, an object case asks to
    // read. Two cases that normalise alike match alike, which is what makes a repeated
    // case detectable.
    template <typename Case>
    using any_case_request_t =
        ::std::conditional_t<::std::is_reference_v<Case>, ::std::remove_reference_t<Case>, Case const>;

    // An object case hands the handler a `const` reference and lets its parameter decide
    // whether that becomes a copy; a reference case is passed as written.
    template <typename Case>
    using any_case_argument_t = ::std::conditional_t<::std::is_reference_v<Case>, Case, Case const &>;

    // The case type of the fallback branch: it selects nothing, so it names no type.
    struct any_switch_fallback
    {};

    // Stands in for the optional a `void` chain never builds, so selection is spelled once.
    struct any_switch_no_result
    {};

    // The handler of a branch that matches and does nothing.
    struct any_switch_no_handler
    {};

    template <typename Case, typename Handler>
    struct any_switch_branch
    {
        using case_type = Case;

        Handler handler;
    };

    // A case names something an object can be reached as, or the empty value. An rvalue
    // reference is excluded for the reason a view excludes it: nothing here is movable-from.
    template <typename Case>
    concept any_switch_case = ::std::is_void_v<Case> || ::std::is_object_v<Case> ||
        (::std::is_lvalue_reference_v<Case> && ::std::is_object_v<::std::remove_reference_t<Case>>);

    template <typename... Branches>
    inline constexpr bool any_switch_has_fallback_v =
        (::std::is_same_v<typename Branches::case_type, any_switch_fallback> || ...);

    // One request covers another when it reaches every referent that one reaches. `accepts`
    // lets a request reach a referent whose qualifiers its own cover, so a covering request
    // is the same type carrying at least the qualifiers of the covered one: `T const`
    // covers `T`, and `T` covers no request but itself.
    template <typename Covering, typename Covered>
    inline constexpr bool any_request_covers_v =
        ::std::is_same_v<::std::remove_cv_t<Covering>, ::std::remove_cv_t<Covered>> &&
        (::std::is_const_v<Covering> || !::std::is_const_v<Covered>) &&
        (::std::is_volatile_v<Covering> || !::std::is_volatile_v<Covered>);

    // Cases are compared as the requests they normalise to, so a case an earlier one
    // already covers is rejected rather than kept as a branch that can never run. Order
    // decides: `in_case<T &>` after `in_case<T const &>` is covered, while the same two
    // the other way round each keep referents of their own.
    template <typename Case, typename... Branches>
    inline constexpr bool any_case_covered_v =
        (any_request_covers_v<any_case_request_t<typename Branches::case_type>, any_case_request_t<Case>> || ...);

    // Spelled as a function rather than a disjunction: the branches are ordered exactly as
    // the chain runs them, so a handler is judged by the form that will actually be called.
    // @p Handler is a reference type, so the same check answers for a `const` chain and for
    // a chain holding a `mutable` handler.
    // A branch produces the result, or the optional holding it. The second form is what
    // lets one chain stand as a branch of another: a chain answers `optional<Result>`, and
    // an empty answer means the branch ran and produced nothing, exactly as it reads.
    // `Result` is tried first, so a chain over an optional `Result` keeps its meaning.
    template <typename Produced, typename Result>
    inline constexpr bool any_switch_produces_v = ::std::is_convertible_v<Produced, Result> ||
        ::std::is_convertible_v<Produced, ::std::optional<Result>>;

    template <typename Handler, typename Result, typename... Argument>
    consteval bool any_switch_handler_check() noexcept
    {
        if constexpr (::std::is_invocable_v<Handler, Argument...>)
            if constexpr (::std::is_void_v<Result>)
                return true;
            else
                return any_switch_produces_v<::std::invoke_result_t<Handler, Argument...>, Result>;
        else if constexpr (::std::is_invocable_v<Handler>)
            if constexpr (::std::is_void_v<Result>)
                return true;
            else
                return any_switch_produces_v<::std::invoke_result_t<Handler>, Result>;
        else if constexpr (::std::is_void_v<Result>)
            return false;
        else
            return any_switch_produces_v<Handler, Result>;
    }

    // What a handler is offered follows from its case: the fallback sees the subject, an
    // empty case has nothing to show, and every other case hands over what it reached.
    template <typename Handler, typename Result, typename Case>
    consteval bool any_switch_branch_check() noexcept
    {
        if constexpr (::std::is_same_v<Case, any_switch_fallback>)
            return any_switch_handler_check<Handler, Result, ::scl::any_arg>();
        else if constexpr (::std::is_void_v<Case>)
            return any_switch_handler_check<Handler, Result>();
        else
            return any_switch_handler_check<Handler, Result, any_case_argument_t<Case>>();
    }

    template <typename Handler, typename Result, typename Case>
    inline constexpr bool any_switch_branch_v = any_switch_branch_check<Handler, Result, Case>();

    // A branch judged through the reference the chain will reach its handler by.
    template <typename Branch, typename Result, typename Handler = decltype(Branch::handler)>
    inline constexpr bool any_switch_branch_const_v =
        any_switch_branch_check<Handler const &, Result, typename Branch::case_type>();

    // Selection itself never throws, so what apply may throw is what its branches may:
    // the call, and storing what the call produced.
    template <typename Handler, typename Result, typename... Argument>
    consteval bool any_switch_nothrow_check() noexcept
    {
        if constexpr (::std::is_same_v<::std::remove_cvref_t<Handler>, any_switch_no_handler>)
            return true;
        // Spelled as a nested branch rather than a disjunction: `optional<void>` is
        // ill-formed, so a `void` chain must not name it even in a condition it never takes.
        else if constexpr (::std::is_void_v<Result>)
            if constexpr (::std::is_invocable_v<Handler, Argument...>)
                return ::std::is_nothrow_invocable_v<Handler, Argument...>;
            else
                return ::std::is_nothrow_invocable_v<Handler>;
        else if constexpr (::std::is_invocable_v<Handler, Argument...>)
            return ::std::is_nothrow_invocable_v<Handler, Argument...> &&
                ::std::is_nothrow_assignable_v<::std::optional<Result> &, ::std::invoke_result_t<Handler, Argument...>>;
        else if constexpr (::std::is_invocable_v<Handler>)
            return ::std::is_nothrow_invocable_v<Handler> &&
                ::std::is_nothrow_assignable_v<::std::optional<Result> &, ::std::invoke_result_t<Handler>>;
        else
            return ::std::is_nothrow_assignable_v<::std::optional<Result> &, Handler>;
    }

    template <typename Handler, typename Result, typename Case>
    consteval bool any_switch_branch_nothrow_check() noexcept
    {
        if constexpr (::std::is_same_v<Case, any_switch_fallback>)
            return any_switch_nothrow_check<Handler, Result, ::scl::any_arg>();
        else if constexpr (::std::is_void_v<Case>)
            return any_switch_nothrow_check<Handler, Result>();
        else
            return any_switch_nothrow_check<Handler, Result, any_case_argument_t<Case>>();
    }

    template <typename Branch, typename Result, typename Handler = decltype(Branch::handler)>
    inline constexpr bool any_switch_branch_nothrow_const_v =
        any_switch_branch_nothrow_check<Handler const &, Result, typename Branch::case_type>();

    template <typename Branch, typename Result, typename Handler = decltype(Branch::handler)>
    inline constexpr bool any_switch_branch_nothrow_mutable_v =
        any_switch_branch_nothrow_check<Handler &, Result, typename Branch::case_type>();
} // namespace scl::detail

namespace scl
{
    template <typename Result = void, typename... Branches>
    class [[nodiscard]] any_switch
    {
        using branches_type = ::std::tuple<Branches...>;
        // A `void` chain produces nothing, so it neither builds an optional nor returns one.
        using storage_type =
            ::std::conditional_t<::std::is_void_v<Result>, detail::any_switch_no_result, ::std::optional<Result>>;

        // Handing the result back is part of what apply does, so a result whose move may
        // throw makes apply throwing however quiet its branches are.
        static constexpr bool nothrow_const_apply =
            (detail::any_switch_branch_nothrow_const_v<Branches, Result> && ...) &&
            ::std::is_nothrow_move_constructible_v<storage_type>;

        static constexpr bool nothrow_mutable_apply =
            (detail::any_switch_branch_nothrow_mutable_v<Branches, Result> && ...) &&
            ::std::is_nothrow_move_constructible_v<storage_type>;

        // A handler declared `mutable` is callable only through a non-`const` chain, so a
        // chain holding one has no `const` apply rather than a broken one.
        static constexpr bool const_applicable = (detail::any_switch_branch_const_v<Branches, Result> && ...);

    public:
        using result_type = ::std::conditional_t<::std::is_void_v<Result>, void, ::std::optional<Result>>;

        constexpr any_switch() noexcept = default;

        template <typename Case, typename Handler>
        [[nodiscard]]
        constexpr auto in_case(Handler && handler) const
            requires detail::any_switch_case<Case> && (!detail::any_switch_has_fallback_v<Branches...>) &&
            (!detail::any_case_covered_v<Case, Branches...>) &&
            detail::any_switch_branch_v<::std::decay_t<Handler> &, Result, Case>
        {
            return append<Case>(::std::forward<Handler>(handler));
        }

        // A branch that matches and does nothing is a complete handler only where doing
        // nothing produces the result, which is `Result = void`.
        template <typename Case>
        [[nodiscard]]
        constexpr auto in_case() const
            requires ::std::is_void_v<Result> && detail::any_switch_case<Case> &&
            (!detail::any_switch_has_fallback_v<Branches...>) &&
            (!detail::any_case_covered_v<Case, Branches...>)
        {
            return append<Case>(detail::any_switch_no_handler{});
        }

        template <typename Handler>
        [[nodiscard]]
        constexpr auto or_else(Handler && handler) const
            requires(!detail::any_switch_has_fallback_v<Branches...>) &&
            detail::any_switch_branch_v<::std::decay_t<Handler> &, Result, detail::any_switch_fallback>
        {
            return append<detail::any_switch_fallback>(::std::forward<Handler>(handler));
        }

        constexpr result_type apply(any_arg subject) const /**/
            noexcept(nothrow_const_apply)
            requires const_applicable
        {
            storage_type result{};
            select(m_branches, subject, result, ::std::index_sequence_for<Branches...>{});

            if constexpr (!::std::is_void_v<Result>)
                return result;
        }

        constexpr result_type apply(any_arg subject) /**/
            noexcept(nothrow_mutable_apply)
        {
            storage_type result{};
            select(m_branches, subject, result, ::std::index_sequence_for<Branches...>{});

            if constexpr (!::std::is_void_v<Result>)
                return result;
        }

        // Spelled out rather than left to a conversion: a chain is callable because that
        // reads well where one is passed as a predicate or a transform, and it carries
        // exactly what apply carries - the same overloads, the same noexcept.
        constexpr result_type operator()(any_arg subject) const noexcept(nothrow_const_apply)
            requires const_applicable
        {
            return apply(subject);
        }

        constexpr result_type operator()(any_arg subject) noexcept(nothrow_mutable_apply)
        {
            return apply(subject);
        }

        [[nodiscard]]
        constexpr bool has_case(any_arg subject) const noexcept
        {
            return covered(subject, ::std::index_sequence_for<Branches...>{});
        }

    private:
        constexpr explicit any_switch(branches_type branches) noexcept
            : m_branches{::std::move(branches)}
        {}

        template <typename Case, typename Handler>
        constexpr auto append(Handler && handler) const
        {
            using branch = detail::any_switch_branch<Case, ::std::decay_t<Handler>>;

            return any_switch<Result, Branches..., branch>{::std::tuple_cat(m_branches,
                ::std::tuple<branch>{branch{::std::forward<Handler>(handler)}})};
        }

        // Branches is deduced as a `const` or non-`const` tuple, so the same walk serves
        // both forms of apply and each reaches its handlers the way that form may.
        template <typename BranchTuple, ::std::size_t... Index>
        static constexpr void select(/**/
            BranchTuple & branches,
            any_arg subject,
            storage_type & result,
            ::std::index_sequence<Index...> /*indices*/)
        {
            bool done = false;
            ((done = done || run<Index>(branches, subject, result)), ...);
        }

        template <::std::size_t... Index>
        [[nodiscard]]
        constexpr bool covered(any_arg subject, ::std::index_sequence<Index...> /*indices*/) const noexcept
        {
            return (matches<Index>(subject) || ...);
        }

        // Selection alone, with no handler in sight: this is what makes has_case answer
        // without a side effect, and it is the same test apply performs.
        template <::std::size_t Index>
        [[nodiscard]]
        constexpr bool matches(any_arg subject) const noexcept
        {
            using case_type = typename ::std::tuple_element_t<Index, branches_type>::case_type;

            if constexpr (::std::is_same_v<case_type, detail::any_switch_fallback>)
                return true;
            // An empty value has nothing to cast to, so it is matched on the state itself.
            else if constexpr (::std::is_void_v<case_type>)
                return !subject.has_value();
            else
                return ::scl::any_cast<detail::any_case_request_t<case_type>>(&subject) != nullptr;
        }

        template <::std::size_t Index, typename BranchTuple>
        static constexpr bool run(BranchTuple & branches, any_arg subject, storage_type & result)
        {
            using branch = ::std::tuple_element_t<Index, branches_type>;
            using case_type = branch::case_type;

            auto & handler = ::std::get<Index>(branches).handler;

            // The fallback runs wherever the branches before it did not, and the subject is
            // all it has to hand a handler.
            if constexpr (::std::is_same_v<case_type, detail::any_switch_fallback>)
            {
                dispatch(result, handler, subject);
                return true;
            }
            else if constexpr (::std::is_void_v<case_type>)
            {
                if (subject.has_value())
                    return false;

                dispatch(result, handler);
                return true;
            }
            else
            {
                auto * reached = ::scl::any_cast<detail::any_case_request_t<case_type>>(&subject);
                if (reached == nullptr)
                    return false;

                dispatch(result, handler, static_cast<detail::any_case_argument_t<case_type>>(*reached));
                return true;
            }
        }

        // The argument-taking form is tried first: it is the more specific reading of a
        // handler that accepts both, and the only one that can see the value at all. A
        // handler that is not invocable at all is a value, which is the branch itself.
        template <typename Handler, typename... Argument>
        static constexpr void
        dispatch(storage_type & result, Handler & handler, Argument &&... argument)
        {
            if constexpr (::std::is_same_v<::std::remove_const_t<Handler>, detail::any_switch_no_handler>)
                return;
            else if constexpr (::std::is_invocable_v<Handler &, Argument...>)
                produce(result, handler, ::std::forward<Argument>(argument)...);
            else if constexpr (::std::is_invocable_v<Handler &>)
                produce(result, handler);
            else if constexpr (!::std::is_void_v<Result>)
                result = handler;
        }

        template <typename Handler, typename... Argument>
        static constexpr void produce(storage_type & result, Handler & handler, Argument &&... argument)
        {
            if constexpr (::std::is_void_v<Result>)
                handler(::std::forward<Argument>(argument)...);
            else
                result = handler(::std::forward<Argument>(argument)...);
        }

        template <typename, typename...>
        friend class ::scl::any_switch;

        branches_type m_branches;
    };
} // namespace scl

// =============================================================================
// Documentation
// =============================================================================

/**
 * @class scl::any_switch
 * @ingroup scl_utility_any
 * @brief Branch chain running one branch, chosen by the type an erased value holds
 *
 * Reading an erased value with @ref scl::any_cast alone is a cascade of probes:
 * one call per candidate type, each with its own `if`, and a fallback spelled
 * separately. `any_switch` collapses that into one expression — every branch
 * names its type once, the first match runs, and the fallback belongs to the
 * same chain. It is the visitor `std::any` never had, and it reads every subject
 * @ref scl::any_arg accepts: a typed lvalue or rvalue, an @ref scl::any_view, or
 * a `std::any` on an RTTI build.
 *
 * The chain holds no subject. `in_case` and `or_else` describe the branches;
 * @ref scl::any_switch::apply runs them over the subject it is given, and
 * @ref scl::any_switch::has_case asks whether there is one to run. So one chain
 * serves any number of subjects, none of them has to outlive it, and a chain is
 * an ordinary object: keep it in a variable, a member or a container, return it
 * from a function, apply it wherever the subject turns up.
 *
 * A case selects by the qualifier-coverage rule of @ref scl::any_cast, mirrored
 * in full: `in_case<T>` and `in_case<T const &>` match a `T` or a `T const`
 * referent, `in_case<T &>` an unqualified one, and `volatile` participates the
 * same way. `in_case<void>` matches an empty value. `in_case<std::any>` does not
 * match the box, exactly as `any_cast<std::any>` does not — a `std::any` subject
 * is unwrapped and the branches see the boxed type.
 *
 * What `in_case` takes after its case is an invocable, or — for a named `Result`,
 * there being nothing for a `void` chain to convert it into — a ready value. An
 * invocable is called with the matched value where it takes one, called with
 * nothing where it takes none. The value it is called with is what its case asks
 * for: `in_case<T const &>` binds without a copy, `in_case<T &>` reaches the
 * caller's object, and `in_case<T>` hands over a value the parameter copies.
 * `or_else` takes the same, with @ref scl::any_arg as the one argument its
 * invocable form may name. Where both calls compile — the only parameter carrying a
 * default argument, say — the matched value is the one passed.
 *
 * A branch produces a `Result` or the `std::optional<Result>` holding one — an
 * invocable and a ready value alike — and an empty optional means the branch ran and
 * produced nothing. That second form is what lets one chain stand as a branch of
 * another: a chain is callable and answers `std::optional<Result>`, so it fits a
 * case of an outer chain or its fallback, and an unmatched subject falls through as
 * an empty result rather than a wrong one. `Result` is tried first, so a chain over
 * an optional `Result` keeps its meaning.
 *
 * @note @ref scl::any_switch::has_case answers for the chain it is asked, not for a
 *       chain nested inside a branch of it: a fallback covers every subject, whatever
 *       the chain behind it then decides.
 *
 * A case an earlier one already covers is a compile error rather than a branch
 * that never runs, and coverage follows what a case matches rather than how it is
 * spelled: `in_case<T>` and `in_case<T const &>` cover the same referents, and
 * both of them cover `in_case<T &>`, which asks for less. Order therefore
 * decides — `in_case<T &>` written after either of them is rejected, while
 * written before them it keeps the unqualified referent for itself. For the same
 * reason a second `or_else`, and any `in_case` written after one, are compile
 * errors too.
 *
 * @tparam Result    What a branch produces. `void` — the default — is a chain that
 *                   runs its branches for their effect and returns nothing.
 * @tparam Branches  The branches accumulated so far; grown by `in_case` and
 *                   `or_else`, never spelled at a use.
 *
 * `any_switch<>{}` starts a chain that produces nothing; `any_switch<Result>{}` one
 * that produces a @p Result. A chain with no branch matches nothing, so a bare chain
 * is only a starting point for `in_case` and `or_else`.
 *
 * @note Building the chain runs nothing: no selection, no branch, no result. A
 *       branch argument that is a *value* is the one thing no chain defers -
 *       `or_else(compute())` evaluates `compute()` where it is written, since
 *       that is an ordinary function argument. Pass an invocable where building
 *       the value is expensive.
 *
 * @note The chain is a literal type wherever its branches are, and the subject
 *       reaches `apply` as a parameter — the one position where an
 *       @ref scl::any_arg cast folds on the C++20 baseline. A `constexpr` chain
 *       applied in a constant expression therefore selects and runs at compile
 *       time, with none of the restrictions a stored `any_arg` carries.
 *
 * @warning The chain catches nothing and wraps nothing: a branch that throws
 *          throws out of `apply`, in the caller's frame. What can still dangle is
 *          a branch capturing by reference, and it dangles the way any captured
 *          reference does.
 *
 * @par Example
 * @code
 * constexpr auto matcher = scl::any_switch<std::string>()
 *     .in_case<void>("has no value")
 *     .in_case<int>([](int number) { return std::to_string(number); })
 *     .in_case<std::string const &>([](std::string const & text) { return text; })
 *     .or_else("undefined");
 *
 * std::optional<std::string> result = matcher.apply(value);   // no precondition
 * bool covered = matcher.has_case(value);                     // runs no branch
 * @endcode
 *
 * @see scl::any_arg — the subject type every branch reads through
 * @see scl::any_view — the storable read-only view, also accepted as a subject
 */

/**
 * @typedef scl::any_switch::result_type
 * @brief What @ref scl::any_switch::apply returns.
 *
 * `std::optional<Result>`, or `void` for a chain that produces nothing.
 */

/**
 * @fn scl::any_switch::any_switch()
 * @brief Constructs the empty chain, the starting point every other chain is
 *        built from with @ref scl::any_switch::in_case.
 */

/**
 * @fn scl::any_switch::in_case(Handler && handler) const
 * @brief Returns a chain with one more case at the end of it.
 *
 * @tparam Case     The type this branch selects: an object type, an lvalue
 *                  reference to one, or `void` for the empty value. An rvalue
 *                  reference is rejected — nothing here is movable-from.
 * @tparam Handler  Deduced from @p handler; never spelled at a call.
 * @param  handler  What this branch produces: an invocable taking no argument or one
 *                  argument constructible from @p Case, or — for a named `Result` —
 *                  a value convertible to `Result` or to `std::optional<Result>`.
 *                  Either way it must produce one of those two.
 *                  Another `any_switch` qualifies, which is how chains nest. Stored by
 *                  value in the returned chain, and called only by
 *                  @ref scl::any_switch::apply.
 * @return A chain carrying every branch of this one, plus this case.
 */

/**
 * @fn scl::any_switch::in_case() const
 * @brief Returns a chain with one more case that matches and does nothing.
 *
 * Doing nothing is a complete branch only where nothing is the result, so this
 * form belongs to `Result = void` and is a compile error for a named `Result`.
 * It is how a type is claimed away from the fallback without writing an empty
 * branch for it.
 *
 * @tparam Case  The type this branch selects, as in the form above.
 * @return A chain carrying every branch of this one, plus this case.
 */

/**
 * @fn scl::any_switch::or_else(Handler && handler) const
 * @brief Returns a chain with the fallback branch attached.
 *
 * The fallback catches everything no case caught — an empty value included when
 * the chain has no `in_case<void>`. It is an ordinary branch: it returns the
 * chain, not a result. Since it catches everything, nothing may follow it and
 * nothing may repeat it.
 *
 * @tparam Handler  Deduced from @p handler; never spelled at a call.
 * @param  handler  What the fallback produces: an invocable taking no argument or the
 *                  subject as @ref scl::any_arg, or — for a named `Result` — a value
 *                  convertible to `Result` or to `std::optional<Result>`. Another `any_switch` qualifies, which
 *                  is how a chain delegates what it did not catch. Stored by value in
 *                  the returned chain.
 * @return A chain carrying every branch of this one, plus the fallback.
 */

/**
 * @fn scl::any_switch::apply(any_arg subject) const
 * @brief Runs the first branch that matches @p subject, and no other.
 *
 * Selection follows the order the cases are written. `apply` may be called any
 * number of times, over as many subjects as the caller has, and does the whole
 * thing again each time: the chain describes the branches and holds no result, so
 * a branch with a side effect fires once per call. It carries no precondition -
 * no branch matching is an ordinary outcome, reported by the result.
 *
 * It is `noexcept` when the branches it may run are and moving the result cannot
 * throw, and `const` wherever those
 * branches are callable on a `const` chain; a chain holding a `mutable` invocable
 * applies through a non-`const` one.
 *
 * @param  subject  The value to read — anything @ref scl::any_arg accepts.
 * @return `std::optional<Result>`, empty when no branch matched; nothing at all
 *         when `Result` is `void`, where an unmatched subject simply runs nothing.
 */

/**
 * @fn scl::any_switch::apply(any_arg subject)
 * @brief Runs the first branch that matches @p subject, and no other.
 *
 * The form a chain holding a `mutable` invocable applies through; identical to the
 * `const` form in every other respect.
 *
 * @param  subject  The value to read — anything @ref scl::any_arg accepts.
 * @return What the `const` form returns.
 */

/**
 * @fn scl::any_switch::operator()(any_arg subject) const
 * @brief Runs the chain over @p subject — @ref scl::any_switch::apply by another
 *        spelling.
 *
 * Identical in every respect: same selection, same result, same `const` and
 * `noexcept` forms. It exists so a chain can be handed to anything that takes a
 * callable — an algorithm, a `std::function`, a parameter expecting a transform —
 * without wrapping it in a lambda that only calls `apply`.
 *
 * It is a call, not a conversion: a chain still converts to nothing at all, and
 * reading its result stays explicit.
 *
 * @param  subject  The value to read — anything @ref scl::any_arg accepts.
 * @return What @ref scl::any_switch::apply returns.
 */

/**
 * @fn scl::any_switch::operator()(any_arg subject)
 * @brief Runs the chain over @p subject — @ref scl::any_switch::apply by another
 *        spelling.
 *
 * The form a chain holding a `mutable` invocable calls through; identical to the
 * `const` form in every other respect.
 *
 * @param  subject  The value to read — anything @ref scl::any_arg accepts.
 * @return What @ref scl::any_switch::apply returns.
 */

/**
 * @fn scl::any_switch::has_case(any_arg subject) const
 * @brief Reports whether some branch matches @p subject.
 *
 * It performs the selection and stops there — no branch is run, so no side
 * effect fires. It exists for what `apply` cannot answer: whether a branch
 * matched. An empty result does not say — a matched branch producing an empty
 * `std::optional` reads the same — and a `void` chain returns nothing at all. It
 * also asks before a branch with a side effect, or an expensive one.
 *
 * @param  subject  The value to test — anything @ref scl::any_arg accepts.
 * @return `true` when a branch matches, which a chain with a fallback always
 *         does; `false` otherwise.
 */
