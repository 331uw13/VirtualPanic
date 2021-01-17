cmake_minimum_required(VERSION 3.18.2)
project(virtual-panic)

set(WARNINGS "-Wall -Wextra -Wunused-function -Wformat")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNINGS} -lSDL2 -lSDL2_image")
file(GLOB_RECURSE SOURCE_FILES "./src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "./src/*.hpp" "./src/*.h")

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

install(
	DIRECTORY ./src/imgui/
	DESTINATION include/VirtualPanic/imgui
)


