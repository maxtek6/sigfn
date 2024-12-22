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

target_include_directories(unit PRIVATE ${SIGFN_INCLUDE} ${CMAKE_CURRENT_SOURCE_DIR}/../src)

if(SIGFN_COVER)
    if(WIN32)
        message("skipping code coverage for windows")
        target_link_libraries(unit PRIVATE Maxtest::Maxtest)
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
    target_link_libraries(unit PRIVATE Maxtest::Maxtest)
endif()

maxtest_add_test(unit sigfn_handle "")
maxtest_add_test(unit sigfn_ignore "")
maxtest_add_test(unit sigfn_reset "")
maxtest_add_test(unit sigfn_error "")
maxtest_add_test(unit sigfn::handle "")
maxtest_add_test(unit sigfn::ignore "")
maxtest_add_test(unit sigfn::reset "")
