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

include(FetchContent)

FetchContent_Declare(
    Maxtest
    GIT_REPOSITORY https://github.com/maxtek6/maxtest.git
    GIT_TAG master
)
FetchContent_MakeAvailable(Maxtest)

add_executable(
    unit 
    unit.cpp
    ${SIGFN_SOURCES})

set_property(TARGET unit PROPERTY CXX_STANDARD 17)

target_include_directories(unit PRIVATE ${SIGFN_INCLUDE} ${CMAKE_CURRENT_SOURCE_DIR}/../src ${CMAKE_BINARY_DIR}/_deps/channels-src)

if(SIGFN_COVER)
    if(WIN32)
        message("skipping code coverage for windows")
        target_link_libraries(unit PRIVATE Maxtest::Maxtest Threads::Threads)
    else()
        target_compile_options(unit PRIVATE -fprofile-arcs -ftest-coverage -g -O0)
        target_link_libraries(unit PRIVATE Maxtest::Maxtest gcov "--coverage")        
        add_custom_target(
            cover
            DEPENDS unit)
        add_custom_command(
            TARGET cover
            COMMAND gcovr -r ${CMAKE_CURRENT_SOURCE_DIR}/.. -e ${CMAKE_CURRENT_SOURCE_DIR})
        endif()
else()
    target_link_libraries(unit PRIVATE Maxtest::Maxtest Threads::Threads)
endif()

maxtest_add_test(unit sigfn_handle "")
maxtest_add_test(unit sigfn_ignore "")
maxtest_add_test(unit sigfn_reset "")
maxtest_add_test(unit sigfn_wait "")
maxtest_add_test(unit sigfn_wait_for "")
maxtest_add_test(unit sigfn_wait_until "")
maxtest_add_test(unit sigfn_error "")
maxtest_add_test(unit sigfn::handle "")
maxtest_add_test(unit sigfn::ignore "")
maxtest_add_test(unit sigfn::reset "")
maxtest_add_test(unit sigfn::wait "")
maxtest_add_test(unit sigfn::wait_for "")
maxtest_add_test(unit sigfn::wait_until "")
