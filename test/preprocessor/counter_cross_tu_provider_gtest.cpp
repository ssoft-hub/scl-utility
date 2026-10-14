#include <scl/utility/preprocessor/counter.h>

#include <cstdint>

/**
 * @brief The tag counter_cross_tu_gtest.cpp advances once; this translation unit advances it twice.
 */
struct CrossTuTag
{};

SCL_COUNTER_NEXT(::CrossTuTag)
SCL_COUNTER_NEXT(::CrossTuTag)

::std::uint_fast32_t cross_tu_provider_count() noexcept { return SCL_COUNTER_VALUE(::CrossTuTag); }
