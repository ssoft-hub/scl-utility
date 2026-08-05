#pragma once

/**
 * @file observer_tuple.h
 * @brief Fan-out observer for scl::hierarchy::tree.
 * @ingroup scl_utility_hierarchy
 */

#include <cstddef>
#include <tuple>

namespace scl::hierarchy
{
    /**
     * @brief Observer that fans out every tree event to a tuple of inner observers.
     * @ingroup scl_utility_hierarchy
     * @tparam TreeType   The tree instantiation.
     * @tparam Observers  One or more inner observer types.
     */
    template <typename TreeType, typename... Observers>
    class observer_tuple
    {
    public:
        using tree = TreeType;
        using node_item = tree::iterator;
        using const_node_item = tree::const_iterator;
        using const_payload_reference = tree::const_payload_reference;

        using tuple_type = ::std::tuple<Observers...>;

    private:
        tuple_type m_observers;

    public:
        [[nodiscard]]
        constexpr tuple_type & tuple() noexcept
        {
            return m_observers;
        }

        [[nodiscard]]
        constexpr tuple_type const & tuple() const noexcept
        {
            return m_observers;
        }

        template <::std::size_t I>
        [[nodiscard]]
        constexpr decltype(auto) get() noexcept
        {
            return ::std::get<I>(m_observers);
        }

        template <::std::size_t I>
        [[nodiscard]]
        constexpr decltype(auto) get() const noexcept
        {
            return ::std::get<I>(m_observers);
        }

        /**
         * @brief Returns a mutable reference to the inner observer of type @p T.
         *
         * @tparam T  One of the `Observers...` types. Ill-formed if `T` does not
         *            appear exactly once in the pack — same rule as `std::get<T>`.
         * @return    The inner observer of type `T`.
         *
         * @par Example
         * @code
         * auto & c = t.get_observer().get<counter<decltype(t)>>();
         * @endcode
         */
        template <typename T>
        [[nodiscard]]
        constexpr decltype(auto) get() noexcept
        {
            return ::std::get<T>(m_observers);
        }

        /**
         * @brief Returns an immutable reference to the inner observer of type @p T.
         * @tparam T  One of the `Observers...` types. Ill-formed if `T` does not
         *            appear exactly once in the pack — same rule as `std::get<T>`.
         */
        template <typename T>
        [[nodiscard]]
        constexpr decltype(auto) get() const noexcept
        {
            return ::std::get<T>(m_observers);
        }

        // Observer contract — the four callbacks scl::hierarchy::tree routes events through.
        constexpr void on_clear()
        {
            ::std::apply([](auto &&... obs) { ((obs.on_clear()), ...); }, m_observers);
        }

        constexpr void on_insert(node_item item)
        {
            ::std::apply([&](auto &&... obs) { ((obs.on_insert(item)), ...); }, m_observers);
        }

        constexpr void on_erase(const_node_item item)
        {
            ::std::apply([&](auto &&... obs) { ((obs.on_erase(item)), ...); }, m_observers);
        }

        constexpr void on_change(const_payload_reference old_value, const_payload_reference new_value)
        {
            ::std::apply([&](auto &&... obs) {
                ((obs.on_change(old_value, new_value)), ...);
            }, m_observers);
        }
    };

} // namespace scl::hierarchy

// =============================================================================
// scl::hierarchy::observer_tuple — Doxygen documentation
// =============================================================================

// -----------------------------------------------------------------------------
// Class overview
// -----------------------------------------------------------------------------

