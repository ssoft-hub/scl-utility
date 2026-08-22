#pragma once

/**
 * @file tree.h
 * @brief Observer-aware multi-root tree.
 * @ingroup scl_utility_hierarchy
 */

#include <scl/utility/hierarchy/algorithm.h>
#include <scl/utility/hierarchy/node.h>

#include <cassert>
#include <concepts>
#include <iterator>

namespace scl::hierarchy::concepts
{
    /**
     * @brief Observer policy usable by @ref scl::hierarchy::tree.
     * @ingroup scl_utility_hierarchy
     *
     * A conforming observer receives the four notifications the tree routes every
     * structural or payload mutation through.
     *
     * @tparam Observer  The observer type (typically `Observer<tree>`).
     * @tparam Tree      The tree instantiation whose iterator and payload types are used.
     */
    template <typename Observer, typename Tree>
    concept observer =
        requires(Observer & obs, Tree::iterator item, Tree::const_iterator const_item, Tree::const_payload_reference value) {
            obs.on_insert(item);
            obs.on_erase(const_item);
            obs.on_clear();
            obs.on_change(value, value);
        };

    // tree<...>::reference only ever appears behind tree<...>::, a non-deduced
    // context per [temp.deduct.type], so the ADL hooks below take a deduced Ref
    // and narrow it here instead of naming the proxies outright.
    template <typename Ref>
    concept node_proxy =
        requires(Ref self) {
            {
                self.leaf()
            };
            {
                self.has_parent()
            } -> ::std::convertible_to<bool>;
            {
                self.parent()
            };
        } && node<::std::remove_cvref_t<decltype(::std::declval<Ref>().leaf())>>;
} // namespace scl::hierarchy::concepts

namespace scl::hierarchy
{
    /**
     * @brief Observer-aware multi-root tree.
     * @ingroup scl_utility_hierarchy
     * @tparam Payload   Payload type stored in each node.
     * @tparam Observer  Observer policy template instantiated as `Observer<tree>`.
     * @tparam Allocator Allocator template forwarded to the underlying node list.
     */
    template <typename Payload, template <typename> typename Observer, template <typename> typename Allocator = ::std::allocator>
    class tree
    {
    public:
        using payload = Payload;
        using payload_reference = payload &;
        using const_payload_reference = payload const &;

        using observer = Observer<tree>;
        using observer_reference = observer &;
        using observer_const_reference = observer const &;

        using node = ::scl::hierarchy::node<Payload, Allocator>;
        using nodes = node::nodes;

        using pointer = void;
        using const_pointer = void;

        class reference;
        class const_reference;

        using size_type = nodes::size_type;
        using difference_type = nodes::difference_type;

        using value_type = reference;
        using allocator_type = nodes::allocator_type;

    private:
        template <typename T>
        class arrow_wrapper
        {
        private:
            T m_value;

        public:
            template <typename... Arguments>
            constexpr arrow_wrapper(Arguments &&... arguments)
                : m_value{::std::forward<Arguments>(arguments)...}
            {}

            constexpr auto * operator->() && noexcept { return ::std::addressof(m_value); }
        };

    public:
        class iterator
        {
            friend class ::scl::hierarchy::tree<Payload, Observer, Allocator>;

        public:
            using iterator_concept = ::std::bidirectional_iterator_tag;
            using iterator_category = ::std::bidirectional_iterator_tag;

            using value_type = tree::reference;
            using difference_type = tree::difference_type;
            using pointer = arrow_wrapper<tree::reference>;
            using reference = tree::reference;
            using const_reference = tree::const_reference;

        private:
            using tree_pointer = tree *;
            using node_iterator = node::iterator;

        private:
            tree_pointer m_tree = nullptr;
            node_iterator m_node_iter;

        private:
            constexpr iterator(tree_pointer t, node_iterator it) noexcept
                : m_tree{::std::move(t)}
                , m_node_iter{::std::move(it)}
            {}

        public:
            constexpr iterator() = default;

            [[nodiscard]]
            constexpr bool operator==(iterator const & other) const noexcept
            {
                assert(m_tree == nullptr || other.m_tree == nullptr || m_tree == other.m_tree);
                return m_node_iter == other.m_node_iter;
            }

            [[nodiscard]]
            constexpr bool operator!=(iterator const & other) const noexcept
            {
                assert(m_tree == nullptr || other.m_tree == nullptr || m_tree == other.m_tree);
                return m_node_iter != other.m_node_iter;
            }

            constexpr iterator & operator++() noexcept { return ++m_node_iter, *this; }
            constexpr iterator & operator--() noexcept { return --m_node_iter, *this; }
            // cppcheck-suppress postfixOperator
            constexpr iterator operator++(int) noexcept { return {m_tree, m_node_iter++}; }
            // cppcheck-suppress postfixOperator
            constexpr iterator operator--(int) noexcept { return {m_tree, m_node_iter--}; }

            constexpr reference operator*() const noexcept { return {*m_tree, *m_node_iter}; }
            constexpr auto operator->() const noexcept
            {
                return arrow_wrapper<reference>{*m_tree, *m_node_iter};
            }
        };

        class const_iterator
        {
            friend class ::scl::hierarchy::tree<Payload, Observer, Allocator>;

        public:
            using iterator_concept = ::std::bidirectional_iterator_tag;
            using iterator_category = ::std::bidirectional_iterator_tag;

            using value_type = tree::const_reference;
            using difference_type = tree::difference_type;
            using pointer = arrow_wrapper<tree::const_reference>;
            using reference = tree::const_reference;
            using const_reference = tree::const_reference;

        private:
            using const_node_iterator = node::const_iterator;
            using const_tree_pointer = tree const *;

        private:
            const_tree_pointer m_tree = nullptr;
            const_node_iterator m_node_iter;

        private:
            constexpr const_iterator(const_tree_pointer t, const_node_iterator it) noexcept
                : m_tree{::std::move(t)}
                , m_node_iter{::std::move(it)}
            {}

        public:
            constexpr const_iterator() = default;
            // cppcheck-suppress noExplicitConstructor
            constexpr const_iterator(iterator const & other) noexcept
                : m_tree{other.m_tree}
                , m_node_iter{other.m_node_iter}
            {}

            constexpr ~const_iterator() = default;
            constexpr const_iterator(const_iterator const &) noexcept = default;
            constexpr const_iterator(const_iterator &&) noexcept = default;
            constexpr const_iterator & operator=(const_iterator const &) noexcept = default;
            constexpr const_iterator & operator=(const_iterator &&) noexcept = default;
            constexpr const_iterator & operator=(iterator const & other) noexcept
            {
                m_tree = other.m_tree;
                m_node_iter = other.m_node_iter;
                return *this;
            }

            [[nodiscard]]
            constexpr bool operator==(const_iterator const & other) const noexcept
            {
                assert(m_tree == nullptr || other.m_tree == nullptr || m_tree == other.m_tree);
                return m_node_iter == other.m_node_iter;
            }

            [[nodiscard]]
            constexpr bool operator!=(const_iterator const & other) const noexcept
            {
                assert(m_tree == nullptr || other.m_tree == nullptr || m_tree == other.m_tree);
                return m_node_iter != other.m_node_iter;
            }

            constexpr const_iterator & operator++() noexcept { return ++m_node_iter, *this; }
            constexpr const_iterator & operator--() noexcept { return --m_node_iter, *this; }

            // cppcheck-suppress postfixOperator
            constexpr const_iterator operator++(int) noexcept { return {m_tree, m_node_iter++}; }
            // cppcheck-suppress postfixOperator
            constexpr const_iterator operator--(int) noexcept { return {m_tree, m_node_iter--}; }

            constexpr const_reference operator*() const noexcept { return {*m_tree, *m_node_iter}; }
            constexpr auto operator->() const noexcept
            {
                return arrow_wrapper<const_reference>{*m_tree, *m_node_iter};
            }
        };

        using reverse_iterator = ::std::reverse_iterator<iterator>;
        using const_reverse_iterator = ::std::reverse_iterator<const_iterator>;

        class reference
        {
            friend class ::scl::hierarchy::tree<Payload, Observer, Allocator>;

        public:
            using tree_reference = tree &;
            using const_tree_reference = tree const &;
            using node_reference = node &;
            using const_node_reference = node const &;

            using payload_reference = payload &;
            using const_payload_reference = payload const &;

            using iterator = tree::iterator;
            using const_iterator = tree::const_iterator;
            using reverse_iterator = tree::reverse_iterator;
            using const_reverse_iterator = tree::const_reverse_iterator;

            using size_type = node::size_type;
            using difference_type = node::difference_type;

        private:
            tree_reference m_tree; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
            node_reference m_node; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)

        private:
            constexpr reference(tree_reference t, node_reference n) noexcept
                : m_tree{t}
                , m_node{n}
            {}

        public:
            // GCC >= 13.3 false-errors on a member named like the type (-Wchanges-meaning).
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wchanges-meaning"
#endif
            [[nodiscard]]
            constexpr tree_reference tree() noexcept
            {
                return m_tree;
            }

            [[nodiscard]]
            constexpr const_tree_reference tree() const noexcept
            {
                return m_tree;
            }
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif

