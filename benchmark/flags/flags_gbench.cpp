#include <benchmark/benchmark.h>
#include <scl/utility/flags.h>

#include <cstddef>

namespace
{

    enum class bit : unsigned
    {
        b00,
        b01,
        b02,
        b03,
        b04,
        b05,
        b06,
        b07,
        b08,
        b09,
        b10,
        b11,
        b12,
        b13,
        b14,
        b15,
        b16,
        b17,
        b18,
        b19,
        b20,
        b21,
        b22,
        b23,
        b24,
        b25,
        b26,
        b27,
        b28,
        b29,
        b30,
        b31
    };

    using bits = ::scl::flags<bit>;

    // Every fourth bit set: a spread the branch predictor cannot memorise, and
    // eight iterations of the sparse walk rather than one or thirty-two.
    bits sparse()
    {
        return bits{bit::b00, bit::b04, bit::b08, bit::b12, bit::b16, bit::b20, bit::b24, bit::b28};
    }

    void flags_or(::benchmark::State & state)
    {
        auto left = sparse();
        auto right = bits{bit::b01, bit::b05, bit::b09};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(left);
            ::benchmark::DoNotOptimize(right);
            auto value = left | right;
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(flags_or);

    void flags_and(::benchmark::State & state)
    {
        auto left = sparse();
        auto right = bits{bit::b00, bit::b05, bit::b08};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(left);
            ::benchmark::DoNotOptimize(right);
            auto value = left & right;
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(flags_and);

    void flags_complement(::benchmark::State & state)
    {
        auto input = sparse();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = ~input;
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(flags_complement);

    void flags_all(::benchmark::State & state)
    {
        auto input = sparse();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = input.all();
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(flags_all);

    void flags_any(::benchmark::State & state)
    {
        auto input = sparse();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = input.any();
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(flags_any);

    void flags_iterate(::benchmark::State & state)
    {
        auto input = sparse();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            ::std::size_t count = 0;
            for (auto value : input)
            {
                ::benchmark::DoNotOptimize(value);
                ++count;
            }
            ::benchmark::DoNotOptimize(count);
        }
    }

    BENCHMARK(flags_iterate);

    void flags_any_of(::benchmark::State & state)
    {
        auto input = sparse();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = input.any_of(bit::b31, bit::b28);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(flags_any_of);

} // namespace
