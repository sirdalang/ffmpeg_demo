set(PROBE_SRC_DIR ${PROJECT_SOURCE_DIR}/ffprobe_demo)

aux_source_directory(${PROBE_SRC_DIR} PROBE_SRC)
aux_source_directory(${PROBE_SRC_DIR}/test PROBE_SRC_TEST)

include_directories(${PROBE_SRC_DIR})

add_executable(ffprobe_demo ${PROBE_SRC}
    ${PROBE_SRC_TEST}
    ${HELPER_SRC})