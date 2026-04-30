#pragma once
#ifndef HIERARCHY_TREE_H
#define HIERARCHY_TREE_H

#include "algorithm.h"
#include "node.h"
#include <tuple>

namespace scl::hierarchy
{
    template <typename Value, template <typename> class Observer, template <typename> class Allocator = ::std::allocator>
    class Tree
    {
        using self_type = Tree<Value, Observer, Allocator>;

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
        class NodeWrapper;
        class ConstNodeWrapper;

        class NodeWrapperIterator;
        class ConstNodeWrapperIterator;

    private:
        template <typename T>
        class arrow
        {
            T m_value_type;

        public:
            template <typename... Arguments>
            arrow(Arguments &&... arguments)
                : m_value_type{::std::forward<Arguments>(arguments)...}
            {}

            auto operator->() && { return ::std::addressof(m_value_type); }
        };

    public:
        class NodeWrapperIterator
        {
            using self_type = NodeWrapperIterator;

            template <typename, template <typename> class, template <typename> class>
            friend class Tree;
            friend class ConstNodeIterator;

        public:
            using Tree = scl::hierarchy::Tree<value_type, Observer, Allocator>;
            using TreeReference = Tree &;
            using ConstTreeReference = Tree const &;

            using NodeWrapper = typename Tree::NodeWrapper;
            using ConstNodeWrapper = typename Tree::ConstNodeWrapper;

        private:
            using NodeIterator = typename node::iterator;
            using TreeAddress = Tree *;

        private:
            TreeAddress m_tree_address = TreeAddress(); //< опциональная ассоциация с деревом
            NodeIterator m_node_iter;                   //< подвижная ассоциация с узлом контейнера

        private:
            NodeWrapperIterator(TreeAddress tree, NodeIterator iter) noexcept
                : m_tree_address(tree)
                , m_node_iter(iter)
            {}

        public:
            NodeWrapperIterator() = default;

            bool operator==(self_type const & other) const noexcept
            {
                return m_node_iter == other.m_node_iter;
            }
            bool operator!=(self_type const & other) const noexcept { return !(*this == other); }

            self_type & operator++()
            {
                ++m_node_iter;
                return *this;
            }
            self_type & operator--()
            {
                --m_node_iter;
                return *this;
            }

            self_type operator++(int) { return {m_tree_address, m_node_iter++}; }
            self_type operator--(int) { return {m_tree_address, m_node_iter--}; }

            NodeWrapper operator*() { return {*m_tree_address, *m_node_iter}; }
            ConstNodeWrapper operator*() const { return {m_tree_address, *m_node_iter}; }

            arrow<NodeWrapper> operator->() { return {*m_tree_address, *m_node_iter}; }
            arrow<ConstNodeWrapper> operator->() const { return {*m_tree_address, *m_node_iter}; }
        };

    public:
        class ConstNodeWrapperIterator
        {
            using self_type = ConstNodeWrapperIterator;

            template <typename, template <typename> class, template <typename> class>
            friend class Tree;

        public:
            using Tree = ::scl::hierarchy::Tree<Value, Observer, Allocator>;
            using TreeReference = Tree &;
            using ConstTreeReference = Tree const &;

            using Wrapper = typename Tree::NodeWrapper;
            using ConstNodeWrapper = typename Tree::ConstNodeWrapper;

        private:
            using ConstNodeIterator = typename node::const_iterator;
            using TreeConstAddress = Tree const *;

        private:
            TreeConstAddress m_tree_address = TreeConstAddress(); //< опциональная ассоциация с деревом
            ConstNodeIterator m_node_iter; //< подвижная ассоциация с узлом контейнера

        private:
            ConstNodeWrapperIterator(TreeConstAddress tree, ConstNodeIterator iter) noexcept
                : m_tree_address(tree)
                , m_node_iter(iter)
            {}

        public:
            ConstNodeWrapperIterator() = default;
            ConstNodeWrapperIterator(self_type &&) noexcept = default;
            ConstNodeWrapperIterator(self_type &) noexcept = default;
            ConstNodeWrapperIterator(self_type const &) noexcept = default;

            ConstNodeWrapperIterator(NodeWrapperIterator const & other) noexcept
                : m_tree_address(other.m_tree_address)
                , m_node_iter(other.m_node_iter)
            {}

            self_type & operator=(self_type const & other) noexcept = default;
            self_type & operator=(NodeWrapperIterator const & other) noexcept
            {
                m_tree_address = other.m_tree_address;
                m_node_iter = other.m_node_iter;
                return *this;
            }

