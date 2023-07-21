add_executable(unit unit.c)

target_compile_options(unit PUBLIC "-pthread")
target_link_libraries(unit sigfn_a Threads::Threads)

add_test(NAME test_handle COMMAND unit HANDLE)
add_test(NAME test_ignore COMMAND unit IGNORE)
add_test(NAME test_reset COMMAND unit RESET)