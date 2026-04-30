#pragma once

/// @file algorithm.h
/// @brief Hierarchy relationship query algorithms.
/// @ingroup scl_utility_hierarchy

#include <memory>

namespace scl::hierarchy
{
    /// @brief Identity comparison policy using operator== for value types.
    /// @ingroup scl_utility_hierarchy
    template <typename FirstValue, typename SecondValue>
    struct identity_of
    {
        static constexpr bool
        check(FirstValue && first, SecondValue && second) noexcept(noexcept(first == second))
        {
            return first == second;
        }
    };

    /// @brief Identity comparison policy for lvalue references — compares by address.
    /// @ingroup scl_utility_hierarchy
    template <typename FirstValue, typename SecondValue>
    struct identity_of<FirstValue &, SecondValue &>
    {
        static constexpr bool check(FirstValue & first, SecondValue & second) noexcept
        {
            auto const * const fp = ::std::addressof(first);
            return fp == ::std::addressof(second) && fp != nullptr;
        }
    };

    /// @brief Returns the parent of @p value by calling value.parent().
    /// @ingroup scl_utility_hierarchy
    template <typename Value>
    inline constexpr decltype(auto)
    parent_of(Value && value) noexcept(noexcept(::std::forward<Value>(value).parent()))
        requires requires(Value && v) { v.parent(); }
    {
        return ::std::forward<Value>(value).parent();
    }

    /// @brief Returns true when @p first and @p second refer to the same object.
    /// @ingroup scl_utility_hierarchy
    template <typename FirstValue, typename SecondValue>
    inline constexpr bool
    are_identical(FirstValue && first, SecondValue && second) noexcept(noexcept(identity_of<FirstValue,
        SecondValue>::check(::std::forward<FirstValue>(first), ::std::forward<SecondValue>(second))))
    {
        return identity_of<FirstValue, SecondValue>::check(::std::forward<FirstValue>(first),
            ::std::forward<SecondValue>(second));
    }

    /// @brief Returns true when @p value has a parent.
    /// @ingroup scl_utility_hierarchy
    ///
    /// Compares parent_of(value) with itself via address; a null parent pointer
    /// results in a null address that fails the != nullptr guard.
    template <typename Value>
    inline constexpr bool
    has_parent(Value && value) noexcept(noexcept(parent_of(::std::forward<Value>(value))))
        requires requires(Value && v) { v.parent(); }
    {
        decltype(auto) p = parent_of(::std::forward<Value>(value));
        return are_identical(p, p);
    }

    /// @brief Returns true when @p parent is the direct parent of @p value.
    /// @ingroup scl_utility_hierarchy
    template <typename Parent, typename Value>
    inline constexpr bool
    is_parent_of(Parent && parent, Value && value) noexcept(noexcept(::scl::hierarchy::are_identical(
        ::std::forward<Parent>(parent), parent_of(::std::forward<Value>(value)))))
        requires requires(Value && v) { v.parent(); }
    {
        return ::scl::hierarchy::are_identical(::std::forward<Parent>(parent),
            parent_of(::std::forward<Value>(value)));
    }

    /// @brief Returns true when @p parent is a direct or transitive ancestor of @p value.
    /// @ingroup scl_utility_hierarchy
    template <typename Parent, typename Value>
    inline constexpr bool is_ancestor_of(Parent && parent, Value && value)
        requires requires(Value && v) { v.parent(); }
    {
        if (!::scl::hierarchy::has_parent(::std::forward<Value>(value)))
            return false;
        if (::scl::hierarchy::is_parent_of(::std::forward<Parent>(parent), ::std::forward<Value>(value)))
            return true;
        return ::scl::hierarchy::is_ancestor_of(::std::forward<Parent>(parent),
            parent_of(::std::forward<Value>(value)));
    }

    /// @brief Returns true when @p first and @p second share the same parent.
    /// @ingroup scl_utility_hierarchy
    template <typename ParentedFirst, typename ParentedSecond>
    inline constexpr bool
    are_sibling(ParentedFirst && first, ParentedSecond && second) noexcept(noexcept(::scl::hierarchy::are_identical(
        parent_of(::std::forward<ParentedFirst>(first)), parent_of(::std::forward<ParentedSecond>(second)))))
        requires requires(ParentedFirst && f, ParentedSecond && s) {
                     f.parent();
                     s.parent();
                 }
    {
        return ::scl::hierarchy::are_identical(parent_of(::std::forward<ParentedFirst>(first)),
            parent_of(::std::forward<ParentedSecond>(second)));
    }

} // namespace scl::hierarchy
