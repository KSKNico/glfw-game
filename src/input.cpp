#include "input.h"


Input::Input(GLFWwindow& window, Camera& camera) : window(window), camera(camera) {
    glfwSetKeyCallback(&window, moveCameraCallback);
}

void Input::moveCameraCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        pcamera->moveCamera(glm::vec3(0.0, 1.0, 0.0), 0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        pcamera->moveCamera(glm::vec3(0.0, -1.0, 0.0), 0.1f);
    }
    // if (key == GLFW_KEY_A && action == GLFW_PRESS) {
    //     pcamera->moveCamera(glm::vec3(0.0, -1.0, 0.0), 1.0f);
    // }
}