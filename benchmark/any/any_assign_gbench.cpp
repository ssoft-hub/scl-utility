#include <benchmark/benchmark.h>
#include <scl/utility/any/any.h>
#include <scl/utility/any/any_view.h>

#include <cstdint>
#include <string>

namespace
{

    // Assignment checks whether the incoming object is the one already held before it
    // destroys anything, and both of those checks carry a hint. The cases below are the
    // two outcomes of each: an ordinary assignment, where the check answers no, and a
    // self-assignment, where it answers yes and the whole body is skipped.

    void any_assign_value(::benchmark::State & state)
    {
        ::scl::any target{::std::int64_t{1}};
        auto subject = ::std::int64_t{42};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(subject);
            target = subject;
            ::benchmark::DoNotOptimize(target);
        }
    }

    BENCHMARK(any_assign_value);

    void any_assign_self(::benchmark::State & state)
    {
        ::scl::any target{::std::int64_t{42}};
        auto * held = ::scl::any_cast<::std::int64_t>(&target);
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(held);
            target = *held;
            ::benchmark::DoNotOptimize(target);
        }
    }

    BENCHMARK(any_assign_self);

    void any_assign_view(::benchmark::State & state)
    {
        ::scl::any target{::std::int64_t{1}};
        auto subject = ::std::int64_t{42};
        auto view = ::scl::any_view{subject};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(view);
            target = view;
            ::benchmark::DoNotOptimize(target);
        }
    }

    BENCHMARK(any_assign_view);

    // An allocating payload, so the skipped body is worth something and the check that
    // skips it has something to save.
    void any_assign_string(::benchmark::State & state)
    {
        ::scl::any target{::std::string{"held"}};
        auto subject = ::std::string{"incoming"};
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(subject);
            target = subject;
            ::benchmark::DoNotOptimize(target);
        }
    }

    BENCHMARK(any_assign_string);

} // namespace
