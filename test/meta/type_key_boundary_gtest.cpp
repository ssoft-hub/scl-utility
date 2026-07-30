#include <gtest_utils.h>

#include <meta/type_key_shared_type.h>

#include <scl/utility/meta/type_key.h>

/**
 * @brief Same-named TU-local type as in type_key_boundary_shared.cpp, on the
 *        executable side of the module boundary.
 */
namespace
{
    struct BoundaryDuck
    {};
} // namespace

::scl::type_key const & boundary_shared_duck_key() noexcept;
::scl::type_key const & boundary_shared_goose_key() noexcept;

/**
 * @test Verify an external named type produces equal keys on both sides of a
 *       shared-library boundary (no false reject from unmerged inline
 *       instantiations). The library returns a reference to its own per-type
 *       constant, distinct from the executable's one, so equality runs the
 *       content fallback; key-object addresses may legitimately differ
 *       between modules and are not asserted.
 */
TEST(TypeKeyBoundaryTest, ExternalTypeMatchesAcrossBoundary)
{
    auto const & exe_key = ::scl::type_key_of<TypeKeyShared::Goose>();
    auto const & lib_key = boundary_shared_goose_key();

    EXPECT_EQ(exe_key.name(), lib_key.name());
    EXPECT_EQ(exe_key, lib_key);
}

/**
 * @test Verify same-named anonymous-namespace types on the two sides of a
 *       shared-library boundary produce unequal keys (no false match).
 */
TEST(TypeKeyBoundaryTest, SameNamedTuLocalTypesDifferAcrossBoundary)
{
    auto const & exe_key = ::scl::type_key_of<BoundaryDuck>();
    auto const & lib_key = boundary_shared_duck_key();

    EXPECT_EQ(exe_key.name(), lib_key.name());
    EXPECT_NE(exe_key, lib_key);
}
