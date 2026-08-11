#include <benchmark/benchmark.h>
#include <scl/utility/hash/jenkins_ota.h>

#include <array>
#include <string>
#include <string_view>

#include "gbench_input.h"

namespace
{

    void jenkins_ota_string_view(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::sample_text;
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::jenkins_ota(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(jenkins_ota_string_view);

    void jenkins_ota_string(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::make_sample_string();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::jenkins_ota(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(jenkins_ota_string);

    void jenkins_ota_char_array(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::make_sample_array();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::jenkins_ota(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(jenkins_ota_char_array);

    void jenkins_ota_hasher_string_view(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::sample_text;
        ::scl::hash::jenkins_ota_hasher const hasher{};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = hasher(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(jenkins_ota_hasher_string_view);

} // namespace
