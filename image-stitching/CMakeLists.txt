cmake_minimum_required(VERSION 2.8) 
PROJECT (stitch)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
SET(OpenCV_LIBRARIES opencv_core opencv_highgui opencv_imgproc opencv_xfeatures2d)

set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")
add_executable(stitch stitch.cpp)
target_link_libraries(stitch ${OpenCV_LIBS} ${OpenCV_LIBRARIES})
