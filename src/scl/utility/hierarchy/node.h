#pragma once

/// @file node.h
/// @brief Parent-child tree node.
/// @ingroup scl_utility_hierarchy

#include <algorithm>
#include <concepts>
#include <list>
#include <memory>
#include <type_traits>

namespace scl::hierarchy
{
    template <typename Payload, template <typename> class Allocator = ::std::allocator>
    class node
    {
    public:
        using payload = Payload;
        using payload_reference = payload &;
        using const_payload_reference = payload const &;

        using nodes = ::std::list<node, Allocator<node>>;

        using value_type = node;
        using allocator_type = nodes::allocator_type;
        using size_type = nodes::size_type;
        using difference_type = nodes::difference_type;

        using pointer = nodes::pointer;
        using const_pointer = nodes::const_pointer;
        using node_pointer = node *;
        using reference = nodes::reference;
        using const_reference = nodes::const_reference;
        using iterator = nodes::iterator;
        using const_iterator = nodes::const_iterator;
        using reverse_iterator = nodes::reverse_iterator;
        using const_reverse_iterator = nodes::const_reverse_iterator;

    private:
        node_pointer m_parent{};
        nodes m_nodes;
        payload m_payload;

    public:
        constexpr node(node const & other) /**/
            noexcept(::std::is_nothrow_copy_constructible_v<nodes> && ::std::is_nothrow_copy_constructible_v<payload>)
            requires(::std::is_copy_constructible_v<nodes> && ::std::is_copy_constructible_v<payload>)
            : m_nodes{other.m_nodes}
            , m_payload{other.m_payload}
        {
            ::std::for_each (m_nodes.begin(), m_nodes.end(), [parent = this](reference child) {
                child.m_parent = parent;
            });
        }

        constexpr node(node && other) /**/
            noexcept(::std::is_nothrow_move_constructible_v<nodes> && ::std::is_nothrow_move_constructible_v<payload>)
            requires(::std::is_move_constructible_v<nodes> && ::std::is_move_constructible_v<payload>)
            : m_nodes{::std::move(other.m_nodes)}
            , m_payload{::std::move(other.m_payload)}
        {
            ::std::for_each (m_nodes.begin(), m_nodes.end(), [parent = this](reference child) {
                child.m_parent = parent;
            });
        }

        template <typename... Arguments>
        constexpr node(Arguments &&... arguments) /**/
            noexcept(::std::is_nothrow_constructible_v<payload, Arguments...>)
            requires(::std::constructible_from<payload, Arguments...>)
            : m_payload{::std::forward<Arguments>(arguments)...}
        {}

        constexpr ~node() = default;
        constexpr void operator=(node const &) = delete;
        constexpr void operator=(node &&) = delete;

        [[nodiscard]]
        constexpr allocator_type get_allocator() const noexcept
        {
            return m_nodes.get_allocator();
        }

        [[nodiscard]]
        constexpr payload_reference get() noexcept
        {
            return m_payload;
        }

        [[nodiscard]]
        constexpr const_payload_reference get() const noexcept
        {
            return m_payload;
        }

        [[nodiscard]]
        constexpr bool has_parent() const noexcept
        {
            return m_parent != nullptr;
        }

        [[nodiscard]]
        constexpr reference parent() noexcept
        {
            return *m_parent;
        }

        [[nodiscard]]
        constexpr const_reference parent() const noexcept
        {
            return *m_parent;
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
            return m_nodes.front();
        }

        [[nodiscard]]
        constexpr const_reference front() const noexcept
        {
            return m_nodes.front();
        }

        [[nodiscard]]
        constexpr reference back() noexcept
        {
            return m_nodes.back();
        }

        [[nodiscard]]
        constexpr const_reference back() const noexcept
        {
            return m_nodes.back();
        }

        template <typename Argument>
        constexpr iterator push_back(Argument && argument) /**/
            noexcept(noexcept(insert(cend(), ::std::forward<Argument>(argument))))
            requires(::std::constructible_from<node, Argument>)
        {
            return insert(cend(), ::std::forward<Argument>(argument));
        }

        template <typename... Arguments>
        constexpr iterator emplace_back(Arguments &&... arguments) /**/
            noexcept(noexcept(emplace(cend(), ::std::forward<Arguments>(arguments)...)))
            requires(::std::constructible_from<node, Arguments...>)
        {
            return emplace(cend(), ::std::forward<Arguments>(arguments)...);
        }

