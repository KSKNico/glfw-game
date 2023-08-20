cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
project(glfw-game VERSION 0.1.0)

set(CMAKE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

set(OpenGL_GL_PREFERENCE "GLVND")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)


# list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(glfw3 3.3 REQUIRED)
find_package(glm CONFIG REQUIRED)


# file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
set(SOURCE
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
  src/shader.cpp
  src/textureCubemap.cpp
  src/textureArray.cpp
  src/texture.cpp
  src/util.cpp
  src/integer_vec3_hasher.cpp
  src/vertex.cpp
  src/std_image.cpp
  src/jsoncpp.cpp)

add_executable(
  ${PROJECT_NAME}
  ${SOURCE}
  )

if(MSVC OR MSYS OR MINGW)
    # for Windows operating system in general
    # windows needs static linking of stdc++ for some reason
    target_link_libraries(${PROJECT_NAME} glfw glm::glm -static-libstdc++)
endif()
if (UNIX)
    # for Linux, BSD, Solaris, Minix
    target_link_libraries(${PROJECT_NAME} glfw glm)
endif()

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_compile_features(${PROJECT_NAME} PRIVATE c_std_11)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall)


set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_NAME_DEBUG "glfw-game_debug")
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_NAME_RELEASE "glfw-game_release")

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
# list(APPEND CMAKE_PREFIX_PATH SDL2) OpenGL::GL

#add_subdirectory(test)

# if(CMAKE_EXPORT_COMPILE_COMMANDS)
#   set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
# endif()