/**
 * @class scl::hierarchy::observer_tuple
 * @ingroup scl_utility_hierarchy
 * @brief Composite observer that fans a `tree`'s four notifications out to
 *        several inner observers.
 *
 * `tree`'s `Observer` template parameter accepts exactly one observer type.
 * `observer_tuple<TreeType, Observers...>` lets that one type be several: it
 * holds a `std::tuple<Observers...>` and, on every notification, forwards it
 * to all of them via `std::apply`, **in declaration order, unconditionally**
 * — there is no way for one inner observer to stop propagation to the next,
 * and inner observers cannot see or influence each other. Each inner
 * observer must independently satisfy @ref scl::hierarchy::concepts::observer.
 *
 * Inner observers are reachable by index or by type through `get()`.
 *
 * @tparam TreeType   The tree instantiation this composite observer attaches to.
 * @tparam Observers  One or more inner observer types, each satisfying
 *                     @ref scl::hierarchy::concepts::observer.
 *
 * @par Composing two observers
 * @code
 * template <typename Tree>
 * struct logger {
 *     void on_insert(typename Tree::iterator it)
 *     { std::cout << "insert: " << (*it).value() << '\n'; }
 *     void on_erase(typename Tree::const_iterator it)
 *     { std::cout << "erase: " << (*it).value() << '\n'; }
 *     void on_clear() { std::cout << "clear\n"; }
 *     void on_change(int old_v, int new_v)
 *     { std::cout << "change: " << old_v << " -> " << new_v << '\n'; }
 * };
 *
 * template <typename Tree>
 * struct counter {
 *     std::size_t count = 0;
 *     void on_insert(typename Tree::iterator) { ++count; }
 *     void on_erase(typename Tree::const_iterator) { --count; }
 *     void on_clear() { count = 0; }
 *     void on_change(int, int) {}
 * };
 *
 * template <typename T>
 * using multi_obs = scl::hierarchy::observer_tuple<T, logger<T>, counter<T>>;
 *
 * scl::hierarchy::tree<int, multi_obs> t;
 * t.push_back(1);   // logger prints "insert: 1"; counter.count becomes 1
 * t.push_back(2);
 * std::cout << t.get_observer().get<counter<decltype(t)>>().count; // 2
 * @endcode
 *
 * @par Accessing inner observers by index vs. by type
 * @code
 * auto & first_by_index = t.get_observer().get<0>();                       // logger<...>
 * auto & second_by_type = t.get_observer().get<counter<decltype(t)>>();    // counter<...>
 * @endcode
 */

// -----------------------------------------------------------------------------
// Type aliases
// -----------------------------------------------------------------------------

/**
 * @typedef scl::hierarchy::observer_tuple::tree
 * @brief The tree instantiation this observer is attached to (same as `TreeType`).
 */

/**
 * @typedef scl::hierarchy::observer_tuple::node_item
 * @brief Iterator type passed to `on_insert`, forwarded from `tree`.
 */

/**
 * @typedef scl::hierarchy::observer_tuple::const_node_item
 * @brief Const iterator type passed to `on_erase`, forwarded from `tree`.
 */

/**
 * @typedef scl::hierarchy::observer_tuple::const_payload_reference
 * @brief Payload reference type passed to `on_change`, forwarded from `tree`.
 */

/**
 * @typedef scl::hierarchy::observer_tuple::tuple_type
 * @brief The underlying `std::tuple<Observers...>` holding the inner observers.
 */

// -----------------------------------------------------------------------------
// Accessors
// -----------------------------------------------------------------------------

/**
 * @fn scl::hierarchy::observer_tuple::tuple()
 * @brief Returns a mutable reference to the underlying `std::tuple<Observers...>`.
 * @return `tuple_type &`
 */

/**
 * @fn scl::hierarchy::observer_tuple::tuple() const
 * @brief Returns an immutable reference to the underlying `std::tuple<Observers...>`.
 * @return `tuple_type const &`
 */

/**
 * @fn scl::hierarchy::observer_tuple::get()
 * @brief Returns a mutable reference to the @p I -th inner observer.
 *
 * @tparam I  Zero-based index into the `Observers...` pack.
 * @return    The inner observer at index `I`, exactly as `std::get<I>` would.
 *
 * @par Example
 * @code
 * auto & first = t.get_observer().get<0>();
 * @endcode
 */

/**
 * @fn scl::hierarchy::observer_tuple::get() const
 * @brief Returns an immutable reference to the @p I -th inner observer.
 * @tparam I  Zero-based index into the `Observers...` pack.
 * @return    The inner observer at index `I`, exactly as `std::get<I>` would.
 */

// -----------------------------------------------------------------------------
// Observer contract (fan-out implementation)
// -----------------------------------------------------------------------------

/**
 * @fn scl::hierarchy::observer_tuple::on_clear()
 * @brief Forwards `on_clear()` to every inner observer, in declaration order.
 */

/**
 * @fn scl::hierarchy::observer_tuple::on_insert(node_item)
 * @brief Forwards `on_insert(item)` to every inner observer, in declaration order.
 * @param item  The iterator `tree` passes to `on_insert`.
 */

/**
 * @fn scl::hierarchy::observer_tuple::on_erase(const_node_item)
 * @brief Forwards `on_erase(item)` to every inner observer, in declaration order.
 * @param item  The const iterator `tree` passes to `on_erase`.
 */

/**
 * @fn scl::hierarchy::observer_tuple::on_change(const_payload_reference, const_payload_reference)
 * @brief Forwards `on_change(old_value, new_value)` to every inner observer, in
 *        declaration order.
 * @param old_value  The payload's value before the change.
 * @param new_value  The payload's value after the change.
 */
