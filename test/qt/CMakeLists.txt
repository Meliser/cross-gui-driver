cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(qt_app)

find_package(Qt5 COMPONENTS Widgets Core REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(qt_app_src 
    main.cpp
    dialog.cpp
)

add_executable(${PROJECT_NAME} ${qt_app_src})
target_link_libraries(${PROJECT_NAME} Qt5::Gui Qt5::Core Qt5::Widgets Boost::filesystem)