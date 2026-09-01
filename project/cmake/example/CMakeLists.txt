# Build one executable per example under module/utility/example/.
#
# An example is the *topmost* directory in a branch that directly contains
# source files. That directory together with its entire sub-tree forms a single
# example: every source beneath it is compiled and linked into one program.
# Directories that contain only sub-directories (no direct sources) are purely
# organizational (theme / sub-theme grouping), so the tree may nest freely:
#
#     example/<theme>/<case>/...
#     example/<theme>/<subtheme>/<case>/...
#
# "Directly contains source files" is literal: a directory holding only headers
# with its sources in a sub-directory is NOT the root -- that sub-directory is.
# Keep an example's primary source at its intended root. A source placed
# directly in example/ (no theme directory) is ignored, not made an example.
#
# The target name is "utility_" followed by the example root's path relative to
# example/ with separators replaced by underscores, then "_example", e.g.
# example/hierarchy/node -> utility_hierarchy_node_example. The trailing word
# is what keeps an example target from colliding with a test target of the same
# group, and makes the target the name of its own source file.
# The example root is added to the include path so an example can reference its
# own files (e.g. #include "support/foo.h").

get_filename_component(SCL_UTILITY_EXAMPLE_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../example" ABSOLUTE)

if (EXISTS "${SCL_UTILITY_EXAMPLE_DIR}")
    file(GLOB_RECURSE SCL_UTILITY_EXAMPLE_SRCS CONFIGURE_DEPENDS
        "${SCL_UTILITY_EXAMPLE_DIR}/*.c"
        "${SCL_UTILITY_EXAMPLE_DIR}/*.cc"
        "${SCL_UTILITY_EXAMPLE_DIR}/*.cpp"
    )

    # Directories (relative to example/) that directly contain a source file.
    set(SCL_UTILITY_SRC_DIRS "")
    foreach(src IN LISTS SCL_UTILITY_EXAMPLE_SRCS)
        get_filename_component(dir "${src}" DIRECTORY)
        file(RELATIVE_PATH rel "${SCL_UTILITY_EXAMPLE_DIR}" "${dir}")
        # A source sitting directly in example/ yields an empty relative path;
        # it has no theme directory to name a target, so it is not an example.
        if (NOT rel STREQUAL "")
            list(APPEND SCL_UTILITY_SRC_DIRS "${rel}")
        endif()
    endforeach()
    list(REMOVE_DUPLICATES SCL_UTILITY_SRC_DIRS)

    # Keep only the roots: a source directory none of whose ancestors also
    # contains sources. Such an ancestor would already own this sub-tree.
    set(SCL_UTILITY_EXAMPLE_ROOTS "")
    foreach(rel IN LISTS SCL_UTILITY_SRC_DIRS)
        set(is_root TRUE)
        foreach(other IN LISTS SCL_UTILITY_SRC_DIRS)
            if (NOT rel STREQUAL other)
                # "other" is an ancestor of "rel" when rel starts with "other/".
                string(FIND "${rel}/" "${other}/" pos)
                if (pos EQUAL 0)
                    set(is_root FALSE)
                    break()
                endif()
            endif()
        endforeach()
        if (is_root)
            list(APPEND SCL_UTILITY_EXAMPLE_ROOTS "${rel}")
        endif()
    endforeach()

    foreach(rel IN LISTS SCL_UTILITY_EXAMPLE_ROOTS)
        set(root_dir "${SCL_UTILITY_EXAMPLE_DIR}/${rel}")

        # Every source/header in the example sub-tree belongs to this program.
        file(GLOB_RECURSE srcs CONFIGURE_DEPENDS
            "${root_dir}/*.c" "${root_dir}/*.cc" "${root_dir}/*.cpp"
        )
        file(GLOB_RECURSE hdrs CONFIGURE_DEPENDS
            "${root_dir}/*.h" "${root_dir}/*.hh" "${root_dir}/*.hpp"
        )

        string(REPLACE "/" "_" slug "${rel}")
        set(tgt "utility_${slug}_example")

        add_executable(${tgt} ${srcs} ${hdrs})
        target_include_directories(${tgt} PRIVATE "${root_dir}")
        target_link_libraries(${tgt} PRIVATE scl::utility)
        target_compile_features(${tgt} PRIVATE cxx_std_20)

        # An example is a program that must reach its end: running it catches a crash and a
        # failed runtime invariant, which building it alone does not.
        if (NOT CMAKE_CROSSCOMPILING)
            add_test(NAME ${tgt} COMMAND ${tgt})
        endif()
    endforeach()
endif()
