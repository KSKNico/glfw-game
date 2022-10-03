#include "input.h"

GLFWwindow* input::window; 
Camera* input::camera;
double input::oldMouseX;
double input::oldMouseY;

void input::init(GLFWwindow* window, Camera* camera) {
    input::window = window;
    input::camera = camera;
    glfwSetKeyCallback(window, input::keyCameraCallback);
    glfwSetCursorPosCallback(window, input::mouseCameraCallback);
    input::oldMouseX = 0;
    input::oldMouseY = 0;
}

void input::keyCameraCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // moving
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        input::camera->moveCamera(glm::vec3(0.0, 1.0, 0.0), 0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        input::camera->moveCamera(glm::vec3(0.0, -1.0, 0.0), 0.1f);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        input::camera->moveCamera(input::camera->lookatDirection, 0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        input::camera->moveCamera(input::camera->lookatDirection, -0.1f);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        input::camera->moveCamera(glm::cross( input::camera->lookatDirection, input::camera->upVector), -0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        input::camera->moveCamera(glm::cross( input::camera->lookatDirection, input::camera->upVector), 0.1f);
    }
}

void input::mouseCameraCallback(GLFWwindow* window, double mouseX, double mouseY) {
    double mouseDeltaX = mouseX - input::oldMouseX;
    double mouseDeltaY = mouseY - input::oldMouseY;

    input::oldMouseX = mouseX;
    input::oldMouseY = mouseY;

    input::camera->rotateCameraY((float) mouseDeltaX * -0.005f);
    input::camera->rotateCameraX((float) mouseDeltaY * -0.005f);
}