cmake_minimum_required(VERSION 3.0)

project(project)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/externals)

execute_process(
    COMMAND ${CMAKE_COMMAND} ${CMAKE_CURRENT_SOURCE_DIR}/externals
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/externals
)

execute_process(
    COMMAND make
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/externals
)

add_subdirectory ("application")
