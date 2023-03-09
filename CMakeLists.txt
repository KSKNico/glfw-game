cmake_minimum_required(VERSION 3.0.0)
project(glfw-game VERSION 0.1.0)

set(CMAKE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/release)

set(OpenGL_GL_PREFERENCE "GLVND")

list(APPEND CMAKE_PREFIX_PATH "E:/vcpkg/installed/x64-windows/share")
list(APPEND CMAKE_PREFIX_PATH "E:/vcpkg/installed/x86-windows/share")


# list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(glfw3 3.3 REQUIRED)
find_package(glm CONFIG REQUIRED)

# file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
add_executable(
  ${PROJECT_NAME}
  src/glad.c
  src/main.cpp
  src/block.cpp
  src/render.cpp
  src/camera.cpp
  src/input.cpp
  src/world.cpp
  src/player.cpp
  src/std_image.cpp
  src/jsoncpp.cpp)


target_link_libraries(${PROJECT_NAME} glfw glm::glm)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_compile_features(${PROJECT_NAME} PRIVATE c_std_11)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
# list(APPEND CMAKE_PREFIX_PATH SDL2) OpenGL::GL