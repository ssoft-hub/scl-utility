#include <benchmark/benchmark.h>
#include <scl/utility/runtime/type.h>

#if SCL_HAS_RTTI

#include <memory>

namespace
{
    namespace app
    {
        struct base
        {
            virtual ~base() = default;
        };

        struct derived : base
        {};

        template <typename T>
        struct task : base
        {};
    } // namespace app

    void type_name_dynamic(::benchmark::State & state)
    {
        ::std::unique_ptr<app::base> const object = ::std::make_unique<app::derived>();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(object.get());
            auto name = ::scl::type_name(*object);
            ::benchmark::DoNotOptimize(name);
        }
    }

    BENCHMARK(type_name_dynamic);

    void type_name_template(::benchmark::State & state)
    {
        ::std::unique_ptr<app::base> const object = ::std::make_unique<app::task<int>>();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(object.get());
            auto name = ::scl::type_name(*object);
            ::benchmark::DoNotOptimize(name);
        }
    }

    BENCHMARK(type_name_template);

    void type_short_name_dynamic(::benchmark::State & state)
    {
        ::std::unique_ptr<app::base> const object = ::std::make_unique<app::derived>();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(object.get());
            auto name = ::scl::type_short_name(*object);
            ::benchmark::DoNotOptimize(name);
        }
    }

    BENCHMARK(type_short_name_dynamic);

    void type_short_name_template(::benchmark::State & state)
    {
        ::std::unique_ptr<app::base> const object = ::std::make_unique<app::task<int>>();
        for (auto _ : state)
        {
            ::benchmark::DoNotOptimize(object.get());
            auto name = ::scl::type_short_name(*object);
            ::benchmark::DoNotOptimize(name);
        }
    }

    BENCHMARK(type_short_name_template);
} // namespace

#endif