            template <typename... Arguments>
            constexpr void set_value(Arguments &&... arguments) /**/
                noexcept(::std::is_nothrow_constructible_v<payload, Arguments...> &&
                    ::std::is_nothrow_move_assignable_v<payload> &&
                    noexcept(::std::declval<observer &>().on_change(::std::declval<const_payload_reference>(),
                        ::std::declval<const_payload_reference>())))
                requires(::std::constructible_from<payload, Arguments...>)
            {
                payload new_value{::std::forward<Arguments>(arguments)...};
                m_tree.notify_on_change(m_node.get(), new_value);
                m_node.get() = ::std::move(new_value);
            }

            [[nodiscard]]
            constexpr const_payload_reference value() const noexcept
            {
                return m_node.get();
            }

            [[nodiscard]]
            constexpr const_node_reference leaf() const noexcept
            {
                return m_node;
            }

            [[nodiscard]]
            constexpr node take(const_iterator position) noexcept
            {
                m_tree.notify_on_erase(position);
                return m_node.take(position.m_node_iter);
            }

            [[nodiscard]]
            constexpr node take_first() noexcept
            {
                return take(cbegin());
            }

            [[nodiscard]]
            constexpr node take_last() noexcept
            {
                return take(--cend());
            }

            [[nodiscard]]
            constexpr bool has_parent() const noexcept
            {
                return m_node.has_parent();
            }

            [[nodiscard]]
            constexpr reference parent() noexcept
            {
                return {m_tree, m_node.parent()};
            }

            [[nodiscard]]
            constexpr const_reference parent() const noexcept
            {
                return {m_tree, m_node.parent()};
            }

            [[nodiscard]]
            constexpr bool operator==(const_reference other) const noexcept;

            [[nodiscard]]
            constexpr bool operator!=(const_reference other) const noexcept
            {
                return !(*this == other);
            }

            [[nodiscard]]
            constexpr bool empty() const noexcept
            {
                return m_node.empty();
            }

            template <typename... Arguments>
            constexpr iterator emplace(const_iterator where, Arguments &&... arguments) /**/
                noexcept(noexcept(::std::declval<node_reference>().emplace(
                             ::std::declval<typename node::const_iterator>(), ::std::declval<Arguments>()...)) &&
                    noexcept(::std::declval<observer &>().on_insert(::std::declval<iterator>())))
                requires(::std::constructible_from<payload, Arguments...>)
            {
                iterator item{::std::addressof(m_tree),
                    m_node.emplace(where.m_node_iter, ::std::forward<Arguments>(arguments)...)};
                m_tree.notify_on_insert(item);
                return item;
            }

            template <typename Iterator>
            constexpr Iterator erase(Iterator position) noexcept
                requires(::std::same_as<Iterator, iterator> || ::std::same_as<Iterator, const_iterator>)
            {
                m_tree.notify_on_erase(position);
                return {::std::addressof(m_tree), m_node.erase(position.m_node_iter)};
            }

            template <typename Iterator>
            constexpr Iterator erase(Iterator first, Iterator last) noexcept
                requires(::std::same_as<Iterator, iterator> || ::std::same_as<Iterator, const_iterator>)
            {
                for (auto item = first; item != last; ++item)
                    m_tree.notify_on_erase(item);
                return {::std::addressof(m_tree), m_node.erase(first.m_node_iter, last.m_node_iter)};
            }

            constexpr void clear() noexcept
            {
                for (auto item = cbegin(); item != cend(); ++item)
                    m_tree.notify_on_erase(item);
                m_node.clear();
            }

            [[nodiscard]]
            constexpr size_type size() const noexcept
            {
                return m_node.size();
            }

            [[nodiscard]]
            constexpr reference front() noexcept
            {
                return {m_tree, m_node.front()};
            }

            [[nodiscard]]
            constexpr const_reference front() const noexcept
            {
                return {m_tree, m_node.front()};
            }

            [[nodiscard]]
            constexpr reference back() noexcept
            {
                return {m_tree, m_node.back()};
            }

            [[nodiscard]]
            constexpr const_reference back() const noexcept
            {
                return {m_tree, m_node.back()};
            }

            template <typename Argument>
            constexpr iterator push_back(Argument && argument) /**/
                noexcept(noexcept(emplace(::std::declval<const_iterator>(), ::std::forward<Argument>(argument))))
                requires(::std::constructible_from<payload, Argument>)
            {
                return emplace(cend(), ::std::forward<Argument>(argument));
            }

            template <typename... Arguments>
            constexpr iterator emplace_back(Arguments &&... arguments) /**/
                noexcept(noexcept(emplace(::std::declval<const_iterator>(), ::std::declval<Arguments>()...)))
                requires(::std::constructible_from<typename tree::payload, Arguments...>)
            {
                return emplace(cend(), ::std::forward<Arguments>(arguments)...);
            }

            template <typename Argument>
            constexpr iterator push_front(Argument && argument) /**/
                noexcept(noexcept(emplace(::std::declval<const_iterator>(), ::std::forward<Argument>(argument))))
                requires(::std::constructible_from<payload, Argument>)
            {
                return emplace(cbegin(), ::std::forward<Argument>(argument));
            }

            template <typename... Arguments>
            constexpr iterator emplace_front(Arguments &&... arguments) /**/
                noexcept(noexcept(emplace(::std::declval<const_iterator>(), ::std::declval<Arguments>()...)))
                requires(::std::constructible_from<typename tree::payload, Arguments...>)
            {
                return emplace(cbegin(), ::std::forward<Arguments>(arguments)...);
            }

            constexpr void pop_back() noexcept { erase(--cend()); }
            constexpr void pop_front() noexcept { erase(cbegin()); }

            template <typename Argument>
            constexpr iterator insert(const_iterator where, Argument && argument) /**/
                noexcept(noexcept(emplace(::std::declval<const_iterator>(), ::std::forward<Argument>(argument))))
                requires(::std::constructible_from<payload, Argument>)
            {
                return emplace(where, ::std::forward<Argument>(argument));
            }

            constexpr void transfer(reference from) noexcept { transfer(cend(), from); }

            constexpr void transfer(const_iterator where, reference from) noexcept
            {
                bool const cross = ::std::addressof(m_tree) != ::std::addressof(from.m_tree);
                size_type n = 0;
                if (cross)
                    for (const_iterator it = from.cbegin(); it != from.cend(); ++it, ++n)
                        from.m_tree.notify_on_erase(it);

                iterator to{::std::addressof(m_tree), from.begin().m_node_iter};
                m_node.transfer(where.m_node_iter, from.m_node);

                if (cross)
                    for (size_type i = 0; i < n; ++i)
                        m_tree.notify_on_insert(to++);
            }

            constexpr void transfer(reference from, const_iterator first) noexcept
            {
                transfer(cend(), from, first);
            }

            constexpr void transfer(const_iterator where, reference from, const_iterator first) noexcept
            {
                bool const cross = ::std::addressof(m_tree) != ::std::addressof(from.m_tree);
                if (cross)
                    from.m_tree.notify_on_erase(first);

                // erase(it, it) is a no-op that returns a mutable iterator to the same position
                iterator to{::std::addressof(m_tree), from.m_node.erase(first.m_node_iter, first.m_node_iter)};
                m_node.transfer(where.m_node_iter, from.m_node, first.m_node_iter);

                if (cross)
                    m_tree.notify_on_insert(to);
            }

            constexpr void transfer(reference from, const_iterator first, const_iterator last) noexcept
            {
                transfer(cend(), from, first, last);
            }

            constexpr void transfer(const_iterator where,
                reference from, /**/
                const_iterator first,
                const_iterator last) noexcept
            {
                bool const cross = ::std::addressof(m_tree) != ::std::addressof(from.m_tree);
                size_type n = 0;
                if (cross)
                    for (const_iterator it = first; it != last; ++it, ++n)
                        from.m_tree.notify_on_erase(it);

                // erase(it, it) is a no-op that returns a mutable iterator to the same position
                iterator to{::std::addressof(m_tree), from.m_node.erase(first.m_node_iter, first.m_node_iter)};
                m_node.transfer(where.m_node_iter, from.m_node, first.m_node_iter, last.m_node_iter);

                if (cross)
                    for (size_type i = 0; i < n; ++i)
                        m_tree.notify_on_insert(to++);
            }

            // Standard begin/end/cbegin/cend and reverse variants over direct children.

            [[nodiscard]]
            constexpr iterator begin() noexcept
            {
                return {::std::addressof(m_tree), m_node.begin()};
            }

            [[nodiscard]]
            constexpr const_iterator begin() const noexcept
            {
                return cbegin();
            }

            [[nodiscard]]
            constexpr const_iterator cbegin() const noexcept
            {
                return {::std::addressof(m_tree), m_node.cbegin()};
            }

            [[nodiscard]]
            constexpr iterator end() noexcept
            {
                return {::std::addressof(m_tree), m_node.end()};
            }

            [[nodiscard]]
            constexpr const_iterator end() const noexcept
            {
                return cend();
            }

            [[nodiscard]]
            constexpr const_iterator cend() const noexcept
            {
                return {::std::addressof(m_tree), m_node.cend()};
            }

            [[nodiscard]]
            constexpr reverse_iterator rbegin() noexcept
            {
                return reverse_iterator{end()};
            }

            [[nodiscard]]
            constexpr reverse_iterator rend() noexcept
            {
                return reverse_iterator{begin()};
            }

            [[nodiscard]]
            constexpr const_reverse_iterator rbegin() const noexcept
            {
                return const_reverse_iterator{cend()};
            }

            [[nodiscard]]
            constexpr const_reverse_iterator rend() const noexcept
            {
                return const_reverse_iterator{cbegin()};
            }

