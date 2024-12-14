cmake_minimum_required(VERSION 3.0)

project(sigfn)

option(SIGFN_TESTS "Build test suite" OFF)
option(SIGFN_COVER "Add code coverage" OFF)
option(SIGFN_EXAMPLES "Build SigFn examples" OFF)
option(SIGFN_DOCS "Build documentation" OFF)

set(SIGFN_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB SIGFN_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)

add_subdirectory(src)

if(SIGFN_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()

if(SIGFN_EXAMPLES)
    add_subdirectory(examples)
endif()

if(SIGFN_DOCS)
    add_subdirectory(docs)
endif()