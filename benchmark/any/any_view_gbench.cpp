#include <benchmark/benchmark.h>
#include <scl/utility/any/any_arg.h>
#include <scl/utility/any/any_switch.h>
#include <scl/utility/any/any_view.h>

#include <any>
#include <cstdint>
#include <string>

namespace
{

    // any_cast walks a chain of branches, and the annotations on it mark which way each
    // is expected to go. Every branch below is the outcome of one of them, so a hint
    // pointing the wrong way shows up as the case it mispredicts.

    void any_view_cast_hit(::benchmark::State & state)
    {
        auto subject = ::std::int64_t{42};
        auto view = ::scl::any_view{subject};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(view);
            auto * value = ::scl::any_cast<::std::int64_t>(&view);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(any_view_cast_hit);

    void any_view_cast_miss(::benchmark::State & state)
    {
        auto subject = ::std::int64_t{42};
        auto view = ::scl::any_view{subject};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(view);
            auto * value = ::scl::any_cast<double>(&view);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(any_view_cast_miss);

    void any_view_cast_empty(::benchmark::State & state)
    {
        auto view = ::scl::any_view{};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(view);
            auto * value = ::scl::any_cast<::std::int64_t>(&view);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(any_view_cast_empty);

    void any_view_cast_boxed(::benchmark::State & state)
    {
        auto boxed = ::std::any{::std::int64_t{42}};
        auto view = ::scl::any_view{boxed};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(view);
            auto * value = ::scl::any_cast<::std::int64_t>(&view);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(any_view_cast_boxed);

    void any_arg_cast_hit(::benchmark::State & state)
    {
        auto subject = ::std::int64_t{42};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(subject);
            auto arg = ::scl::any_arg{subject};
            auto * value = ::scl::any_cast<::std::int64_t>(&arg);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(any_arg_cast_hit);

    void any_arg_cast_miss(::benchmark::State & state)
    {
        auto subject = ::std::int64_t{42};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(subject);
            auto arg = ::scl::any_arg{subject};
            auto * value = ::scl::any_cast<double>(&arg);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(any_arg_cast_miss);

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
