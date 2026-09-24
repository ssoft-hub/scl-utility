get_filename_component(SCL_UTILITY_TEST_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../test" ABSOLUTE)

if (EXISTS "${SCL_UTILITY_TEST_DIR}")
    file(GLOB SCL_UTILITY_TEST_SUBDIRS LIST_DIRECTORIES true "${SCL_UTILITY_TEST_DIR}/*")

    foreach(subdir IN LISTS SCL_UTILITY_TEST_SUBDIRS)
        if (IS_DIRECTORY "${subdir}")
            get_filename_component(name "${subdir}" NAME)

            # --- Companion shared library ---
            # Without a test framework no test target exists to link the library into.
            set(common_libs "")
            if (GTEST_FOUND OR DOCTEST_FOUND OR CATCH2_FOUND)
                file(GLOB_RECURSE shared_srcs CONFIGURE_DEPENDS "${subdir}/*_shared.cpp")
                if (shared_srcs)
                    set(shared_lib "utility_${name}_shared")
                    add_library(${shared_lib} SHARED ${shared_srcs})
                    target_link_libraries(${shared_lib} PRIVATE scl::utility)
                    target_include_directories(${shared_lib} PRIVATE "${SCL_UTILITY_TEST_DIR}")
                    list(APPEND common_libs ${shared_lib})
                endif()
            endif()

            # --- GoogleTest ---
            if (GTEST_FOUND)
                file(GLOB_RECURSE srcs CONFIGURE_DEPENDS
                    "${subdir}/*gtest.c" "${subdir}/*gtest.cc" "${subdir}/*gtest.cpp"
                )
                if (srcs)
                    set(tgt "utility_${name}_gtest")
                    add_executable(${tgt} ${srcs})
                    target_link_libraries(${tgt} PRIVATE scl::utility GTest::gtest_main ${common_libs})
                    target_compile_features(${tgt} PRIVATE cxx_std_20)
                    target_include_directories(${tgt} PRIVATE "${SCL_UTILITY_TEST_DIR}")
                    include(GoogleTest)
                    gtest_discover_tests(${tgt} DISCOVERY_TIMEOUT 30 DISCOVERY_MODE PRE_TEST)
                endif()
            endif()

            # --- doctest ---
            if (DOCTEST_FOUND)
                file(GLOB_RECURSE srcs CONFIGURE_DEPENDS
                    "${subdir}/*doctest.c" "${subdir}/*doctest.cc" "${subdir}/*doctest.cpp"
                )
                if (srcs)
                    set(tgt "utility_${name}_doctest")
                    add_executable(${tgt} ${srcs})
                    target_link_libraries(${tgt} PRIVATE scl::utility doctest::doctest ${common_libs})
                    target_compile_features(${tgt} PRIVATE cxx_std_20)
                    add_test(NAME ${tgt} COMMAND ${tgt})
                endif()
            endif()

            # --- Catch2 ---
            if (CATCH2_FOUND)
                file(GLOB_RECURSE srcs CONFIGURE_DEPENDS
                    "${subdir}/*catch2.c" "${subdir}/*catch2.cc" "${subdir}/*catch2.cpp"
                )
                if (srcs)
                    if (TARGET Catch2::Catch2WithMain)
                        set(tgt "utility_${name}_catch2")
                        add_executable(${tgt} ${srcs})
                        target_link_libraries(${tgt} PRIVATE scl::utility Catch2::Catch2WithMain ${common_libs})
                        target_compile_features(${tgt} PRIVATE cxx_std_20)
                        include(Catch)
                        catch_discover_tests(${tgt})
                    elseif (TARGET Catch2::Catch2)
                        set(tgt "utility_${name}_catch2_v2")
                        add_executable(${tgt} ${srcs})
                        target_link_libraries(${tgt} PRIVATE scl::utility Catch2::Catch2 ${common_libs})
                        target_compile_features(${tgt} PRIVATE cxx_std_20)
                        add_test(NAME ${tgt} COMMAND ${tgt})
                    endif()
                endif()
            endif()
        endif()
    endforeach()
endif()
