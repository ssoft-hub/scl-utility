#include <benchmark/benchmark.h>
#include <scl/utility/hierarchy/tree.h>

#include <cstddef>

namespace
{

    // The tree notifies an observer on every insert, erase and clear. This one does
    // nothing, so what the cases measure is the notification path itself rather than
    // whatever an observer would do with it.
    template <typename Tree>
    struct silent_observer
    {
    public:
        using iterator = Tree::iterator;
        using const_iterator = Tree::const_iterator;
        using const_reference = Tree::const_payload_reference;

    public:
        constexpr void on_insert(iterator) {}
        constexpr void on_erase(const_iterator) {}
        constexpr void on_clear() {}
        constexpr void on_change(const_reference, const_reference) {}
    };

    using int_tree = ::scl::hierarchy::tree<int, silent_observer>;

    constexpr int fanout = 8;
    constexpr int depth = 4;

    template <typename Node>
    void grow(Node && parent, int level)
    {
        if (level == 0)
            return;

        for (int index = 0; index < fanout; ++index)
        {
            auto child = parent.push_back(level * fanout + index);
            grow(*child, level - 1);
        }
    }

    int_tree make_tree()
    {
        int_tree tree;
        for (int index = 0; index < fanout; ++index)
        {
            auto child = tree.push_back(index);
            grow(*child, depth - 1);
        }
        return tree;
    }

    void tree_walk_children(::benchmark::State & state)
    {
        auto tree = make_tree();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(tree);
            ::std::size_t count = 0;
            for (auto it = tree.begin(); it != tree.end(); ++it)
            {
                auto value = (*it).value();
                ::benchmark::DoNotOptimize(value);
                ++count;
            }
            ::benchmark::DoNotOptimize(count);
        }
    }

    BENCHMARK(tree_walk_children);

    void tree_walk_recursive(::benchmark::State & state)
    {
        auto tree = make_tree();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(tree);
            auto total = ::std::size_t{0};
            auto visit = [&total](auto && self, auto && node) -> void {
                ++total;
                for (auto it = node.begin(); it != node.end(); ++it)
                    self(self, *it);
            };
            for (auto it = tree.begin(); it != tree.end(); ++it)
                visit(visit, *it);
            ::benchmark::DoNotOptimize(total);
        }
    }

    BENCHMARK(tree_walk_recursive);

    void tree_push_back(::benchmark::State & state)
    {
        auto tree = make_tree();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(tree);
            auto added = tree.push_back(1);
            ::benchmark::DoNotOptimize(added);
            tree.pop_back();
        }
    }

    BENCHMARK(tree_push_back);

} // namespace
