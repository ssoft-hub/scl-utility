#pragma once

/// @file tree.h
/// @brief Observer-aware multi-root tree.
/// @ingroup scl_utility_hierarchy

#include "algorithm.h"
#include "node.h"
#include <tuple>

namespace scl::hierarchy
{
    /// @brief Observer-aware multi-root tree.
    /// @ingroup scl_utility_hierarchy
    ///
    /// Wraps a collection of root node instances and routes all structural
    /// mutations through an Observer policy.  Users access nodes via
    /// node_wrapper / const_node_wrapper proxies.
    ///
    /// @tparam Value     Payload type stored in each node.
    /// @tparam Observer  Observer policy template instantiated as Observer<tree>.
    /// @tparam Allocator Allocator template forwarded to the underlying node list.
    template <typename Value, template <typename> class Observer, template <typename> class Allocator = ::std::allocator>
    class tree
    {
        using self_type = tree<Value, Observer, Allocator>;

    public:
        using value_type = Value;
        using reference_type = value_type &;
        using const_reference_type = value_type const &;

        using observer_type = Observer<self_type>;
        using observer_reference = observer_type &;
        using observer_const_reference = observer_type const &;

        using node = ::scl::hierarchy::node<Value, Allocator>;
        using nodes = typename node::nodes;

    public:
        class node_wrapper;
        class const_node_wrapper;
        class node_wrapper_iterator;
        class const_node_wrapper_iterator;

        using iterator = node_wrapper_iterator;
        using const_iterator = const_node_wrapper_iterator;
        using size_type = typename nodes::size_type;
        using difference_type = typename nodes::difference_type;
        using reference = node_wrapper;
        using const_reference = const_node_wrapper;

    private:
        template <typename T>
        class arrow
        {
            T m_value;

        public:
            template <typename... Arguments>
            constexpr arrow(Arguments &&... arguments)
                : m_value{::std::forward<Arguments>(arguments)...}
            {}

            constexpr auto operator->() && { return ::std::addressof(m_value); }
        };

    public:
        /// @brief Mutable iterator over sibling nodes.
        class node_wrapper_iterator
        {
            using self_type = node_wrapper_iterator;

            template <typename, template <typename> class, template <typename> class>
            friend class tree;
            friend class const_node_wrapper_iterator;
            friend class node_wrapper;
            friend class const_node_wrapper;

        public:
            using tree_type = scl::hierarchy::tree<Value, Observer, Allocator>;
            using tree_reference = tree_type &;
            using node_wrapper_type = typename tree_type::node_wrapper;
            using const_node_wrapper_type = typename tree_type::const_node_wrapper;

        private:
            using node_iterator = typename node::iterator;
            using tree_ptr = tree_type *;

            tree_ptr m_tree = nullptr;
            node_iterator m_node_iter;

            constexpr node_wrapper_iterator(tree_ptr t, node_iterator it) noexcept
                : m_tree{t}
                , m_node_iter{it}
            {}

        public:
            using iterator_category = ::std::bidirectional_iterator_tag;
            using value_type = node_wrapper_type;
            using difference_type = ::std::ptrdiff_t;
            using pointer = void;
            using reference = node_wrapper_type;

            node_wrapper_iterator() = default;

            constexpr bool operator==(self_type const & other) const noexcept
            {
                return m_node_iter == other.m_node_iter;
            }
            constexpr bool operator!=(self_type const & other) const noexcept
            {
                return !(*this == other);
            }

            self_type & operator++() noexcept
            {
                ++m_node_iter;
                return *this;
            }
            self_type & operator--() noexcept
            {
                --m_node_iter;
                return *this;
            }
            self_type operator++(int) noexcept { return {m_tree, m_node_iter++}; }
            self_type operator--(int) noexcept { return {m_tree, m_node_iter--}; }

            node_wrapper_type operator*() noexcept { return {*m_tree, *m_node_iter}; }
            const_node_wrapper_type operator*() const noexcept { return {*m_tree, *m_node_iter}; }

            arrow<node_wrapper_type> operator->() noexcept { return {*m_tree, *m_node_iter}; }
            arrow<const_node_wrapper_type> operator->() const noexcept
            {
                return {*m_tree, *m_node_iter};
            }
        };