        template <typename Argument>
        constexpr iterator push_front(Argument && argument) /**/
            noexcept(noexcept(insert(cbegin(), ::std::forward<Argument>(argument))))
            requires(::std::constructible_from<node, Argument>)
        {
            return insert(cbegin(), ::std::forward<Argument>(argument));
        }

        template <typename... Arguments>
        constexpr iterator emplace_front(Arguments &&... arguments) /**/
            noexcept(noexcept(emplace(cbegin(), ::std::forward<Arguments>(arguments)...)))
            requires(::std::constructible_from<node, Arguments...>)
        {
            return emplace(cbegin(), ::std::forward<Arguments>(arguments)...);
        }

        template <typename Argument>
        constexpr iterator insert(const_iterator where, Argument && argument) /**/
            noexcept(noexcept(emplace(where, ::std::forward<Argument>(argument))))
            requires(::std::constructible_from<node, Argument>)
        {
            return emplace(where, ::std::forward<Argument>(argument));
        }

        template <typename... Arguments>
        constexpr iterator emplace(const_iterator where, Arguments &&... arguments) /**/
            noexcept(noexcept(m_nodes.emplace(where, ::std::forward<Arguments>(arguments)...)))
            requires(::std::constructible_from<node, Arguments...>)
        {
            iterator result = m_nodes.emplace(where, ::std::forward<Arguments>(arguments)...);
            result->m_parent = this;
            return result;
        }

        constexpr void pop_back() noexcept { m_nodes.pop_back(); }
        constexpr void pop_front() noexcept { m_nodes.pop_front(); }

        constexpr iterator erase(const_iterator position) noexcept
        {
            return m_nodes.erase(position);
        }

        constexpr iterator erase(const_iterator first, const_iterator last) noexcept
        {
            return m_nodes.erase(first, last);
        }

        constexpr void clear() noexcept { m_nodes.clear(); }

        [[nodiscard]]
        constexpr node take(const_iterator position) /**/
            noexcept(::std::is_nothrow_move_constructible_v<node>)
        {
            node & child = const_cast<node &>(*position);
            child.m_parent = nullptr;
            node result = ::std::move(child);
            m_nodes.erase(position);
            return result;
        }

        [[nodiscard]]
        constexpr node take_first() /**/
            noexcept(noexcept(take(cbegin())))
        {
            return take(cbegin());
        }

        [[nodiscard]]
        constexpr node take_last() /**/
            noexcept(noexcept(take(--cend())))
        {
            return take(--cend());
        }

        constexpr void transfer(reference from) noexcept { transfer(cend(), from); }

        constexpr void transfer(reference from, const_iterator first) noexcept
        {
            transfer(cend(), from, first);
        }

        constexpr void transfer(reference from, const_iterator first, const_iterator last) noexcept
        {
            transfer(cend(), from, first, last);
        }

        constexpr void transfer(const_iterator where, reference from) noexcept
        {
            ::std::for_each (from.begin(), from.end(), [parent = this](reference child) {
                child.m_parent = parent;
            });
            m_nodes.splice(where, from.m_nodes);
        }

        constexpr void transfer(const_iterator where, reference from, const_iterator it) noexcept
        {
            const_cast<node &>(*it).m_parent = this;
            m_nodes.splice(where, from.m_nodes, it);
        }

        constexpr void
        transfer(const_iterator where, reference from, const_iterator first, const_iterator last) noexcept
        {
            ::std::for_each (first, last, [parent = this](const_reference child) {
                const_cast<node &>(child).m_parent = parent;
            });
            m_nodes.splice(where, from.m_nodes, first, last);
        }

        [[nodiscard]]
        constexpr iterator begin() noexcept
        {
            return m_nodes.begin();
        }

        [[nodiscard]]
        constexpr iterator end() noexcept
        {
            return m_nodes.end();
        }

        [[nodiscard]]
        constexpr const_iterator begin() const noexcept
        {
            return m_nodes.begin();
        }

        [[nodiscard]]
        constexpr const_iterator end() const noexcept
        {
            return m_nodes.end();
        }

        [[nodiscard]]
        constexpr const_iterator cbegin() const noexcept
        {
            return m_nodes.cbegin();
        }

        [[nodiscard]]
        constexpr const_iterator cend() const noexcept
        {
            return m_nodes.cend();
        }

        [[nodiscard]]
        constexpr reverse_iterator rbegin() noexcept
        {
            return m_nodes.rbegin();
        }

        [[nodiscard]]
        constexpr reverse_iterator rend() noexcept
        {
            return m_nodes.rend();
        }

