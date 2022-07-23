cmake_minimum_required(VERSION 3.0.0)
project(glfw-game VERSION 0.1.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# find_package(OpenGL REQUIRED)

find_package(glfw3 REQUIRED)


find_package(glm REQUIRED)
# find_package(glad REQUIRED)

# file(GLOB SOURCES src/main.cpp)
add_executable(${PROJECT_NAME} src/main.cpp src/triangle.cpp src/render.cpp src/glad.c)

target_link_libraries(${PROJECT_NAME} glfw glm::glm)
# list(APPEND CMAKE_PREFIX_PATH SDL2) OpenGL::GL



