add_executable(unit unit.cpp)

target_link_libraries(unit sigfn++_a)

add_test(NAME test_c_handle COMMAND unit HANDLE_C)
add_test(NAME test_c_ignore COMMAND unit IGNORE_C)
add_test(NAME test_c_reset COMMAND unit RESET_C)
add_test(NAME test_cpp_handle COMMAND unit HANDLE_CPP)
add_test(NAME test_cpp_ignore COMMAND unit IGNORE_CPP)
add_test(NAME test_cpp_reset COMMAND unit RESET_CPP)
add_test(NAME test_cpp_exception COMMAND unit EXCEPTION_CPP)