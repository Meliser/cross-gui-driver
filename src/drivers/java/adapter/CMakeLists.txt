cmake_minimum_required(VERSION 3.10)

project(java-driver-adapter)

find_package(JNI REQUIRED)

set(java_driver_src
    java_endpoint_handler.cpp
    bridge.cpp
)

add_library(${PROJECT_NAME} SHARED ${java_driver_src})
target_include_directories(${PROJECT_NAME} PRIVATE ${JNI_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} cross-gui-driver-interface ${JNI_LIBRARIES})

