cmake_minimum_required(VERSION 3.0.0)
project(glfw-game VERSION 0.1.0)

# specify the C++ standard
SET(CMAKE_CXX_COMPILER "E:/Programme/mingw64/bin/g++.exe")


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_MODULE_PATH "E:/Programme/mingw64/x86_64-w64-mingw32/lib/cmake")

find_package(glfw3 3.3 REQUIRED)
target_link_libraries(glfw-game glfw)
# list(APPEND CMAKE_PREFIX_PATH SDL2)
find_package(OpenGL REQUIRED)
target_link_libraries(glfw-game OpenGL::GL)


# file(GLOB SOURCES src/main.cpp)
add_executable(glfw-game src/main.cpp)
