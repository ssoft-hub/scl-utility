#pragma once
#ifndef SCL_META_VALUE_OF_H
#define SCL_META_VALUE_OF_H

namespace ScL
{
    namespace Detail
    {
        template < typename _Type >
        struct ValueOf : _Type {};
            //!< This struct must be specialized for not standard cases.
    }

    template < typename _Type >
    auto constexpr value_of = Detail::ValueOf< _Type >::value;
}

#endif
