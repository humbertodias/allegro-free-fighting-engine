cmake_minimum_required (VERSION 3.10)
project (koflinux)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

#find_package(Threads REQUIRED)

# Update to C99 standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS OFF)

# Add warning flags but disable some that are too noisy for this codebase
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-deprecated-declarations -Wno-stringop-overflow -Wno-unused-parameter -Wno-implicit-fallthrough -Wno-format -Wno-incompatible-pointer-types")

add_subdirectory(src)
#add_subdirectory(test)

