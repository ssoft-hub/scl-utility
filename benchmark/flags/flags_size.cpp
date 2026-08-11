// Code-size counterpart to flags_gbench.cpp: one externally visible wrapper per
// operation, so -ffunction-sections gives each its own section.
//
// Never linked or run. The wrappers exist to be compiled, not called.

#include <scl/utility/flags.h>

#include <cstddef>

namespace scl::benchmarks
{

    enum class bit : unsigned
    {
        b00,
        b01,
        b02,
        b03
    };

    using bits = ::scl::flags<bit>;

    bits flags_or(bits left, bits right) { return left | right; }

    bits flags_and(bits left, bits right) { return left & right; }

    bits flags_xor(bits left, bits right) { return left ^ right; }

    bits flags_complement(bits value) { return ~value; }

    bool flags_all(bits value) { return value.all(); }

    bool flags_any(bits value) { return value.any(); }

    ::std::size_t flags_iterate(bits value)
    {
        ::std::size_t count = 0;
        for ([[maybe_unused]]
             auto element : value)
            ++count;
        return count;
    }

    bool flags_any_of(bits value) { return value.any_of(bit::b00, bit::b03); }

} // namespace scl::benchmarks
