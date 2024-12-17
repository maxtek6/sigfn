add_executable(pause pause.c)

add_executable(pause++ pause.cpp)

target_link_libraries(pause sigfn_a)

target_link_libraries(pause++ sigfn_a)

target_link_libraries(pause SigFn::StaticLibrary)

target_link_libraries(pause++ SigFn::StaticLibrary)