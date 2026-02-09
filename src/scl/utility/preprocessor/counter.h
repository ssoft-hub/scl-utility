#pragma once

#include <scl/utility/preprocessor/forward.h>

#include <cstdint>
#include <type_traits>

/**
 * @file
 * @brief Implements a compile-time counter mechanism.
 * @ingroup scl_utility_preprocessor
 * @details
 * This file provides a set of macros and templates to create and increment
 * counters that are evaluated at compile time. This technique is based on
 * function overloading and argument-dependent lookup (ADL).
 * It's useful for generating unique IDs or counting occurrences of something
 * within a translation unit during compilation.
 *
 * @see https://stackoverflow.com/a/6174263/1190123
 */

namespace scl::preprocessor::detail
{
    /**
     * @internal
     * @brief A type that wraps a compile-time unsigned integer constant.
     * @ingroup scl_utility_preprocessor
     * @tparam n The unsigned integer value to hold.
     */
    template <::std::uint_fast32_t n>
    using counter = ::std::integral_constant<::std::uint_fast32_t, n>;

    /**
     * @internal
     * @brief A fallback `crumb` function used to terminate the recursive lookup.
     * @ingroup scl_utility_preprocessor
     * @details
     * This function is chosen via overload resolution when no more specific `crumb`
     * has been defined for a given `Tag` and `rank_`. It contributes `acc_` to
     * the final sum. The initial call starts with `acc_ = 0`.
     *
     * @tparam Tag_ The type used to identify a specific counter.
     * @tparam rank_ The bit-rank being tested.
     * @tparam acc_ The accumulated value from lower-rank bits.
     * @return A `scl::counter` type holding the accumulated value `acc_`.
     */
    template <typename Tag_, ::std::uint_fast32_t rank_, ::std::uint_fast32_t acc_>
    constexpr counter<acc_> crumb(Tag_, counter<rank_>, counter<acc_>)
    {
        return {};
    }
} // namespace scl::preprocessor::detail

/**
 * @internal
 * @def SCL_COUNTER_CRUMB(Tag, rank, acc)
 * @brief A helper macro to call the `crumb` function with the correct arguments.
 * @ingroup scl_utility_preprocessor
 *
 * @param Tag The counter's identifier tag.
 * @param rank The bit-rank to check.
 * @param acc The currently accumulated value.
 */
#define SCL_COUNTER_CRUMB(Tag, rank, acc)                                                   \
    ::scl::preprocessor::detail::crumb(Tag{}, ::scl::preprocessor::detail::counter<rank>(), \
        ::scl::preprocessor::detail::counter<acc>())

/**
 * @def SCL_COUNTER_VALUE(Tag)
 * @brief Reads the current value of a compile-time counter.
 * @ingroup scl_utility_preprocessor
 * @details
 * This macro constructs the counter's value by checking for `crumb` overloads
 * for each bit from 0 to 31. It chains `SCL_COUNTER_CRUMB` calls, where each
 * call resolves to the most specific `crumb` function available for its rank.
 *
 * @param Tag The unique identifier for the counter. Can be a template type.
 * @return A `constexpr` value of type `std::uint_fast32_t`.
 * 
 * @code{.cpp}
 * template <typename T>
 * struct TemplateTag {};
 *  
 * // The SCL_FORWARD macro is crucial here
 * constexpr auto tpl_val_0 = SCL_COUNTER_VALUE(TemplateTag<int>);
 * static_assert(tpl_val_0 == 0);
 * 
 * SCL_COUNTER_NEXT(TemplateTag<int>)
 * constexpr auto tpl_val_1 = SCL_COUNTER_VALUE(TemplateTag<int>);
 * static_assert(tpl_val_1 == 1);
 * 
 * // A different specialization of the same template tag should be a different counter
 * constexpr auto tpl_val_float_0 = SCL_COUNTER_VALUE(TemplateTag<float>);
 * static_assert(tpl_val_float_0 == 0);
 @endcode
 */