        [[nodiscard]]
        constexpr const_reverse_iterator rbegin() const noexcept
        {
            return m_nodes.rbegin();
        }

        [[nodiscard]]
        constexpr const_reverse_iterator rend() const noexcept
        {
            return m_nodes.rend();
        }

        [[nodiscard]]
        constexpr const_reverse_iterator crbegin() const noexcept
        {
            return m_nodes.crbegin();
        }

        [[nodiscard]]
        constexpr const_reverse_iterator crend() const noexcept
        {
            return m_nodes.crend();
        }
    };

} // namespace scl::hierarchy

namespace scl::hierarchy::concepts
{
    namespace detail
    {
        template <typename T>
        struct is_node : ::std::false_type
        {};

        template <typename Payload, template <typename> class Allocator>
        struct is_node<::scl::hierarchy::node<Payload, Allocator>> : ::std::true_type
        {};
    } // namespace detail

    /// @brief Satisfied by any `scl::hierarchy::node<Payload, Allocator>` instantiation.
    /// @ingroup scl_utility_hierarchy
    ///
    /// Used by `tree.h` to narrow its generic proxy-shape concept: a type must
    /// unwrap (via `.leaf()`) to a real `node`, not merely something with a matching
    /// method name, before it is accepted as a node-reference proxy.
    template <typename T>
    concept node = detail::is_node<T>::value;
} // namespace scl::hierarchy::concepts

namespace scl::hierarchy
{
    // ADL hooks implementing the scl::hierarchy::parent_of / has_parent /
    // are_identical contract (see algorithm.h) for node. Internal — not part
    // of the public API; call parent_of / has_parent / are_identical instead.
    // adl_parent has two overloads (mutable / const) rather than one
    // forwarding-reference template, so constness of the parent always
    // matches constness of self.
    template <typename Payload, template <typename> class Allocator>
    [[nodiscard]]
    constexpr node<Payload, Allocator> & adl_parent(node<Payload, Allocator> & self) /**/
        noexcept(noexcept(self.parent()))
    {
        return self.parent();
    }

    template <typename Payload, template <typename> class Allocator>
    [[nodiscard]]
    constexpr node<Payload, Allocator> const & adl_parent(node<Payload, Allocator> const & self) /**/
        noexcept(noexcept(self.parent()))
    {
        return self.parent();
    }

    template <typename Payload, template <typename> class Allocator>
    [[nodiscard]]
    constexpr bool adl_has_parent(node<Payload, Allocator> const & self) /**/
        noexcept(noexcept(self.has_parent()))
    {
        return self.has_parent();
    }

    template <typename Payload, template <typename> class Allocator>
    [[nodiscard]]
    constexpr void const * adl_identity(node<Payload, Allocator> const & self) noexcept
    {
        return ::std::addressof(self);
    }

} // namespace scl::hierarchy

// =============================================================================
// scl::hierarchy::node — Doxygen documentation
// =============================================================================

// -----------------------------------------------------------------------------
// Class overview
// -----------------------------------------------------------------------------

