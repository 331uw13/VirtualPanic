cmake_minimum_required(VERSION 3.13.4)
project(virtual-panic C)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

set(WARNINGS "-Wall -Wextra -Wunused-function -Wformat -Wformat-overflow")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WARNINGS}")
file(GLOB_RECURSE SOURCE_FILES "./src/*.c")
file(GLOB_RECURSE SOURCE_HEADER_FILES "./src/*.h")

add_library(VirtualPanic ${SOURCE_FILES})

include(GNUInstallDirs)
install(TARGETS VirtualPanic DESTINATION lib)
install(FILES ${SOURCE_HEADER_FILES} DESTINATION include/VirtualPanic)

