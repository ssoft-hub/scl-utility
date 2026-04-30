#pragma once

#include <memory>

namespace scl::hierarchy
{
    template <typename FirstValue, typename SecondValue>
    struct IdentityOf
    {
        static constexpr bool check(FirstValue && first, SecondValue && second)
        {
            return first == second;
        }
    };

    template <typename FirstValue, typename SecondValue>
    struct IdentityOf<FirstValue &, SecondValue &>
    {
        static constexpr bool check(FirstValue & first, SecondValue & second)
        {
            return ::std::addressof(first) == ::std::addressof(second) && ::std::addressof(first) != nullptr;
        }
    };
} // namespace scl::hierarchy

namespace scl::hierarchy
{
    template <typename Value>
    inline constexpr decltype(auto) parent_of(Value && value)
    {
        return ::std::forward<Value>(value).parent();
    }

    template <typename FirstValue, typename SecondValue>
    inline constexpr bool are_identical(FirstValue && first, SecondValue && second)
    {
        return IdentityOf<FirstValue, SecondValue>::check(::std::forward<FirstValue>(first),
            ::std::forward<SecondValue>(second));
    }

    template <typename Value>
    inline constexpr bool has_parent(Value && value)
    {
        return are_identical(parent_of(::std::forward<Value>(value)),
            parent_of(::std::forward<Value>(value)));
    }

    template <typename Parent, typename Value>
    inline constexpr bool is_parent_of(Parent && parent, Value && value)
    {
        return ::scl::hierarchy::are_identical(::std::forward<Parent>(parent),
            parent_of(::std::forward<Value>(value)));
    }

    template <typename Parent, typename Value>
    inline constexpr bool is_ancestor_of(Parent && parent, Value && value)
    {
        if (!::scl::hierarchy::has_parent(::std::forward<Value>(value)))
            return false;

        if (::scl::hierarchy::is_parent_of(::std::forward<Parent>(parent), ::std::forward<Value>(value)))
            return true;

        return ::scl::hierarchy::is_ancestor_of(::std::forward<Parent>(parent),
            parent_of(::std::forward<Value>(value)));
    }

    template <typename ParentedFirst, typename ParentedSecond>
    inline constexpr bool are_sibling(ParentedFirst && first, ParentedSecond && second)
    {
        return ::scl::hierarchy::are_identical(parent_of(::std::forward<ParentedFirst>(first)),
            parent_of(::std::forward<ParentedSecond>(second)));
    }
} // namespace scl::hierarchy
