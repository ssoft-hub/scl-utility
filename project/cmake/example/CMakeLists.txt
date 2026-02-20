# Build one executable per subdirectory under module/utility/example/*

get_filename_component(SCL_UTILITY_EXAMPLE_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../example" ABSOLUTE)

if (EXISTS "${SCL_UTILITY_EXAMPLE_DIR}")
    file(GLOB SCL_UTILITY_EXAMPLE_SUBDIRS LIST_DIRECTORIES true "${SCL_UTILITY_EXAMPLE_DIR}/*")

    foreach(subdir IN LISTS SCL_UTILITY_EXAMPLE_SUBDIRS)
        if (IS_DIRECTORY "${subdir}")
            get_filename_component(name "${subdir}" NAME)
            file(GLOB_RECURSE srcs CONFIGURE_DEPENDS
                "${subdir}/*.h" "${subdir}/*.hh" "${subdir}/*.hpp"
                "${subdir}/*.c" "${subdir}/*.cc" "${subdir}/*.cpp"
            )

            if (srcs)
                set(tgt "utility_${name}_example")
                add_executable(${tgt} ${srcs})
                target_link_libraries(${tgt} PRIVATE scl::utility)
                target_compile_features(${tgt} PRIVATE cxx_std_20)
            endif()
        endif()
    endforeach()
endif()
