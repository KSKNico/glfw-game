#include "input.h"

void Input::init() {
    glfwSetKeyCallback(&window, Input::moveCameraCallback);
}

void Input::moveCameraCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        camera.moveCamera(camera, glm::vec3(0.f,1.f,0.f), 1.f);
}