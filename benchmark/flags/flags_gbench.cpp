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

    // Ordinals a byte apart over a 256-bit mask: eight set bits again, so the walk finds
    // the same number over eight times the storage. What separates the two widths is the
    // length of every whole-byte loop, which is what an annotation can change.
    enum class wide_bit : unsigned
    {
        w000 = 0,
        w032 = 32,
        w064 = 64,
        w096 = 96,
        w128 = 128,
        w160 = 160,
        w192 = 192,
        w224 = 224,
        w255 = 255
    };

    using wide_bits = ::scl::flags<wide_bit, 256>;

    wide_bits wide_sparse()
    {
        return wide_bits{wide_bit::w000, wide_bit::w032, wide_bit::w064, wide_bit::w096,
            wide_bit::w128, wide_bit::w160, wide_bit::w192, wide_bit::w224};
    }

    void flags_construct_32(::benchmark::State & state)
    {
        auto value = bit::b12;
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(value);
            auto result = bits{bit::b00, value, bit::b28};
            ::benchmark::DoNotOptimize(result);
        }
    }

    BENCHMARK(flags_construct_32);

    void flags_union_32(::benchmark::State & state)
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

    BENCHMARK(flags_union_32);

    void flags_intersection_32(::benchmark::State & state)
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

    BENCHMARK(flags_intersection_32);

    void flags_symmetric_difference_32(::benchmark::State & state)
    {
        auto left = sparse();
        auto right = bits{bit::b00, bit::b05, bit::b08};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(left);
            ::benchmark::DoNotOptimize(right);
            auto value = left ^ right;
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(flags_symmetric_difference_32);

    void flags_difference_32(::benchmark::State & state)
    {
        auto left = sparse();
        auto right = bits{bit::b00, bit::b12, bit::b24};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(left);
            ::benchmark::DoNotOptimize(right);
            auto value = left - right;
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(flags_difference_32);

    void flags_all_of_flags_32(::benchmark::State & state)
    {
        auto input = sparse();
        auto wanted = bits{bit::b00, bit::b12, bit::b24};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            ::benchmark::DoNotOptimize(wanted);
            auto value = input.all_of(wanted);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(flags_all_of_flags_32);

    void flags_any_of_pack_32(::benchmark::State & state)
    {
        auto input = sparse();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = input.any_of(bit::b31, bit::b28);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(flags_any_of_pack_32);

    // The one query that checks its argument against the width and rejects out of range.
    void flags_index_32(::benchmark::State & state)
    {
        auto input = sparse();
        auto value = bit::b12;
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            ::benchmark::DoNotOptimize(value);
            auto held = input[value];
            ::benchmark::DoNotOptimize(held);
        }
    }

    BENCHMARK(flags_index_32);

    void flags_any_32(::benchmark::State & state)
    {
        auto input = sparse();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = input.any();
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(flags_any_32);

    void flags_size_32(::benchmark::State & state)
    {
        auto input = sparse();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = input.size();
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(flags_size_32);

    void flags_iterate_32(::benchmark::State & state)
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

    BENCHMARK(flags_iterate_32);

    void flags_reverse_iterate_32(::benchmark::State & state)
    {
        auto input = sparse();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            ::std::size_t count = 0;
            for (auto it = input.rbegin(); it != input.rend(); ++it)
            {
                auto value = *it;
                ::benchmark::DoNotOptimize(value);
                ++count;
            }
            ::benchmark::DoNotOptimize(count);
        }
    }

    BENCHMARK(flags_reverse_iterate_32);

    void flags_union_256(::benchmark::State & state)
    {
        auto left = wide_sparse();
        auto right = wide_bits{wide_bit::w032, wide_bit::w255};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(left);
            ::benchmark::DoNotOptimize(right);
            auto value = left | right;
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(flags_union_256);

    void flags_index_256(::benchmark::State & state)
    {
        auto input = wide_sparse();
        auto value = wide_bit::w192;
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            ::benchmark::DoNotOptimize(value);
            auto held = input[value];
            ::benchmark::DoNotOptimize(held);
        }
    }

    BENCHMARK(flags_index_256);

    void flags_any_256(::benchmark::State & state)
    {
        auto input = wide_sparse();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = input.any();
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(flags_any_256);

    void flags_size_256(::benchmark::State & state)
    {
        auto input = wide_sparse();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            auto value = input.size();
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(flags_size_256);

    void flags_iterate_256(::benchmark::State & state)
    {
        auto input = wide_sparse();
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

    BENCHMARK(flags_iterate_256);

    void flags_reverse_iterate_256(::benchmark::State & state)
    {
        auto input = wide_sparse();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(input);
            ::std::size_t count = 0;
            for (auto it = input.rbegin(); it != input.rend(); ++it)
            {
                auto value = *it;
                ::benchmark::DoNotOptimize(value);
                ++count;
            }
            ::benchmark::DoNotOptimize(count);
        }
    }

    BENCHMARK(flags_reverse_iterate_256);

} // namespace
