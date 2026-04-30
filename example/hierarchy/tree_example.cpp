/// @file tree_example.cpp
/// @brief Demonstrates scl::hierarchy::tree with an observer and tuple_of_observers.
///
/// Shows how to use the observer-aware tree wrapper, query hierarchy
/// relationships via node_wrapper, and compose multiple observers with
/// tuple_of_observers.

#include <scl/utility/hierarchy/tree.h>

#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// ============================================================================
// Observer 1: index — maps each value to its node_wrapper_iterator
// ============================================================================

template <typename Tree>
class index_observer
{
public:
    using iterator = typename Tree::node_wrapper_iterator;
    using const_iterator = typename Tree::const_node_wrapper_iterator;
    using value_type = typename Tree::value_type;

    void after_insert(iterator it) { m_index[(*it).value()] = it; }
    void before_erase(const_iterator it) { m_index.erase((*it).value()); }
    void before_clear() { m_index.clear(); }
    void on_change(value_type const & old_val, value_type const &) { m_index.erase(old_val); }

    iterator find(value_type const & val)
    {
        auto pos = m_index.find(val);
        return pos != m_index.end() ? pos->second : iterator{};
    }

    ::std::size_t size() const { return m_index.size(); }

private:
    ::std::unordered_map<value_type, iterator> m_index;
};

// ============================================================================
// Observer 2: counter — counts all nodes currently in the tree
// ============================================================================

template <typename Tree>
struct count_observer
{
    ::std::size_t count = 0;
    void after_insert(typename Tree::node_wrapper_iterator) { ++count; }
    void before_erase(typename Tree::const_node_wrapper_iterator) { --count; }
    void before_clear() { count = 0; }
    void on_change(typename Tree::const_reference_type, typename Tree::const_reference_type) {}
};

// ============================================================================
// Tree type wiring
// ============================================================================

template <typename T>
using composed_observer = scl::hierarchy::tuple_of_observers<T, index_observer<T>, count_observer<T>>;

using string_tree = scl::hierarchy::tree<::std::string, composed_observer>;

// ============================================================================
// main
// ============================================================================

int main()
{
    string_tree tree;

    // ---- Build tree ----

    auto world_it = tree.push_back("World");
    auto m1_it = (*world_it).push_back("Model1");
    auto m2_it = (*m1_it).push_back("Model2");
    auto m3_it = (*world_it).push_back("Model3");

    ::std::cout << "Node count: " << tree.observer().get<count_observer<string_tree>>().count << '\n';
    // Expected: 4

    // ---- Query via index observer ----

    auto & idx = tree.observer().get<index_observer<string_tree>>();

    auto found = idx.find("Model1");
    assert((*found).value() == "Model1");
    ::std::cout << "Found: " << (*found).value() << '\n';

    // ---- Hierarchy query on node_wrapper ----

    assert(scl::hierarchy::are_identical(*world_it, *world_it));
    assert(!scl::hierarchy::are_identical(*m1_it, *m2_it));
    assert(scl::hierarchy::are_sibling(*m1_it, *m3_it));
    assert(!scl::hierarchy::are_sibling(*m2_it, *m3_it));
    assert(scl::hierarchy::is_parent_of(*m1_it, *m2_it));
    assert(!scl::hierarchy::is_parent_of(*world_it, *m2_it));
    assert(scl::hierarchy::is_ancestor_of(*world_it, *m2_it));

    // ---- set_value triggers on_change ----

    (*m3_it).set_value("Model3.1");
    assert((*m3_it).value() == "Model3.1");

    // ---- transfer ----

    assert(scl::hierarchy::is_ancestor_of(*m1_it, *m2_it));
    (*m3_it).transfer(*m1_it);
    assert(scl::hierarchy::is_ancestor_of(*m3_it, *m2_it));

    // ---- range-based for ----

    ::std::cout << "World children:\n";
    for (auto n : *world_it)
        ::std::cout << "  " << n.value() << '\n';

    // ---- release / re-insert ----

    auto raw = (*world_it).release(m1_it);
    ::std::cout << "Released: " << raw.value() << '\n';
    auto reinserted = (*world_it).push_back(raw.value());
    ::std::cout << "Reinserted: " << (*reinserted).value() << '\n';

    // ---- erase ----

    tree.erase(world_it);
    ::std::cout << "Count after erase: " << tree.observer().get<count_observer<string_tree>>().count << '\n';
    // Expected: 0

    return 0;
}