        /// @brief Const iterator over sibling nodes.
        class const_node_wrapper_iterator
        {
            using self_type = const_node_wrapper_iterator;

            template <typename, template <typename> class, template <typename> class>
            friend class tree;
            friend class node_wrapper;
            friend class const_node_wrapper;

        public:
            using tree_type = ::scl::hierarchy::tree<Value, Observer, Allocator>;
            using const_node_wrapper_type = typename tree_type::const_node_wrapper;

        private:
            using const_node_iterator = typename node::const_iterator;
            using const_tree_ptr = tree_type const *;

            const_tree_ptr m_tree = nullptr;
            const_node_iterator m_node_iter;

            constexpr const_node_wrapper_iterator(const_tree_ptr t, const_node_iterator it) noexcept
                : m_tree{t}
                , m_node_iter{it}
            {}

        public:
            using iterator_category = ::std::bidirectional_iterator_tag;
            using value_type = const_node_wrapper_type;
            using difference_type = ::std::ptrdiff_t;
            using pointer = void;
            using reference = const_node_wrapper_type;

            const_node_wrapper_iterator() = default;
            const_node_wrapper_iterator(self_type &&) noexcept = default;
            const_node_wrapper_iterator(self_type &) noexcept = default;
            const_node_wrapper_iterator(self_type const &) noexcept = default;

            constexpr const_node_wrapper_iterator(node_wrapper_iterator const & other) noexcept
                : m_tree{other.m_tree}
                , m_node_iter{other.m_node_iter}
            {}

            self_type & operator=(self_type const &) noexcept = default;
            constexpr self_type & operator=(node_wrapper_iterator const & other) noexcept
            {
                m_tree = other.m_tree;
                m_node_iter = other.m_node_iter;
                return *this;
            }

            constexpr bool operator==(self_type const & other) const noexcept
            {
                return m_node_iter == other.m_node_iter;
            }
            constexpr bool operator!=(self_type const & other) const noexcept
            {
                return !(*this == other);
            }

            self_type & operator++() noexcept
            {
                ++m_node_iter;
                return *this;
            }
            self_type & operator--() noexcept
            {
                --m_node_iter;
                return *this;
            }
            self_type operator++(int) noexcept { return {m_tree, m_node_iter++}; }
            self_type operator--(int) noexcept { return {m_tree, m_node_iter--}; }

            const_node_wrapper_type operator*() const noexcept { return {*m_tree, *m_node_iter}; }
            arrow<const_node_wrapper_type> operator->() const noexcept
            {
                return {*m_tree, *m_node_iter};
            }
        };

        /// @brief Mutable proxy over a tree node.
        class node_wrapper
        {
            using self_type = node_wrapper;

            template <typename, template <typename> class, template <typename> class>
            friend class tree;
            friend class const_node_wrapper;

        public:
            using tree_type = ::scl::hierarchy::tree<value_type, Observer, Allocator>;
            using tree_reference = tree_type &;
            using const_tree_reference = tree_type const &;

            using node_reference_type = node &;
            using node_const_reference_type = node const &;

            using reference_type = value_type &;
            using const_reference_type = value_type const &;

            using node_wrapper_iterator_type = typename tree_type::node_wrapper_iterator;
            using const_node_wrapper_iterator_type = typename tree_type::const_node_wrapper_iterator;

            using node_count = typename node::node_count;
            using iterator = node_wrapper_iterator_type;
            using const_iterator = const_node_wrapper_iterator_type;
            using size_type = node_count;
            using difference_type = ::std::ptrdiff_t;
            using reference = self_type;
            using const_reference = const_node_wrapper;

        private:
            tree_reference m_tree;
            node_reference_type m_node;

            constexpr node_wrapper(tree_reference t, node_reference_type n) noexcept
                : m_tree{t}
                , m_node{n}
            {}

        public:
            constexpr tree_reference tree() noexcept { return m_tree; }
            constexpr const_tree_reference tree() const noexcept { return m_tree; }

            /// @brief Replaces the payload and notifies the observer.
            void set_value(const_reference_type val)
            {
                m_tree.notify_on_change(m_node.value(), val);
                m_node.value() = val;
            }
            /// @brief Returns const reference to the payload value.
            constexpr const_reference_type value() const noexcept { return m_node.value(); }

