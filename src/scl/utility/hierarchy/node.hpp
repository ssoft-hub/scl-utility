#pragma once

#include "algorithm.h"
#include "node.h"

namespace scl::hierarchy
{
    template <typename Value, template <typename> class Allocator>
    node<Value, Allocator>::node(self_type const & other)
        : m_parent()
        , m_nodes()
        , m_value(other.m_value)
    {
        for (node_const_reference_type child : other)
            append(child);
    }

    template <typename Value, template <typename> class Allocator>
    node<Value, Allocator>::node(self_type && other)
        : m_parent()
        , m_nodes(::std::forward<nodes>(other.m_nodes))
        , m_value(::std::forward<Value>(other.m_value))
    {
        if (other.m_parent)
        // Извлекаем узел из родителя (линейный алгоритм)
        {
            self_type & parent = *other.m_parent;
            for (auto iter = parent.cbegin(); iter != parent.cend(); ++iter)
            // Перебираем у родителя other последовательно все элементы
            {
                if (::scl::hierarchy::are_identical(*this, *iter))
                // пока не встретим other
                {
                    erase(iter);
                    break;
                }
            }
        }

        for (self_type & node : m_nodes)
            node.m_parent = this;
        // всем дочерним узлам other задаем в качестве родителя this
    }

    template <typename Value, template <typename> class Allocator>
    template <typename... _Arguments>
    inline typename node<Value, Allocator>::iterator
    node<Value, Allocator>::emplace(const_iterator before_position, _Arguments &&... arguments)
    {
        assert(before_position == end() || ::scl::hierarchy::is_parent_of(*this, *before_position));
        //< позиция before_position должна принадлежать узлу this или быть end.

        iterator result = m_nodes.emplace(before_position, ::std::forward<_Arguments>(arguments)...);
        result->m_parent = this;
        return result;
    }

    template <typename Value, template <typename> class Allocator>
    inline node<Value, Allocator>::iterator node<Value, Allocator>::erase(iterator position) noexcept
    {
        assert(position != end() && ::scl::hierarchy::is_parent_of(*this, *position));
        return {m_nodes.erase(position)};
    }

    template <typename Value, template <typename> class Allocator>
    inline node<Value, Allocator>::iterator
    node<Value, Allocator>::erase(iterator first, iterator last) noexcept
    {
        assert(first != end() && ::scl::hierarchy::is_parent_of(*first));
        assert(last == end() || ::scl::hierarchy::is_parent_of(*last));

        return m_nodes.erase(first, last);
    }

    template <typename Value, template <typename> class Allocator>
    inline node<Value, Allocator>::const_iterator
    node<Value, Allocator>::erase(const_iterator position) noexcept
    {
        assert(position != end() && ::scl::hierarchy::is_parent_of(*this, *position));
        return {m_nodes.erase(position)};
    }

    template <typename Value, template <typename> class Allocator>
    inline node<Value, Allocator>::const_iterator
    node<Value, Allocator>::erase(const_iterator first, const_iterator last) noexcept
    {
        assert(first != end() && ::scl::hierarchy::is_parent_of(*first));
        assert(last == end() || ::scl::hierarchy::is_parent_of(*last));

        return m_nodes.erase(first, last);
    }

    template <typename Value, template <typename> class Allocator>
    inline node<Value, Allocator> node<Value, Allocator>::take(const_iterator position) noexcept
    {
        assert(position != cend() && ::scl::hierarchy::is_parent_of(*this, *position));
        //< позиция position должна принадлежать узлу this и не быть end
        node_const_reference_type child = *position;
        child.m_parent = ParentOptionalAssociation();
        //< чтобы перенос не затронул этот узел
        node result = ::std::move(child);
        //< перенос содержимого узла в result
        m_nodes.erase(position);
        //< удаление теперь уже пустого узла
        return result;
    }

    template <typename Value, template <typename> class Allocator>
    inline node<Value, Allocator> node<Value, Allocator>::takeFirst() noexcept
    {
        return take(cbegin());
    }

    template <typename Value, template <typename> class Allocator>
    inline node<Value, Allocator> node<Value, Allocator>::takeLast() noexcept
    {
        return take(--cend());
    }

    template <typename Value, template <typename> class Allocator>
    void
    node<Value, Allocator>::transfer(const_iterator before_position, node_reference_type from_node) noexcept
    {
        assert(before_position == cend() || ::scl::hierarchy::is_parent_of(*this, *before_position));
        //< позиция before_position должна принадлежать узлу this или быть end
        assert(!::scl::hierarchy::are_identical(from_node, *this));
        //< from_node не должен быть this
        assert(!::scl::hierarchy::is_ancestor_of(from_node, *this));
        //< from_node не должен содержать this

        for (node_reference_type child : from_node)
            child.m_parent = this;

        m_nodes.splice(before_position, from_node.m_nodes);
        // Complexity - Constant.
    }

    template <typename Value, template <typename> class Allocator>
    inline void node<Value, Allocator>::transfer(
        const_iterator before_position, node_reference_type from_node, const_iterator from_position) noexcept
    {
        assert(before_position == cend() || ::scl::hierarchy::is_parent_of(*this, *before_position));
        //< позиция before_position должна принадлежать узлу this или быть end
        assert(from_position != from_node.cend() && ::scl::hierarchy::is_parent_of(from_node, *from_position));
        //< позиция from_position должна принадлежать узлу from_node и не быть end
        assert(!::scl::hierarchy::are_identical(*from_position, *before_position));
        // before_position не должна быть from_position
        assert(!::scl::hierarchy::is_ancestor_of(*from_position, *before_position));
        // before_position не должна быть частью from_position

        (*from_position).m_parent = this;

        m_nodes.splice(before_position, from_node.m_nodes, from_position);
        // Complexity - Constant.
    }

    template <typename Value, template <typename> class Allocator>
    void node<Value, Allocator>::transfer(const_iterator before_position,
        node_reference_type from_node,
        const_iterator from_first,
        const_iterator from_last) noexcept
    {
        assert(before_position == cend() || ::scl::hierarchy::is_parent_of(*this, *before_position));
        //< позиция before_position должна принадлежать узлу this или быть end
        //< позиция from_last должна принадлежать узлу from_node или быть end
        assert(from_first != from_node.cend() && ::scl::hierarchy::is_parent_of(from_node, *from_first));
        //< позиция from_first должна принадлежать узлу from_node и не быть end
        assert(from_last == from_node.cend() || ::scl::hierarchy::is_parent_of(from_node, *from_last));
        //< позиция from_last должна принадлежать узлу from_node или быть end

#ifndef NDEBUG
        if (::scl::hierarchy::are_identical(from_node, *this))
        // проверяем, что before_position не входит в диапазон from_position .. end
        {
            for (const_iterator iter = from_first; iter != from_last; ++iter)
                assert(!::scl::hierarchy::are_identical(*iter, *before_position));
            //< before_position не должен быть в диапазоне [from_position, end).
        }
        else if (::scl::hierarchy::is_ancestor_of(from_node, *this))
        // проверяем, что this не является частью узлов из интервала from_position .. end
        {
            for (const_iterator iter = from_first; iter != from_last; ++iter)
                assert(!::scl::hierarchy::is_ancestor_of(*iter, *before_position));
            //< before_position не должен быть в диапазоне [from_position, end).
        }
#endif
        for (const_iterator iter = from_first; iter != from_last; ++iter)
            iter->m_parent = this;

        m_nodes.splice(before_position, from_node.m_nodes, from_first, from_last);
        // Complexity - Up to linear in the number of elements transferred.
    }
} // namespace scl::hierarchy