/// @class scl::hierarchy::node
/// @ingroup scl_utility_hierarchy
/// @brief Tree node that owns zero or more typed child nodes and maintains a
///        back-pointer to its parent.
///
/// `node` plays two roles simultaneously: it is a **value container** (stores a
/// `payload`) and a **sequence container** (owns a `std::list` of child nodes of
/// the same type).  The two roles are reflected in the type-alias design:
///
/// | Alias                     | Refers to                     | Purpose                        |
/// |---------------------------|-------------------------------|--------------------------------|
/// | `payload`                 | `Payload`                     | Type of the stored value       |
/// | `payload_reference`       | `Payload &`                   | Mutable payload access         |
/// | `const_payload_reference` | `Payload const &`             | Immutable payload access       |
/// | `value_type`              | `node<Payload, Allocator>`    | STL element type (child node)  |
/// | `reference`               | `value_type &`              | STL reference to a child node  |
/// | `iterator`                | list iterator               | Mutable child iterator         |
///
/// @note `value_type` is the **node** itself, not `Value`.  This follows the STL
///       container convention where `value_type` is the element stored in the
///       container.  Use `payload` / `payload_reference` to access the stored value.
///
/// ### Parent tracking
///
/// Every node holds a nullable raw pointer to its parent.  A freshly constructed
/// node has no parent (`has_parent()` returns `false`).  Inserting a node as a
/// child sets its parent pointer; erasing or taking it clears the pointer.
///
/// ### Copy and move semantics
///
/// - **Copy** — deep-copies the entire subtree; the result has no parent.
/// - **Move** — transfers the child list in O(1); children's parent pointers are
///   rewritten to the destination; the moved-from node is left empty.
/// - **Assignment** — deleted.  Use `transfer()` to move children between nodes.
///
/// ### Insertion policy
///
/// - `push_back` / `push_front` / `insert` accept an existing child `node` (by
///   copy or by move) and return an `iterator` to the inserted child.
/// - `emplace_back` / `emplace_front` / `emplace` construct a child's `payload`
///   in-place from forwarded arguments and return an `iterator`.
///
/// @note `push_back` / `push_front` return `iterator` rather than `void` — a
///       deliberate deviation from `std::list` that avoids a redundant linear
///       search to recover the position after insertion.
///
/// ### Zero-copy subtree transfer
///
/// `transfer()` wraps `std::list::splice`: O(1), no allocation, no copy.  The six
/// overloads mirror the three splice forms (whole list / single element / subrange)
/// with an optional `where` prefix that controls the insertion point.
///
/// @tparam Payload   Payload type stored in each node.  Must be at minimum
///                   move-constructible for `emplace`; copy-constructible for
///                   `push_back` and copy construction.
/// @tparam Allocator Rebindable allocator template applied to the internal child
///                   list.  Defaults to `std::allocator`.
///
/// @par Building a tree and querying relationships
/// @code
/// using Node = scl::hierarchy::node<std::string>;
///
/// Node root{"root"};
/// auto a_it = root.emplace_back("a");        // root → a
/// auto b_it = a_it->emplace_back("b");       // root → a → b
/// b_it->emplace_back("c");                   // root → a → b → c
///
/// assert(!scl::hierarchy::has_parent(root));
/// assert( scl::hierarchy::has_parent(*a_it));
/// assert( scl::hierarchy::is_parent_of(root, *a_it));
/// assert(!scl::hierarchy::is_parent_of(root, *b_it));   // b is a grandchild
/// assert( scl::hierarchy::is_ancestor_of(root, root.front().front().front()));
/// @endcode
///
/// @par Deep copy
/// @code
/// Node src{"src"};
/// src.emplace_back("x");
/// src.front().emplace_back("y");   // grandchild
///
/// Node copy = src;                 // deep copy; copy.has_parent() == false
/// assert(copy.size() == 1);
/// assert(copy.front().size() == 1);
/// assert(&copy.front().parent() == &copy);   // parent pointers correct
/// @endcode
///
/// @par Zero-copy subtree transfer
/// @code
/// Node donor{"donor"};
/// donor.emplace_back("x");
/// donor.emplace_back("y");
///
/// Node sink{"sink"};
/// sink.transfer(donor);            // O(1) splice; donor is now empty
/// assert(donor.empty());
/// assert(sink.size() == 2);
/// assert(scl::hierarchy::is_parent_of(sink, sink.front()));
/// assert(scl::hierarchy::is_parent_of(sink, sink.back()));
/// @endcode
///
/// @par Extracting a subtree
/// @code
/// Node root{"root"};
/// root.emplace_back("a");
/// root.emplace_back("b");
/// root.emplace_back("c");
///
/// Node b = root.take(std::next(root.cbegin()));   // detach "b"
/// assert(b.get() == "b");
/// assert(!b.has_parent());
/// assert(root.size() == 2);                       // "a" and "c" remain
/// @endcode

// -----------------------------------------------------------------------------
// Type aliases
// -----------------------------------------------------------------------------

/// @typedef scl::hierarchy::node::payload
/// @brief Payload type stored in this node (same as template parameter `Payload`).

/// @typedef scl::hierarchy::node::payload_reference
/// @brief Mutable lvalue reference to the payload (`payload &`).

/// @typedef scl::hierarchy::node::const_payload_reference
/// @brief Immutable lvalue reference to the payload (`payload const &`).

/// @typedef scl::hierarchy::node::nodes
/// @brief Internal child-list container type (`std::list<node, Allocator<node>>`).

/// @typedef scl::hierarchy::node::value_type
/// @brief STL element type of the child container — the node itself, not `payload`.
///        Required for STL range and algorithm compatibility (e.g. `std::ranges::for_each`).

/// @typedef scl::hierarchy::node::pointer
/// @brief Pointer to `value_type` (`node *`), derived from `nodes`.

/// @typedef scl::hierarchy::node::const_pointer
/// @brief Const pointer to `value_type` (`node const *`), derived from `nodes`.

