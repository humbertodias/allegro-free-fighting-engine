cmake_minimum_required (VERSION 2.6)
project (koflinux)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

#find_package(Threads REQUIRED)

set(CMAKE_C_STANDARD 90)
#set(CMAKE_C_EXTENSIONS OFF )

add_subdirectory(src)
#add_subdirectory(test)

