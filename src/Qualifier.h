#pragma once
#ifndef SCL_UTILITY_AS_CONST_H
#define SCL_UTILITY_AS_CONST_H

#include <memory>
#include <type_traits>

namespace ScL
{
    template <typename _Type>
    constexpr ::std::add_const_t<_Type> & asConst(_Type & value) noexcept
    {
        return value;
    }

    template <typename _Type>
    constexpr ::std::add_const_t<_Type> && asConst(_Type && value) noexcept
    {
        return ::std::forward< ::std::add_const_t<_Type> &&>(value);
    }

    template <typename _Type>
    constexpr ::std::add_volatile_t<_Type> & asVolatile(_Type & value) noexcept
    {
        return value;
    }

    template <typename _Type>
    constexpr ::std::add_volatile_t<_Type> && asVolatile(_Type && value) noexcept
    {
        return ::std::forward< ::std::add_volatile_t<_Type> &&>(value);
    }

    template <typename _Type>
    constexpr ::std::add_cv_t<_Type> & asConstVolatile(_Type & value) noexcept
    {
        return value;
    }

    template <typename _Type>
    constexpr ::std::add_cv_t<_Type> && asConstVolatile(_Type && value) noexcept
    {
        return ::std::forward< ::std::add_cv_t<_Type> &&>(value);
    }
} // namespace ScL

#endif
