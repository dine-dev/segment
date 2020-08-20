
cmake_minimum_required (VERSION 3.13)
project ("OpenCV CMake Reference Sample")

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/3rdparty)
set(EXTERNAL_DOWNLOAD_LOCATION ${CMAKE_BINARY_DIR}/Download)

include(cmake/build_external.cmake)

build_external(
	opencv-contrib 
	${EXTERNAL_INSTALL_LOCATION} 
	https://github.com/opencv/opencv_contrib.git
	5fae408
    SOURCE_DIR "${EXTERNAL_DOWNLOAD_LOCATION}/opencv-contrib"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

build_external(
	opencv 
	${EXTERNAL_DOWNLOAD_LOCATION}/opencv
	https://github.com/opencv/opencv.git 
    c3bb57a
        -DBUILD_opencv_python:BOOL=OFF
        -DBUILD_opencv_java:BOOL=OFF
        -DBUILD_opencv_contrib:BOOL=ON
        -DBUILD_DOCS:BOOL=OFF
        -DBUILD_EXAMPLES:BOOL=OFF
        -DBUILD_TESTS:BOOL=OFF
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DBUILD_SHARED_LIBS:BOOL=ON
        -DBUILD_NEW_PYTHON_SUPPORT:BOOL=OFF
        -DBUILD_WITH_DEBUG_INFO:BOOL=OFF
        -DWITH_CUDA:BOOL=OFF
        -DWITH_FFMPEG:BOOL=OFF
        -DWITH_MSMF:BOOL=OFF
        -DWITH_IPP:BOOL=OFF
        -DBUILD_PERF_TESTS:BOOL=OFF
        -DBUILD_PNG:BOOL=ON
        -DBUILD_JPEG:BOOL=ON
        -DBUILD_WITH_STATIC_CRT:BOOL=OFF
        -DBUILD_FAT_JAVA_LIB:BOOL=OFF
        -DBUILD_DOCS:BOOL=OFF
        -DCMAKE_INSTALL_PREFIX:PATH=${EXTERNAL_INSTALL_LOCATION}/opencv
        -DOPENCV_ENABLE_NONFREE:BOOL=ON
        -DOPENCV_EXTRA_MODULES_PATH:PATH=${EXTERNAL_DOWNLOAD_LOCATION}/opencv-contrib/modules
)

add_subdirectory ("segment")
