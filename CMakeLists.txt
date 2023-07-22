cmake_minimum_required(VERSION 3.0)

project(sigfn)

set(SIGFN_INCLUDE ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)

include(CTest)
enable_testing()
add_subdirectory(test)