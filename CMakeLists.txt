# Copyright (c) 2024 Maxtek Consulting

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.15)

project(sigfn)

find_package(Threads REQUIRED)

include(FetchContent)

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