            [[nodiscard]]
            constexpr const_reverse_iterator crbegin() const noexcept
            {
                return const_reverse_iterator{cend()};
            }

            [[nodiscard]]
            constexpr const_reverse_iterator crend() const noexcept
            {
                return const_reverse_iterator{cbegin()};
            }
        };

        class const_reference
        {
            friend class ::scl::hierarchy::tree<Payload, Observer, Allocator>;

        public:
            using const_tree_reference = tree const &;
            using const_node_reference = node const &;

            using const_payload_reference = payload const &;

            using const_iterator = tree::const_iterator;
            using const_reverse_iterator = tree::const_reverse_iterator;

            using size_type = node::size_type;
            using difference_type = node::difference_type;

        private:
            const_tree_reference m_tree; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
            const_node_reference m_node; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)

        private:
            constexpr const_reference(const_tree_reference t, const_node_reference n) noexcept
                : m_tree{t}
                , m_node{n}
            {}

        public:
            // cppcheck-suppress noExplicitConstructor
            constexpr const_reference(reference other) noexcept
                : m_tree{other.m_tree}
                , m_node{other.m_node}
            {}

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wchanges-meaning" // see reference::tree()
#endif
            [[nodiscard]]
            constexpr const_tree_reference tree() const noexcept
            {
                return m_tree;
            }
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
            [[nodiscard]]
            constexpr const_payload_reference value() const noexcept
            {
                return m_node.get();
            }

            [[nodiscard]]
            constexpr bool has_parent() const noexcept
            {
                return m_node.has_parent();
            }

            [[nodiscard]]
            constexpr const_reference parent() const noexcept
            {
                return {m_tree, m_node.parent()};
            }

            [[nodiscard]]
            constexpr bool operator==(const_reference other) const noexcept
            {
                return ::scl::hierarchy::are_identical(m_node, other.m_node);
            }

            [[nodiscard]]
            constexpr bool operator!=(const_reference other) const noexcept
            {
                return !(*this == other);
            }

            [[nodiscard]]
            constexpr bool empty() const noexcept
            {
                return m_node.empty();
            }

            [[nodiscard]]
            constexpr size_type size() const noexcept
            {
                return m_node.size();
            }

            [[nodiscard]]
            constexpr const_node_reference leaf() const noexcept
            {
                return m_node;
            }

            [[nodiscard]]
            constexpr const_reference front() const noexcept
            {
                return {m_tree, m_node.front()};
            }

            [[nodiscard]]
            constexpr const_reference back() const noexcept
            {
                return {m_tree, m_node.back()};
            }

            // Standard begin/end/cbegin/cend and reverse variants over direct children.

            [[nodiscard]]
            constexpr const_iterator begin() const noexcept
            {
                return cbegin();
            }

            [[nodiscard]]
            constexpr const_iterator cbegin() const noexcept
            {
                return {::std::addressof(m_tree), m_node.cbegin()};
            }

            [[nodiscard]]
            constexpr const_iterator end() const noexcept
            {
                return cend();
            }

            [[nodiscard]]
            constexpr const_iterator cend() const noexcept
            {
                return {::std::addressof(m_tree), m_node.cend()};
            }

            [[nodiscard]]
            constexpr const_reverse_iterator rbegin() const noexcept
            {
                return const_reverse_iterator{cend()};
            }

            [[nodiscard]]
            constexpr const_reverse_iterator rend() const noexcept
            {
                return const_reverse_iterator{cbegin()};
            }

            [[nodiscard]]
            constexpr const_reverse_iterator crbegin() const noexcept
            {
                return const_reverse_iterator{cend()};
            }

            [[nodiscard]]
            constexpr const_reverse_iterator crend() const noexcept
            {
                return const_reverse_iterator{cbegin()};
            }
        };

    private:
        observer m_observer;
        nodes m_nodes;

    public:
        [[nodiscard]]
        constexpr observer_reference get_observer() noexcept
        {
            return m_observer;
        }

        [[nodiscard]]
        constexpr observer_const_reference get_observer() const noexcept
        {
            return m_observer;
        }

        [[nodiscard]]
        constexpr bool empty() const noexcept
        {
            return m_nodes.empty();
        }

        [[nodiscard]]
        constexpr size_type size() const noexcept
        {
            return m_nodes.size();
        }

        [[nodiscard]]
        constexpr reference front() noexcept
        {
            return *begin();
        }

        [[nodiscard]]
        constexpr const_reference front() const noexcept
        {
            return *cbegin();
        }

        [[nodiscard]]
        constexpr reference back() noexcept
        {
            return *--end();
        }

        [[nodiscard]]
        constexpr const_reference back() const noexcept
        {
            return *--cend();
        }

        template <typename Argument>
        constexpr iterator push_back(Argument && argument) /**/
            noexcept(noexcept(emplace(::std::declval<const_iterator>(), ::std::forward<Argument>(argument))))
            requires(::std::constructible_from<payload, Argument>)
        {
            return emplace(cend(), ::std::forward<Argument>(argument));
        }

        template <typename... Arguments>
        constexpr iterator emplace_back(Arguments &&... arguments) /**/
            noexcept(noexcept(emplace(::std::declval<const_iterator>(), ::std::declval<Arguments>()...)))
            requires(::std::constructible_from<payload, Arguments...>)
        {
            return emplace(cend(), ::std::forward<Arguments>(arguments)...);
        }

        template <typename Argument>
        constexpr iterator push_front(Argument && argument) /**/
            noexcept(noexcept(emplace(::std::declval<const_iterator>(), ::std::forward<Argument>(argument))))
            requires(::std::constructible_from<payload, Argument>)
        {
            return emplace(cbegin(), ::std::forward<Argument>(argument));
        }

        template <typename... Arguments>
        constexpr iterator emplace_front(Arguments &&... arguments) /**/
            noexcept(noexcept(emplace(::std::declval<const_iterator>(), ::std::declval<Arguments>()...)))
            requires(::std::constructible_from<payload, Arguments...>)
        {
            return emplace(cbegin(), ::std::forward<Arguments>(arguments)...);
        }

        constexpr void pop_back() noexcept { erase(--cend()); }
        constexpr void pop_front() noexcept { erase(cbegin()); }

        template <typename Argument>
        constexpr iterator insert(const_iterator where, Argument && argument) /**/
            noexcept(noexcept(emplace(::std::declval<const_iterator>(), ::std::forward<Argument>(argument))))
            requires(::std::constructible_from<payload, Argument>)
        {
            return emplace(where, ::std::forward<Argument>(argument));
        }

        template <typename... Arguments>
        constexpr iterator emplace(const_iterator where, Arguments &&... arguments) /**/
            noexcept(noexcept(::std::declval<nodes &>().emplace(
                         ::std::declval<typename nodes::const_iterator>(), ::std::declval<Arguments>()...)) &&
                noexcept(::std::declval<observer &>().on_insert(::std::declval<iterator>())))
            requires(::std::constructible_from<payload, Arguments...>)
        {
            iterator iter{this, m_nodes.emplace(where.m_node_iter, ::std::forward<Arguments>(arguments)...)};
            notify_on_insert(iter);
            return iter;
        }

        template <typename Iterator>
        constexpr Iterator erase(Iterator position) /**/
            noexcept(noexcept(notify_on_erase(::std::declval<const_iterator>())))
            requires(::std::same_as<Iterator, iterator> || ::std::same_as<Iterator, const_iterator>)
        {
            assert(!(*position).has_parent());
            notify_on_erase(position);
            return {this, m_nodes.erase(position.m_node_iter)};
        }

        template <typename Iterator>
        constexpr Iterator erase(Iterator first, Iterator last) /**/
            noexcept(noexcept(notify_on_erase(::std::declval<const_iterator>())))
            requires(::std::same_as<Iterator, iterator> || ::std::same_as<Iterator, const_iterator>)
        {
            assert(!(*first).has_parent());
            for (auto it = first; it != last; ++it)
                notify_on_erase(it);
            return {this, m_nodes.erase(first.m_node_iter, last.m_node_iter)};
        }

        constexpr void clear() noexcept
        {
            notify_on_clear();
            m_nodes.clear();
        }

        template <typename Iterator>
        constexpr void remove(Iterator position) /**/
            noexcept(noexcept(erase(::std::declval<Iterator>())))
            requires(::std::same_as<Iterator, iterator> || ::std::same_as<Iterator, const_iterator>)
        {
            assert(this == position.m_tree);
            reference nw{*this, const_cast<node &>(*position.m_node_iter)};
            if (nw.has_parent())
                nw.parent().erase(position);
            else
                erase(position);
        }

        constexpr void transfer(tree & from) noexcept { transfer(cend(), from); }

        constexpr void transfer(const_iterator where, tree & from) noexcept
        {
            bool const cross = this != &from;
            size_type n = 0;
            if (cross)
                for (const_iterator it = from.cbegin(); it != from.cend(); ++it, ++n)
                    from.notify_on_erase(it);

            iterator to{this, from.m_nodes.begin()};
            m_nodes.splice(where.m_node_iter, from.m_nodes);

            if (cross)
                for (size_type i = 0; i < n; ++i)
                    notify_on_insert(to++);
        }

        constexpr void transfer(tree & from, const_iterator first) noexcept
        {
            transfer(cend(), from, first);
        }

        constexpr void transfer(const_iterator where, tree & from, const_iterator first) noexcept
        {
            bool const cross = this != &from;
            if (cross)
                from.notify_on_erase(first);

            iterator to{this, first.m_node_iter};
            m_nodes.splice(where.m_node_iter, from.m_nodes, first.m_node_iter);

            if (cross)
                notify_on_insert(to);
        }

        constexpr void transfer(tree & from, const_iterator first, const_iterator last) noexcept
        {
            transfer(cend(), from, first, last);
        }

        constexpr void
        transfer(const_iterator where, tree & from, const_iterator first, const_iterator last) noexcept
        {
            bool const cross = this != &from;
            size_type n = 0;
            if (cross)
                for (const_iterator it = first; it != last; ++it, ++n)
                    from.notify_on_erase(it);

            iterator to{this, first.m_node_iter};
            m_nodes.splice(where.m_node_iter, from.m_nodes, first.m_node_iter, last.m_node_iter);

            if (cross)
                for (size_type i = 0; i < n; ++i)
                    notify_on_insert(to++);
        }

        // Standard begin/end/cbegin/cend and reverse variants over root nodes.

        [[nodiscard]]
        constexpr iterator begin() noexcept
        {
            return {this, m_nodes.begin()};
        }

        [[nodiscard]]
        constexpr const_iterator begin() const noexcept
        {
            return cbegin();
        }

        [[nodiscard]]
        constexpr const_iterator cbegin() const noexcept
        {
            return {this, m_nodes.cbegin()};
        }

        [[nodiscard]]
        constexpr iterator end() noexcept
        {
            return {this, m_nodes.end()};
        }

        [[nodiscard]]
        constexpr const_iterator end() const noexcept
        {
            return cend();
        }

        [[nodiscard]]
        constexpr const_iterator cend() const noexcept
        {
            return {this, m_nodes.cend()};
        }

        [[nodiscard]]
        constexpr reverse_iterator rbegin() noexcept
        {
            return reverse_iterator{end()};
        }

        [[nodiscard]]
        constexpr reverse_iterator rend() noexcept
        {
            return reverse_iterator{begin()};
        }

        [[nodiscard]]
        constexpr const_reverse_iterator rbegin() const noexcept
        {
            return const_reverse_iterator{cend()};
        }

        [[nodiscard]]
        constexpr const_reverse_iterator rend() const noexcept
        {
            return const_reverse_iterator{cbegin()};
        }

        [[nodiscard]]
        constexpr const_reverse_iterator crbegin() const noexcept
        {
            return const_reverse_iterator{cend()};
        }

        [[nodiscard]]
        constexpr const_reverse_iterator crend() const noexcept
        {
            return const_reverse_iterator{cbegin()};
        }

    private:
        constexpr void notify_on_insert(iterator iter) /**/
            noexcept(noexcept(m_observer.on_insert(iter)))
        {
            m_observer.on_insert(iter);
            reference nw = *iter;
            for (auto sub = nw.begin(); sub != nw.end(); ++sub)
                notify_on_insert(sub);
        }

        constexpr void notify_on_erase(const_iterator iter) /**/
            noexcept(noexcept(m_observer.on_erase(iter)))
        {
            const_reference nw = *iter;
            for (auto sub = nw.begin(); sub != nw.end(); ++sub)
                notify_on_erase(sub);
            m_observer.on_erase(iter);
        }

        constexpr void notify_on_clear() /**/
            noexcept(noexcept(m_observer.on_clear()))
        {
            m_observer.on_clear();
        }

        constexpr void
        notify_on_change(const_payload_reference current, const_payload_reference next) /**/
            noexcept(noexcept(m_observer.on_change(current, next)))
        {
            m_observer.on_change(current, next);
        }

        static_assert(::scl::hierarchy::concepts::observer<observer, tree>,
            "Observer<tree> must satisfy scl::hierarchy::concepts::observer "
            "(on_insert / on_erase / on_clear / on_change)");
    };

    // Out-of-line definition for reference::operator== (needs const_reference complete)
    template <typename Payload, template <typename> typename Observer, template <typename> typename Allocator>
    constexpr bool tree<Payload, Observer, Allocator>::reference::operator==(/**/
        const_reference other) const noexcept
    {
        return ::scl::hierarchy::are_identical(m_node, other.m_node);
    }

    template <concepts::node_proxy Ref>
    [[nodiscard]]
    constexpr Ref adl_parent(Ref self) /**/
        noexcept(noexcept(self.parent()))
    {
        return self.parent();
    }

    template <concepts::node_proxy Ref>
    [[nodiscard]]
    constexpr bool adl_has_parent(Ref self) /**/
        noexcept(noexcept(self.has_parent()))
    {
        return self.has_parent();
    }

    template <concepts::node_proxy Ref>
    [[nodiscard]]
    constexpr identity adl_identity(Ref self) noexcept
    {
        return identity{self.leaf()};
    }

} // namespace scl::hierarchy