            bool operator==(self_type const & other) const noexcept
            {
                return m_node_iter == other.m_node_iter;
            }
            bool operator!=(self_type const & other) const noexcept { return !(*this == other); }

            self_type & operator++()
            {
                ++m_node_iter;
                return *this;
            }
            self_type & operator--()
            {
                --m_node_iter;
                return *this;
            }

            self_type operator++(int) { return {*m_tree_address, m_node_iter++}; }
            self_type operator--(int) { return {*m_tree_address, m_node_iter--}; }

            ConstNodeWrapper operator*() const { return {*m_tree_address, *m_node_iter}; }
            arrow<ConstNodeWrapper> operator->() const { return {*m_tree_address, *m_node_iter}; }
        };

    public:
        class NodeWrapper
        {
            using self_type = NodeWrapper;

            template <typename, template <typename> class, template <typename> class>
            friend class Tree;
            friend class ConstNodeWrapper;

        public:
            using Tree = ::scl::hierarchy::Tree<value_type, Observer, Allocator>;
            using TreeReference = Tree &;
            using ConstTreeReference = Tree const &;

            using node_reference_type = node &;
            using node_const_reference_type = node const &;

            using reference_type = value_type &;
            using const_reference_type = value_type const &;

            using NodeWrapperIterator = typename Tree::NodeWrapperIterator;
            using ConstNodeWrapperIterator = typename Tree::ConstNodeWrapperIterator;

            using node_count = typename node::node_count;

        private:
            TreeReference m_tree_ref;       //< неизменяемая ассоциация с деревом
            node_reference_type m_node_ref; //< неизменяемая ассоциация с узлом

        private:
            NodeWrapper(TreeReference tree, node_reference_type node) noexcept
                : m_tree_ref(tree)
                , m_node_ref(node)
            {}

        public:
            TreeReference tree() noexcept { return m_tree_ref; }
            ConstTreeReference tree() const noexcept { return m_tree_ref; }

            void setvalue_type(const_reference_type value_type)
            {
                m_tree_ref.notifyOnChange(m_node_ref.value_type(), value_type);
                m_node_ref.value_type() = value_type;
            }
            const_reference_type value_type() const { return m_node_ref.value_type(); }

            //value_typeWrapper value_type () { return m_node_ref.value_type(); }
            //Constvalue_typeWrapper value_type () const { return m_node_ref.value_type(); }

            // Далее следует повтор интерфейса  ParentChild::node, так как
            // данный вид дерева подразумевает полный повтор функциональности node.
            // Для других видов деревьев это не актуально.

            NodeWrapper parent() noexcept { return {m_tree_ref, m_node_ref.parent()}; }
            ConstNodeWrapper parent() const noexcept { return {m_tree_ref, m_node_ref.parent()}; }

            bool operator==(ConstNodeWrapper other) const noexcept
            {
                return ::scl::hierarchy::are_identical(m_node_ref, other.m_node_ref);
            }
            bool operator!=(ConstNodeWrapper other) const noexcept { return !(*this == other); }

            bool empty() const noexcept { return m_node_ref.empty(); }
            node_count count() const { return m_node_ref.count(); }

            NodeWrapper first() noexcept { return {m_tree_ref, m_node_ref.first()}; }
            ConstNodeWrapper first() const noexcept { return {m_tree_ref, m_node_ref.first()}; }
            NodeWrapper last() noexcept { return {m_tree_ref, m_node_ref.last()}; }
            ConstNodeWrapper last() const noexcept { return {m_tree_ref, m_node_ref.last()}; }

            template <typename... _Arguments>
            NodeWrapperIterator append(_Arguments &&... arguments)
            {
                return emplace(cend(), ::std::forward<_Arguments>(arguments)...);
            }
            template <typename... _Arguments>
            NodeWrapperIterator prepend(_Arguments &&... arguments)
            {
                return emplace(cbegin(), ::std::forward<_Arguments>(arguments)...);
            }

            template <typename... _Arguments>
            NodeWrapperIterator
            emplace(ConstNodeWrapperIterator before_position, _Arguments &&... arguments)
            {
                NodeWrapperIterator item{::std::addressof(m_tree_ref),
                    m_node_ref.emplace(before_position.m_node_iter, ::std::forward<_Arguments>(arguments)...)};
                m_tree_ref.notifyAfterInsert(item);
                return item;
            }

            template <typename _Item>
            _Item erase(_Item position) noexcept
            {
                m_tree_ref.notifyBeforeErase(position);
                return {::std::addressof(m_tree_ref), m_node_ref.erase(position.m_node_iter)};
            }

