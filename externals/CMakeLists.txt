cmake_minimum_required(VERSION 3.0)

project(externals)

# third party
include(ExternalProject)
get_filename_component(MAIN_BUILD_DIR ${CMAKE_BINARY_DIR} DIRECTORY)
set(EXTERNAL_INSTALL_LOCATION ${MAIN_BUILD_DIR}/3rdparty)
set(EXTERNAL_DOWNLOAD_LOCATION ${CMAKE_BINARY_DIR}/Download)

# OpenCV
ExternalProject_Add(opencv-contrib
  GIT_REPOSITORY https://github.com/opencv/opencv_contrib.git
  GIT_TAG 5fae408
  SOURCE_DIR ${EXTERNAL_DOWNLOAD_LOCATION}/opencv-contrib
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
)

ExternalProject_Add(opencv
  GIT_REPOSITORY https://github.com/opencv/opencv.git
  GIT_TAG c3bb57a
  SOURCE_DIR ${EXTERNAL_DOWNLOAD_LOCATION}/opencv
  CMAKE_ARGS
    -DBUILD_DOCS:BOOL=OFF
    -DBUILD_EXAMPLES:BOOL=OFF
    -DBUILD_NEW_PYTHON_SUPPORT:BOOL=OFF
    -DBUILD_opencv_python:BOOL=OFF
    -DBUILD_opencv_java:BOOL=OFF
    -DBUILD_PACKAGE:BOOL=OFF
    -DBUILD_SHARED_LIBS:BOOL=ON
    -DBUILD_TESTS:BOOL=OFF
    -DCMAKE_BUILD_TYPE:STRING=Release
    -DWITH_FFMPEG:BOOL=OFF
    -DCMAKE_INSTALL_PREFIX:PATH=${EXTERNAL_INSTALL_LOCATION}/opencv
    -DOPENCV_ENABLE_NONFREE:BOOL=ON
    -DOPENCV_EXTRA_MODULES_PATH:PATH=${EXTERNAL_DOWNLOAD_LOCATION}/opencv-contrib/modules
)

add_dependencies(opencv opencv-contrib)

ExternalProject_Add(curl
  GIT_REPOSITORY https://github.com/curl/curl.git
  GIT_TAG 9d954e4
  SOURCE_DIR ${EXTERNAL_DOWNLOAD_LOCATION}/curl
  CMAKE_ARGS
    -DCMAKE_USE_OPENSSL:BOOL=OFF
    -DCMAKE_BUILD_TYPE:STRING=Release
    -DCMAKE_INSTALL_PREFIX:PATH=${EXTERNAL_INSTALL_LOCATION}/curl
)