#include <benchmark/benchmark.h>
#include <scl/utility/hash/fnv1a.h>

#include <array>
#include <string>
#include <string_view>

#include "gbench_input.h"

namespace
{

    void fnv1a_string_view(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::sample_text;
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::fnv1a(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(fnv1a_string_view);

    void fnv1a_string(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::make_sample_string();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::fnv1a(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(fnv1a_string);

    void fnv1a_char_array(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::make_sample_array();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::fnv1a(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(fnv1a_char_array);

    void fnv1a_hasher_string_view(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::sample_text;
        ::scl::hash::fnv1a_hasher const hasher{};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = hasher(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(fnv1a_hasher_string_view);

} // namespace
