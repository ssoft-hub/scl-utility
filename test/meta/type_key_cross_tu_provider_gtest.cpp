#include <meta/type_key_shared_type.h>

#include <scl/utility/meta/type_key.h>

/**
 * @brief Same-named TU-local type as in type_key_cross_tu_gtest.cpp:
 *        a distinct type, expected to produce a different key.
 */
namespace
{
    struct CrossTuDuck
    {};
} // namespace

::scl::type_key const & cross_tu_provider_duck_key() noexcept
{
    return ::scl::type_key_of<CrossTuDuck>();
}

::scl::type_key const & cross_tu_provider_goose_key() noexcept
{
    return ::scl::type_key_of<TypeKeyShared::Goose>();
}
