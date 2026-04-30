#pragma once

/// @file node.h
/// @brief Parent-child tree node.
/// @ingroup scl_utility_hierarchy

#include <cassert>
#include <concepts>
#include <list>
#include <memory>

namespace scl::hierarchy
{
    /// @brief Tree node that maintains a parent-child relationship.
    /// @ingroup scl_utility_hierarchy
    ///
    /// Each node owns zero or more child nodes stored in an intrusive std::list.
    /// The parent back-pointer is a raw pointer; use @ref has_parent to test
    /// whether a node is attached to a parent.
    ///
    /// @tparam Value     Payload type stored in each node.
    /// @tparam Allocator Allocator template for the child list.
    template <typename Value, template <typename> class Allocator = ::std::allocator>
    class node
    {
        using self_type = node<Value, Allocator>;

    public:
        /// @brief Mutable reference to the payload value.
        using reference_type = Value &;
        /// @brief Const reference to the payload value.
        using const_reference_type = Value const &;

        /// @brief Mutable reference to a child node.
        using node_reference_type = self_type &;
        /// @brief Const reference to a child node.
        using node_const_reference_type = self_type const &;

        /// @brief List type used to store child nodes.
        using nodes = ::std::list<self_type, Allocator<self_type>>;
        /// @brief Type for child-count values.
        using node_count = typename nodes::size_type;

        // STL container compatibility
        using value_type = self_type;
        using pointer = typename nodes::pointer;
        using const_pointer = typename nodes::const_pointer;
        using reference = typename nodes::reference;
        using const_reference = typename nodes::const_reference;
        using iterator = typename nodes::iterator;
        using const_iterator = typename nodes::const_iterator;
        using reverse_iterator = typename nodes::reverse_iterator;
        using const_reverse_iterator = typename nodes::const_reverse_iterator;
        using size_type = typename nodes::size_type;
        using difference_type = typename nodes::difference_type;
        using allocator_type = typename nodes::allocator_type;

    private:
        using parent_ptr = self_type *;

        mutable parent_ptr m_parent = nullptr;
        nodes m_nodes;
        Value m_value;

    public:
        node(self_type && other) noexcept(::std::is_nothrow_move_constructible_v<Value>);
        node(self_type const && other)
            : node{static_cast<self_type const &>(other)}
        {}
        node(self_type & other)
            : node{const_cast<self_type const &>(other)}
        {}
        node(self_type const & other);

        /// @brief Constructs the payload in-place by forwarding @p arguments.
        template <typename... Arguments>
        node(Arguments &&... arguments) noexcept(::std::is_nothrow_constructible_v<Value, Arguments...>)
            requires(::std::constructible_from<Value, Arguments...>)
            : m_parent{}
            , m_value{::std::forward<Arguments>(arguments)...}
        {}

        void operator=(self_type const &) = delete;
        void operator=(self_type &&) = delete;

        /// @brief Returns mutable reference to the payload value.
        constexpr reference_type value() noexcept { return m_value; }
        /// @brief Returns const reference to the payload value.
        constexpr const_reference_type value() const noexcept { return m_value; }

        /// @brief Returns a reference to the parent node.
        /// @pre has_parent(*this) must be true.
        constexpr node_reference_type parent() noexcept { return *m_parent; }
        /// @brief Returns a const reference to the parent node.
        /// @pre has_parent(*this) must be true.
        constexpr node_const_reference_type parent() const noexcept { return *m_parent; }

        bool empty() const noexcept { return m_nodes.empty(); }
        size_type size() const noexcept { return m_nodes.size(); }

        reference front() noexcept { return m_nodes.front(); }
        const_reference front() const noexcept { return m_nodes.front(); }
        reference back() noexcept { return m_nodes.back(); }
        const_reference back() const noexcept { return m_nodes.back(); }

        template <typename... Arguments>
        iterator push_back(Arguments &&... arguments)
            requires(::std::constructible_from<Value, Arguments...>)
        {
            return emplace(end(), ::std::forward<Arguments>(arguments)...);
        }

        template <typename... Arguments>
        iterator push_front(Arguments &&... arguments)
            requires(::std::constructible_from<Value, Arguments...>)
        {
            return emplace(begin(), ::std::forward<Arguments>(arguments)...);
        }

        template <typename... Arguments>
        iterator insert(const_iterator before_position, Arguments &&... arguments)
            requires(::std::constructible_from<Value, Arguments...>)
        {
            return emplace(before_position, ::std::forward<Arguments>(arguments)...);
        }

        template <typename... Arguments>
        iterator emplace(const_iterator before_position, Arguments &&... arguments)
            requires(::std::constructible_from<Value, Arguments...>);

        void pop_back() noexcept { m_nodes.pop_back(); }
        void pop_front() noexcept { m_nodes.pop_front(); }

        iterator erase(iterator position) noexcept;
        iterator erase(iterator first, iterator last) noexcept;
        const_iterator erase(const_iterator position) noexcept;
        const_iterator erase(const_iterator first, const_iterator last) noexcept;

        void clear() noexcept { m_nodes.clear(); }