// =============================================================================
// scl::hierarchy::tree — Doxygen documentation
// =============================================================================

// -----------------------------------------------------------------------------
// tree — class overview
// -----------------------------------------------------------------------------

/**
 * @class scl::hierarchy::tree
 * @ingroup scl_utility_hierarchy
 * @brief Owns a root-level list of `node<Payload, Allocator>` instances and
 *        routes every structural or payload mutation through an `Observer` policy.
 *
 * "Multi-root" means exactly this: the tree holds a *list* of root nodes (a
 * forest), not a single mandatory root — `push_back`/`push_front` each add an
 * independent root subtree.
 *
 * Nodes are never accessed directly; every access goes through a `reference`
 * (mutable) or `const_reference` (read-only) proxy, which pairs a reference to
 * the tree with a reference to a node — so a node cannot be mutated without the
 * tree observing it. Composing several observers into one is `observer_tuple`
 * (`observer_tuple.h`).
 *
 * The `Observer` template parameter is checked against
 * @ref scl::hierarchy::concepts::observer at instantiation — a non-conforming
 * type fails to compile with a clear message, not a wall of substitution errors.
 * A conforming observer implements:
 * @code
 * template <typename Tree>
 * struct my_observer {
 *     void on_insert(Tree::iterator);              // after a node is inserted
 *     void on_erase (Tree::const_iterator);        // before a node is erased
 *     void on_clear ();                                     // before the tree is cleared
 *     void on_change(Tree::const_payload_reference old_value,
 *                    Tree::const_payload_reference new_value);
 * };
 * @endcode
 * `on_insert` fires top-down (a parent before its children, when a subtree is
 * inserted); `on_erase` fires bottom-up (the deepest descendant first).
 *
 * @tparam Payload   Payload type stored in each node.
 * @tparam Observer  Observer policy template instantiated as `Observer<tree>`.
 * @tparam Allocator Allocator template forwarded to the underlying node list.
 *
 * @par A simple observer
 * @code
 * template <typename Tree>
 * struct logger {
 *     void on_insert(Tree::iterator it)
 *     { std::cout << "insert: " << (*it).value() << '\n'; }
 *     void on_erase(Tree::const_iterator it)
 *     { std::cout << "erase: " << (*it).value() << '\n'; }
 *     void on_clear() { std::cout << "clear\n"; }
 *     void on_change(int old_v, int new_v)
 *     { std::cout << "change: " << old_v << " -> " << new_v << '\n'; }
 * };
 *
 * scl::hierarchy::tree<int, logger> t;
 * auto root = t.push_back(1);    // insert: 1
 * (*root).push_back(2);          // insert: 2
 * (*root).set_value(10);         // change: 1 -> 10
 * t.clear();                     // clear
 * @endcode
 *
 * @par Querying the hierarchy through a reference
 * The free algorithm functions in `algorithm.h` find `reference`/`const_reference`
 * via ADL — call them directly, no unwrapping needed:
 * @code
 * scl::hierarchy::tree<std::string, logger> t;
 * auto root  = t.push_back("root");
 * auto child = (*root).push_back("child");
 * auto grand = (*child).push_back("grand");
 *
 * assert(scl::hierarchy::is_ancestor_of(*root, *grand));
 * assert(scl::hierarchy::are_sibling(*child, *child));
 * @endcode
 *
 * @par remove() detaches by identity, from parent or root list
 * @code
 * scl::hierarchy::tree<int, logger> t;
 * auto root  = t.push_back(1);          // insert: 1
 * auto child = (*root).push_back(2);    // insert: 2
 *
 * t.remove(child);    // child has a parent -> erased through (*root), not through t
 * assert((*root).empty());
 *
 * t.remove(root);      // root has no parent -> erased from the root list directly
 * assert(t.empty());
 * @endcode
 *
 * @note A cross-tree `transfer()` fires `on_erase` on the source tree and
 *       `on_insert` on the destination tree.
 * @note `reference`/`const_reference` are proxies, not owning handles — they
 *       stay valid exactly as long as the underlying node does.
 */

// -----------------------------------------------------------------------------
// tree — type aliases
// -----------------------------------------------------------------------------

/**
 * @typedef scl::hierarchy::tree::payload
 * @brief Payload type stored in each node (same as template parameter `Payload`).
 */

/**
 * @typedef scl::hierarchy::tree::payload_reference
 * @brief Mutable reference to a stored payload (`payload &`).
 */

/**
 * @typedef scl::hierarchy::tree::const_payload_reference
 * @brief Immutable reference to a stored payload (`payload const &`).
 */

/**
 * @typedef scl::hierarchy::tree::observer
 * @brief The observer type, `Observer<tree>`.
 */

