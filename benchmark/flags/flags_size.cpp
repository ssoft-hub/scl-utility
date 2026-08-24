// Code-size counterpart to flags_gbench.cpp: one externally visible wrapper per
// operation, at both widths the timing suite uses, so -ffunction-sections gives each
// its own section and arm-none-eabi-size reports what annotating the class costs.
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

    enum class wide_bit : unsigned
    {
        w000 = 0,
        w192 = 192,
        w255 = 255
    };

    using wide_bits = ::scl::flags<wide_bit, 256>;

    bits flags_construct(bit first, bit second) { return bits{first, second}; }

    bits flags_or(bits left, bits right) { return left | right; }

    bits flags_and(bits left, bits right) { return left & right; }

    bits flags_xor(bits left, bits right) { return left ^ right; }

    bits flags_difference(bits left, bits right) { return left - right; }

    bool flags_all_of(bits left, bits right) { return left.all_of(right); }

    bool flags_any_of(bits value) { return value.any_of(bit::b00, bit::b03); }

    bool flags_index(bits value, bit which) { return value[which]; }

    bool flags_any(bits value) { return value.any(); }

    ::std::size_t flags_size(bits value) { return value.size(); }

    ::std::size_t flags_iterate(bits value)
    {
        ::std::size_t count = 0;
        for ([[maybe_unused]]
             auto element : value)
            ++count;
        return count;
    }

    ::std::size_t flags_reverse_iterate(bits value)
    {
        ::std::size_t count = 0;
        for (auto it = value.rbegin(); it != value.rend(); ++it)
            ++count;
        return count;
    }

    wide_bits wide_flags_or(wide_bits left, wide_bits right) { return left | right; }

    bool wide_flags_index(wide_bits value, wide_bit which) { return value[which]; }

    bool wide_flags_any(wide_bits value) { return value.any(); }

    ::std::size_t wide_flags_size(wide_bits value) { return value.size(); }

    ::std::size_t wide_flags_iterate(wide_bits value)
    {
        ::std::size_t count = 0;
        for ([[maybe_unused]]
             auto element : value)
            ++count;
        return count;
    }

    ::std::size_t wide_flags_reverse_iterate(wide_bits value)
    {
        ::std::size_t count = 0;
        for (auto it = value.rbegin(); it != value.rend(); ++it)
            ++count;
        return count;
    }

} // namespace scl::benchmarks
