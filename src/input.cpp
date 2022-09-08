#include "input.h"


Input::Input(GLFWwindow& window, Camera& camera) : window(window), camera(camera) {
    glfwSetKeyCallback(&window, moveCameraCallback);
}

void Input::moveCameraCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        pcamera->moveCamera(glm::vec3(0.0, 1.0, 0.0), 1.0f);
    }
}