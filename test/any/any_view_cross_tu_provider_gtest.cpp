#include <any/any_view_shared_type.h>

#include <scl/utility/any/any_view.h>

/**
 * @brief Same-named TU-local type as in any_view_cross_tu_gtest.cpp: a distinct
 *        type, so a view built here must not answer that TU's any_cast.
 */
namespace
{
    struct AnyCrossTuDuck
    {
        int id = 0;
    };
} // namespace

::scl::any_view any_view_cross_tu_provider_duck() noexcept
{
    static AnyCrossTuDuck const duck{7};
    return ::scl::any_view{duck};
}

::scl::any_view any_view_cross_tu_provider_goose() noexcept
{
    static AnyViewShared::Goose const goose{9};
    return ::scl::any_view{goose};
}
