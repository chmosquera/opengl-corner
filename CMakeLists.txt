cmake_minimum_required(VERSION 3.15)
project(OpenGL_Corner)

file(GLOB_RECURSE SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/*.c
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    )

file(GLOB_RECURSE HEADER_FILES
    ${CMAKE_SOURCE_DIR}/src/*.h
    ${CMAKE_SOURCE_DIR}/src/*.hpp
)

# Add .lib files
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# We need a CMAKE_DIR with some code to find external dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# OpenGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

# GLFW
find_package(GLFW3 REQUIRED)
# add_library(GLFW SHARED IMPORTED "lib/libglfw.3.3.dylib")

# GLEW
find_package(GLEW REQUIRED)
# add_library(GLEW SHARED IMPORTED "lib/libGLEW.2.2.0.dylib")

# GLAD
add_library(GLAD "lib/glad.c")

# Put all libraries into a variable
set(LIBS
    ${GLFW3_LIBRARY}
    GLAD
    # GLEW
)

# Define the include DIRs
include_directories(
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/include"
	"${CMAKE_SOURCE_DIR}/include/GLFW"
	"${CMAKE_SOURCE_DIR}/include/GL"
    "${CMAKE_SOURCE_DIR}/include/glad"
    "${CMAKE_SOURCE_DIR}/include/KHR"
)

# Define the link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})
