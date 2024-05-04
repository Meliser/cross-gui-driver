cmake_minimum_required(VERSION 3.10)

project(cgd-autoloaders)

# set(Boost_DEBUG ON)
# find_package(Boost COMPONENTS ALL)
find_package(Boost REQUIRED COMPONENTS filesystem)

add_library(injector SHARED injector.cpp)
target_link_libraries(injector Boost::filesystem)
set_target_properties(injector PROPERTIES CXX_VISIBILITY_PRESET hidden)
#target_link_libraries(cross-gui-driver ${LIBHTTPSERVER_LIBRARIES} nlohmann_json::nlohmann_json)
