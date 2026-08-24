#include <benchmark/benchmark.h>
#include <scl/utility/attribute/inline.h>
#include <scl/utility/flags.h>

#include <array>
#include <cstddef>
#include <utility>

namespace
{

    // Every case walks the mask this many times; what changes between them is how many
    // distinct bodies those walks are spread over. At one site the walk stays resident in
    // the instruction cache, and at a thousand it cannot, so an inlined walk is measured
    // both where duplication is free and where it is paid for.
    constexpr ::std::size_t site_count = 1024;

    enum class wide : unsigned
    {
        w000 = 0,
        w064 = 64,
        w128 = 128,
        w192 = 192,
        w255 = 255
    };

    using wide_bits = ::scl::flags<wide, 256>;

    ::std::size_t accumulator = 0;

    // The mask arrives as a parameter of a function the compiler may not inline, so the
    // walk cannot be folded away: every site runs it. Writing to the accumulator makes
    // the call impure, so repeated calls with equal arguments survive too. Index enters
    // the result to keep the bodies distinct, since identical ones get folded into a
    // single copy.
    template <::std::size_t Index>
    SCL_NOINLINE void site(wide_bits const & input) noexcept
    {
        auto count = Index;
        for (auto const value : input)
            count += static_cast<::std::size_t>(value);
        accumulator += count;
    }

    using site_function = void (*)(wide_bits const &) noexcept;

    template <::std::size_t... Indices>
    ::std::array<site_function, sizeof...(Indices)> make_sites(::std::index_sequence<Indices...>) noexcept
    {
        return {&site<Indices>...};
    }

    ::std::array<site_function, site_count> const sites = make_sites(::std::make_index_sequence<site_count>{});

    wide_bits sparse()
    {
        return wide_bits{wide::w000, wide::w064, wide::w128, wide::w192, wide::w255};
    }

    template <::std::size_t Count>
    void walk_from_sites(::benchmark::State & state)
    {
        auto input = sparse();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            for (::std::size_t repeat = 0; repeat < site_count / Count; ++repeat)
                for (::std::size_t index = 0; index < Count; ++index)
                    sites[index](input);
            ::benchmark::DoNotOptimize(accumulator);
        }
    }

    BENCHMARK(walk_from_sites<1>)->Name("flags_icache_sites_1");
    BENCHMARK(walk_from_sites<8>)->Name("flags_icache_sites_8");
    BENCHMARK(walk_from_sites<64>)->Name("flags_icache_sites_64");
    BENCHMARK(walk_from_sites<256>)->Name("flags_icache_sites_256");
    BENCHMARK(walk_from_sites<1024>)->Name("flags_icache_sites_1024");

} // namespace
