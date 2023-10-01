cmake_minimum_required (VERSION 3.7)
project (koflinux)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

#find_package(Threads REQUIRED)
find_package(Allegro REQUIRED)

set(CMAKE_C_STANDARD 90)
#set(CMAKE_C_EXTENSIONS OFF )

file(GLOB SOURCE src/*.c)
add_executable(${PROJECT_NAME} ${SOURCE})

#add_definitions(-DSO=LINUX)
# Add directories to the include search path
target_include_directories(${PROJECT_NAME} PRIVATE ${ALLEGRO_INCLUDE_DIR} inc)
# Link with dependencies
target_link_libraries(${PROJECT_NAME} ${ALLEGRO_LIBRARY})