/// @typedef scl::hierarchy::node::reference
/// @brief Lvalue reference to `value_type` (`node &`), derived from `nodes`.

/// @typedef scl::hierarchy::node::const_reference
/// @brief Const lvalue reference to `value_type` (`node const &`), derived from `nodes`.

/// @typedef scl::hierarchy::node::iterator
/// @brief Mutable bidirectional iterator over direct children, derived from `nodes`.

/// @typedef scl::hierarchy::node::const_iterator
/// @brief Immutable bidirectional iterator over direct children, derived from `nodes`.

/// @typedef scl::hierarchy::node::reverse_iterator
/// @brief Mutable reverse bidirectional iterator over direct children, derived from `nodes`.

/// @typedef scl::hierarchy::node::const_reverse_iterator
/// @brief Immutable reverse bidirectional iterator over direct children, derived from `nodes`.

/// @typedef scl::hierarchy::node::size_type
/// @brief Unsigned integer type for child count, derived from `nodes`.

/// @typedef scl::hierarchy::node::difference_type
/// @brief Signed integer type for iterator distances, derived from `nodes`.

/// @typedef scl::hierarchy::node::allocator_type
/// @brief Allocator type used by the internal child list, derived from `nodes`.

// -----------------------------------------------------------------------------
// Constructors
// -----------------------------------------------------------------------------

/// @fn scl::hierarchy::node::node(node const &)
/// @brief Deep-copies @p other and its entire descendant subtree.
///
/// Every node in the subtree is copy-constructed.  After construction, each
/// child's parent pointer points into the newly created tree, not into the
/// source tree.  The resulting node has no parent regardless of `other`'s
/// parent state.
///
/// @param other  Source node to copy from.
///
/// @par Example
/// @code
/// Node src{"root"};
/// src.emplace_back("child");
/// src.front().emplace_back("grandchild");
///
/// Node copy = src;
/// assert(!copy.has_parent());                      // copy is a root
/// assert(copy.size() == 1);
/// assert(copy.front().size() == 1);                // grandchild copied too
/// assert(&copy.front().parent() == &copy);         // parent → copy, not src
/// @endcode

/// @fn scl::hierarchy::node::node(node &&)
/// @brief Moves the child list from @p other in O(1) and resets parent pointers.
///
/// The underlying `std::list` is moved (O(1)).  Every transferred child's
/// `m_parent` is rewritten to `this`.  The moved-from node is left with no
/// children and no parent; its `payload` is in a valid but unspecified state.
///
/// @param other  Source node to move from.
///
/// @par Example
/// @code
/// Node src{"root"};
/// src.emplace_back("a");
/// src.emplace_back("b");
///
/// Node dst = std::move(src);
/// assert(src.empty());                          // src has no children
/// assert(dst.size() == 2);
/// assert(&dst.front().parent() == &dst);        // parent pointers updated
/// assert(&dst.back().parent()  == &dst);
/// @endcode

/// @fn scl::hierarchy::node::node(Arguments &&...)
/// @brief Constructs the payload in-place by forwarding @p arguments.
///
/// The node is created with no children and no parent.  Enabled only when
/// `payload` is constructible from `Arguments...`.  This constructor is
/// intentionally greedy (unconstrained forwarding) and acts as a catch-all for
/// non-node arguments; the four explicit `node` overloads take priority for
/// copy/move from another `node`.
///
/// @tparam Arguments  Argument types forwarded to the `payload` constructor.
/// @param  arguments  Arguments forwarded to `payload{arguments...}`.
///
/// @par Example
/// @code
/// scl::hierarchy::node<std::string> a{"hello"};          // payload = "hello"
/// scl::hierarchy::node<int>         b{42};               // payload = 42
/// scl::hierarchy::node<int>         c{};                 // payload = int{} = 0
/// scl::hierarchy::node<std::pair<int,int>> d{1, 2};      // piecewise pair
/// @endcode

// -----------------------------------------------------------------------------
// Payload access
// -----------------------------------------------------------------------------

/// @fn scl::hierarchy::node::get()
/// @brief Returns a mutable reference to the stored payload.
/// @return `payload_reference`
///
/// @par Example
/// @code
/// scl::hierarchy::node<int> n{10};
/// n.get() += 5;
/// assert(n.get() == 15);
/// @endcode

/// @fn scl::hierarchy::node::get() const
/// @brief Returns an immutable reference to the stored payload.
/// @return `const_payload_reference`

// -----------------------------------------------------------------------------
// Parent access
// -----------------------------------------------------------------------------

