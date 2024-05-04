cmake_minimum_required(VERSION 3.10)

project(cgd-utils)

set(${PROJECT_NAME}_src
    elf_parser.cpp
)
add_library(${PROJECT_NAME} STATIC ${cgd-utils_src})
target_include_directories(${PROJECT_NAME} INTERFACE .)
target_sources(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/elf_parser.h
)

# todo: why?
target_compile_options(${PROJECT_NAME} PUBLIC "-fPIC")

target_link_libraries(${PROJECT_NAME})

