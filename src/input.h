#pragma once

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"


namespace input {
    void keyCameraCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void mouseCameraCallback(GLFWwindow* window, double xpos, double ypos);
    void init(GLFWwindow* window, Camera* camera);

    extern GLFWwindow* window; 
    extern Camera* camera;
    extern double oldMouseX;
    extern double oldMouseY;
}
