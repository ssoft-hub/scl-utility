#include <benchmark/benchmark.h>
#include <scl/utility/hash/siphash.h>

#include <array>
#include <string>
#include <string_view>

#include "gbench_input.h"

namespace
{

    void siphash_string_view(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::sample_text;
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::siphash(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(siphash_string_view);

    void siphash_string(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::make_sample_string();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::siphash(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(siphash_string);

    void siphash_char_array(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::make_sample_array();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::siphash(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(siphash_char_array);

    void siphash_hasher_string_view(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::sample_text;
        ::scl::hash::siphash_hasher<> const hasher{};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = hasher(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(siphash_hasher_string_view);

} // namespace
