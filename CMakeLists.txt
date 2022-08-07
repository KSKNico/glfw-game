cmake_minimum_required(VERSION 3.0.0)
project(glfw-game VERSION 0.1.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

list(APPEND CMAKE_PREFIX_PATH "E:/vcpkg/installed/x64-windows/share")

# list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# find_package(OpenGL REQUIRED)

find_package(glfw3 CONFIG REQUIRED)


find_package(glm CONFIG REQUIRED)
# find_package(glad REQUIRED)


add_compile_options(-g -Wall)

# file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
add_executable(${PROJECT_NAME} src/main.cpp src/triangle.cpp src/render.cpp src/camera.cpp src/glad.c)

target_link_libraries(${PROJECT_NAME} glfw glm::glm)
# list(APPEND CMAKE_PREFIX_PATH SDL2) OpenGL::GL