        /// @brief Removes and returns (deep-copies) the child at @p position.
        node take(const_iterator position);
        node take_first();
        node take_last();

        void transfer(node_reference_type from_node) noexcept { transfer(cend(), from_node); }
        void transfer(node_reference_type from_node, const_iterator from_position) noexcept
        {
            transfer(cend(), from_node, from_position);
        }
        void
        transfer(node_reference_type from_node, const_iterator from_first, const_iterator from_last) noexcept
        {
            transfer(cend(), from_node, from_first, from_last);
        }
        void transfer(const_iterator before_position, node_reference_type from_node) noexcept;
        void
        transfer(const_iterator before_position, node_reference_type from_node, const_iterator from_position) noexcept;
        void transfer(const_iterator before_position,
            node_reference_type from_node,
            const_iterator from_first,
            const_iterator from_last) noexcept;

        iterator begin() noexcept { return m_nodes.begin(); }
        iterator end() noexcept { return m_nodes.end(); }
        const_iterator begin() const noexcept { return m_nodes.begin(); }
        const_iterator end() const noexcept { return m_nodes.end(); }
        const_iterator cbegin() const noexcept { return m_nodes.cbegin(); }
        const_iterator cend() const noexcept { return m_nodes.cend(); }
    };

    // ---- Implementations ----

    template <typename Value, template <typename> class Allocator>
    node<Value, Allocator>::node(self_type const & other)
        : m_parent{}
        , m_nodes{other.m_nodes}
        , m_value{other.m_value}
    {
        for (node_reference_type child : m_nodes)
            child.m_parent = this;
    }

    template <typename Value, template <typename> class Allocator>
    node<Value, Allocator>::node(self_type && other) noexcept(::std::is_nothrow_move_constructible_v<Value>)
        : m_parent{}
        , m_nodes{::std::forward<nodes>(other.m_nodes)}
        , m_value{::std::forward<Value>(other.m_value)}
    {
        for (node_reference_type child : m_nodes)
            child.m_parent = this;
    }

    template <typename Value, template <typename> class Allocator>
    template <typename... Arguments>
    inline typename node<Value, Allocator>::iterator
    node<Value, Allocator>::emplace(const_iterator before_position, Arguments &&... arguments)
        requires(::std::constructible_from<Value, Arguments...>)
    {
        iterator result = m_nodes.emplace(before_position, ::std::forward<Arguments>(arguments)...);
        result->m_parent = this;
        return result;
    }

    template <typename Value, template <typename> class Allocator>
    inline typename node<Value, Allocator>::iterator
    node<Value, Allocator>::erase(iterator position) noexcept
    {
        return m_nodes.erase(position);
    }

    template <typename Value, template <typename> class Allocator>
    inline typename node<Value, Allocator>::iterator
    node<Value, Allocator>::erase(iterator first, iterator last) noexcept
    {
        return m_nodes.erase(first, last);
    }

    template <typename Value, template <typename> class Allocator>
    inline typename node<Value, Allocator>::const_iterator
    node<Value, Allocator>::erase(const_iterator position) noexcept
    {
        return m_nodes.erase(position);
    }

    template <typename Value, template <typename> class Allocator>
    inline typename node<Value, Allocator>::const_iterator
    node<Value, Allocator>::erase(const_iterator first, const_iterator last) noexcept
    {
        return m_nodes.erase(first, last);
    }

    template <typename Value, template <typename> class Allocator>
    inline node<Value, Allocator> node<Value, Allocator>::take(const_iterator position)
    {
        node_const_reference_type child = *position;
        child.m_parent = nullptr;
        node result = ::std::move(child);
        m_nodes.erase(position);
        return result;
    }

    template <typename Value, template <typename> class Allocator>
    inline node<Value, Allocator> node<Value, Allocator>::take_first()
    {
        return take(cbegin());
    }

    template <typename Value, template <typename> class Allocator>
    inline node<Value, Allocator> node<Value, Allocator>::take_last()
    {
        return take(--cend());
    }

    template <typename Value, template <typename> class Allocator>
    void
    node<Value, Allocator>::transfer(const_iterator before_position, node_reference_type from_node) noexcept
    {
        for (node_reference_type child : from_node)
            child.m_parent = this;
        m_nodes.splice(before_position, from_node.m_nodes);
    }

    template <typename Value, template <typename> class Allocator>
    inline void node<Value, Allocator>::transfer(
        const_iterator before_position, node_reference_type from_node, const_iterator from_position) noexcept
    {
        const_cast<node_reference_type>(*from_position).m_parent = this;
        m_nodes.splice(before_position, from_node.m_nodes, from_position);
    }

    template <typename Value, template <typename> class Allocator>
    void node<Value, Allocator>::transfer(const_iterator before_position,
        node_reference_type from_node,
        const_iterator from_first,
        const_iterator from_last) noexcept
    {
        for (const_iterator it = from_first; it != from_last; ++it)
            const_cast<node_reference_type>(*it).m_parent = this;
        m_nodes.splice(before_position, from_node.m_nodes, from_first, from_last);
    }

} // namespace scl::hierarchy
