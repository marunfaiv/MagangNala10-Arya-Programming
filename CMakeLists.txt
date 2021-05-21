cmake_minimum_required(VERSION 3.1)
project(Barunastra)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(OpenCV REQUIRED PATHS /usr/local/lib/opencv4/)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(main_prog main.cpp)
target_link_libraries(main_prog ${OpenCV_LIBS} )