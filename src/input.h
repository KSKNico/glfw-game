#pragma once

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"


class Input {
    public:
    static void moveCameraCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    Input(GLFWwindow& window, Camera& camera);

    GLFWwindow& window;
    Camera& camera;
};