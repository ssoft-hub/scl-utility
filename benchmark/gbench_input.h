#pragma once

#include <algorithm>
#include <array>
#include <string>
#include <string_view>

namespace scl::benchmarks
{

    // 63 bytes, the input size every recorded hash measurement was taken at.
    inline constexpr ::std::string_view sample_text = "the quick brown fox jumps over the lazy dog, and then some more";

    static_assert(sample_text.size() == 63);

    inline ::std::string make_sample_string() { return ::std::string{sample_text}; }

    // std::array rather than char[63]: a bounded array of character type has its last
    // element dropped as a string terminator, which would hash 62 bytes here and leave
    // the three inputs measuring different amounts of work.
    inline ::std::array<char, sample_text.size()> make_sample_array()
    {
        ::std::array<char, sample_text.size()> result{};
        ::std::ranges::copy(sample_text, result.begin());
        return result;
    }

} // namespace scl::benchmarks
