#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <string>
#include <string_view>

namespace scl::benchmarks
{

    // Three sizes, because what an annotation is worth depends on them. Forcing a hash
    // inline removes per-call overhead, whose share grows as the input shrinks, and it
    // exposes a compile-time length to the loop, which only a fixed-size array carries.
    // 63 bytes is the size the claim on this work was first made at; 8 and 16 are what a
    // hash-map key usually is.
    inline constexpr ::std::string_view sample_text_8 = "quickfox";
    inline constexpr ::std::string_view sample_text_16 = "quick brown foxy";
    inline constexpr ::std::string_view sample_text_63 = "the quick brown fox jumps over the lazy dog, and then some more";

    inline constexpr ::std::u16string_view sample_wide_63 = u"the quick brown fox jumps over the lazy dog, and then some more";
    inline constexpr ::std::string_view sample_text_126 =
        "the quick brown fox jumps over the lazy dog, and then some more"
        "the quick brown fox jumps over the lazy dog, and then some more";

    static_assert(sample_text_8.size() == 8);
    static_assert(sample_text_16.size() == 16);
    static_assert(sample_text_63.size() == 63);
    static_assert(sample_wide_63.size() == 63);
    static_assert(sample_text_126.size() == 126);
    static_assert(sample_wide_63.size() * sizeof(char16_t) == sample_text_126.size());

    inline ::std::string make_sample_string(::std::string_view text) { return ::std::string{text}; }

    // std::array rather than char[N]: a bounded array of character type has its last
    // element dropped as a string terminator, which would hash one byte fewer and leave
    // the forms measuring different amounts of work.
    template <::std::size_t Size>
    ::std::array<char, Size> make_sample_array(::std::string_view text)
    {
        ::std::array<char, Size> result{};
        ::std::ranges::copy(text, result.begin());
        return result;
    }

} // namespace scl::benchmarks
