#pragma once

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "camera.h"


class Input {
    public:
        static void moveCameraCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void init();
    private:
        static GLFWwindow& window;
        static Camera& camera;
};