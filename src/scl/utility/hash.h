#pragma once

/**
 * @file hash.h
 * @brief ScL non-cryptographic hash utilities.
 * @details This header aggregates all ScL hash utilities.
 */

/**
 * @defgroup scl_utility_hash ScL Hash Utilities
 * @brief Non-cryptographic compile-time hash functions and digest types.
 * @{
 */

#include <scl/utility/hash/djb2.h>
#include <scl/utility/hash/fnv1a.h>
#include <scl/utility/hash/jenkins_ota.h>
#include <scl/utility/hash/key.h>
#include <scl/utility/hash/sdbm.h>
#include <scl/utility/hash/siphash.h>

/** @} */ // end of group scl_utility_hash
