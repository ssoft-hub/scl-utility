#include <benchmark/benchmark.h>
#include <scl/utility/attribute/inline.h>
#include <scl/utility/hash/jenkins_ota.h>

#include <array>
#include <string>
#include <string_view>

#include "gbench_input.h"

namespace
{

    // A run-time length, the shape a std::string_view or a std::string key has.
    void jenkins_ota_string_view_8(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::sample_text_8;
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::jenkins_ota(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(jenkins_ota_string_view_8);

    void jenkins_ota_string_view_16(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::sample_text_16;
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::jenkins_ota(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(jenkins_ota_string_view_16);

    void jenkins_ota_string_view_63(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::sample_text_63;
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::jenkins_ota(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(jenkins_ota_string_view_63);

    // A length the compiler sees, the shape a fixed-width key has.
    void jenkins_ota_char_array_8(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::make_sample_array<8>(::scl::benchmarks::sample_text_8);
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::jenkins_ota(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(jenkins_ota_char_array_8);

    void jenkins_ota_char_array_16(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::make_sample_array<16>(::scl::benchmarks::sample_text_16);
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::jenkins_ota(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(jenkins_ota_char_array_16);

    void jenkins_ota_char_array_63(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::make_sample_array<63>(::scl::benchmarks::sample_text_63);
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::jenkins_ota(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(jenkins_ota_char_array_63);

    // Keeps the call: a literal's hash is loop-invariant and the loop would hoist it out.
    SCL_NOINLINE ::std::uint32_t jenkins_ota_of_literal()
    {
        return ::scl::hash::jenkins_ota("the quick brown fox jumps over the lazy dog, and then some more");
    }

    void jenkins_ota_literal_63(::benchmark::State & state)
    {
        for (auto _ : state)
        {
            auto value = jenkins_ota_of_literal();
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(jenkins_ota_literal_63);

    // An owning container, to show it tracks the view it is copied from.
    void jenkins_ota_string_63(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::make_sample_string(::scl::benchmarks::sample_text_63);
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::jenkins_ota(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(jenkins_ota_string_63);

    // The hasher wrapper, to show it costs nothing over the free function.
    void jenkins_ota_hasher_string_view_63(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::sample_text_63;
        ::scl::hash::jenkins_ota_hasher const hasher{};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = hasher(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(jenkins_ota_hasher_string_view_63);

} // namespace