            template <typename _Item>
            _Item erase(_Item first, _Item last) noexcept
            {
                for (auto item = first; item != last; ++item)
                    m_tree_ref.notifyBeforeErase(item);
                return {::std::addressof(m_tree_ref), m_node_ref.erase(first.m_node_iter, last.m_node_iter)};
            }

            void clear() noexcept
            {
                for (auto item = cbegin(); item != cend(); ++item)
                    m_tree_ref.notifyBeforeErase(item);
                m_node_ref.clear();
            }

            node release(ConstNodeWrapperIterator position) noexcept
            {
                m_tree_ref.notifyBeforeErase(position);
                return m_node_ref.take(position.m_node_iter);
            }

            node_const_reference_type access() const noexcept { return m_node_ref; }

            void transfer(NodeWrapper from_node) noexcept { transfer(cend(), from_node); }

            void transfer(ConstNodeWrapperIterator before_position, NodeWrapper from_node) noexcept
            {
                bool need_to_notify = ::std::addressof(m_tree_ref) != ::std::addressof(from_node.m_tree_ref);
                // Если перенос в другое дерево, то нужно оповещать наблюдателей

                if (need_to_notify)
                    for (ConstNodeWrapperIterator item = from_node.cbegin(); item != from_node.cend(); ++item)
                        from_node.m_tree_ref.notifyBeforeErase(item);

                node_count transfer_count = from_node.count();
                NodeWrapperIterator to_position{::std::addressof(m_tree_ref), from_node.begin().m_node_iter};
                // определяем откуда и сколько итерировать после переноса afterErase.

                m_node_ref.transfer(before_position.m_node_iter, from_node.m_node_ref);

                if (need_to_notify)
                    for (node_count i = node_count(); i < transfer_count; ++i)
                        m_tree_ref.notifyAfterInsert(to_position++);
            }

            void transfer(NodeWrapper from_node, ConstNodeWrapperIterator from_position) noexcept
            {
                transfer(cend(), from_node, from_position);
            }

            void transfer(ConstNodeWrapperIterator before_position,
                NodeWrapper from_node,
                ConstNodeWrapperIterator from_position) noexcept
            {
                bool need_to_notify = ::std::addressof(m_tree_ref) != ::std::addressof(from_node.m_tree_ref);
                // Если перенос в другое дерево, то нужно оповещать наблюдателей

                if (need_to_notify)
                    from_node.m_tree_ref.notifyBeforeErase(from_position);

                ConstNodeWrapperIterator to_position{::std::addressof(m_tree_ref), from_position.m_node_iter};
                m_node_ref.transfer(before_position.m_node_iter, from_node.m_node_ref, from_position.m_node_iter);

                if (need_to_notify)
                    m_tree_ref.notifyAfterInsert(to_position);
            }

            void
            transfer(NodeWrapper from_node, ConstNodeWrapperIterator from_first, ConstNodeWrapperIterator from_last) noexcept
            {
                transfer(cend(), from_node, from_first, from_last);
            }

            void transfer(ConstNodeWrapperIterator before_position,
                NodeWrapper from_node,
                ConstNodeWrapperIterator from_first,
                ConstNodeWrapperIterator from_last) noexcept
            {
                bool need_to_notify = ::std::addressof(m_tree_ref) != ::std::addressof(from_node.m_tree_ref);
                // Если перенос в другое дерево, то нужно оповещать наблюдателей

                node_count transfer_count = node_count();

                if (need_to_notify)
                    for (ConstNodeWrapperIterator item = from_first; item != from_last; ++item, ++transfer_count)
                        from_node.m_tree_ref.notifyBeforeErase(item);

                ConstNodeWrapperIterator to_position{::std::addressof(m_tree_ref), from_first.m_node_iter};
                // определяем откуда и сколько итерировать после переноса afterErase.

                m_node_ref.transfer(before_position.m_node_iter, from_node.m_node_ref,
                    from_first.m_node_iter, from_last.m_node_iter);

                if (need_to_notify)
                    for (node_count i = node_count(); i < transfer_count; ++i)
                        m_tree_ref.notifyAfterInsert(to_position++);
            }

            NodeWrapperIterator begin() noexcept
            {
                return {::std::addressof(m_tree_ref), m_node_ref.begin()};
            }
            ConstNodeWrapperIterator begin() const noexcept { return cbegin(); }
            ConstNodeWrapperIterator cbegin() const noexcept
            {
                return {::std::addressof(m_tree_ref), m_node_ref.cbegin()};
            }

