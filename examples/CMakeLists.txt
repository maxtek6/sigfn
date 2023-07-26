add_executable(pause pause.c)

target_link_libraries(pause sigfn_a)

add_executable(sigchain sigchain.cpp)

target_link_libraries(sigchain sigfn++_a)