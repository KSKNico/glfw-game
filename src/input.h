#pragma once

#include "glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"
#include "render.h"


namespace input {
    void keyCameraCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void mouseCameraCallback(GLFWwindow* window, double xpos, double ypos);
    void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void init(GLFWwindow* window, Camera* camera, Renderer* renderer);

    extern GLFWwindow* window; 
    extern Camera* camera;
    extern Renderer* renderer;
    extern double oldMouseX;
    extern double oldMouseY;
    extern bool isFullscreen;

    extern int oldWidth;
    extern int oldHeight;
    extern int oldX;
    extern int oldY;
}
