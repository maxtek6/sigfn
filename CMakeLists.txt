cmake_minimum_required(VERSION 3.0)

project(sigfn)

option(TESTS "Build test suite" OFF)
option(COVER "Add code coverage" OFF)
option(EXAMPLES "Build SigFn examples" OFF)

set(SIGFN_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(src)

if(TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()

if(EXAMPLES)
    add_subdirectory(examples)
endif()