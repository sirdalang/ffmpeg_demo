cmake_minimum_required(VERSION 3.15)

project(ffdemo)

set (CMAKE_CXX_STANDARD 11)

# -> compile*.json
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/output)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/output)

set(USER_FFMPEG_LIB_DIR ${PROJECT_SOURCE_DIR}/thd/ffmpeg/lib)
set(USER_FFMPEG_INC_DIR ${PROJECT_SOURCE_DIR}/thd/ffmpeg/include)

set(USER_GTEST_LIB_DIR ${PROJECT_SOURCE_DIR}/thd/googletest/lib)
set(USER_GTEST_INC_DIR ${PROJECT_SOURCE_DIR}/thd/googletest/include)

link_directories(${USER_FFMPEG_LIB_DIR})
link_directories(${USER_GTEST_LIB_DIR})
link_directories(${USER_SDL_LIB_DIR})

link_libraries(avformat)
link_libraries(avcodec)
link_libraries(avutil)
link_libraries(swscale)
link_libraries(swresample)
link_libraries(gtest)
link_libraries(pthread)

include_directories(${USER_FFMPEG_INC_DIR})
include_directories(${USER_GTEST_INC_DIR})
include_directories(${PROJECT_SOURCE_DIR}/ffhelper)
include_directories(${PROJECT_SOURCE_DIR}/include)

SET(HELPER_DIR ${PROJECT_SOURCE_DIR}/ffhelper)

aux_source_directory(${HELPER_DIR} HELPER_SRC)

SET(CMAKE_BUILD_TYPE "Debug")
# SET(CMAKE_BUILD_TYPE "Release")

add_subdirectory(ffprobe_demo)
# add_subdirectory(ffplay_demo)