/**
 * @typedef scl::hierarchy::tree::observer_reference
 * @brief Mutable reference to the observer (`observer &`).
 */

/**
 * @typedef scl::hierarchy::tree::observer_const_reference
 * @brief Immutable reference to the observer (`observer const &`).
 */

/**
 * @typedef scl::hierarchy::tree::node
 * @brief The underlying `scl::hierarchy::node<Payload, Allocator>` type.
 */

/**
 * @typedef scl::hierarchy::tree::nodes
 * @brief The node container backing the root list, taken from `node`.
 */

/**
 * @typedef scl::hierarchy::tree::pointer
 * @brief `void`: an element is reached through a transient `reference` proxy,
 *        so there is no pointer to it to hand out.
 */

/**
 * @typedef scl::hierarchy::tree::const_pointer
 * @brief `void`, for the same reason as @ref scl::hierarchy::tree::pointer.
 */

/**
 * @typedef scl::hierarchy::tree::size_type
 * @brief Unsigned integer type for element counts, taken from `nodes`.
 */

/**
 * @typedef scl::hierarchy::tree::difference_type
 * @brief Signed integer type for iterator distances, taken from `nodes`.
 */

/**
 * @typedef scl::hierarchy::tree::value_type
 * @brief STL element type of the root list — `reference`, not `payload`.
 */

/**
 * @typedef scl::hierarchy::tree::allocator_type
 * @brief Allocator the root list was built with, taken from `nodes`.
 */

/**
 * @typedef scl::hierarchy::tree::reverse_iterator
 * @brief Mutable reverse iterator over the root list.
 */

/**
 * @typedef scl::hierarchy::tree::const_reverse_iterator
 * @brief Immutable reverse iterator over the root list.
 */

// -----------------------------------------------------------------------------
// tree — observer access
// -----------------------------------------------------------------------------

/**
 * @fn scl::hierarchy::tree::get_observer()
 * @brief Returns a mutable reference to the observer.
 */

/**
 * @fn scl::hierarchy::tree::get_observer() const
 * @brief Returns an immutable reference to the observer.
 */

// -----------------------------------------------------------------------------
// tree — root queries
// -----------------------------------------------------------------------------

/**
 * @fn scl::hierarchy::tree::empty() const
 * @brief Returns `true` when the tree has no root nodes.
 */

/**
 * @fn scl::hierarchy::tree::size() const
 * @brief Returns the number of root nodes.
 */

/**
 * @fn scl::hierarchy::tree::front()
 * @brief Returns a proxy over the first root node.
 * @pre `!empty()`
 */

/**
 * @fn scl::hierarchy::tree::front() const
 * @brief Returns a proxy over the first root node.
 * @pre `!empty()`
 */

/**
 * @fn scl::hierarchy::tree::back()
 * @brief Returns a proxy over the last root node.
 * @pre `!empty()`
 */

/**
 * @fn scl::hierarchy::tree::back() const
 * @brief Returns a proxy over the last root node.
 * @pre `!empty()`
 */

// -----------------------------------------------------------------------------
// tree — root insertion
// -----------------------------------------------------------------------------

/**
 * @fn scl::hierarchy::tree::push_back(Argument &&)
 * @brief Appends a new root at the back.
 * @return Iterator to the appended root; notifies the observer (`on_insert`).
 */

/**
 * @fn scl::hierarchy::tree::emplace_back(Arguments &&...)
 * @brief Constructs a root payload in-place at the back.
 * @return Iterator to the newly constructed root; notifies the observer (`on_insert`).
 */

/**
 * @fn scl::hierarchy::tree::push_front(Argument &&)
 * @brief Prepends a new root at the front.
 * @return Iterator to the prepended root; notifies the observer (`on_insert`).
 */

/**
 * @fn scl::hierarchy::tree::emplace_front(Arguments &&...)
 * @brief Constructs a root payload in-place at the front.
 * @return Iterator to the newly constructed root; notifies the observer (`on_insert`).
 */

/**
 * @fn scl::hierarchy::tree::insert(const_iterator, Argument &&)
 * @brief Inserts a new root at @p where.
 * @return Iterator to the inserted root; notifies the observer (`on_insert`).
 */

/**
 * @fn scl::hierarchy::tree::emplace(const_iterator, Arguments &&...)
 * @brief Constructs a root payload in-place at @p where.
 * @return Iterator to the newly constructed root; notifies the observer (`on_insert`).
 */

// -----------------------------------------------------------------------------
// tree — root removal
// -----------------------------------------------------------------------------

/**
 * @fn scl::hierarchy::tree::pop_back()
 * @brief Removes the last root.
 * @pre `!empty()`
 */

/**
 * @fn scl::hierarchy::tree::pop_front()
 * @brief Removes the first root.
 * @pre `!empty()`
 */

/**
 * @fn scl::hierarchy::tree::erase(Iterator)
 * @brief Erases the root at @p position; notifies the observer (`on_erase`,
 *        bottom-up).
 * @pre    @p position has no parent (it is a root).
 * @return Iterator to the root that followed the erased one, or `end()`.
 */

/**
 * @fn scl::hierarchy::tree::erase(Iterator, Iterator)
 * @brief Erases roots in [@p first, @p last); notifies the observer (`on_erase`,
 *        bottom-up, once per erased node).
 * @pre    @p first has no parent (it is a root).
 * @return Iterator to the root that followed the last erased one, or `end()`.
 */

/**
 * @fn scl::hierarchy::tree::clear()
 * @brief Removes all root nodes and their subtrees; notifies the observer
 *        (`on_clear`) — unlike `reference::clear()`, does not fire `on_erase`
 *        per node.
 * @post `empty()` returns `true`.
 */

/**
 * @fn scl::hierarchy::tree::remove(Iterator)
 * @brief Removes the node at @p position from its parent or the root list —
 *        whichever it currently lives in; the caller doesn't need to know which.
 */

// -----------------------------------------------------------------------------
// tree — transfer (O(1) splice, root level)
// -----------------------------------------------------------------------------

/**
 * @fn scl::hierarchy::tree::transfer(tree &)
 * @brief Moves all roots of @p from to the back of this tree's root list. O(1).
 *        Cross-tree: fires `on_erase` on the source and `on_insert` on the
 *        destination, once per moved node.
 */

/**
 * @fn scl::hierarchy::tree::transfer(const_iterator, tree &)
 * @brief Moves all roots of @p from to where @p where. O(1).
 *        Cross-tree: fires `on_erase` on the source and `on_insert` on the
 *        destination, once per moved node.
 */

/**
 * @fn scl::hierarchy::tree::transfer(tree &, const_iterator)
 * @brief Moves the single root at @p first in @p from to the back of this
 *        tree's root list. O(1). Cross-tree: fires `on_erase` on the source and
 *        `on_insert` on the destination.
 */

/**
 * @fn scl::hierarchy::tree::transfer(const_iterator, tree &, const_iterator)
 * @brief Moves the single root at @p first in @p from to where @p where. O(1).
 *        Cross-tree: fires `on_erase` on the source and `on_insert` on the
 *        destination.
 */

/**
 * @fn scl::hierarchy::tree::transfer(tree &, const_iterator, const_iterator)
 * @brief Moves roots in [@p first, @p last) from @p from to the back of this
 *        tree's root list. O(distance). Cross-tree: fires `on_erase` on the
 *        source and `on_insert` on the destination, once per moved node.
 */

/**
 * @fn scl::hierarchy::tree::transfer(const_iterator, tree &, const_iterator, const_iterator)
 * @brief Moves roots in [@p first, @p last) from @p from to where @p where.
 *        O(distance). Cross-tree: fires `on_erase` on the source and
 *        `on_insert` on the destination, once per moved node.
 */

/**
 * @par Transfer between references
 * @code
 * scl::hierarchy::tree<int, logger> t;
 * auto r1 = t.push_back(0);
 * auto r2 = t.push_back(0);
 * (*r1).push_back(1);
 * (*r1).push_back(2);
 *
 * (*r2).transfer(*r1);  // move all children of r1 into r2
 * assert((*r1).empty());
 * assert((*r2).size() == 2);
 * @endcode
 */

// -----------------------------------------------------------------------------
// tree — iteration
// -----------------------------------------------------------------------------

/**
 * @fn scl::hierarchy::tree::begin()
 * @brief Returns a mutable iterator to the first root node.
 */

/**
 * @fn scl::hierarchy::tree::end()
 * @brief Returns a mutable past-the-end sentinel for the root list.
 */

/**
 * @fn scl::hierarchy::tree::begin() const
 * @brief Returns an immutable iterator to the first root node.
 */

/**
 * @fn scl::hierarchy::tree::end() const
 * @brief Returns an immutable past-the-end sentinel for the root list.
 */

/**
 * @fn scl::hierarchy::tree::cbegin() const
 * @brief Returns an immutable iterator to the first root node.
 */

/**
 * @fn scl::hierarchy::tree::cend() const
 * @brief Returns an immutable past-the-end sentinel for the root list.
 */

/**
 * @fn scl::hierarchy::tree::rbegin()
 * @brief Returns a mutable reverse iterator to the last root node.
 */

/**
 * @fn scl::hierarchy::tree::rend()
 * @brief Returns a mutable reverse past-the-end sentinel for the root list.
 */

/**
 * @fn scl::hierarchy::tree::rbegin() const
 * @brief Returns an immutable reverse iterator to the last root node.
 */

