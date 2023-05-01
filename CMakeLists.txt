cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
project(glfw-game VERSION 0.1.0)

set(CMAKE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

set(OpenGL_GL_PREFERENCE "GLVND")


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
  src/skybox.cpp
  src/chunk.cpp
  src/util.cpp
  src/std_image.cpp
  src/jsoncpp.cpp)

option(LLVM_SANITIZE "Enable LLVM sanitizers" ON)
if (${LLVM_SANITIZE})
  message("LLVM Sanitize enabled")
  add_compile_options(-fno-omit-frame-pointer -fno-sanitize-recover=all -fsanitize=address,undefined)
  add_link_options(-fuse-ld=lld, -Wl, --gdb-index, -fsanitize=address,undefined)
endif()

target_link_libraries(${PROJECT_NAME} glfw glm::glm)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_compile_features(${PROJECT_NAME} PRIVATE c_std_11)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall)


set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_NAME_DEBUG "glfw-game_debug")
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_NAME_RELEASE "glfw-game_release")

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
# list(APPEND CMAKE_PREFIX_PATH SDL2) OpenGL::GL

if(CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()