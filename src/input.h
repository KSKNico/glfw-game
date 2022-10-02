#pragma once

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"


class Input {
    public:
    static void keyCameraCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseCameraCallback(GLFWwindow* window, double xpos, double ypos);
    Input(GLFWwindow& window, Camera& camera);


    GLFWwindow& window;
    Camera& camera;

    double oldMouseX;
    double oldMouseY;
};

extern Input* pinput;