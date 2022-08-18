cmake_minimum_required(VERSION 3.15)

project (ffmpegdemo)

set (CMAKE_CXX_STANDARD 11)

# -> compile*.json
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/output)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/output)

set(USER_FFMPEG_LIB_DIR /home/test/opensrc/ffmpeg/build/output/lib)
set(USER_FFMPEG_INC_DIR /home/test/opensrc/ffmpeg/build/output/include)

aux_source_directory(. SRC_ROOT)
aux_source_directory(ffprobe_demo SRC_FFPROBE_DEMO)
aux_source_directory(ffhelper SRC_FFHELPER)

link_directories(${USER_FFMPEG_LIB_DIR})

link_libraries(avformat)
link_libraries(avcodec)
link_libraries(avutil)
link_libraries(swscale)
link_libraries(swresample)

include_directories(${USER_FFMPEG_INC_DIR})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/ffprobe_demo)
include_directories(${PROJECT_SOURCE_DIR}/ffhelper)

SET(CMAKE_BUILD_TYPE "Debug")
# SET(CMAKE_BUILD_TYPE "Release")

add_executable(${PROJECT_NAME} ${SRC_FFPROBE_DEMO} ${SRC_FFHELPER} ${SRC_ROOT})