/**
 * @fn scl::hierarchy::tree::rend() const
 * @brief Returns an immutable reverse past-the-end sentinel for the root list.
 */

/**
 * @fn scl::hierarchy::tree::crbegin() const
 * @brief Returns an immutable reverse iterator to the last root node.
 */

/**
 * @fn scl::hierarchy::tree::crend() const
 * @brief Returns an immutable reverse past-the-end sentinel for the root list.
 */

// -----------------------------------------------------------------------------
// tree::iterator / tree::const_iterator
// -----------------------------------------------------------------------------

/**
 * @class scl::hierarchy::tree::iterator
 * @brief Mutable bidirectional iterator over sibling nodes. Dereferences to a
 *        `reference` proxy, not the underlying node.
 */

/**
 * @typedef scl::hierarchy::tree::iterator::iterator_concept
 * @brief `std::bidirectional_iterator_tag` — the concept this iterator models.
 */

/**
 * @typedef scl::hierarchy::tree::iterator::iterator_category
 * @brief `std::bidirectional_iterator_tag`, for pre-C++20 algorithm dispatch.
 */

/**
 * @typedef scl::hierarchy::tree::iterator::value_type
 * @brief What dereferencing yields — a `tree::reference` proxy, not a node.
 */

/**
 * @typedef scl::hierarchy::tree::iterator::difference_type
 * @brief Signed integer type for iterator distances.
 */

/**
 * @typedef scl::hierarchy::tree::iterator::pointer
 * @brief What `operator->` hands back: a wrapper holding the proxy for the
 *        duration of the access, there being no pointer into the sequence to
 *        name. Naming it here is what lets a reverse iterator offer `->` too.
 */

/**
 * @typedef scl::hierarchy::tree::iterator::reference
 * @brief The proxy `operator*` returns — `tree::reference`, by value.
 */

/**
 * @typedef scl::hierarchy::tree::iterator::const_reference
 * @brief The read-only proxy over the same node — `tree::const_reference`.
 */

/**
 * @fn scl::hierarchy::tree::iterator::iterator()
 * @brief Constructs a singular iterator belonging to no tree, as the forward
 *        iterator requirements demand. Comparable, but not dereferenceable.
 */

/**
 * @fn scl::hierarchy::tree::iterator::operator==(iterator const & other) const
 * @brief Reports whether both iterators denote the same position.
 * @param  other  Iterator to compare against; must belong to the same tree
 *                unless either side is singular.
 * @return `true` when the two denote the same position.
 */

/**
 * @fn scl::hierarchy::tree::iterator::operator!=(iterator const & other) const
 * @brief Negation of @ref scl::hierarchy::tree::iterator::operator==.
 * @param  other  Iterator to compare against.
 * @return `true` when the two denote different positions.
 */

/**
 * @fn scl::hierarchy::tree::iterator::operator++()
 * @brief Advances to the next sibling.
 * @return `*this`, advanced.
 */

/**
 * @fn scl::hierarchy::tree::iterator::operator--()
 * @brief Steps back to the previous sibling.
 * @return `*this`, stepped back.
 */

/**
 * @fn scl::hierarchy::tree::iterator::operator++(int)
 * @brief Advances to the next sibling, returning the position left behind.
 * @return A copy denoting the position before the advance.
 */

/**
 * @fn scl::hierarchy::tree::iterator::operator--(int)
 * @brief Steps back to the previous sibling, returning the position left behind.
 * @return A copy denoting the position before the step.
 */

/**
 * @fn scl::hierarchy::tree::iterator::operator*() const
 * @brief Returns a proxy over the node at the current position.
 * @return A `reference` proxy, by value — it denotes the node, it is not one.
 */

/**
 * @fn scl::hierarchy::tree::iterator::operator->() const
 * @brief Reaches the members of the node at the current position.
 * @return A wrapper holding the proxy, so it outlives the member access made
 *         through it.
 */

/**
 * @class scl::hierarchy::tree::const_iterator
 * @brief Immutable bidirectional iterator over sibling nodes. Dereferences to a
 *        `const_reference` proxy. Implicitly constructible from `iterator`.
 */

/**
 * @typedef scl::hierarchy::tree::const_iterator::iterator_concept
 * @brief `std::bidirectional_iterator_tag` — the concept this iterator models.
 */

/**
 * @typedef scl::hierarchy::tree::const_iterator::iterator_category
 * @brief `std::bidirectional_iterator_tag`, for pre-C++20 algorithm dispatch.
 */

/**
 * @typedef scl::hierarchy::tree::const_iterator::value_type
 * @brief What dereferencing yields — a proxy, not a node.
 */

/**
 * @typedef scl::hierarchy::tree::const_iterator::difference_type
 * @brief Signed integer type for iterator distances.
 */

/**
 * @typedef scl::hierarchy::tree::const_iterator::pointer
 * @brief The wrapper `operator->` hands back, as
 *        @ref scl::hierarchy::tree::iterator::pointer describes.
 */

/**
 * @typedef scl::hierarchy::tree::const_iterator::reference
 * @brief The immutable proxy `operator*` yields, the same type as
 *        `const_reference`.
 */

/**
 * @typedef scl::hierarchy::tree::const_iterator::const_reference
 * @brief The proxy `operator*` returns — `tree::const_reference`, by value.
 */

/**
 * @fn scl::hierarchy::tree::const_iterator::const_iterator()
 * @brief Constructs a singular iterator belonging to no tree, as the forward
 *        iterator requirements demand. Comparable, but not dereferenceable.
 */

/**
 * @fn scl::hierarchy::tree::const_iterator::const_iterator(iterator const & other)
 * @brief Converts a mutable iterator to an immutable one over the same position.
 *
 * Implicit, so a mutable iterator is accepted wherever an immutable one is
 * expected — the conversion only ever narrows access.
 *
 * @param other  Iterator to convert.
 */

/**
 * @fn scl::hierarchy::tree::const_iterator::const_iterator(const_iterator const &)
 * @brief Copies the position; the copy denotes the same node.
 */

/**
 * @fn scl::hierarchy::tree::const_iterator::const_iterator(const_iterator &&)
 * @brief Same as the copy constructor: the position is two pointers, so moving
 *        it copies it and leaves the source usable.
 */

/**
 * @fn scl::hierarchy::tree::const_iterator::~const_iterator()
 * @brief Trivial: an iterator owns neither the tree nor the node it denotes.
 */

/**
 * @fn scl::hierarchy::tree::const_iterator::operator=(const_iterator const &)
 * @brief Rebinds this iterator to the position @p other denotes.
 * @return `*this`.
 */

/**
 * @fn scl::hierarchy::tree::const_iterator::operator=(const_iterator &&)
 * @brief Same as copy assignment.
 * @return `*this`.
 */

/**
 * @fn scl::hierarchy::tree::const_iterator::operator=(iterator const & other)
 * @brief Rebinds this iterator to the position a mutable iterator denotes.
 * @param  other  Iterator to take the position from.
 * @return `*this`.
 */

/**
 * @fn scl::hierarchy::tree::const_iterator::operator==(const_iterator const & other) const
 * @brief Reports whether both iterators denote the same position.
 * @param  other  Iterator to compare against; must belong to the same tree
 *                unless either side is singular.
 * @return `true` when the two denote the same position.
 */

/**
 * @fn scl::hierarchy::tree::const_iterator::operator!=(const_iterator const & other) const
 * @brief Negation of @ref scl::hierarchy::tree::const_iterator::operator==.
 * @param  other  Iterator to compare against.
 * @return `true` when the two denote different positions.
 */

/**
 * @fn scl::hierarchy::tree::const_iterator::operator++()
 * @brief Advances to the next sibling.
 * @return `*this`, advanced.
 */

/**
 * @fn scl::hierarchy::tree::const_iterator::operator--()
 * @brief Steps back to the previous sibling.
 * @return `*this`, stepped back.
 */

/**
 * @fn scl::hierarchy::tree::const_iterator::operator++(int)
 * @brief Advances to the next sibling, returning the position left behind.
 * @return A copy denoting the position before the advance.
 */

/**
 * @fn scl::hierarchy::tree::const_iterator::operator--(int)
 * @brief Steps back to the previous sibling, returning the position left behind.
 * @return A copy denoting the position before the step.
 */

/**
 * @fn scl::hierarchy::tree::const_iterator::operator*() const
 * @brief Returns a read-only proxy over the node at the current position.
 * @return A `const_reference` proxy, by value.
 */

/**
 * @fn scl::hierarchy::tree::const_iterator::operator->() const
 * @brief Reaches the members of the node at the current position.
 * @return A wrapper holding the proxy, so it outlives the member access made
 *         through it.
 */

// -----------------------------------------------------------------------------
// tree::reference — class overview
// -----------------------------------------------------------------------------

/**
 * @class scl::hierarchy::tree::reference
 * @brief Mutable proxy over a tree node — pairs a reference to the owning
 *        `tree` with a reference to the underlying `node`, so every mutation
 *        routes through the tree's observer.
 *
 * Never constructed directly: obtained by dereferencing a `tree::iterator`, or
 * returned from a `tree`/`reference` insertion method. Mirrors `node`'s full
 * child-management API (`push_back`, `emplace`, `erase`, `take`, `transfer`,
 * iterators, …), plus `set_value()` (which fires `on_change`) and `leaf()`
 * (the underlying `node`, usable with the free algorithm functions in
 * `algorithm.h` without unwrapping — see `adl_parent`/`adl_has_parent`/
 * `adl_identity` above).
 *
 * @par Extracting and re-inserting a subtree
 * @code
 * scl::hierarchy::tree<std::string, logger> t;
 * auto world = t.push_back("World");
 * auto model = (*world).push_back("Model1");
 *
 * auto raw = (*world).take(model);        // detach; fires on_erase
 * std::cout << "Taken: " << raw.get() << '\n';
 * auto reinserted = (*world).push_back(raw.get());   // fires on_insert
 * @endcode
 */

