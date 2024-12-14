add_executable(pause pause.c)

target_link_libraries(pause sigfn_a)

add_executable(pause++ pause.cpp)

target_link_libraries(pause++ sigfn_a)