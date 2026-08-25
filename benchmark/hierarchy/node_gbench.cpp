#include <benchmark/benchmark.h>
#include <scl/utility/hierarchy/algorithm.h>
#include <scl/utility/hierarchy/node.h>

#include <cstddef>

namespace
{

    using node = ::scl::hierarchy::node<int>;

    // A tree wide enough that a walk costs more than the call reaching it, and deep
    // enough that the ancestor query has something to climb.
    constexpr int fanout = 8;
    constexpr int depth = 4;

    void grow(node & parent, int level)
    {
        if (level == 0)
            return;

        for (int index = 0; index < fanout; ++index)
        {
            auto child = parent.emplace_back(level * fanout + index);
            grow(*child, level - 1);
        }
    }

    node make_tree()
    {
        node root{0};
        grow(root, depth);
        return root;
    }

    void hierarchy_build(::benchmark::State & state)
    {
        for (auto _ : state)
        {
            auto tree = make_tree();
            ::benchmark::DoNotOptimize(tree);
        }
    }

    BENCHMARK(hierarchy_build);

    void hierarchy_walk_children(::benchmark::State & state)
    {
        auto tree = make_tree();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(tree);
            ::std::size_t count = 0;
            // DoNotOptimize's const-reference overload is deprecated: it lets the
            // compiler optimise through the barrier.
            for (auto & child : tree)
            {
                ::benchmark::DoNotOptimize(child);
                ++count;
            }
            ::benchmark::DoNotOptimize(count);
        }
    }

    BENCHMARK(hierarchy_walk_children);

    void hierarchy_walk_recursive(::benchmark::State & state)
    {
        auto tree = make_tree();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(tree);
            auto total = ::std::size_t{0};
            auto visit = [&total](auto && self, node const & current) -> void {
                ++total;
                for (auto const & child : current)
                    self(self, child);
            };
            visit(visit, tree);
            ::benchmark::DoNotOptimize(total);
        }
    }

    BENCHMARK(hierarchy_walk_recursive);

    void hierarchy_is_ancestor_of(::benchmark::State & state)
    {
        auto tree = make_tree();
        auto const & leaf = *tree.begin();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(tree);
            auto value = ::scl::hierarchy::is_ancestor_of(tree, leaf);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(hierarchy_is_ancestor_of);

    // The deepest leaf, so the climb runs the whole depth rather than one step.
    void hierarchy_is_ancestor_of_deep(::benchmark::State & state)
    {
        auto tree = make_tree();
        auto const * current = &*tree.begin();
        while (!current->empty())
            current = &*current->begin();
        auto const & leaf = *current;
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(tree);
            auto value = ::scl::hierarchy::is_ancestor_of(tree, leaf);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(hierarchy_is_ancestor_of_deep);

    void hierarchy_is_parent_of(::benchmark::State & state)
    {
        auto tree = make_tree();
        auto const & child = *tree.begin();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(tree);
            auto value = ::scl::hierarchy::is_parent_of(tree, child);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(hierarchy_is_parent_of);

    void hierarchy_are_sibling(::benchmark::State & state)
    {
        auto tree = make_tree();
        auto const & first = tree.front();
        auto const & last = tree.back();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(tree);
            auto value = ::scl::hierarchy::are_sibling(first, last);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(hierarchy_are_sibling);

    void hierarchy_size(::benchmark::State & state)
    {
        auto tree = make_tree();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(tree);
            auto value = tree.size();
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(hierarchy_size);

    void hierarchy_emplace_back(::benchmark::State & state)
    {
        auto tree = make_tree();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(tree);
            auto child = tree.emplace_back(1);
            ::benchmark::DoNotOptimize(child);
            tree.pop_back();
        }
    }

    BENCHMARK(hierarchy_emplace_back);

} // namespace
