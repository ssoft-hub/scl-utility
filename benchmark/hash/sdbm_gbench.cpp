#include <benchmark/benchmark.h>
#include <scl/utility/attribute/inline.h>
#include <scl/utility/hash/sdbm.h>

#include <array>
#include <string>
#include <string_view>

#include "gbench_input.h"

namespace
{

    // A run-time length, the shape a std::string_view or a std::string key has.
    void sdbm_string_view_8(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::sample_text_8;
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::sdbm(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(sdbm_string_view_8);

    void sdbm_string_view_16(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::sample_text_16;
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::sdbm(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(sdbm_string_view_16);

    void sdbm_string_view_63(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::sample_text_63;
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::sdbm(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(sdbm_string_view_63);

    // A length the compiler sees, the shape a fixed-width key has.
    void sdbm_char_array_8(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::make_sample_array<8>(::scl::benchmarks::sample_text_8);
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::sdbm(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(sdbm_char_array_8);

    void sdbm_char_array_16(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::make_sample_array<16>(::scl::benchmarks::sample_text_16);
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::sdbm(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(sdbm_char_array_16);

    void sdbm_char_array_63(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::make_sample_array<63>(::scl::benchmarks::sample_text_63);
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::sdbm(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(sdbm_char_array_63);

    // Keeps the call: a literal's hash is loop-invariant and the loop would hoist it out.
    SCL_NOINLINE ::std::uint64_t sdbm_of_literal()
    {
        return ::scl::hash::sdbm("the quick brown fox jumps over the lazy dog, and then some more");
    }

    void sdbm_literal_63(::benchmark::State & state)
    {
        for (auto _ : state)
        {
            auto value = sdbm_of_literal();
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(sdbm_literal_63);

    // An owning container, to show it tracks the view it is copied from.
    void sdbm_string_63(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::make_sample_string(::scl::benchmarks::sample_text_63);
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::sdbm(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(sdbm_string_63);

    // The hasher wrapper, to show it costs nothing over the free function.
    void sdbm_hasher_string_view_63(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::sample_text_63;
        ::scl::hash::sdbm_hasher const hasher{};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = hasher(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(sdbm_hasher_string_view_63);

} // namespace
