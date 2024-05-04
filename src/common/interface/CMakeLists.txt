cmake_minimum_required(VERSION 3.10)

project(cgd-interface)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE .)

target_sources(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/endpoint_handler_i.h
)

