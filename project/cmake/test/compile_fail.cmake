execute_process(
    COMMAND "${CMAKE_COMMAND}" --build "${BUILD_DIR}" --target "${TARGET}" --config "${CONFIG}"
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE output
)
if (result EQUAL 0)
    message(FATAL_ERROR "The build of ${TARGET} succeeded:\n${output}")
endif()
if (NOT output MATCHES "${PATTERN}")
    message(FATAL_ERROR "The build of ${TARGET} stopped with no message matching ${PATTERN}:\n${output}")
endif()
