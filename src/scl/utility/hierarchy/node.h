#pragma once

#include <cassert>
#include <list>
#include <memory>

namespace scl::hierarchy
{
    template <typename Value, template <typename> class Allocator = ::std::allocator>
    class node
    {
        using self_type = node<Value, Allocator>;
        using allocator_type = Allocator<self_type>;

    public:
        using value_type = Value;
        using reference_type = Value &;
        using const_reference_type = Value const &;

        using node_reference_type = self_type &;
        using node_const_reference_type = self_type const &;
        using nodes = ::std::list<self_type, allocator_type>;

        using iterator = typename nodes::iterator;
        using const_iterator = typename nodes::const_iterator;

        using node_count = typename nodes::size_type;

        using NodeOptionalReference = node_reference_type; // TODO: что-то типа ::std::optional< ::std::reference_wrapper< self_type > >;
        using ConstNodeOptionalReference = node_const_reference_type;

    public:
        using ParentOptionalAssociation = self_type *;

    private:
        mutable ParentOptionalAssociation m_parent;
        nodes m_nodes;
        value_type m_value;

    public:
        node(self_type && other);
        node(self_type const && other)
            : node{static_cast<self_type const &>(other)}
        {}
        node(self_type & other)
            : node{const_cast<self_type const &>(other)}
        {}
        node(self_type const & other);

        template <typename... Arguments>
        node(Arguments &&... arguments)
            : m_parent{}
            , m_value{::std::forward<Arguments>(arguments)...}
        {}

        void operator=(self_type const &) = delete;
        void operator=(self_type &&) = delete;

        reference_type value() noexcept { return m_value; }
        const_reference_type value() const noexcept { return m_value; }

        NodeOptionalReference parent() noexcept { return *m_parent; }
        ConstNodeOptionalReference parent() const noexcept { return *m_parent; }

        bool empty() const noexcept { return m_nodes.empty(); }
        node_count count() const noexcept { return m_nodes.size(); }

        node_reference_type first() noexcept { return m_nodes.front(); }
        node_const_reference_type first() const noexcept { return m_nodes.front(); }
        node_reference_type last() noexcept { return m_nodes.back(); }
        node_const_reference_type last() const noexcept { return m_nodes.back(); }

        template <typename... _Arguments>
        iterator append(_Arguments &&... arguments)
        {
            return emplace(end(), ::std::forward<_Arguments>(arguments)...);
        }
        template <typename... _Arguments>
        iterator prepend(_Arguments &&... arguments)
        {
            return emplace(begin(), ::std::forward<_Arguments>(arguments)...);
        }
        template <typename... _Arguments>
        iterator emplace(const_iterator before_position, _Arguments &&... arguments);

        iterator erase(iterator position) noexcept;
        iterator erase(iterator first, iterator last) noexcept;

        const_iterator erase(const_iterator position) noexcept;
        const_iterator erase(const_iterator first, const_iterator last) noexcept;

        void clear() noexcept { m_nodes.clear(); }

        node take(const_iterator position) noexcept;
        node takeFirst() noexcept;
        node takeLast() noexcept;

        void transfer(node_reference_type from_node) noexcept
        {
            return transfer(cend(), from_node);
        }
        void transfer(node_reference_type from_node, const_iterator from_position) noexcept
        {
            return transfer(cend(), from_node, from_position);
        }
        void
        transfer(node_reference_type from_node, const_iterator from_first, const_iterator from_last) noexcept
        {
            return transfer(cend(), from_node, from_first, from_last);
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

    public:
        // std concepts compatibility
        using value_type = typename nodes::value_type;
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

        reference front() noexcept { return first(); }
        const_reference front() const noexcept { return first(); }
        reference back() noexcept { return last(); }
        const_reference back() const noexcept { return last(); }

        node_count size() const noexcept { return count(); }
    };
} // namespace scl::hierarchy
