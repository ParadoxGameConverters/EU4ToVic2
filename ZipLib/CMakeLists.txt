cmake_minimum_required(VERSION 3.5)
#set(CMAKE_VERBOSE_MAKEFILE on)
add_compile_options("-std=c++17")
add_compile_options("-lstdc++fs")
add_compile_options("-O2")

project(ZIPLIB)
SET(CMAKE_CXX_COMPILER g++-8)

file(GLOB CPP_FILES *.cpp)
file(GLOB CPP_FILES detail/*.cpp)
file(GLOB CPP_FILES extlibs/bzip2/*.c)
file(GLOB CPP_FILES extlibs/lzma/*.c)
file(GLOB CPP_FILES extlibs/lzma/unix/*.c)
file(GLOB CPP_FILES extlibs/lzma/unix/LzmaUtil/*.c)
file(GLOB CPP_FILES extlibs/zlib/*.c)

add_library(${PROJECT_NAME} STATIC ${CPP_FILES})
