#pragma once
#ifndef SCL_META_VALUE_OF_H
#define SCL_META_VALUE_OF_H

namespace ScL::Detail
{
    //! This struct must be specialized for not standard cases.
    template <typename _Type>
    struct ValueOf : _Type
    {};
} // namespace ScL::Detail

namespace ScL
{
    template <typename _Type>
    constexpr auto value_of = Detail::ValueOf<_Type>::value;
} // namespace ScL

#endif
