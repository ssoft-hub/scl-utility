#include <benchmark/benchmark.h>
#include <scl/utility/hash/djb2.h>

#include <array>
#include <string>
#include <string_view>

#include "gbench_input.h"

namespace
{

    void djb2_string_view(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::sample_text;
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::djb2(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(djb2_string_view);

    void djb2_string(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::make_sample_string();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::djb2(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(djb2_string);

    void djb2_char_array(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::make_sample_array();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::djb2(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(djb2_char_array);

    void djb2_hasher_string_view(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::sample_text;
        ::scl::hash::djb2_hasher const hasher{};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = hasher(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(djb2_hasher_string_view);

} // namespace
