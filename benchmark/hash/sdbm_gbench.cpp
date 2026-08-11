#include <benchmark/benchmark.h>
#include <scl/utility/hash/sdbm.h>

#include <array>
#include <string>
#include <string_view>

#include "gbench_input.h"

namespace
{

    void sdbm_string_view(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::sample_text;
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::sdbm(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(sdbm_string_view);

    void sdbm_string(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::make_sample_string();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::sdbm(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(sdbm_string);

    void sdbm_char_array(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::make_sample_array();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::sdbm(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(sdbm_char_array);

    void sdbm_hasher_string_view(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::sample_text;
        ::scl::hash::sdbm_hasher const hasher{};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = hasher(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(sdbm_hasher_string_view);

} // namespace