            constexpr node_wrapper parent() noexcept { return {m_tree, m_node.parent()}; }
            constexpr const_node_wrapper parent() const noexcept
            {
                return {m_tree, m_node.parent()};
            }

            constexpr bool operator==(const_node_wrapper other) const noexcept;
            constexpr bool operator!=(const_node_wrapper other) const noexcept
            {
                return !(*this == other);
            }

            bool empty() const noexcept { return m_node.empty(); }

            template <typename... Arguments>
            node_wrapper_iterator_type
            emplace(const_node_wrapper_iterator_type before_pos, Arguments &&... arguments)
                requires(::std::constructible_from<value_type, Arguments...>)
            {
                node_wrapper_iterator_type item{::std::addressof(m_tree),
                    m_node.emplace(before_pos.m_node_iter, ::std::forward<Arguments>(arguments)...)};
                m_tree.notify_after_insert(item);
                return item;
            }

            template <typename Item>
            Item erase(Item position) noexcept
                requires(::std::same_as<Item, node_wrapper_iterator_type> ||
                    ::std::same_as<Item, const_node_wrapper_iterator_type>)
            {
                m_tree.notify_before_erase(position);
                return {::std::addressof(m_tree), m_node.erase(position.m_node_iter)};
            }

            template <typename Item>
            Item erase(Item first, Item last) noexcept
                requires(::std::same_as<Item, node_wrapper_iterator_type> ||
                    ::std::same_as<Item, const_node_wrapper_iterator_type>)
            {
                for (auto item = first; item != last; ++item)
                    m_tree.notify_before_erase(item);
                return {::std::addressof(m_tree), m_node.erase(first.m_node_iter, last.m_node_iter)};
            }

            void clear() noexcept
            {
                for (auto item = cbegin(); item != cend(); ++item)
                    m_tree.notify_before_erase(item);
                m_node.clear();
            }

            size_type size() const noexcept { return m_node.size(); }

            reference front() noexcept { return {m_tree, m_node.front()}; }
            const_reference front() const noexcept { return {m_tree, m_node.front()}; }
            reference back() noexcept { return {m_tree, m_node.back()}; }
            const_reference back() const noexcept { return {m_tree, m_node.back()}; }

            template <typename... Arguments>
            iterator push_back(Arguments &&... arguments)
                requires(::std::constructible_from<typename tree_type::value_type, Arguments...>)
            {
                return emplace(cend(), ::std::forward<Arguments>(arguments)...);
            }

            template <typename... Arguments>
            iterator push_front(Arguments &&... arguments)
                requires(::std::constructible_from<typename tree_type::value_type, Arguments...>)
            {
                return emplace(cbegin(), ::std::forward<Arguments>(arguments)...);
            }

            void pop_back() noexcept { erase(--cend()); }
            void pop_front() noexcept { erase(cbegin()); }

            template <typename... Arguments>
            iterator insert(const_iterator before_pos, Arguments &&... arguments)
                requires(::std::constructible_from<typename tree_type::value_type, Arguments...>)
            {
                return emplace(before_pos, ::std::forward<Arguments>(arguments)...);
            }

            /// @brief Removes and returns (deep-copies) the child at @p position.
            node release(const_node_wrapper_iterator_type position) noexcept
            {
                m_tree.notify_before_erase(position);
                return m_node.take(position.m_node_iter);
            }

            constexpr node_const_reference_type access() const noexcept { return m_node; }

            void transfer(node_wrapper from) noexcept { transfer(cend(), from); }

            void transfer(const_node_wrapper_iterator_type before_pos, node_wrapper from) noexcept
            {
                bool const cross = ::std::addressof(m_tree) != ::std::addressof(from.m_tree);
                node_count n = 0;
                if (cross)
                    for (const_node_wrapper_iterator_type it = from.cbegin(); it != from.cend(); ++it, ++n)
                        from.m_tree.notify_before_erase(it);

                node_wrapper_iterator_type to{::std::addressof(m_tree), from.begin().m_node_iter};
                m_node.transfer(before_pos.m_node_iter, from.m_node);

                if (cross)
                    for (node_count i = 0; i < n; ++i)
                        m_tree.notify_after_insert(to++);
            }

