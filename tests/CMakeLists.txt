add_executable(
    unit 
    unit.cpp
    ${CMAKE_SOURCE_DIR}/src/sigfn.c
    ${CMAKE_SOURCE_DIR}/src/sigfn.cpp)

target_include_directories(unit PRIVATE ${SIGFN_INCLUDE})

if(SIGFN_COVER)
    if(WIN32)
        message("skipping code coverage for windows")
    else()
        target_compile_options(unit PRIVATE -fprofile-arcs -ftest-coverage -g -O0)
        target_link_libraries(unit PRIVATE gcov)        
        add_custom_target(
            cover
            DEPENDS unit)
        add_custom_command(
            TARGET cover
            COMMAND gcovr -r ${CMAKE_CURRENT_SOURCE_DIR}/.. -e ${CMAKE_CURRENT_SOURCE_DIR})
        endif()
endif()

add_test(NAME test_c_handle COMMAND unit HANDLE_C)
add_test(NAME test_c_ignore COMMAND unit IGNORE_C)
add_test(NAME test_c_reset COMMAND unit RESET_C)
add_test(NAME test_cpp_handle COMMAND unit HANDLE_CPP)
add_test(NAME test_cpp_ignore COMMAND unit IGNORE_CPP)
add_test(NAME test_cpp_reset COMMAND unit RESET_CPP)