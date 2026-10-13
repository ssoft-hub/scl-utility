#pragma once

#include <scl/utility/preprocessor/forward.h>

#include <cstdint>
#include <type_traits>

/**
 * @file
 * @brief Compile-time counters, one per tag type.
 * @ingroup scl_utility_preprocessor
 * @details
 * @ref SCL_COUNTER_NEXT declares an overload of a helper function that encodes the next
 * count, and @ref SCL_COUNTER_VALUE calls that function by its qualified name, so overload
 * resolution over the overloads declared above the call assembles the count, and the count grows
 * through the translation unit.
 */

namespace scl::preprocessor::detail
{
    /**
     * @internal
     * @brief A type that wraps a compile-time unsigned integer constant.
     * @ingroup scl_utility_preprocessor
     * @tparam N The unsigned integer value to hold.
     */
    template <::std::uint_fast32_t N>
    using counter = ::std::integral_constant<::std::uint_fast32_t, N>;

    /**
     * @internal
     * @brief The `crumb` chosen for a bit that the count leaves clear.
     * @ingroup scl_utility_preprocessor
     * @details
     * Overload resolution prefers an overload that @ref SCL_COUNTER_NEXT declared for the same
     * `Tag`, `Rank` and `Acc`. Where none was declared, this one is chosen and returns `Acc`
     * unchanged, so the bit `Rank` stays clear. @ref SCL_COUNTER_VALUE reads the bits from
     * the highest down, starting with `Acc = 0`.
     *
     * @tparam Tag  The type used to identify a specific counter.
     * @tparam Rank The value of the bit being read, a power of two.
     * @tparam Acc  The bits of the count above `Rank`, read so far.
     * @return A `counter` holding `Acc`.
     */
    template <typename Tag, ::std::uint_fast32_t Rank, ::std::uint_fast32_t Acc>
    constexpr counter<Acc>
    crumb(Tag /*tag*/, counter<Rank> /*rank*/, counter<Acc> /*acc*/) // NOLINT(readability-named-parameter)
    {
        return {};
    }
} // namespace scl::preprocessor::detail

/**
 * @internal
 * @def SCL_DETAIL_COUNTER_CRUMB(Tag, rank, acc)
 * @brief A helper macro to call the `crumb` function with the correct arguments.
 * @ingroup scl_utility_preprocessor
 *
 * @param Tag The counter's identifier tag.
 * @param rank The value of the bit to read, a power of two.
 * @param acc The bits of the count above @p rank, read so far.
 */
#define SCL_DETAIL_COUNTER_CRUMB(Tag, rank, acc)                                            \
    ::scl::preprocessor::detail::crumb(Tag{}, ::scl::preprocessor::detail::counter<rank>(), \
        ::scl::preprocessor::detail::counter<acc>())

/**
 * @def SCL_COUNTER_VALUE(Tag)
 * @brief Reads the counter of a tag at the point of expansion.
 * @ingroup scl_utility_preprocessor
 * @details
 * Reads `0` before the first @ref SCL_COUNTER_NEXT of @p Tag in the translation unit, and one
 * more after each of them. Each type is the tag of a counter of its own, and two specializations
 * of one class template are two tags.
 *
 * @param Tag The type whose counter is read.
 * @return A constant expression of type `std::uint_fast32_t`.
 *
 * @snippet preprocessor/counter/preprocessor_counter_example.cpp count
 */
#define SCL_COUNTER_VALUE(Tag)                                                                                                                                                        \
    (SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 0,                                                                                                                             \
        SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 1,                                                                                                                          \
            SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 2,                                                                                                                      \
                SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 3,                                                                                                                  \
                    SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 4,                                                                                                              \
                        SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 5,                                                                                                          \
                            SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 6,                                                                                                      \
                                SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 7,                                                                                                  \
                                    SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 8,                                                                                              \
                                        SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 9,                                                                                          \
                                            SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 10,                                                                                     \
                                                SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 11,                                                                                 \
                                                    SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 12,                                                                             \
                                                        SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 13,                                                                         \
                                                            SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 14,                                                                     \
                                                                SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 15,                                                                 \
                                                                    SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 16,                                                             \
                                                                        SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 17,                                                         \
                                                                            SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 18,                                                     \
                                                                                SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 19,                                                 \
                                                                                    SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 20,                                             \
                                                                                        SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 21,                                         \
                                                                                            SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 22,                                     \
                                                                                                SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 23,                                 \
                                                                                                    SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 24,                             \
                                                                                                        SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 25,                         \
                                                                                                            SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 26,                     \
                                                                                                                SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 27,                 \
                                                                                                                    SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 28,             \
                                                                                                                        SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 29,         \
                                                                                                                            SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 30,     \
                                                                                                                                SCL_DETAIL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 31, \
                                                                                                                                    0))))))))))))))))))))))))))))))))                 \
            .value)

/**
 * @def SCL_COUNTER_NEXT(Tag)
 * @brief Adds one to the counter of a tag for the rest of the translation unit.
 * @ingroup scl_utility_preprocessor
 * @details
 * Expands to a declaration, so every later @ref SCL_COUNTER_VALUE of @p Tag reads one more
 * than before. Being a declaration rather than a statement, it stands at namespace or class
 * scope, not in a function body.
 *
 * @param Tag The type whose counter advances.
 *
 * @snippet preprocessor/counter/preprocessor_counter_example.cpp template_tag
 */
#define SCL_COUNTER_NEXT(Tag)                                                                          \
    namespace scl::preprocessor::detail                                                                \
    {                                                                                                  \
        constexpr counter<SCL_COUNTER_VALUE(SCL_FORWARD(Tag)) + 1> crumb(Tag,                          \
            counter<(SCL_COUNTER_VALUE(SCL_FORWARD(Tag)) + 1) & ~SCL_COUNTER_VALUE(SCL_FORWARD(Tag))>, \
            counter<(SCL_COUNTER_VALUE(SCL_FORWARD(Tag)) + 1) & SCL_COUNTER_VALUE(SCL_FORWARD(Tag))>)  \
        {                                                                                              \
            return {};                                                                                 \
        }                                                                                              \
    }
