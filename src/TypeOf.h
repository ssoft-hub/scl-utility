#pragma once
#ifndef SCL_META_TYPE_OF_H
#define SCL_META_TYPE_OF_H

namespace ScL::Detail
{
    //! This struct must be specialized for not standard cases.
    template <typename _Type>
    struct TypeOf
    {
        using Type = typename _Type::type;
    };
} // namespace ScL::Detail

namespace ScL
{
    template <typename _Type>
    using TypeOf = typename Detail::TypeOf<_Type>::Type;
} // namespace ScL

#endif
