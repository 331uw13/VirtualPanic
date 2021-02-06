cmake_minimum_required(VERSION 3.13.4)
project(virtual-panic)

set(WARNINGS "-Wall -Wextra -Wunused-function -Wformat")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNINGS} -lSDL2 -lGL")
file(GLOB_RECURSE SOURCE_FILES "./src/*.cpp" "./src/*.c")
file(GLOB_RECURSE HEADER_FILES "./src/*.hpp")

find_package(OpenGL REQUIRED)

add_library (
	VirtualPanic
	${SOURCE_FILES}
)

include(GNUInstallDirs)

install(
	TARGETS VirtualPanic
	DESTINATION lib
)

install(
	FILES ${HEADER_FILES}
	DESTINATION include/VirtualPanic
)

#install(
#	DIRECTORY ./src/ImGui
#	DESTINATION include/VirtualPanic/
#)


