cmake_minimum_required(VERSION 3.26)

project(cross-gui-driver-interface)

add_library(${PROJECT_NAME} INTERFACE)

target_sources(${PROJECT_NAME} PUBLIC
  FILE_SET HEADERS
    FILES endpoint_handler_i.h
)

