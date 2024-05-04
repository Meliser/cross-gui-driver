cmake_minimum_required(VERSION 3.10)

project(cgd-java-manual-init)

set(IMPL_LIB cgd-java-manual-init-impl)
set(OUTPUT_IMPL_LIB_NAME lib${IMPL_LIB}.so)

find_package(JNI REQUIRED)
find_package(Java 11 COMPONENTS Development REQUIRED)
include(UseJava)

add_jar(java-interface src/main/java/org/kds/Plugin.java GENERATE_NATIVE_HEADERS java-interface-native DESTINATION)

set(${IMPL_LIB}_src
    src/main/native/src/jni.cpp
)
add_library(${IMPL_LIB} SHARED ${${IMPL_LIB}_src})
target_link_libraries(${IMPL_LIB} java-interface-native cgd-server)

add_custom_target(${PROJECT_NAME} ALL
COMMAND cmake -E copy java-interface.jar ${PROJECT_NAME}.jar
COMMAND cmake -E copy ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_IMPL_LIB_NAME} ${CMAKE_CURRENT_BINARY_DIR}/native/x86/linux/${OUTPUT_IMPL_LIB_NAME}
COMMAND ${Java_JAR_EXECUTABLE} uf  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.jar native/x86/linux/${OUTPUT_IMPL_LIB_NAME}
DEPENDS ${IMPL_LIB}
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.jar
        DESTINATION ${CGD_INSTALL_DIR}/init/manual/java
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