// -----------------------------------------------------------------------------
// tree::reference — type aliases
// -----------------------------------------------------------------------------

/**
 * @typedef scl::hierarchy::tree::reference::tree_reference
 * @brief Mutable reference to the owning tree (`tree &`).
 */

/**
 * @typedef scl::hierarchy::tree::reference::const_tree_reference
 * @brief Immutable reference to the owning tree (`tree const &`).
 */

/**
 * @typedef scl::hierarchy::tree::reference::node_reference
 * @brief Mutable reference to the underlying `node` (`node &`).
 */

/**
 * @typedef scl::hierarchy::tree::reference::const_node_reference
 * @brief Immutable reference to the underlying `node` (`node const &`).
 */

/**
 * @typedef scl::hierarchy::tree::reference::payload_reference
 * @brief Mutable reference to the node's payload (`payload &`).
 */

/**
 * @typedef scl::hierarchy::tree::reference::const_payload_reference
 * @brief Immutable reference to the node's payload (`payload const &`).
 */

/**
 * @typedef scl::hierarchy::tree::reference::iterator
 * @brief Mutable iterator over this node's direct children.
 */

/**
 * @typedef scl::hierarchy::tree::reference::const_iterator
 * @brief Immutable iterator over this node's direct children.
 */

/**
 * @typedef scl::hierarchy::tree::reference::reverse_iterator
 * @brief Mutable reverse iterator over this node's direct children.
 */

/**
 * @typedef scl::hierarchy::tree::reference::const_reverse_iterator
 * @brief Immutable reverse iterator over this node's direct children.
 */

/**
 * @typedef scl::hierarchy::tree::reference::size_type
 * @brief Unsigned integer type for child counts.
 */

/**
 * @typedef scl::hierarchy::tree::reference::difference_type
 * @brief Signed integer type for iterator distances.
 */

// -----------------------------------------------------------------------------
// tree::reference — identity and payload
// -----------------------------------------------------------------------------

/**
 * @fn scl::hierarchy::tree::reference::tree()
 * @brief Returns the tree this node belongs to.
 */

/**
 * @fn scl::hierarchy::tree::reference::tree() const
 * @brief Returns the tree this node belongs to, immutably.
 */

/**
 * @fn scl::hierarchy::tree::reference::set_value(Arguments &&...)
 * @brief Replaces the payload and notifies the observer (`on_change`).
 */

/**
 * @fn scl::hierarchy::tree::reference::value() const
 * @brief Returns a const reference to the node's payload.
 */

/**
 * @fn scl::hierarchy::tree::reference::leaf() const
 * @brief Returns the underlying node this proxy wraps.
 */

/**
 * @fn scl::hierarchy::tree::reference::operator==(const_reference) const
 * @brief Returns `true` when @p other denotes the same node — identity, not value.
 */

/**
 * @fn scl::hierarchy::tree::reference::operator!=(const_reference) const
 * @brief Negation of @ref scl::hierarchy::tree::reference::operator==, spelled
 *        out because the proxies compare across their mutable and immutable
 *        forms rather than only against their own type.
 */

// -----------------------------------------------------------------------------
// tree::reference — parent access
// -----------------------------------------------------------------------------

/**
 * @fn scl::hierarchy::tree::reference::has_parent() const
 * @brief Returns `true` when this node is attached to a parent.
 */

/**
 * @fn scl::hierarchy::tree::reference::parent()
 * @brief Returns a proxy over the parent node.
 * @pre `has_parent()`
 */

/**
 * @fn scl::hierarchy::tree::reference::parent() const
 * @brief Returns an immutable proxy over the parent node.
 * @pre `has_parent()`
 */

// -----------------------------------------------------------------------------
// tree::reference — child queries
// -----------------------------------------------------------------------------

/**
 * @fn scl::hierarchy::tree::reference::empty() const
 * @brief Returns `true` when this node has no direct children.
 */

/**
 * @fn scl::hierarchy::tree::reference::size() const
 * @brief Returns the number of direct children.
 */

/**
 * @fn scl::hierarchy::tree::reference::front()
 * @brief Returns a proxy over the first direct child.
 * @pre `!empty()`
 */

/**
 * @fn scl::hierarchy::tree::reference::front() const
 * @brief Returns an immutable proxy over the first direct child.
 * @pre `!empty()`
 */

/**
 * @fn scl::hierarchy::tree::reference::back()
 * @brief Returns a proxy over the last direct child.
 * @pre `!empty()`
 */

/**
 * @fn scl::hierarchy::tree::reference::back() const
 * @brief Returns an immutable proxy over the last direct child.
 * @pre `!empty()`
 */

// -----------------------------------------------------------------------------
// tree::reference — insertion
// -----------------------------------------------------------------------------

/**
 * @fn scl::hierarchy::tree::reference::emplace(const_iterator, Arguments &&...)
 * @brief Constructs a child in-place at @p where.
 * @return Iterator to the newly constructed child; notifies the observer
 *         (`on_insert`, top-down when the child brings its own subtree).
 */

/**
 * @fn scl::hierarchy::tree::reference::push_back(Argument &&)
 * @brief Appends a child at the back.
 * @return Iterator to the appended child; notifies the observer (`on_insert`).
 */

/**
 * @fn scl::hierarchy::tree::reference::emplace_back(Arguments &&...)
 * @brief Constructs a child payload in-place at the back.
 * @return Iterator to the newly constructed child; notifies the observer (`on_insert`).
 */

/**
 * @fn scl::hierarchy::tree::reference::push_front(Argument &&)
 * @brief Prepends a child at the front.
 * @return Iterator to the prepended child; notifies the observer (`on_insert`).
 */

/**
 * @fn scl::hierarchy::tree::reference::emplace_front(Arguments &&...)
 * @brief Constructs a child payload in-place at the front.
 * @return Iterator to the newly constructed child; notifies the observer (`on_insert`).
 */

/**
 * @fn scl::hierarchy::tree::reference::insert(const_iterator, Argument &&)
 * @brief Inserts a child at @p where.
 * @return Iterator to the inserted child; notifies the observer (`on_insert`).
 */

// -----------------------------------------------------------------------------
// tree::reference — removal and extraction
// -----------------------------------------------------------------------------

/**
 * @fn scl::hierarchy::tree::reference::pop_back()
 * @brief Removes the last child.
 * @pre `!empty()`
 */

/**
 * @fn scl::hierarchy::tree::reference::pop_front()
 * @brief Removes the first child.
 * @pre `!empty()`
 */

/**
 * @fn scl::hierarchy::tree::reference::erase(Iterator)
 * @brief Erases the child at @p position; notifies the observer (`on_erase`,
 *        bottom-up).
 * @return Iterator to the child that followed the erased one, or `end()`.
 */

/**
 * @fn scl::hierarchy::tree::reference::erase(Iterator, Iterator)
 * @brief Erases children in [@p first, @p last); notifies the observer
 *        (`on_erase`, bottom-up, once per erased node).
 * @return Iterator to the child that followed the last erased one, or `end()`.
 */

/**
 * @fn scl::hierarchy::tree::reference::clear()
 * @brief Removes all direct children; notifies the observer once per child
 *        (`on_erase`, bottom-up) — unlike `tree::clear()`, does not fire `on_clear`.
 */

/**
 * @fn scl::hierarchy::tree::reference::take(const_iterator)
 * @brief Detaches the child at @p position and returns it as a free-standing
 *        node; notifies the observer (`on_erase`, bottom-up).
 * @param  position  Valid iterator to a direct child of this node.
 * @return The extracted child node (moved out, no parent).
 */

/**
 * @fn scl::hierarchy::tree::reference::take_first()
 * @brief Detaches and returns the first child.
 * @pre `!empty()`
 */

/**
 * @fn scl::hierarchy::tree::reference::take_last()
 * @brief Detaches and returns the last child.
 * @pre `!empty()`
 */

// -----------------------------------------------------------------------------
// tree::reference — transfer (O(1) splice)
// -----------------------------------------------------------------------------

/**
 * @fn scl::hierarchy::tree::reference::transfer(reference)
 * @brief Moves all children of @p from to the back of this node's child list.
 *        O(1). Cross-tree: fires `on_erase` on the source and `on_insert` on
 *        the destination, once per moved node.
 */

/**
 * @fn scl::hierarchy::tree::reference::transfer(const_iterator, reference)
 * @brief Moves all children of @p from to where @p where. O(1).
 *        Cross-tree: fires `on_erase` on the source and `on_insert` on the
 *        destination, once per moved node.
 */

/**
 * @fn scl::hierarchy::tree::reference::transfer(reference, const_iterator)
 * @brief Moves the single child at @p first in @p from to the back of this
 *        node's child list. O(1). Cross-tree: fires `on_erase` on the source
 *        and `on_insert` on the destination.
 */

/**
 * @fn scl::hierarchy::tree::reference::transfer(const_iterator, reference, const_iterator)
 * @brief Moves the single child at @p first in @p from to where @p where. O(1).
 *        Cross-tree: fires `on_erase` on the source and `on_insert` on the
 *        destination.
 */

