# Generate benchmarks for utility: one benchmark target per subdirectory under
# module/utility/benchmark/*
# Supports Google Benchmark depending on availability/toggles.

include(${CMAKE_SOURCE_DIR}/project/cmake/SCLDependencies.cmake)

get_filename_component(SCL_UTILITY_BENCHMARK_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../benchmark" ABSOLUTE)

if (EXISTS "${SCL_UTILITY_BENCHMARK_DIR}")
    file(GLOB SCL_UTILITY_BENCHMARK_SUBDIRS LIST_DIRECTORIES true "${SCL_UTILITY_BENCHMARK_DIR}/*")

    foreach(subdir IN LISTS SCL_UTILITY_BENCHMARK_SUBDIRS)
        if (IS_DIRECTORY "${subdir}")
            get_filename_component(name "${subdir}" NAME)

            # --- Google Benchmark ---
            if (BENCHMARK_FOUND)
                file(GLOB_RECURSE srcs CONFIGURE_DEPENDS
                    "${subdir}/*gbench.c" "${subdir}/*gbench.cc" "${subdir}/*gbench.cpp"
                )
                if (srcs)
                    set(tgt "utility_${name}_gbench")
                    add_executable(${tgt} ${srcs})
                    target_link_libraries(${tgt} PRIVATE scl::utility benchmark::benchmark_main)
                    target_compile_features(${tgt} PRIVATE cxx_std_20)
                    target_include_directories(${tgt} PRIVATE "${SCL_UTILITY_BENCHMARK_DIR}")
                endif()
            endif()
        endif()
    endforeach()
endif()
