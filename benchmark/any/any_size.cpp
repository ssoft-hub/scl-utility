// Code-size counterpart to the *_gbench sources: one externally visible wrapper per
// cast, so -ffunction-sections gives each its own section and arm-none-eabi-size reports
// what annotating the cast chain costs in .text.
//
// Never linked or run. The wrappers exist to be compiled, not called.

#include <scl/utility/any/any_arg.h>
#include <scl/utility/any/any_mutable_view.h>
#include <scl/utility/any/any_view.h>

#include <cstdint>

namespace scl::benchmarks
{

    ::std::int64_t const * view_cast_hit(::scl::any_view const * view)
    {
        return ::scl::any_cast<::std::int64_t>(view);
    }

    double const * view_cast_miss(::scl::any_view const * view)
    {
        return ::scl::any_cast<double>(view);
    }

    ::std::int64_t * mutable_view_cast(::scl::any_mutable_view * view)
    {
        return ::scl::any_cast<::std::int64_t>(view);
    }

    ::std::int64_t const * arg_cast(::scl::any_argument const * arg)
    {
        return ::scl::any_cast<::std::int64_t>(arg);
    }

} // namespace scl::benchmarks
