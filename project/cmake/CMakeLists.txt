# Header-only library: utility
# Produces targets:
#   - scl-utility (real interface target)
#   - SCL::utility (ALIAS in build tree)
# Exported/imported name is set to "utility" so installed import is SCL::utility.

# Resolve module root directory (module/utility)
get_filename_component(SCL_UTILITY_ROOT "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)

set(MODULE utility)

# ---- Make headers visible in IDE ----
set(SCL_UTILITY_SRC_DIR "${SCL_UTILITY_ROOT}/src")
file(GLOB_RECURSE SCL_UTILITY_ALL_HEADERS CONFIGURE_DEPENDS
    "${SCL_UTILITY_SRC_DIR}/*.h"
    "${SCL_UTILITY_SRC_DIR}/*.hpp"
    "${SCL_UTILITY_SRC_DIR}/*.hh"
    "${SCL_UTILITY_SRC_DIR}/*.hxx"
    "${SCL_UTILITY_SRC_DIR}/*.inl"
)
set(SCL_UTILITY_PUBLIC_HEADERS "${SCL_UTILITY_ALL_HEADERS}")
list(FILTER SCL_UTILITY_PUBLIC_HEADERS EXCLUDE REGEX ".*/private/.*")

# ---- Private headers and sources (implementation files) ----
file(GLOB_RECURSE SCL_UTILITY_PRIVATE_SOURCES CONFIGURE_DEPENDS
    "${SCL_UTILITY_SRC_DIR}/*.c"
    "${SCL_UTILITY_SRC_DIR}/*.cpp"
)

# ---- Make target scl-${MODULE}: INTERFACE/STATIC/SHARED ----
if (SCL_UTILITY_PRIVATE_SOURCES)
    if (BUILD_SHARED_LIBS)
        add_library(scl-${MODULE} SHARED)
    else()
        add_library(scl-${MODULE} STATIC)
    endif()
    set(SCL_UTILITY_SCOPE PUBLIC)
else()
    add_library(scl-${MODULE} INTERFACE)
    set(SCL_UTILITY_SCOPE INTERFACE)
endif()

add_library(SCL::${MODULE} ALIAS scl-${MODULE})

# Requires for C++
target_compile_features(scl-${MODULE} ${SCL_UTILITY_SCOPE} cxx_std_20)

# Ensure the imported/install name is SCL::utility
set_target_properties(scl-${MODULE} PROPERTIES EXPORT_NAME utility)

target_include_directories(scl-${MODULE}
    ${SCL_UTILITY_SCOPE}
        $<BUILD_INTERFACE:${SCL_UTILITY_ROOT}/src>
        $<INSTALL_INTERFACE:include>
)

# ---- Add implementation sources to the target normally ----
target_sources(scl-${MODULE} PRIVATE
    ${SCL_UTILITY_PRIVATE_SOURCES}
)

# Prefer CMake 3.23+ FILE_SET HEADERS so IDEs (VS, Xcode) attach headers to the target itself
# IMPORTANT:
# - The HEADERS file set must always keep a single scope across the entire project.
# - We use PUBLIC for the HEADERS file set in this target.
# - Do NOT add private headers to the HEADERS file set; that causes scope conflicts.
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.23")
    target_sources(scl-${MODULE} ${SCL_UTILITY_SCOPE}
        FILE_SET HEADERS
            BASE_DIRS ${SCL_UTILITY_SRC_DIR}
            FILES ${SCL_UTILITY_PUBLIC_HEADERS}
    )
endif()

# --- IDE-only aggregation target (Qt Creator, etc., will show these files) ---
add_custom_target(scl_utility ALL
    SOURCES
        ${SCL_UTILITY_ALL_HEADERS}
        ${SCL_UTILITY_PRIVATE_SOURCES}
)

# ---- Group files nicely in IDEs (Visual Studio/Xcode) ----
source_group(TREE "${SCL_UTILITY_SRC_DIR}"
    FILES
        ${SCL_UTILITY_ALL_HEADERS}
        ${SCL_UTILITY_PRIVATE_SOURCES}
)
