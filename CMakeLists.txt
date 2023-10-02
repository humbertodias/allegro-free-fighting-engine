cmake_minimum_required(VERSION 3.7)

project(kof2003)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_C_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(Allegro REQUIRED)
message("ALLEGRO_LIBRARY: ${ALLEGRO_LIBRARY} ALLEGRO_INCLUDE_DIR: ${ALLEGRO_INCLUDE_DIR}")

IF (APPLE)
    # Fix linking on 10.14+. See https://stackoverflow.com/questions/54068035
    LINK_DIRECTORIES(/usr/local/lib)
ENDIF ()

file(GLOB SOURCE src/*.c)
add_executable(${PROJECT_NAME} ${SOURCE})

# Add directories to the include search path
target_include_directories(${PROJECT_NAME} PRIVATE ${ALLEGRO_INCLUDE_DIR} inc)

# Link with dependencies
set(ALLEGRO_LIBRARIES alleg allegro allegro_audio allegro_main)
target_link_libraries(${PROJECT_NAME} ${ALLEGRO_LIBRARY} ${ALLEGRO_LIBRARIES})

# Binary output on ../
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/..")