/**
 * @fn scl::hierarchy::tree::reference::transfer(reference, const_iterator, const_iterator)
 * @brief Moves children in [@p first, @p last) from @p from to the back of
 *        this node's child list. O(distance). Cross-tree: fires `on_erase`
 *        on the source and `on_insert` on the destination, once per moved node.
 */

/**
 * @fn scl::hierarchy::tree::reference::transfer(const_iterator, reference, const_iterator, const_iterator)
 * @brief Moves children in [@p first, @p last) from @p from to where @p where.
 *        O(distance). Cross-tree: fires `on_erase` on the source and
 *        `on_insert` on the destination, once per moved node.
 */

// -----------------------------------------------------------------------------
// tree::reference — iteration
// -----------------------------------------------------------------------------

/**
 * @fn scl::hierarchy::tree::reference::begin()
 * @brief Returns a mutable iterator to the first direct child.
 */

/**
 * @fn scl::hierarchy::tree::reference::end()
 * @brief Returns a mutable past-the-end sentinel for the direct child list.
 */

/**
 * @fn scl::hierarchy::tree::reference::begin() const
 * @brief Returns an immutable iterator to the first direct child.
 */

/**
 * @fn scl::hierarchy::tree::reference::end() const
 * @brief Returns an immutable past-the-end sentinel for the direct child list.
 */

/**
 * @fn scl::hierarchy::tree::reference::cbegin() const
 * @brief Returns an immutable iterator to the first direct child.
 */

/**
 * @fn scl::hierarchy::tree::reference::cend() const
 * @brief Returns an immutable past-the-end sentinel for the direct child list.
 */

/**
 * @fn scl::hierarchy::tree::reference::rbegin()
 * @brief Returns a mutable reverse iterator to the last direct child.
 */

/**
 * @fn scl::hierarchy::tree::reference::rend()
 * @brief Returns a mutable reverse past-the-end sentinel for the direct child list.
 */

/**
 * @fn scl::hierarchy::tree::reference::rbegin() const
 * @brief Returns an immutable reverse iterator to the last direct child.
 */

/**
 * @fn scl::hierarchy::tree::reference::rend() const
 * @brief Returns an immutable reverse past-the-end sentinel for the direct child list.
 */

/**
 * @fn scl::hierarchy::tree::reference::crbegin() const
 * @brief Returns an immutable reverse iterator to the last direct child.
 */

/**
 * @fn scl::hierarchy::tree::reference::crend() const
 * @brief Returns an immutable reverse past-the-end sentinel for the direct child list.
 */

// -----------------------------------------------------------------------------
// tree::const_reference
// -----------------------------------------------------------------------------

/**
 * @class scl::hierarchy::tree::const_reference
 * @brief The read-only counterpart of `reference`.
 *
 * Obtained by dereferencing a `tree::const_iterator` or converting from a
 * `reference`. Mirrors `reference`'s read-only subset — `value`, `tree`, `leaf`,
 * `has_parent`, `parent`, `empty`, `size`, `front`, `back`, `operator==`, and
 * the iterators. There is no mutation, insertion, removal, or transfer on
 * `const_reference`.
 */

/**
 * @typedef scl::hierarchy::tree::const_reference::const_tree_reference
 * @brief Immutable reference to the owning tree (`tree const &`).
 */

/**
 * @typedef scl::hierarchy::tree::const_reference::const_node_reference
 * @brief Immutable reference to the underlying `node` (`node const &`).
 */

/**
 * @typedef scl::hierarchy::tree::const_reference::const_payload_reference
 * @brief Immutable reference to the node's payload (`payload const &`).
 */

/**
 * @typedef scl::hierarchy::tree::const_reference::const_iterator
 * @brief Immutable iterator over this node's direct children.
 */

/**
 * @typedef scl::hierarchy::tree::const_reference::const_reverse_iterator
 * @brief Immutable reverse iterator over this node's direct children.
 */

/**
 * @typedef scl::hierarchy::tree::const_reference::size_type
 * @brief Unsigned integer type for child counts.
 */

/**
 * @typedef scl::hierarchy::tree::const_reference::difference_type
 * @brief Signed integer type for iterator distances.
 */

/**
 * @fn scl::hierarchy::tree::const_reference::const_reference(reference)
 * @brief Converts a mutable `reference` to a `const_reference`, over the same node.
 */

/**
 * @fn scl::hierarchy::tree::const_reference::tree() const
 * @brief Returns the tree this node belongs to.
 */

/**
 * @fn scl::hierarchy::tree::const_reference::value() const
 * @brief Returns a const reference to the node's payload.
 */

/**
 * @fn scl::hierarchy::tree::const_reference::has_parent() const
 * @brief Returns `true` when this node is attached to a parent.
 */

/**
 * @fn scl::hierarchy::tree::const_reference::parent() const
 * @brief Returns a proxy over the parent node.
 * @pre `has_parent()`
 */

/**
 * @fn scl::hierarchy::tree::const_reference::operator==(const_reference) const
 * @brief Returns `true` when @p other denotes the same node — identity, not value.
 */

/**
 * @fn scl::hierarchy::tree::const_reference::operator!=(const_reference) const
 * @brief Negation of @ref scl::hierarchy::tree::const_reference::operator==,
 *        spelled out because the proxies compare across their mutable and
 *        immutable forms rather than only against their own type.
 */

/**
 * @fn scl::hierarchy::tree::const_reference::empty() const
 * @brief Returns `true` when this node has no direct children.
 */

/**
 * @fn scl::hierarchy::tree::const_reference::size() const
 * @brief Returns the number of direct children.
 */

/**
 * @fn scl::hierarchy::tree::const_reference::leaf() const
 * @brief Returns the underlying node this proxy wraps.
 */

/**
 * @fn scl::hierarchy::tree::const_reference::front() const
 * @brief Returns a proxy over the first direct child.
 * @pre `!empty()`
 */

/**
 * @fn scl::hierarchy::tree::const_reference::back() const
 * @brief Returns a proxy over the last direct child.
 * @pre `!empty()`
 */

/**
 * @fn scl::hierarchy::tree::const_reference::begin() const
 * @brief Returns an immutable iterator to the first direct child.
 */

/**
 * @fn scl::hierarchy::tree::const_reference::end() const
 * @brief Returns an immutable past-the-end sentinel for the direct child list.
 */

/**
 * @fn scl::hierarchy::tree::const_reference::cbegin() const
 * @brief Returns an immutable iterator to the first direct child.
 */

/**
 * @fn scl::hierarchy::tree::const_reference::cend() const
 * @brief Returns an immutable past-the-end sentinel for the direct child list.
 */

/**
 * @fn scl::hierarchy::tree::const_reference::rbegin() const
 * @brief Returns an immutable reverse iterator to the last direct child.
 */

/**
 * @fn scl::hierarchy::tree::const_reference::rend() const
 * @brief Returns an immutable reverse past-the-end sentinel for the direct child list.
 */

/**
 * @fn scl::hierarchy::tree::const_reference::crbegin() const
 * @brief Returns an immutable reverse iterator to the last direct child.
 */

/**
 * @fn scl::hierarchy::tree::const_reference::crend() const
 * @brief Returns an immutable reverse past-the-end sentinel for the direct child list.
 */

// -----------------------------------------------------------------------------
// ADL customization points
// -----------------------------------------------------------------------------

/**
 * @fn scl::hierarchy::adl_parent(Ref self)
 * @ingroup scl_utility_hierarchy
 * @brief Customization point answering @ref scl::hierarchy::parent_of for a
 *        @ref scl::hierarchy::tree proxy.
 *
 * Found by argument-dependent lookup, not called directly: the algorithms in
 * `algorithm.h` are the interface, and this is how the proxies satisfy them.
 * The parameter is a single deduced `Ref` rather than `tree::reference`, whose
 * template arguments only ever appear behind `tree<...>::` and so cannot be
 * deduced ([temp.deduct.type]); `concepts::node_proxy` narrows it back down.
 *
 * @tparam Ref   @ref scl::hierarchy::tree::reference or
 *               @ref scl::hierarchy::tree::const_reference.
 * @param  self  The proxy whose parent to return.
 * @return A proxy for the parent, of the same kind as @p self.
 */

/**
 * @fn scl::hierarchy::adl_has_parent(Ref self)
 * @ingroup scl_utility_hierarchy
 * @brief Customization point answering @ref scl::hierarchy::has_parent for a
 *        @ref scl::hierarchy::tree proxy.
 *
 * @tparam Ref   @ref scl::hierarchy::tree::reference or
 *               @ref scl::hierarchy::tree::const_reference.
 * @param  self  The proxy to query.
 * @return `true` when the denoted node is attached to a parent.
 */

/**
 * @fn scl::hierarchy::adl_identity(Ref self)
 * @ingroup scl_utility_hierarchy
 * @brief Customization point answering @ref scl::hierarchy::are_identical for a
 *        @ref scl::hierarchy::tree proxy.
 *
 * The token stands for the wrapped node rather than for the proxy: the proxies
 * are transient and passed by value, so two of them denoting the same node must
 * still compare identical.
 *
 * @tparam Ref   @ref scl::hierarchy::tree::reference or
 *               @ref scl::hierarchy::tree::const_reference.
 * @param  self  The proxy to identify.
 * @return A @ref scl::hierarchy::identity standing for the node @p self denotes.
 */
