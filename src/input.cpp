#include "input.h"

Input* pinput;

Input::Input(GLFWwindow& window, Camera& camera) : window(window), camera(camera) {
    glfwSetKeyCallback(&window, keyCameraCallback);
    glfwSetCursorPosCallback(&window, mouseCameraCallback);
    pinput = this;
    pinput->oldMouseX = 0;
    pinput->oldMouseY = 0;

}

void Input::keyCameraCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // moving
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        pcamera->moveCamera(glm::vec3(0.0, 1.0, 0.0), 0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        pcamera->moveCamera(glm::vec3(0.0, -1.0, 0.0), 0.1f);
    }



    // rotating
    // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    //     pcamera->rotateCameraY(0.1f*mouseDeltaX);
    // }
    // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    //     pcamera->rotateCameraY(-0.1f    mouseDeltaX);
    // }

    // if (key == GLFW_KEY_A && action == GLFW_PRESS) {
    //     pcamera->moveCamera(glm::vec3(0.0, -1.0, 0.0), 1.0f);
    // }
}

void Input::mouseCameraCallback(GLFWwindow* window, double mouseX, double mouseY) {
    double mouseDeltaX = mouseX - pinput->oldMouseX;
    double mouseDeltaY = mouseY - pinput->oldMouseY;

    pinput->oldMouseX = mouseX;
    pinput->oldMouseY = mouseY;

    pcamera->rotateCameraY((float) mouseDeltaX * -0.005f);
    pcamera->rotateCameraX((float) mouseDeltaY * -0.005f);
}