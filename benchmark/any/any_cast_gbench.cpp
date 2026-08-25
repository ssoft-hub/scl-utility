#include <benchmark/benchmark.h>
#include <scl/utility/any/any_arg.h>
#include <scl/utility/any/any_mutable_view.h>
#include <scl/utility/any/any_view.h>

#include <any>
#include <cstdint>

namespace
{

    // any_cast walks a chain of branches, and the annotations on it mark which way each
    // is expected to go. Every case below is the outcome of one of them, so a hint
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

    void any_mutable_view_cast_hit(::benchmark::State & state)
    {
        auto subject = ::std::int64_t{42};
        auto view = ::scl::any_mutable_view{subject};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(view);
            auto * value = ::scl::any_cast<::std::int64_t>(&view);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(any_mutable_view_cast_hit);

    void any_mutable_view_cast_miss(::benchmark::State & state)
    {
        auto subject = ::std::int64_t{42};
        auto view = ::scl::any_mutable_view{subject};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(view);
            auto * value = ::scl::any_cast<double>(&view);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(any_mutable_view_cast_miss);

    void any_arg_cast_hit(::benchmark::State & state)
    {
        auto subject = ::std::int64_t{42};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(subject);
            ::scl::any_arg arg = subject;
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
            ::scl::any_arg arg = subject;
            auto * value = ::scl::any_cast<double>(&arg);
            ::benchmark::DoNotOptimize(value);
        }
    }

    BENCHMARK(any_arg_cast_miss);

} // namespace
