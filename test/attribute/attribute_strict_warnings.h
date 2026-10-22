#pragma once

#if defined(__clang__)
#pragma clang diagnostic error "-Wall"
#pragma clang diagnostic error "-Wextra"
#pragma clang diagnostic error "-Wimplicit-fallthrough"
#pragma clang diagnostic error "-Wunknown-attributes"
#pragma clang diagnostic error "-Wignored-attributes"
#pragma clang diagnostic error "-Wdeprecated-declarations"
#pragma clang diagnostic error "-Wunused-result"
#pragma clang diagnostic error "-Wdangling"
#elif defined(__GNUC__)
#pragma GCC diagnostic error "-Wall"
#pragma GCC diagnostic error "-Wextra"
#pragma GCC diagnostic error "-Wimplicit-fallthrough=3"
#pragma GCC diagnostic error "-Wattributes"
#pragma GCC diagnostic error "-Wdeprecated-declarations"
#pragma GCC diagnostic error "-Wunused-result"
#elif defined(_MSC_VER)
#pragma warning(1: 4068 4100 4101 4189 4505 4702 4715 4834 4858 4869 4996 5030 5245 5262)
#pragma warning(error: 4068 4100 4101 4189 4505 4702 4715 4834 4858 4869 4996 5030 5245 5262)
#endif
