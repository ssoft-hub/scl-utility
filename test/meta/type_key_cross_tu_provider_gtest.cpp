#include <meta/type_key_shared_type.h>

#include <scl/utility/meta/type_key.h>

#include <vector>

/**
 * @brief Same-named TU-local types as in type_key_cross_tu_gtest.cpp:
 *        distinct types, expected to produce different keys.
 */
namespace
{
    struct CrossTuDuck
    {};

    struct CrossTuPointing
    {
        CrossTuPointing const * operator->() const { return this; }

        int operator-(CrossTuPointing) const { return 0; }
    };

    using cross_tu_arrow = TypeKeyShared::Wrap<TypeKeyShared::Holder<&CrossTuPointing::operator- >>;

    constexpr ::scl::type_key arrow_copy = ::scl::type_key_of<TypeKeyShared::arrow_type>();
} // namespace

::scl::type_key const & cross_tu_provider_duck_key() noexcept
{
    return ::scl::type_key_of<CrossTuDuck>();
}

::scl::type_key const & cross_tu_provider_goose_key() noexcept
{
    return ::scl::type_key_of<TypeKeyShared::Goose>();
}

::scl::type_key const & cross_tu_provider_arrow_copy() noexcept { return arrow_copy; }

::scl::type_key const & cross_tu_provider_arrow_key() noexcept
{
    return ::scl::type_key_of<cross_tu_arrow>();
}

::scl::type_key const & cross_tu_provider_nested_duck_key() noexcept
{
    return ::scl::type_key_of<::std::vector<int CrossTuDuck::*>>();
}
