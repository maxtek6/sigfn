cmake_minimum_required(VERSION 3.0)

project(sigfn)

option(TESTS "Build test suite" OFF)

set(SIGFN_INCLUDE ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)

if(TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()