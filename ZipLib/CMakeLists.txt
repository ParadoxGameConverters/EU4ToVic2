cmake_minimum_required(VERSION 3.5)
#set(CMAKE_VERBOSE_MAKEFILE on)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
add_compile_options("-std=c++17")
add_compile_options("-lstdc++fs")
add_compile_options("-pthread")
add_compile_options("-O2")

project(ZIPLIB)
SET(CMAKE_CXX_COMPILER g++-8)

file(GLOB CPP_FILES *.cpp)
file(GLOB DETAIL_FILES detail/*.cpp)
file(GLOB BZIP2_FILES extlibs/bzip2/*.c)
file(GLOB LZMA_UNIX_FILES extlibs/lzma/unix/*.c)
file(GLOB LZMA_UNIX_UTIL_FILES extlibs/lzma/unix/LzmaUtil/*.c)
file(GLOB ZLIB_FILES extlibs/zlib/*.c)

add_library(${PROJECT_NAME} STATIC 
${CPP_FILES}
${DETAIL_FILES}
${BZIP2_FILES}
${LZMA_UNIX_FILES}
${LZMA_UNIX_UTIL_FILES}
${ZLIB_FILES})

target_link_libraries(${PROJECT_NAME} Threads::Threads)
target_include_directories (${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})