#include <benchmark/benchmark.h>
#include <scl/utility/attribute/inline.h>
#include <scl/utility/hash/fnv1a.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <utility>

#include "gbench_input.h"

namespace
{

    // Wide enough that whatever the hash expands to at each site, repeated this many
    // times, overflows a 32 KiB L1 instruction cache, while a single shared body does
    // not. The other hash cases keep one body resident and so cannot show what
    // duplicating it costs.
    constexpr ::std::size_t site_count = 1024;

    ::std::uint64_t accumulator = 0;

    // The input arrives as a parameter of a function the compiler may not inline, so the
    // constexpr hash cannot fold: every site runs the loop. Writing to the accumulator
    // makes the call impure, so repeated calls with equal arguments survive too. Index
    // enters the result to keep the bodies distinct, since identical ones get folded
    // into a single copy.
    template <::std::size_t Index>
    SCL_NOINLINE void site(::std::string_view input) noexcept
    {
        accumulator += ::scl::hash::fnv1a(input) + Index;
    }

    using site_function = void (*)(::std::string_view) noexcept;

    template <::std::size_t... Indices>
    ::std::array<site_function, sizeof...(Indices)> make_sites(::std::index_sequence<Indices...>) noexcept
    {
        return {&site<Indices>...};
    }

    ::std::array<site_function, site_count> const sites = make_sites(::std::make_index_sequence<site_count>{});

    void hash_icache_many_sites(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::sample_text;
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            for (auto const call : sites)
                call(input);
            ::benchmark::DoNotOptimize(accumulator);
        }
    }

    BENCHMARK(hash_icache_many_sites);

    void hash_icache_one_site(::benchmark::State & state)
    {
        auto input = ::scl::benchmarks::sample_text;
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            for (::std::size_t index = 0; index < site_count; ++index)
                site<0>(input);
            ::benchmark::DoNotOptimize(accumulator);
        }
    }

    BENCHMARK(hash_icache_one_site);

} // namespace
