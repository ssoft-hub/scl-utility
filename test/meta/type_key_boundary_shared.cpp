#include <meta/type_key_shared_type.h>

#include <scl/utility/meta/type_key.h>

#if defined(_WIN32)
#define SCL_TEST_EXPORT __declspec(dllexport)
#else
#define SCL_TEST_EXPORT
#endif

/**
 * @brief Same-named TU-local type as in type_key_boundary_gtest.cpp, on the
 *        shared-library side of the module boundary.
 */
namespace
{
    struct BoundaryDuck
    {};
} // namespace

SCL_TEST_EXPORT ::scl::type_key const & boundary_shared_duck_key() noexcept
{
    return ::scl::type_key_of<BoundaryDuck>();
}

SCL_TEST_EXPORT ::scl::type_key const & boundary_shared_goose_key() noexcept
{
    return ::scl::type_key_of<TypeKeyShared::Goose>();
}