            void transfer(node_wrapper from, const_node_wrapper_iterator_type from_pos) noexcept
            {
                transfer(cend(), from, from_pos);
            }

            void transfer(const_node_wrapper_iterator_type before_pos,
                node_wrapper from,
                const_node_wrapper_iterator_type from_pos) noexcept
            {
                bool const cross = ::std::addressof(m_tree) != ::std::addressof(from.m_tree);
                if (cross)
                    from.m_tree.notify_before_erase(from_pos);

                const_node_wrapper_iterator_type to{::std::addressof(m_tree), from_pos.m_node_iter};
                m_node.transfer(before_pos.m_node_iter, from.m_node, from_pos.m_node_iter);

                if (cross)
                    m_tree.notify_after_insert(to);
            }

            void transfer(node_wrapper from,
                const_node_wrapper_iterator_type from_first,
                const_node_wrapper_iterator_type from_last) noexcept
            {
                transfer(cend(), from, from_first, from_last);
            }

            void transfer(const_node_wrapper_iterator_type before_pos,
                node_wrapper from,
                const_node_wrapper_iterator_type from_first,
                const_node_wrapper_iterator_type from_last) noexcept
            {
                bool const cross = ::std::addressof(m_tree) != ::std::addressof(from.m_tree);
                node_count n = 0;
                if (cross)
                    for (const_node_wrapper_iterator_type it = from_first; it != from_last; ++it, ++n)
                        from.m_tree.notify_before_erase(it);

                const_node_wrapper_iterator_type to{::std::addressof(m_tree), from_first.m_node_iter};
                m_node.transfer(before_pos.m_node_iter, from.m_node, from_first.m_node_iter,
                    from_last.m_node_iter);

                if (cross)
                    for (node_count i = 0; i < n; ++i)
                        m_tree.notify_after_insert(to++);
            }

            node_wrapper_iterator_type begin() noexcept
            {
                return {::std::addressof(m_tree), m_node.begin()};
            }
            const_node_wrapper_iterator_type begin() const noexcept { return cbegin(); }
            const_node_wrapper_iterator_type cbegin() const noexcept
            {
                return {::std::addressof(m_tree), m_node.cbegin()};
            }

            node_wrapper_iterator_type end() noexcept
            {
                return {::std::addressof(m_tree), m_node.end()};
            }
            const_node_wrapper_iterator_type end() const noexcept { return cend(); }
            const_node_wrapper_iterator_type cend() const noexcept
            {
                return {::std::addressof(m_tree), m_node.cend()};
            }
        };

        /// @brief Const proxy over a tree node.
        class const_node_wrapper
        {
            using self_type = const_node_wrapper;

            template <typename, template <typename> class, template <typename> class>
            friend class tree;
            friend class node_wrapper;

        public:
            using tree_type = scl::hierarchy::tree<value_type, Observer, Allocator>;
            using const_tree_reference = tree_type const &;
            using node_const_reference_type = node const &;
            using const_reference_type = value_type const &;

            using const_node_wrapper_iterator_type = typename tree_type::const_node_wrapper_iterator;

            using node_count = typename node::node_count;
            using iterator = const_node_wrapper_iterator_type;
            using const_iterator = const_node_wrapper_iterator_type;
            using size_type = node_count;
            using difference_type = ::std::ptrdiff_t;
            using reference = self_type;
            using const_reference = self_type;

        private:
            const_tree_reference m_tree;
            node_const_reference_type m_node;

            constexpr const_node_wrapper(const_tree_reference t, node_const_reference_type n) noexcept
                : m_tree{t}
                , m_node{n}
            {}

        public:
            const_node_wrapper(self_type &&) noexcept = default;
            const_node_wrapper(self_type &) noexcept = default;
            const_node_wrapper(self_type const &) noexcept = default;

            constexpr const_node_wrapper(node_wrapper other) noexcept
                : m_tree{other.m_tree}
                , m_node{other.m_node}
            {}

            constexpr const_tree_reference tree() const noexcept { return m_tree; }
            constexpr const_reference_type value() const noexcept { return m_node.value(); }

