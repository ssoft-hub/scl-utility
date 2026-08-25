#include <benchmark/benchmark.h>
#include <scl/utility/any/any_arg.h>
#include <scl/utility/any/any_switch.h>

#include <cstdint>
#include <string>

namespace
{

    // A chain runs any_cast once per branch until one accepts, so which branch matches
    // decides how many casts a call pays for.
    auto const chain =
        ::scl::any_switch<int>().in_case<::std::int64_t>([](::std::int64_t) {
        return 1;
    }).in_case<double>([](double) {
        return 2;
    }).in_case<::std::string const &>([](::std::string const &) {
        return 3;
    }).or_else([](::scl::any_arg) { return 0; });

    void any_switch_first_branch(::benchmark::State & state)
    {
        auto subject = ::std::int64_t{42};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(subject);
            auto value = chain.apply(subject);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(any_switch_first_branch);

    void any_switch_last_branch(::benchmark::State & state)
    {
        auto subject = ::std::string{"text"};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(subject);
            auto value = chain.apply(subject);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(any_switch_last_branch);

    void any_switch_fallback(::benchmark::State & state)
    {
        auto subject = 'c';
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(subject);
            auto value = chain.apply(subject);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(any_switch_fallback);

} // namespace