            NodeWrapperIterator end() noexcept
            {
                return {::std::addressof(m_tree_ref), m_node_ref.end()};
            }
            ConstNodeWrapperIterator end() const noexcept { return cend(); }
            ConstNodeWrapperIterator cend() const noexcept
            {
                return {::std::addressof(m_tree_ref), m_node_ref.cend()};
            }
        };

    public:
        class ConstNodeWrapper
        {
            using self_type = ConstNodeWrapper;

            template <typename, template <typename> class, template <typename> class>
            friend class Tree;
            friend class NodeWrapper;

        public:
            using Tree = scl::hierarchy::Tree<value_type, Observer, Allocator>;
            using TreeReference = Tree &;
            using ConstTreeReference = Tree const &;

            using node_reference_type = node &;
            using node_const_reference_type = node const &;

            using reference_type = value_type const &;
            using const_reference_type = value_type const &;
            // TODO: реализовать обертку value_typeWrapper
            // для возможности реакции Tree на изменение значения
            // без явного применения метода setvalue_type.

            using NodeWrapperIterator = typename Tree::NodeWrapperIterator;
            using ConstNodeWrapperIterator = typename Tree::ConstNodeWrapperIterator;

            using node_count = typename node::node_count;

        private:
            ConstTreeReference m_tree_ref;        //< неизменяемая ассоциация с деревом
            node_const_reference_type m_node_ref; //< неизменяемая ассоциация с узлом

        private:
            ConstNodeWrapper(ConstTreeReference tree, node_const_reference_type node) noexcept
                : m_tree_ref(tree)
                , m_node_ref(node)
            {}

        public:
            ConstNodeWrapper(self_type &&) noexcept = default;
            ConstNodeWrapper(self_type &) noexcept = default;
            ConstNodeWrapper(self_type const &) noexcept = default;

            ConstNodeWrapper(NodeWrapper other)
                : m_tree_ref(other.m_tree_ref)
                , m_node_ref(other.m_node_ref)
            {}

            ConstTreeReference tree() const noexcept { return m_tree_ref; }
            const_reference_type value_type() const noexcept { return m_node_ref.value_type(); }

            ConstNodeWrapper parent() const noexcept { return {m_tree_ref, m_node_ref.parent()}; }
            node_const_reference_type access() const noexcept { return m_node_ref; }

            bool operator==(ConstNodeWrapper other) const noexcept
            {
                return scl::hierarchy::are_identical(m_node_ref, other.m_node_ref);
            }
            bool operator!=(ConstNodeWrapper other) const noexcept { return !(*this == other); }

            bool empty() const noexcept { return m_node_ref.empty(); }
            node_count count() const { return m_node_ref.count(); }

            ConstNodeWrapper first() const noexcept { return {m_tree_ref, m_node_ref.first()}; }
            ConstNodeWrapper last() const noexcept { return {m_tree_ref, m_node_ref.last()}; }

            ConstNodeWrapperIterator begin() const noexcept { return cbegin(); }
            ConstNodeWrapperIterator cbegin() const noexcept
            {
                return {::std::addressof(m_tree_ref), m_node_ref.cbegin()};
            }

            ConstNodeWrapperIterator end() const noexcept { return cend(); }
            ConstNodeWrapperIterator cend() const noexcept
            {
                return {::std::addressof(m_tree_ref), m_node_ref.cend()};
            }
        };

    private:
        observer_type m_observer;
        nodes m_nodes;

    public:
        observer_reference observer() noexcept { return m_observer; }
        observer_const_reference observer() const noexcept { return m_observer; }

        bool empty() const noexcept { return m_nodes.empty(); }

        template <typename... _Arguments>
        NodeWrapperIterator append(_Arguments &&... arguments)
        {
            return emplace(cend(), ::std::forward<_Arguments>(arguments)...);
        }
        template <typename... _Arguments>
        NodeWrapperIterator prepend(_Arguments &&... arguments)
        {
            return emplace(cbegin(), ::std::forward<_Arguments>(arguments)...);
        }
        template <typename... _Arguments>
        NodeWrapperIterator
        emplace(ConstNodeWrapperIterator before_position, _Arguments &&... arguments)
        {
            NodeWrapperIterator iter{this,
                m_nodes.emplace(before_position.m_node_iter, ::std::forward<_Arguments>(arguments)...)};
            notifyAfterInsert(iter);
            return iter;
        }

