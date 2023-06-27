#pragma once

#include "glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <array>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>

#include "camera.h"
#include "render.h"

namespace input {
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void init(GLFWwindow* window, Camera* camera, Renderer* renderer);
void handle();

extern GLFWwindow* window;
extern Camera* camera;
extern Renderer* renderer;
extern double oldMouseX;
extern double oldMouseY;
extern bool isFullscreen;
constexpr int KEYS = 1024;
extern std::array<bool, KEYS> pressed;

extern int oldWidth;
extern int oldHeight;
extern int oldX;
extern int oldY;
}  // namespace input
