#pragma once
#ifndef SCL_META_TYPE_OF_H
#define SCL_META_TYPE_OF_H

namespace ScL
{
    namespace Detail
    {
        template <typename _Type>
        struct TypeOf
        {
            using Type = typename _Type::type;
        };
        //!< This struct must be specialized for not standard cases.
    } // namespace Detail

    template <typename _Type>
    using TypeOf = typename Detail::TypeOf<_Type>::Type;
} // namespace ScL

#endif