        template <typename _Iterator>
        _Iterator erase(_Iterator position) noexcept
        {
            assert(!::scl::hierarchy::has_parent(*position));
            // It must be a root iter.
            notifyBeforeErase(position);
            return {this, m_nodes.erase(position.m_node_iter)};
        }

        template <typename _Iterator>
        _Iterator erase(_Iterator first, _Iterator && last) noexcept
        {
            assert(!::scl::hierarchy::has_parent(*first) && !::scl::hierarchy::has_parent(*last));
            // It must be a root iter.
            for (auto iter = first; iter != last; ++iter)
                (*iter).beforeErase(iter);
            return {this, m_nodes.erase(first.m_node_iter, last.m_node_iter)};
        }

        void clear() noexcept
        {
            notifyBeforeClear();
            m_nodes.clear();
        }

        template <typename _Iterator>
        void remove(_Iterator position)
        {
            assert(this == position.m_tree_address);
            NodeWrapper node_wrapper{*this, const_cast<node &>(*position.m_node_iter)};
            if (::scl::hierarchy::has_parent(node_wrapper))
                node_wrapper.parent().erase(position);
            else
                erase(position);
        }

        NodeWrapperIterator begin() noexcept { return {this, m_nodes.begin()}; }
        ConstNodeWrapperIterator begin() const noexcept { return cbegin(); }
        ConstNodeWrapperIterator cbegin() const noexcept { return {this, m_nodes.cbegin()}; }

        NodeWrapperIterator end() noexcept { return {this, m_nodes.end()}; }
        ConstNodeWrapperIterator end() const noexcept { return cend(); }
        ConstNodeWrapperIterator cend() const noexcept { return {this, m_nodes.cend()}; }

    private:
        // Using observer contract methods
        void notifyAfterInsert(NodeWrapperIterator iter)
        {
            m_observer.afterInsert(iter);
            NodeWrapper node = (*iter);
            for (auto subiter = node.begin(); subiter != node.end(); ++subiter)
                notifyAfterInsert(subiter);
        }

        void notifyBeforeErase(ConstNodeWrapperIterator iter)
        {
            ConstNodeWrapper node = (*iter);
            for (auto subiter = node.begin(); subiter != node.end(); ++subiter)
                notifyBeforeErase(subiter);
            m_observer.beforeErase(iter);
        }

        void notifyBeforeClear() { m_observer.beforeClear(); }

        void notifyOnChange(const_reference_type current, const_reference_type next)
        {
            m_observer.onChange(current, next);
        }
    };
} // namespace scl::hierarchy

namespace hierarchy
{
    template <typename _Tree, typename... _Observers>
    class TupleOfObservers
    {
        friend _Tree;

    public:
        using Tree = _Tree;

        using NodeItem = typename Tree::NodeWrapperIterator;
        using ConstNodeItem = typename Tree::ConstNodeWrapperIterator;
        using const_reference_type = typename Tree::const_reference_type;

        using Observers = ::std::tuple<_Observers...>;
        using ObserversReference = Observers &;
        using ConstObserversReference = Observers const &;

    private:
        Observers m_observers;

    public:
        constexpr ObserversReference tuple() noexcept { return m_observers; }
        constexpr ConstObserversReference tuple() const noexcept { return m_observers; }

        template < ::std::size_t _I>
        constexpr decltype(auto) get() noexcept
        {
            return ::std::get<_I>(m_observers);
        }
        template < ::std::size_t _I>
        constexpr decltype(auto) get() const noexcept
        {
            return ::std::get<_I>(m_observers);
        }

        template <typename _Type>
        constexpr decltype(auto) get() noexcept
        {
            return ::std::get<_Type>(m_observers);
        }
        template <typename _Type>
        constexpr decltype(auto) get() const noexcept
        {
            return ::std::get<_Type>(m_observers);
        }

    private:
        constexpr void beforeClear()
        {
            ::std::apply([](auto &&... observers) { ((observers.beforeClear()), ...); }, m_observers);
        }

        constexpr void afterInsert(NodeItem item)
        {
            ::std::apply([&](auto &&... observers) {
                ((observers.afterInsert(item)), ...);
            }, m_observers);
        }

        constexpr void beforeErase(ConstNodeItem item)
        {
            ::std::apply([&](auto &&... observers) {
                ((observers.beforeErase(item)), ...);
            }, m_observers);
        }

        constexpr void
        onChange(const_reference_type old_value_type, const_reference_type new_value_type)
        {
            ::std::apply([&](auto &&... observers) {
                ((observers.onChange(old_value_type, new_value_type)), ...);
            }, m_observers);
        }
    };
} // namespace hierarchy

#endif
