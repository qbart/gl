cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(SHADERS_PATH "${CMAKE_SOURCE_DIR}/res/shaders/")

if(WIN32)
	set(CMAKE_FIND_LIBRARY_PREFIXES "")
	set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
else()
	set(CMAKE_FIND_LIBRARY_PREFIXES "lib")
	set(CMAKE_FIND_LIBRARY_SUFFIXES ".so" ".a")
endif()

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

project("GL")

file(GLOB_RECURSE SRC_FILES
	src/*.cc
	src/*.hh
)
file(GLOB_RECURSE GLM_SRC_FILES
	deps/glm/*.h
	deps/glm/*.hpp
	deps/glm/*.cpp
	deps/glm/*.inl
	deps/glm/*.h
)
file(GLOB_RECURSE GLFW_SRC_FILES
	deps/glfw-3.3/include/*.h
)

source_group("src" FILES ${SRC_FILES})
source_group("glm" FILES ${GLM_SRC_FILES})
source_group("glfw" FILES ${GLFW_SRC_FILES})

add_definitions(
	-D_WINDOWS
	-DWIN32
	-DSHADERS_PATH="${SHADERS_PATH}"
)


add_executable(${PROJECT_NAME}
	${SRC_FILES}
	${GLM_SRC_FILES}
	${GLFW_SRC_FILES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
	src/
	deps/
	deps/glfw-3.3/include/
	${OPENGL_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
)
link_directories(${PROJECT_NAME}
	deps/glfw-3.3/lib-vs2017/
)

find_library(GLFW_LIB glfw3 HINTS deps/glfw-3.3/lib-vc2017)

target_link_libraries(${PROJECT_NAME} PRIVATE
	${GLFW_LIB}
	${GLEW_LIBRARIES}
	${OPENGL_LIBRARIES}

)
