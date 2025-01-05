find_package(Doxygen)

set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)

set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

configure_file(${doxyfile_in} ${doxyfile} @ONLY)

message(${PROJECT_SOURCE_DIR})

add_custom_target(docs
    COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM)