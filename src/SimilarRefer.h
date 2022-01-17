#pragma once
#ifndef SCL_UTILITY_SIMILAR_REFER_H
#define SCL_UTILITY_SIMILAR_REFER_H

#include <type_traits>

namespace ScL
{
    namespace Detail
    {
        template <typename _Type, typename _Refer>
        struct SimilarReferHelper
        {
            static_assert(::std::is_reference<_Refer>::value,
                "The template parameter _Refer must to be a reference type.");

            using ValueFromRefer = ::std::remove_reference_t<_Refer>;
            static constexpr bool is_lvalue = ::std::is_lvalue_reference<_Refer>::value;
            static constexpr bool is_rvalue = ::std::is_rvalue_reference<_Refer>::value;
            static constexpr bool is_const = ::std::is_const<ValueFromRefer>::value;
            static constexpr bool is_volatile = ::std::is_volatile<ValueFromRefer>::value;

            using VCheckedType = ::std::conditional_t<is_volatile, ::std::add_volatile_t<_Type>, _Type>;
            using CVCheckedType = ::std::conditional_t<is_const, ::std::add_const_t<VCheckedType>, VCheckedType>;
            using LCVCheckedType =
                ::std::conditional_t<is_lvalue, ::std::add_lvalue_reference_t<CVCheckedType>, CVCheckedType>;
            using Type = ::std::conditional_t<is_rvalue, ::std::add_rvalue_reference_t<LCVCheckedType>, LCVCheckedType>;

            // NOTE: _Type может являться reference типом, в этом случае применение свойств
            // lvalue_reference/rvalue_reference может иметь результат отличный от _Refer.

            // NOTE: _Type может изначально иметь const/volatile свойства, в этом
            // случае результат может не соответствовать этим свойствам для _Refer.

            // static_assert( ::std::is_reference< _Type >::value
            //     || ::ScL::Feature::is_similar< Type, _Refer >, "Result Type must to be similar
            //     _Refer" );
        };
    } // namespace Detail
} // namespace ScL

namespace ScL
{
    template <typename _Type, typename _Refer>
    using SimilarRefer = typename ::ScL::Detail::SimilarReferHelper<_Type, _Refer>::Type;
    //!< Наделение типа свойствами ссылки.
} // namespace ScL

#endif
