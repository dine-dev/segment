cmake_minimum_required (VERSION 3.13)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/3rdparty)

set(BUILD_SHARED_LIBS OFF)
set(OpenCV_DIR ${EXTERNAL_INSTALL_LOCATION}/opencv/lib/cmake/opencv4)

find_package(OpenCV REQUIRED)
add_executable (OpenCVCMake "main.cpp")

target_include_directories(OpenCVCMake PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(OpenCVCMake PRIVATE ${OpenCV_LIBS})
