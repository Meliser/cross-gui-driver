cmake_minimum_required(VERSION 3.10)

project(cgd-javafx-driver)

set(HANDLER_LIB cgd-javafx-handler)
set(OUTPUT_LIB_NAME lib${PROJECT_NAME}.so)

find_package(JNI REQUIRED)

set(${HANDLER_LIB}_src
    java_endpoint_handler.cpp
    bridge.cpp
)
add_library(${HANDLER_LIB} SHARED ${${HANDLER_LIB}_src})
target_include_directories(${HANDLER_LIB} PRIVATE ${JNI_INCLUDE_DIRS})
target_link_libraries(${HANDLER_LIB} cgd-interface cgd-utils ${JNI_LIBRARIES})


set(CLASS_PATH ${CMAKE_CURRENT_BINARY_DIR}/driver/target/classes/org/kds/Controller.class)
add_custom_command(OUTPUT ${CLASS_PATH}
                   COMMAND cmake -E rm -rf ${CMAKE_CURRENT_BINARY_DIR}/driver
                   COMMAND cmake -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/driver ${CMAKE_CURRENT_BINARY_DIR}/driver
                   COMMAND mvn -f driver/ compile 
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/driver/src/main/java/org/kds/Controller.java)
add_custom_target(${PROJECT_NAME} ALL objcopy --add-section 
    .cgd-javafx-driver-adapter=${CLASS_PATH}
    --set-section-flags .cgd-javafx-driver-adapter=noload,readonly 
    $<TARGET_FILE:${HANDLER_LIB}> ${OUTPUT_LIB_NAME}
    DEPENDS $<TARGET_FILE:${HANDLER_LIB}> ${CLASS_PATH}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_LIB_NAME}
        DESTINATION ${CGD_INSTALL_DIR}/drivers/javafx
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)