/// @fn scl::hierarchy::node::has_parent() const
/// @brief Returns `true` when this node is attached to a parent.
///
/// A node has no parent immediately after construction, after being moved from,
/// and after being detached via `take()`.
///
/// @par Example
/// @code
/// Node root{"root"};
/// assert(!root.has_parent());
///
/// auto it = root.emplace_back("child");
/// assert(it->has_parent());
///
/// Node taken = root.take_first();
/// assert(!taken.has_parent());    // detached
/// @endcode

/// @fn scl::hierarchy::node::parent()
/// @brief Returns a mutable reference to the parent node.
///
/// @pre `has_parent()` must be `true`; dereferencing a null pointer otherwise.
///
/// @par Example
/// @code
/// Node root{"root"};
/// auto it = root.emplace_back("child");
/// assert(&it->parent() == &root);
/// it->parent().get() = "renamed";   // mutate parent through child
/// @endcode

/// @fn scl::hierarchy::node::parent() const
/// @brief Returns an immutable reference to the parent node.
/// @pre `has_parent()` must be `true`; dereferencing a null pointer otherwise.

// -----------------------------------------------------------------------------
// Argument container observers
// -----------------------------------------------------------------------------

/// @fn scl::hierarchy::node::empty() const
/// @brief Returns `true` when this node has no direct children.

/// @fn scl::hierarchy::node::size() const
/// @brief Returns the number of direct children.
///
/// @note `std::list::size()` is O(1) since C++11 (required by the standard).

/// @fn scl::hierarchy::node::front()
/// @brief Returns a mutable reference to the first direct child.
/// @pre `!empty()`

/// @fn scl::hierarchy::node::front() const
/// @brief Returns an immutable reference to the first direct child.
/// @pre `!empty()`

/// @fn scl::hierarchy::node::back()
/// @brief Returns a mutable reference to the last direct child.
/// @pre `!empty()`

/// @fn scl::hierarchy::node::back() const
/// @brief Returns an immutable reference to the last direct child.
/// @pre `!empty()`

// -----------------------------------------------------------------------------
// Insertion
// -----------------------------------------------------------------------------

/// @fn scl::hierarchy::node::push_back(Argument &&)
/// @brief Appends @p child to the back of the child list.
///
/// Accepts any `Argument` from which `node` is constructible — covers both
/// copy (`node const &`) and move (`node &&`) construction.  The new
/// child's parent pointer is set to `this`.
///
/// @note Returns `iterator`, not `void`, unlike `std::list::push_back`.
///
/// @tparam Argument   Any type from which `node` is constructible.
/// @param  argument  Node to insert (forwarded).
/// @return Iterator to the appended child.
///
/// @par Example
/// @code
/// Node parent{"p"};
/// Node child{"c"};
/// child.emplace_back("gc");           // child has a grandchild
///
/// auto it = parent.push_back(child);  // copy: child (and "gc") deep-copied
/// assert(it->get() == "c");
/// assert(it->size()  == 1);           // grandchild copied too
/// assert(scl::hierarchy::is_parent_of(parent, *it));
///
/// auto it2 = parent.push_back(Node{"d"});  // move: rvalue inserted directly
/// assert(it2->get() == "d");
/// @endcode

/// @fn scl::hierarchy::node::emplace_back(Arguments &&...)
/// @brief Constructs a child at the back with payload built from @p arguments.
///
/// The child's payload is constructed in-place — no temporary node is created.
/// The new child's parent pointer is set to `this`.
///
/// @tparam Arguments  Argument types forwarded to the `payload` constructor.
/// @param  arguments  Arguments forwarded to the new child's `payload`.
/// @return Iterator to the newly constructed child.
///
/// @par Example
/// @code
/// scl::hierarchy::node<std::vector<int>> root{};
/// auto it = root.emplace_back(3, 0);   // std::vector<int>(3, 0) = {0, 0, 0}
/// assert(it->get().size() == 3);
/// @endcode

/// @fn scl::hierarchy::node::push_front(Argument &&)
/// @brief Prepends @p child to the front of the child list.
///
/// Accepts any `Argument` from which `node` is constructible.  The new
/// child's parent pointer is set to `this`.
///
/// @note Returns `iterator`, not `void`, unlike `std::list::push_front`.
///
/// @tparam Argument   Any type from which `node` is constructible.
/// @param  argument  Node to insert (forwarded).
/// @return Iterator to the prepended child.

