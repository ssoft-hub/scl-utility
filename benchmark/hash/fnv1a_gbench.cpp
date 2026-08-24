#include <benchmark/benchmark.h>
#include <scl/utility/attribute/inline.h>
#include <scl/utility/hash/fnv1a.h>

#include <array>
#include <cstdint>
#include <string>
#include <string_view>

#include "gbench_input.h"

namespace
{

    // A run-time length, the shape a std::string_view or a std::string key has.
    void fnv1a_string_view_8(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::sample_text_8;
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::fnv1a(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(fnv1a_string_view_8);

    void fnv1a_string_view_16(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::sample_text_16;
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::fnv1a(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(fnv1a_string_view_16);

    void fnv1a_string_view_63(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::sample_text_63;
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::fnv1a(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(fnv1a_string_view_63);

    // A length the compiler sees, the shape a fixed-width key has.
    void fnv1a_char_array_8(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::make_sample_array<8>(::scl::benchmarks::sample_text_8);
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::fnv1a(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(fnv1a_char_array_8);

    void fnv1a_char_array_16(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::make_sample_array<16>(::scl::benchmarks::sample_text_16);
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::fnv1a(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(fnv1a_char_array_16);

    void fnv1a_char_array_63(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::make_sample_array<63>(::scl::benchmarks::sample_text_63);
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::fnv1a(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(fnv1a_char_array_63);

    // Keeps the call: a literal's hash is loop-invariant and the loop would hoist it out.
    SCL_NOINLINE ::std::uint64_t fnv1a_of_literal()
    {
        return ::scl::hash::fnv1a("the quick brown fox jumps over the lazy dog, and then some more");
    }

    void fnv1a_literal_63(::benchmark::State & state)
    {
        for (auto _ : state)
        {
            auto value = fnv1a_of_literal();
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(fnv1a_literal_63);

    // An owning container, to show it tracks the view it is copied from.
    void fnv1a_string_63(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::make_sample_string(::scl::benchmarks::sample_text_63);
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ::scl::hash::fnv1a(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(fnv1a_string_63);

    // A barrier on the result forces it to memory every iteration; accumulating keeps
    // it in a register. The pair bounds how much of a difference is the barrier's own.
    void fnv1a_string_view_63_accumulated(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::sample_text_63;
        auto sum = ::std::uint64_t{0};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            sum += ::scl::hash::fnv1a(input);
        }
        ::benchmark::DoNotOptimize(sum);
    }

    BENCHMARK(fnv1a_string_view_63_accumulated);

    void fnv1a_char_array_63_accumulated(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::make_sample_array<63>(::scl::benchmarks::sample_text_63);
        auto sum = ::std::uint64_t{0};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            sum += ::scl::hash::fnv1a(input);
        }
        ::benchmark::DoNotOptimize(sum);
    }

    BENCHMARK(fnv1a_char_array_63_accumulated);

    // The hasher wrapper, to show it costs nothing over the free function.
    void fnv1a_hasher_string_view_63(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::sample_text_63;
        ::scl::hash::fnv1a_hasher const hasher{};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = hasher(input);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(fnv1a_hasher_string_view_63);

} // namespace
