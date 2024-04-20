cmake_minimum_required(VERSION 3.20.0)

project(cross-gui-driver-project)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17")

add_subdirectory(src)
add_subdirectory(test/qt)