            constexpr const_node_wrapper parent() const noexcept
            {
                return {m_tree, m_node.parent()};
            }
            constexpr node_const_reference_type access() const noexcept { return m_node; }

            constexpr bool operator==(const_node_wrapper other) const noexcept
            {
                return scl::hierarchy::are_identical(m_node, other.m_node);
            }
            constexpr bool operator!=(const_node_wrapper other) const noexcept
            {
                return !(*this == other);
            }

            bool empty() const noexcept { return m_node.empty(); }
            size_type size() const noexcept { return m_node.size(); }

            reference front() const noexcept { return {m_tree, m_node.front()}; }
            reference back() const noexcept { return {m_tree, m_node.back()}; }

            const_node_wrapper_iterator_type begin() const noexcept { return cbegin(); }
            const_node_wrapper_iterator_type cbegin() const noexcept
            {
                return {::std::addressof(m_tree), m_node.cbegin()};
            }

            const_node_wrapper_iterator_type end() const noexcept { return cend(); }
            const_node_wrapper_iterator_type cend() const noexcept
            {
                return {::std::addressof(m_tree), m_node.cend()};
            }
        };

    private:
        observer_type m_observer;
        nodes m_nodes;

    public:
        constexpr observer_reference observer() noexcept { return m_observer; }
        constexpr observer_const_reference observer() const noexcept { return m_observer; }

        bool empty() const noexcept { return m_nodes.empty(); }
        size_type size() const noexcept { return m_nodes.size(); }

        reference front() noexcept { return *begin(); }
        const_reference front() const noexcept { return *cbegin(); }
        reference back() noexcept { return *--end(); }
        const_reference back() const noexcept { return *--cend(); }

        template <typename... Arguments>
        iterator push_back(Arguments &&... arguments)
            requires(::std::constructible_from<value_type, Arguments...>)
        {
            return emplace(cend(), ::std::forward<Arguments>(arguments)...);
        }

        template <typename... Arguments>
        iterator push_front(Arguments &&... arguments)
            requires(::std::constructible_from<value_type, Arguments...>)
        {
            return emplace(cbegin(), ::std::forward<Arguments>(arguments)...);
        }

        void pop_back() noexcept { erase(--cend()); }
        void pop_front() noexcept { erase(cbegin()); }

        template <typename... Arguments>
        iterator insert(const_iterator before_pos, Arguments &&... arguments)
            requires(::std::constructible_from<value_type, Arguments...>)
        {
            return emplace(before_pos, ::std::forward<Arguments>(arguments)...);
        }

        template <typename... Arguments>
        node_wrapper_iterator emplace(const_node_wrapper_iterator before_pos, Arguments &&... arguments)
            requires(::std::constructible_from<value_type, Arguments...>)
        {
            node_wrapper_iterator iter{this,
                m_nodes.emplace(before_pos.m_node_iter, ::std::forward<Arguments>(arguments)...)};
            notify_after_insert(iter);
            return iter;
        }

        template <typename Iterator>
        Iterator erase(Iterator position) noexcept
            requires(::std::same_as<Iterator, node_wrapper_iterator> || ::std::same_as<Iterator, const_node_wrapper_iterator>)
        {
            assert(!::scl::hierarchy::has_parent(*position));
            notify_before_erase(position);
            return {this, m_nodes.erase(position.m_node_iter)};
        }

        template <typename Iterator>
        Iterator erase(Iterator first, Iterator last) noexcept
            requires(::std::same_as<Iterator, node_wrapper_iterator> || ::std::same_as<Iterator, const_node_wrapper_iterator>)
        {
            assert(!::scl::hierarchy::has_parent(*first));
            for (auto it = first; it != last; ++it)
                notify_before_erase(it);
            return {this, m_nodes.erase(first.m_node_iter, last.m_node_iter)};
        }

        void clear() noexcept
        {
            notify_before_clear();
            m_nodes.clear();
        }

