cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)

project(segment)

#https://stackoverflow.com/questions/36702888/download-an-image-from-an-url-using-curl-in-c

# add opencv external dependency
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/3rdparty)
set(OpenCV_DIR ${EXTERNAL_INSTALL_LOCATION}/opencv/lib/cmake/opencv4)
find_package(OpenCV 4.4.0 REQUIRED PATHS ${EXTERNAL_INSTALL_LOCATION}/opencv)

set(curl_DIR ${EXTERNAL_INSTALL_LOCATION}/curl/lib/cmake/CURL)
find_package(curl 7.72.0 REQUIRED PATHS ${EXTERNAL_INSTALL_LOCATION}/curl)

# set source directories
set(PROJECT_SOURCE_DIR .)
set(INCLUDE_DIR include)

# set source files
file(GLOB_RECURSE SRCS ${PROJECT_SOURCE_DIR}/*)
file(GLOB_RECURSE HEADERS ${INCLUDE_DIR}/*)

# build library
include_directories(${EXTERNAL_INSTALL_LOCATION}/opencv/include/opencv4)
include_directories(${EXTERNAL_INSTALL_LOCATION}/curl/include)
include_directories(${INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${SRCS} ${HEADERS})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} curl)