#define SCL_COUNTER_VALUE(Tag)                                                                                                                                                \
    (SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 0,                                                                                                                             \
        SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 1,                                                                                                                          \
            SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 2,                                                                                                                      \
                SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 3,                                                                                                                  \
                    SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 4,                                                                                                              \
                        SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 5,                                                                                                          \
                            SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 6,                                                                                                      \
                                SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 7,                                                                                                  \
                                    SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 8,                                                                                              \
                                        SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 9,                                                                                          \
                                            SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 10,                                                                                     \
                                                SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 11,                                                                                 \
                                                    SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 12,                                                                             \
                                                        SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 13,                                                                         \
                                                            SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 14,                                                                     \
                                                                SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 15,                                                                 \
                                                                    SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 16,                                                             \
                                                                        SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 17,                                                         \
                                                                            SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 18,                                                     \
                                                                                SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 19,                                                 \
                                                                                    SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 20,                                             \
                                                                                        SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 21,                                         \
                                                                                            SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 22,                                     \
                                                                                                SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 23,                                 \
                                                                                                    SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 24,                             \
                                                                                                        SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 25,                         \
                                                                                                            SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 26,                     \
                                                                                                                SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 27,                 \
                                                                                                                    SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 28,             \
                                                                                                                        SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 29,         \
                                                                                                                            SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 30,     \
                                                                                                                                SCL_COUNTER_CRUMB(SCL_FORWARD(Tag), 1ul << 31, \
                                                                                                                                    0))))))))))))))))))))))))))))))))         \
            .value)

/**
 * @def SCL_COUNTER_NEXT(Tag)
 * @brief Increments a compile-time counter.
 * @ingroup scl_utility_preprocessor
 * @details
 * This macro defines a new `crumb` function overload in the `scl` namespace.
 * This new function is a better match for overload resolution for a specific
 * bit-rank, effectively "setting" that bit and incrementing the counter's value
 * for subsequent reads via `SCL_COUNTER_VALUE`.
 *
 * @param Tag The unique identifier for the counter to increment. Can be a template type.
 * 
 * @code{.cpp}
 * struct MyCounterTag1 {};
 * 
 * // The counter starts at 0 before any increments
 * constexpr auto val1_0 = SCL_COUNTER_VALUE(MyCounterTag1);
 * static_assert(val1_0, 0);
 * 
 * SCL_COUNTER_NEXT(MyCounterTag1)
 * constexpr auto val1_1 = SCL_COUNTER_VALUE(MyCounterTag1);
 * static_assert(val1_1, 1);
 * 
 * SCL_COUNTER_NEXT(MyCounterTag1)
 * constexpr auto val1_2 = SCL_COUNTER_VALUE(MyCounterTag1);
 * static_assert(val1_2, 2);
 * @endcode
 */
#define SCL_COUNTER_NEXT(Tag)                                                                        \
    namespace scl::preprocessor::detail                                                              \
    {                                                                                                \
        constexpr counter<SCL_COUNTER_VALUE(SCL_FORWARD(Tag)) + 1> crumb(Tag,                         \
            counter<(SCL_COUNTER_VALUE(SCL_FORWARD(Tag)) + 1) & ~SCL_COUNTER_VALUE(SCL_FORWARD(Tag))>, \
            counter<(SCL_COUNTER_VALUE(SCL_FORWARD(Tag)) + 1) & SCL_COUNTER_VALUE(SCL_FORWARD(Tag))>)  \
        {                                                                                            \
            return {};                                                                               \
        }                                                                                            \
    }

// http://b.atch.se/posts/non-constant-constant-expressions/
// http://b.atch.se/posts/constexpr-counter/#prereq-adl
// https://stackoverflow.com/questions/44267673/is-stateful-metaprogramming-ill-formed-yet
// http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#2118

// namespace scl
//{
//     template < typename Tag_ >
//     class counter
//     {
//         using Value = int;

//        template < Value n >
//        struct Index
//        {
//            friend constexpr Value adlValue ( Index< n > );
//        };

//        template < Value n >
//        struct Writer
//        {
//            friend constexpr Value adlValue ( Index< n > ) { return n; }
//            static constexpr Value value = n;
//        };

//        template < Value n >
//        static constexpr Value increment ( float, Index< n > )
//        { return n; }

//        template < Value n, Value = adlValue( Index< n >{} ) >
//        static constexpr Value increment ( Value, Index< n >, Value v = increment( 0, Index< n + 1 >{} ) ) { return v; }

//    public:
//        template < Value n = 0 >
//        static constexpr Value next ( Value v = value_of< Writer< increment( 0, Index< 0 >{} ) + n > > ) { return v + 1; }

//        template < Value n = 0 >
//        static constexpr Value value ( Value v = value_of< Writer< increment( 0, Index< 0 >{} ) + n - 1 > > ) { return v + 1; }
//    };
//}
