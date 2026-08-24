// Code-size counterpart to the *_gbench sources: one externally visible wrapper per
// hash function per input type, so -ffunction-sections gives each its own section and
// arm-none-eabi-size reports what annotating the public API costs in .text.
//
// Never linked or run. The wrappers exist to be compiled, not called.

#include <scl/utility/hash/djb2.h>
#include <scl/utility/hash/fnv1a.h>
#include <scl/utility/hash/jenkins_ota.h>
#include <scl/utility/hash/sdbm.h>
#include <scl/utility/hash/siphash.h>

#include <array>
#include <cstdint>
#include <string>
#include <string_view>

namespace scl::benchmarks
{

    using byte_array = ::std::array<char, 63>;

    ::std::uint64_t fnv1a_of_view(::std::string_view text) { return ::scl::hash::fnv1a(text); }

    ::std::uint64_t fnv1a_of_string(::std::string const & text) { return ::scl::hash::fnv1a(text); }

    ::std::uint64_t fnv1a_of_array(byte_array const & bytes) { return ::scl::hash::fnv1a(bytes); }

    ::std::uint64_t djb2_of_view(::std::string_view text) { return ::scl::hash::djb2(text); }

    ::std::uint64_t djb2_of_string(::std::string const & text) { return ::scl::hash::djb2(text); }

    ::std::uint64_t djb2_of_array(byte_array const & bytes) { return ::scl::hash::djb2(bytes); }

    ::std::uint64_t sdbm_of_view(::std::string_view text) { return ::scl::hash::sdbm(text); }

    ::std::uint64_t sdbm_of_string(::std::string const & text) { return ::scl::hash::sdbm(text); }

    ::std::uint64_t sdbm_of_array(byte_array const & bytes) { return ::scl::hash::sdbm(bytes); }

    ::std::uint64_t jenkins_ota_of_view(::std::string_view text)
    {
        return ::scl::hash::jenkins_ota(text);
    }

    ::std::uint64_t jenkins_ota_of_string(::std::string const & text)
    {
        return ::scl::hash::jenkins_ota(text);
    }

    ::std::uint64_t jenkins_ota_of_array(byte_array const & bytes)
    {
        return ::scl::hash::jenkins_ota(bytes);
    }

    ::std::uint64_t siphash_of_view(::std::string_view text) { return ::scl::hash::siphash(text); }

    ::std::uint64_t siphash_of_string(::std::string const & text)
    {
        return ::scl::hash::siphash(text);
    }

    ::std::uint64_t siphash_of_array(byte_array const & bytes)
    {
        return ::scl::hash::siphash(bytes);
    }

} // namespace scl::benchmarks