/// @fn scl::hierarchy::node::emplace_front(Arguments &&...)
/// @brief Constructs a child at the front with payload built from @p arguments.
///
/// @tparam Arguments  Argument types forwarded to the `payload` constructor.
/// @param  arguments  Arguments forwarded to the new child's `payload`.
/// @return Iterator to the newly constructed child.

/// @fn scl::hierarchy::node::insert(const_iterator, Argument &&)
/// @brief Inserts @p child at @p where.
///
/// Accepts any `Argument` from which `node` is constructible.  The inserted
/// child's parent pointer is set to `this`.
///
/// @tparam Argument   Any type from which `node` is constructible.
/// @param  where   Valid iterator into this node's child list.
/// @param  argument  Node to insert (forwarded).
/// @return Iterator to the inserted child.
///
/// @par Example
/// @code
/// Node parent{};
/// parent.emplace_back(1);
/// parent.emplace_back(3);
///
/// auto mid = std::next(parent.cbegin());   // points at "3"
/// parent.insert(mid, Node{2});             // inserts "2" before "3"
///
/// int expected = 1;
/// for (auto const & child : parent)
///     assert(child.get() == expected++); // 1, 2, 3
/// @endcode

/// @fn scl::hierarchy::node::emplace(const_iterator, Arguments &&...)
/// @brief Constructs a child in-place where @p where.
///
/// @tparam Arguments  Argument types forwarded to the `payload` constructor.
/// @param  where  Valid iterator into this node's child list.
/// @param  arguments        Arguments forwarded to the new child's `payload`.
/// @return Iterator to the newly constructed child.

// -----------------------------------------------------------------------------
// Removal
// -----------------------------------------------------------------------------

/// @fn scl::hierarchy::node::pop_back()
/// @brief Removes and destroys the last child and its subtree.
/// @pre `!empty()`

/// @fn scl::hierarchy::node::pop_front()
/// @brief Removes and destroys the first child and its subtree.
/// @pre `!empty()`

/// @fn scl::hierarchy::node::erase(const_iterator)
/// @brief Erases the child at @p position and destroys its subtree.
///
/// @param  position  Valid iterator to a direct child of this node.
/// @return Iterator to the child that followed the erased one, or `end()`.
///
/// @par Example
/// @code
/// Node parent{};
/// parent.emplace_back(1);
/// auto it2 = parent.emplace_back(2);
/// parent.emplace_back(3);
///
/// parent.erase(it2);
/// assert(parent.size() == 2);
/// assert(parent.front().get() == 1);
/// assert(parent.back().get()  == 3);
/// @endcode

/// @fn scl::hierarchy::node::erase(const_iterator, const_iterator)
/// @brief Erases children in [@p first, @p last) and destroys their subtrees.
///
/// @param  first  Begin of the range to erase.
/// @param  last   Past-the-end of the range to erase (not erased).
/// @return Iterator to the child that followed the last erased one, or `end()`.

/// @fn scl::hierarchy::node::clear()
/// @brief Removes and destroys all direct children and their subtrees.
/// @post `empty()` returns `true`.

// -----------------------------------------------------------------------------
// Extraction
// -----------------------------------------------------------------------------

/// @fn scl::hierarchy::node::take(const_iterator)
/// @brief Detaches the child at @p position and returns it as a free-standing node.
///
/// The extracted node's own parent pointer is cleared (`has_parent()` returns
/// `false`).  Its descendants' parent pointers are unchanged — they still point
/// within the extracted subtree.
///
/// @param  position  Valid iterator to a direct child of this node.
/// @return The extracted child node (moved out, no parent).
///
/// @par Example
/// @code
/// Node root{"root"};
/// root.emplace_back("a");
/// root.emplace_back("b").emplace_back("b1");   // "b" has child "b1"
/// root.emplace_back("c");
///
/// Node b = root.take(std::next(root.cbegin())); // extract "b"
/// assert(b.get() == "b");
/// assert(!b.has_parent());
/// assert(b.size() == 1);                        // "b1" came along
/// assert(&b.front().parent() == &b);            // parent ptr intact
/// assert(root.size() == 2);                     // "a" and "c" remain
/// @endcode

/// @fn scl::hierarchy::node::take_first()
/// @brief Detaches and returns the first child as a free-standing node.
/// @pre `!empty()`
/// @return The extracted first child (no parent).

/// @fn scl::hierarchy::node::take_last()
/// @brief Detaches and returns the last child as a free-standing node.
/// @pre `!empty()`
/// @return The extracted last child (no parent).

// -----------------------------------------------------------------------------
// Transfer (zero-copy splice)
// -----------------------------------------------------------------------------