        /// @brief Removes the node at @p position from its parent or the root list.
        template <typename Iterator>
        void remove(Iterator position)
            requires(::std::same_as<Iterator, node_wrapper_iterator> || ::std::same_as<Iterator, const_node_wrapper_iterator>)
        {
            assert(this == position.m_tree);
            node_wrapper nw{*this, const_cast<node &>(*position.m_node_iter)};
            if (::scl::hierarchy::has_parent(nw))
                nw.parent().erase(position);
            else
                erase(position);
        }

        node_wrapper_iterator begin() noexcept { return {this, m_nodes.begin()}; }
        const_node_wrapper_iterator begin() const noexcept { return cbegin(); }
        const_node_wrapper_iterator cbegin() const noexcept { return {this, m_nodes.cbegin()}; }

        node_wrapper_iterator end() noexcept { return {this, m_nodes.end()}; }
        const_node_wrapper_iterator end() const noexcept { return cend(); }
        const_node_wrapper_iterator cend() const noexcept { return {this, m_nodes.cend()}; }

    private:
        void notify_after_insert(node_wrapper_iterator iter)
        {
            m_observer.after_insert(iter);
            node_wrapper nw = *iter;
            for (auto sub = nw.begin(); sub != nw.end(); ++sub)
                notify_after_insert(sub);
        }

        void notify_before_erase(const_node_wrapper_iterator iter)
        {
            const_node_wrapper nw = *iter;
            for (auto sub = nw.begin(); sub != nw.end(); ++sub)
                notify_before_erase(sub);
            m_observer.before_erase(iter);
        }

        void notify_before_clear() { m_observer.before_clear(); }

        void notify_on_change(const_reference_type current, const_reference_type next)
        {
            m_observer.on_change(current, next);
        }
    };

    // Out-of-line definition for node_wrapper::operator== (needs const_node_wrapper complete)
    template <typename Value, template <typename> class Observer, template <typename> class Allocator>
    constexpr bool
    tree<Value, Observer, Allocator>::node_wrapper::operator==(const_node_wrapper other) const noexcept
    {
        return ::scl::hierarchy::are_identical(m_node, other.m_node);
    }

    /// @brief Observer that fans out events to a tuple of inner observers.
    /// @ingroup scl_utility_hierarchy
    ///
    /// Each inner observer must provide after_insert, before_erase, before_clear,
    /// and on_change callbacks.
    ///
    /// @tparam TreeType   The tree instantiation.
    /// @tparam Observers  One or more inner observer types.
    template <typename TreeType, typename... Observers>
    class tuple_of_observers
    {
        friend TreeType;

    public:
        using tree_type = TreeType;
        using node_item = typename tree_type::node_wrapper_iterator;
        using const_node_item = typename tree_type::const_node_wrapper_iterator;
        using const_reference_type = typename tree_type::const_reference_type;

        using observer_tuple = ::std::tuple<Observers...>;

    private:
        observer_tuple m_observers;

    public:
        constexpr observer_tuple & tuple() noexcept { return m_observers; }
        constexpr observer_tuple const & tuple() const noexcept { return m_observers; }

        template <::std::size_t I>
        constexpr decltype(auto) get() noexcept
        {
            return ::std::get<I>(m_observers);
        }
        template <::std::size_t I>
        constexpr decltype(auto) get() const noexcept
        {
            return ::std::get<I>(m_observers);
        }

        template <typename T>
        constexpr decltype(auto) get() noexcept
        {
            return ::std::get<T>(m_observers);
        }
        template <typename T>
        constexpr decltype(auto) get() const noexcept
        {
            return ::std::get<T>(m_observers);
        }

    private:
        constexpr void before_clear()
        {
            ::std::apply([](auto &&... obs) { ((obs.before_clear()), ...); }, m_observers);
        }

        constexpr void after_insert(node_item item)
        {
            ::std::apply([&](auto &&... obs) { ((obs.after_insert(item)), ...); }, m_observers);
        }

        constexpr void before_erase(const_node_item item)
        {
            ::std::apply([&](auto &&... obs) { ((obs.before_erase(item)), ...); }, m_observers);
        }

        constexpr void on_change(const_reference_type old_val, const_reference_type new_val)
        {
            ::std::apply([&](auto &&... obs) {
                ((obs.on_change(old_val, new_val)), ...);
            }, m_observers);
        }
    };

} // namespace scl::hierarchy
