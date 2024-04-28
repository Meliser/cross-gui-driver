cmake_minimum_required(VERSION 3.10)

project(cross-gui-driver)

set (CMAKE_MODULE_PATH /usr/local/share/cmake/Modules/)
find_package(LibHttpServer REQUIRED)
# set(Boost_DEBUG ON)
# find_package(Boost COMPONENTS ALL)
find_package(Boost REQUIRED COMPONENTS filesystem)

add_subdirectory(json-3.11.3)
add_subdirectory(common)
add_subdirectory(drivers)

set(server_src 
    main.cpp
    endpoints/session.cpp
    endpoints/title.cpp
    endpoints/window_rect.cpp
)

#add_executable(${PROJECT_NAME} ${server_src})
add_library(${PROJECT_NAME} SHARED ${server_src})
target_link_libraries(${PROJECT_NAME} ${LIBHTTPSERVER_LIBRARIES} nlohmann_json::nlohmann_json Boost::filesystem cross-gui-driver-interface)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)

add_library(injector SHARED injector.cpp)
target_link_libraries(injector Boost::filesystem)
set_target_properties(injector PROPERTIES CXX_VISIBILITY_PRESET hidden)
#target_link_libraries(cross-gui-driver ${LIBHTTPSERVER_LIBRARIES} nlohmann_json::nlohmann_json)
