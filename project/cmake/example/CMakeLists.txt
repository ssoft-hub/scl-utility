# Build one executable per .cpp file under module/utility/example/*

get_filename_component(SCL_UTILITY_EXAMPLE_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../example" ABSOLUTE)

if (EXISTS "${SCL_UTILITY_EXAMPLE_DIR}")
    file(GLOB_RECURSE SCL_UTILITY_EXAMPLE_SRCS CONFIGURE_DEPENDS
        "${SCL_UTILITY_EXAMPLE_DIR}/*.c"
        "${SCL_UTILITY_EXAMPLE_DIR}/*.cc"
        "${SCL_UTILITY_EXAMPLE_DIR}/*.cpp"
    )

    foreach(src IN LISTS SCL_UTILITY_EXAMPLE_SRCS)
        get_filename_component(stem "${src}" NAME_WE)
        file(RELATIVE_PATH rel "${SCL_UTILITY_EXAMPLE_DIR}" "${src}")
        get_filename_component(subdir "${rel}" DIRECTORY)
        string(REPLACE "/" "_" subdir_slug "${subdir}")
        set(tgt "utility_${subdir_slug}_${stem}")

        add_executable(${tgt} "${src}")
        target_link_libraries(${tgt} PRIVATE scl::utility)
        target_compile_features(${tgt} PRIVATE cxx_std_20)
    endforeach()
endif()
