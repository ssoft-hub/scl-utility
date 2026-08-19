#include <any/any_view_shared_type.h>

#include <scl/utility/any/any.h>

/**
 * @brief Same-named TU-local type as in any_cross_tu_gtest.cpp: a distinct type,
 *        so an any built here must not answer that TU's any_cast.
 */
namespace
{
    struct AnyCrossTuDuck
    {
        int id = 0;
    };
} // namespace

::scl::any any_cross_tu_provider_duck() { return ::scl::any{AnyCrossTuDuck{7}}; }

::scl::any any_cross_tu_provider_goose() { return ::scl::any{AnyViewShared::Goose{9}}; }
