# add_executable(
#     unit 
#     unit.cpp
#     ${SIGFN_SOURCES})

# target_include_directories(unit PRIVATE ${SIGFN_INCLUDE} ${CMAKE_CURRENT_SOURCE_DIR}/../src)

# if(SIGFN_COVER)
#     if(WIN32)
#         message("skipping code coverage for windows")
#     else()
#         target_compile_options(unit PRIVATE -fprofile-arcs -ftest-coverage -g -O0)
#         target_link_libraries(unit PRIVATE gcov "--coverage")        
#         add_custom_target(
#             cover
#             DEPENDS unit)
#         add_custom_command(
#             TARGET cover
#             COMMAND gcovr -r ${CMAKE_CURRENT_SOURCE_DIR}/.. -e ${CMAKE_CURRENT_SOURCE_DIR})
#         endif()
# endif()

# add_test(NAME sigfn_handle COMMAND unit sigfn_handle )
# add_test(NAME sigfn_ignore COMMAND unit sigfn_ignore)
# add_test(NAME sigfn_reset COMMAND unit sigfn_reset)
# add_test(NAME sigfn_error COMMAND unit sigfn_error)
# add_test(NAME sigfn::handle COMMAND unit sigfn::handle)
# add_test(NAME sigfn::ignore COMMAND unit sigfn::ignore)
# add_test(NAME sigfn::reset COMMAND unit sigfn::reset)

add_library(threadpool_a STATIC threadpool.cpp)
target_include_directories(threadpool_a PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(threadpool SHARED threadpool.cpp)
target_include_directories(threadpool PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(threadpool INTERFACE)

target_include_directories(threadpool INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})