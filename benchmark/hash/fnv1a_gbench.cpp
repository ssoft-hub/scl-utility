#include <benchmark/benchmark.h>
#include <scl/utility/hash/fnv1a.h>

#include <array>
#include <cstdint>
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

    // A barrier on the result forces it to memory every iteration; accumulating keeps it
    // in a register. The pair bounds how much of a difference is the barrier's own doing.
    void fnv1a_string_view_accumulated(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::sample_text;
        auto sum = ::std::uint64_t{0};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            sum += ::scl::hash::fnv1a(input);
        }
        ::benchmark::DoNotOptimize(sum);
    }

    BENCHMARK(fnv1a_string_view_accumulated);

    void fnv1a_char_array_accumulated(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::make_sample_array();
        auto sum = ::std::uint64_t{0};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            sum += ::scl::hash::fnv1a(input);
        }
        ::benchmark::DoNotOptimize(sum);
    }

    BENCHMARK(fnv1a_char_array_accumulated);

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
