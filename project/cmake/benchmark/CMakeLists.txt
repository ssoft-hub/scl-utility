get_filename_component(SCL_UTILITY_BENCHMARK_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../benchmark" ABSOLUTE)

file(GLOB SCL_UTILITY_BENCHMARK_SUBDIRS LIST_DIRECTORIES true "${SCL_UTILITY_BENCHMARK_DIR}/*")

foreach(subdir IN LISTS SCL_UTILITY_BENCHMARK_SUBDIRS)
    if (IS_DIRECTORY "${subdir}")
        get_filename_component(name "${subdir}" NAME)

        # --- Google Benchmark ---
        if (BENCHMARK_FOUND)
            file(GLOB_RECURSE srcs CONFIGURE_DEPENDS
                "${subdir}/*_gbench.c" "${subdir}/*_gbench.cc" "${subdir}/*_gbench.cpp"
            )
            if (srcs)
                set(tgt "utility_${name}_gbench")
                add_executable(${tgt} ${srcs})
                target_link_libraries(${tgt} PRIVATE scl::utility benchmark::benchmark_main)
                target_compile_features(${tgt} PRIVATE cxx_std_20)
                target_include_directories(${tgt} PRIVATE "${SCL_UTILITY_BENCHMARK_DIR}")
            endif()
        endif()

        # --- Code size ---
        # Outside the framework guard: a bare-metal target has no framework and no
        # startup code, so these sources are compiled and never linked.
        file(GLOB_RECURSE srcs CONFIGURE_DEPENDS
            "${subdir}/*_size.c" "${subdir}/*_size.cc" "${subdir}/*_size.cpp"
        )
        if (srcs)
            set(tgt "utility_${name}_size")
            add_library(${tgt} STATIC ${srcs})
            target_link_libraries(${tgt} PRIVATE scl::utility)
            target_compile_features(${tgt} PRIVATE cxx_std_20)
            target_include_directories(${tgt} PRIVATE "${SCL_UTILITY_BENCHMARK_DIR}")
        endif()
    endif()
endforeach()
