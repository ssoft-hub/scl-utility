#include <benchmark/benchmark.h>
#include <scl/utility/runtime/enum.h>

#include <array>
#include <charconv>
#include <cstdint>
#include <limits>
#include <string>

namespace
{
    enum class color : int
    {
        blue = -3,
    };

    enum class wide : ::std::int64_t
    {
        lowest = ::std::numeric_limits<::std::int64_t>::min(),
    };

    struct hex_format
    {
        template <typename Number>
        [[nodiscard]]
        ::std::string operator()(Number number) const
        {
            ::std::array<char, 32> buf{};
            auto const end = ::std::to_chars(buf.data(), buf.data() + buf.size(), number, 16).ptr;
            return ::std::string{buf.data(), end};
        }
    };

    void enum_string_decimal(::benchmark::State & state)
    {
        auto value = color::blue;
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(value);
            auto text = ::scl::enum_string(value);
            ::benchmark::DoNotOptimize(text);
        }
    }

    BENCHMARK(enum_string_decimal);

    void enum_string_decimal_wide(::benchmark::State & state)
    {
        auto value = wide::lowest;
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(value);
            auto text = ::scl::enum_string(value);
            ::benchmark::DoNotOptimize(text);
        }
    }

    BENCHMARK(enum_string_decimal_wide);

    void enum_string_format(::benchmark::State & state)
    {
        auto value = color::blue;
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(value);
            auto text = ::scl::enum_string(value, hex_format{});
            ::benchmark::DoNotOptimize(text);
        }
    }

    BENCHMARK(enum_string_format);
} // namespace
