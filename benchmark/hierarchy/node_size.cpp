// Code-size counterpart to node_gbench.cpp: one externally visible wrapper per
// operation, so -ffunction-sections gives each its own section.
//
// Never linked or run. The wrappers exist to be compiled, not called.

#include <scl/utility/hierarchy/algorithm.h>
#include <scl/utility/hierarchy/node.h>

#include <cstddef>

namespace scl::benchmarks
{

    using node = ::scl::hierarchy::node<int>;

    ::std::size_t walk_children(node const & parent)
    {
        ::std::size_t count = 0;
        for ([[maybe_unused]]
             auto const & child : parent)
            ++count;
        return count;
    }

    ::std::size_t walk_recursive(node const & parent)
    {
        ::std::size_t count = 1;
        for (auto const & child : parent)
            count += walk_recursive(child);
        return count;
    }

    bool ancestor_of(node const & parent, node const & value)
    {
        return ::scl::hierarchy::is_ancestor_of(parent, value);
    }

    bool parent_of(node const & parent, node const & value)
    {
        return ::scl::hierarchy::is_parent_of(parent, value);
    }

} // namespace scl::benchmarks