/// @fn scl::hierarchy::node::transfer(reference)
/// @brief Moves **all** children of @p from_node to the back of this node's child
///        list.  O(1); no allocation, no element copy or move.
///
/// @p from_node is left empty after the call.  All transferred children's parent
/// pointers are updated to `this`.
///
/// @param from  Source node.  Must not alias `*this`.
///
/// @par Example
/// @code
/// Node src{"src"};
/// src.emplace_back("a");
/// src.emplace_back("b");
///
/// Node dst{"dst"};
/// dst.emplace_back("x");
/// dst.transfer(src);         // append src's children after "x"
///
/// assert(src.empty());
/// assert(dst.size() == 3);   // x, a, b
/// for (Node const & child : dst)
///     assert(scl::hierarchy::is_parent_of(dst, child));
/// @endcode

/// @fn scl::hierarchy::node::transfer(reference, const_iterator)
/// @brief Moves the single child at @p first in @p from to the back
///        of this node's child list.  O(1).
///
/// @param from      Source node.
/// @param first     Iterator to the child to transfer.
///
/// @par Example
/// @code
/// Node src{"src"};
/// src.emplace_back("a");
/// src.emplace_back("b");
///
/// Node dst{"dst"};
/// dst.transfer(src, src.cbegin());  // transfer only "a"
///
/// assert(src.size() == 1 && src.front().get() == "b");
/// assert(dst.size() == 1 && dst.front().get() == "a");
/// assert(scl::hierarchy::is_parent_of(dst, dst.front()));
/// @endcode

/// @fn scl::hierarchy::node::transfer(reference, const_iterator, const_iterator)
/// @brief Moves children in [@p from_first, @p from_last) from @p from_node to the
///        back of this node's child list.  O(distance).
///
/// @param from        Source node.
/// @param first       Begin of the range to transfer.
/// @param last        Past-the-end of the range to transfer (not transferred).

/// @fn scl::hierarchy::node::transfer(const_iterator, reference)
/// @brief Moves **all** children of @p from_node to where @p where.
///        O(1).
///
/// @param where  Insertion point in this node.
/// @param from   Source node.  Must not alias `*this`.

/// @fn scl::hierarchy::node::transfer(const_iterator, reference, const_iterator)
/// @brief Moves the single child at @p first in @p from to where
///        @p where.  O(1).
///
/// @param where      Insertion point in this node.
/// @param from       Source node.
/// @param first      Iterator to the child to transfer.

/// @fn scl::hierarchy::node::transfer(const_iterator, reference, const_iterator, const_iterator)
/// @brief Moves children in [@p from_first, @p from_last) from @p from_node to
///        where @p where.  O(distance).
///
/// @param where      Insertion point in this node.
/// @param from       Source node.
/// @param first      Begin of the range to transfer.
/// @param last       Past-the-end of the range to transfer (not transferred).

// -----------------------------------------------------------------------------
// Iteration
// -----------------------------------------------------------------------------

/// @fn scl::hierarchy::node::begin()
/// @brief Returns a mutable iterator to the first direct child.
///        Equal to `end()` when `empty()`.

/// @fn scl::hierarchy::node::end()
/// @brief Returns a mutable past-the-end sentinel for the direct child list.

/// @fn scl::hierarchy::node::begin() const
/// @brief Returns an immutable iterator to the first direct child.

/// @fn scl::hierarchy::node::end() const
/// @brief Returns an immutable past-the-end sentinel for the direct child list.

/// @fn scl::hierarchy::node::cbegin() const
/// @brief Returns an immutable iterator to the first direct child.

/// @fn scl::hierarchy::node::cend() const
/// @brief Returns an immutable past-the-end sentinel for the direct child list.

/// @fn scl::hierarchy::node::rbegin()
/// @brief Returns a mutable reverse iterator to the last direct child.

/// @fn scl::hierarchy::node::rend()
/// @brief Returns a mutable reverse past-the-end sentinel for the direct child list.

/// @fn scl::hierarchy::node::rbegin() const
/// @brief Returns an immutable reverse iterator to the last direct child.

/// @fn scl::hierarchy::node::rend() const
/// @brief Returns an immutable reverse past-the-end sentinel for the direct child list.

/// @fn scl::hierarchy::node::crbegin() const
/// @brief Returns an immutable reverse iterator to the last direct child.

/// @fn scl::hierarchy::node::crend() const
/// @brief Returns an immutable reverse past-the-end sentinel for the direct child list.
