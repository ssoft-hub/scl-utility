/**
 * @example hash_byte_view_example.cpp
 * @brief Hashing a range whose elements are wider than a byte.
 *
 * The hash functions read one byte per element, so they take a range of byte-sized
 * elements only. A `std::u16string_view` is not one: each of its code units carries two
 * bytes, and there is no single answer to which of them the hash function should see. Passing it
 * is a compile error rather than a silent truncation — @ref scl::hash::byte_view is where
 * the answer is spelled out.
 */

#include <scl/utility/hash/byte_view.h>
#include <scl/utility/hash/fnv1a.h>
#include <scl/utility/hash/key.h>

#include <cstdint>
#include <iostream>
#include <string_view>
#include <vector>

using ::scl::hash::byte_view;
using ::scl::hash::fnv1a;
using ::scl::hash::key;

// ============================================================================
// A wide string reaches a hash value through byte_view, and only through it
// ============================================================================

// fnv1a(::std::u16string_view{u"start"}) does not compile: char16_t is two bytes wide.
// Saying byte_view is saying which bytes are meant.
constexpr auto wide_value = fnv1a(byte_view(::std::u16string_view{u"start"}));

// Every bit of every code unit reaches the hash value, so two texts that differ above the low
// byte are two inputs — the low bytes of both spell "\x00\x00".
static_assert(fnv1a(byte_view(::std::u16string_view{u"Ā"})) != fnv1a(byte_view(::std::u16string_view{u"Ȁ"})));

// A wide spelling is its own input: it carries the zero bytes the narrow one does not.
static_assert(wide_value != fnv1a(::std::string_view{"start"}));

// ============================================================================
// The byte order is the value's, not the machine's
// ============================================================================

// Each element contributes its bytes least significant first, whatever the host does with
// them in memory, so two machines hash one input alike.
constexpr ::std::uint32_t numbers[]{0x04030201U};
static_assert(fnv1a(byte_view(numbers)) == fnv1a(::std::string_view{"\x01\x02\x03\x04", 4}));

// ============================================================================
// A byte-sized element passes straight through
// ============================================================================

// Saying byte_view where none is needed costs nothing and changes nothing.
static_assert(fnv1a(byte_view(::std::string_view{"start"})) == fnv1a(::std::string_view{"start"}));

int code_of(::std::u16string_view const command)
{
    switch (key<>{byte_view(command)})
    {
    case key<>{byte_view(::std::u16string_view{u"start"})}:
        return 1;
    case key<>{byte_view(::std::u16string_view{u"stop"})}:
        return 2;
    default:
        return 0;
    }
}

int main()
{
    ::std::cout << code_of(u"start") << code_of(u"stop") << code_of(u"pause") << '\n'; // 120

    ::std::vector<::std::uint32_t> const identifiers{1U, 2U, 3U};
    ::std::cout << (fnv1a(byte_view(identifiers)) != 0) << '\n'; // 1
}
