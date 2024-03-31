cmake_minimum_required(VERSION 3.26)

project(qt-driver)

find_package(Qt5 COMPONENTS Widgets Core REQUIRED)
set(CMAKE_AUTOMOC ON)

set(qt_driver_src
    qt_endpoint_handler.cpp
)

#add_executable(${PROJECT_NAME} ${server_src})
add_library(${PROJECT_NAME} SHARED ${qt_driver_src})
target_link_libraries(${PROJECT_NAME} Qt5::Gui Qt5::Core Qt5::Widgets cross-gui-driver-interface)
#target_link_libraries(cross-gui-driver ${LIBHTTPSERVER_LIBRARIES} nlohmann_json::nlohmann_json Qt5::Gui Qt5::Core Qt5::Widgets)
