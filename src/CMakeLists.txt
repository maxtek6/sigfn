# Copyright (c) 2025 Maxtek Consulting

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

FetchContent_Declare(
    Channels
    GIT_REPOSITORY https://github.com/johnpatek/channels.git
    GIT_TAG master
)
FetchContent_MakeAvailable(Channels)

add_library(sigfn SHARED ${SIGFN_SOURCES})
add_library(sigfn_a STATIC ${SIGFN_SOURCES})

target_include_directories(sigfn PUBLIC ${SIGFN_INCLUDE} ${CMAKE_BINARY_DIR}/_deps/channels-src)
target_include_directories(sigfn_a PUBLIC ${SIGFN_INCLUDE} ${CMAKE_BINARY_DIR}/_deps/channels-src)

set_property(TARGET sigfn PROPERTY CXX_STANDARD 17)
set_property(TARGET sigfn_a PROPERTY CXX_STANDARD 17)

target_link_libraries(sigfn PUBLIC Threads::Threads)
target_link_libraries(sigfn_a PUBLIC Threads::Threads)

if(WIN32)
    target_compile_options(sigfn PRIVATE /W4 /WX)
    target_compile_options(sigfn_a PRIVATE /W4 /WX)
    target_compile_definitions(sigfn PRIVATE WIN32_LEAN_AND_MEAN)
    target_compile_definitions(sigfn_a PRIVATE WIN32_LEAN_AND_MEAN)
else()
    target_compile_options(sigfn PRIVATE -O3 -Wall -Wextra -Wpedantic -march=native)
    target_compile_options(sigfn_a PRIVATE -O3 -Wall -Wextra -Wpedantic -march=native)
endif()

add_library(SigFn::SharedLibrary ALIAS sigfn)
add_library(SigFn::StaticLibrary ALIAS sigfn_a)