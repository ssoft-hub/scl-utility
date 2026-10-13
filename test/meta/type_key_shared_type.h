#pragma once

/**
 * @brief External-linkage type shared by several test translation units.
 */
namespace TypeKeyShared
{
    struct Goose
    {};

    struct Pointing
    {
        Pointing const * operator->() const { return this; }

        int operator-(Pointing) const { return 0; }
    };

    template <auto Function>
    struct Holder
    {};

    template <typename T>
    struct Wrap
    {};

    using arrow_type = Wrap<Holder<&Pointing::operator- >>;

} // namespace TypeKeyShared
