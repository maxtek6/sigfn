add_executable(unit unit.c)

target_link_libraries(unit sigfn_a)

add_test(NAME test_handle COMMAND unit HANDLE)
add_test(NAME test_ignore COMMAND unit IGNORE)
add_test(NAME test_reset COMMAND unit RESET)