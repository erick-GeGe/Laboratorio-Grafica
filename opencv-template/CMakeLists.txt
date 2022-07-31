cmake_minimum_required(VERSION 3.0.0)
project(opencv-project)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# include_directories("./include/**")
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(opencv-project main.cpp)
target_link_libraries( opencv-project ${OpenCV_LIBS})