cmake_minimum_required(VERSION 3.10)

project(cgd-server)

set (CMAKE_MODULE_PATH /usr/local/share/cmake/Modules/)
find_package(LibHttpServer REQUIRED)
# set(Boost_DEBUG ON)
# find_package(Boost COMPONENTS ALL)
find_package(Boost REQUIRED COMPONENTS filesystem)

add_subdirectory(json-3.11.3)

set(${PROJECT_NAME}_src
    main.cpp
    endpoints/session.cpp
    endpoints/title.cpp
    endpoints/window_rect.cpp
)

add_library(${PROJECT_NAME} SHARED ${cgd-server_src})
target_link_libraries(${PROJECT_NAME} ${LIBHTTPSERVER_LIBRARIES} nlohmann_json::nlohmann_json Boost::filesystem cgd-interface)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)

install(TARGETS ${PROJECT_NAME}
        DESTINATION ${CGD_INSTALL_DIR}/server
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
