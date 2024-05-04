cmake_minimum_required(VERSION 3.20.0)

project(cgd)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17")
set(CGD_INSTALL_DIR /opt/kds/cgd CACHE STRING "CGD install dir root")

add_subdirectory(src)
add_subdirectory(test/qt)