cmake_minimum_required(VERSION 3.10)

project(cgd-qt-driver)

find_package(Qt5 COMPONENTS Widgets Core REQUIRED)
set(CMAKE_AUTOMOC ON)

set(${PROJECT_NAME}_src
    qt_endpoint_handler.cpp
)

#add_executable(${PROJECT_NAME} ${server_src})
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_src})
target_link_libraries(${PROJECT_NAME} Qt5::Gui Qt5::Core Qt5::Widgets cgd-interface)
#target_link_libraries(cross-gui-driver ${LIBHTTPSERVER_LIBRARIES} nlohmann_json::nlohmann_json Qt5::Gui Qt5::Core Qt5::Widgets)

install(TARGETS ${PROJECT_NAME}
        DESTINATION ${CGD_INSTALL_DIR}/drivers/qt
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)