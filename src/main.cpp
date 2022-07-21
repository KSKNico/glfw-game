#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>

#include "shader.h"
#include "render.h"


int main(int argc, char* argv[]) {
    std::cout << "STARTING" << std::endl;
    if (!glfwInit()) {
        std::cout << "glfw initialization failed" << std::endl;
    }
    GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
    if (!window) {
        std::cout << "Window or OpenGL context creation failed" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);


    init();
    while (!glfwWindowShouldClose(window)) {
        render(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}