#include "input.h"

GLFWwindow* input::window;
Camera* input::camera;
Renderer* input::renderer;
double input::oldMouseX;
double input::oldMouseY;
bool input::isFullscreen;
int input::oldWidth;
int input::oldHeight;
int input::oldX;
int input::oldY;
std::array<bool, input::KEYS> input::pressed;

void input::init(GLFWwindow* window, Camera* camera, Renderer* renderer) {
    input::window = window;
    input::camera = camera;
    input::renderer = renderer;
    glfwSetKeyCallback(window, input::keyCallback);
    glfwSetCursorPosCallback(window, input::mouseCallback);
    glfwSetFramebufferSizeCallback(window, input::framebufferSizeCallback);
    input::oldMouseX = 0;
    input::oldMouseY = 0;
    input::isFullscreen = false;
    input::pressed = std::array<bool, KEYS>();
}

void input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_UNKNOWN) return;  // Don't accept unknown keys
    if (action == GLFW_PRESS) {
        input::pressed[key] = true;
    } else if (action == GLFW_RELEASE) {
        input::pressed[key] = false;
    }
}

void input::handle() {
    for (int i = 0; i < KEYS; ++i) {
        if (!input::pressed[i]) {
            continue;
        }
        switch (i) {
            case GLFW_KEY_SPACE:
                input::camera->moveCamera(glm::vec3(0.0, 1.0, 0.0), 0.1f);
                break;
            case GLFW_KEY_LEFT_SHIFT:
                input::camera->moveCamera(glm::vec3(0.0, -1.0, 0.0), 0.1f);
                break;
            case GLFW_KEY_W:
                input::camera->moveCamera(input::camera->lookAtDirection, 0.1f);
                break;
            case GLFW_KEY_S:
                input::camera->moveCamera(input::camera->lookAtDirection, -0.1f);
                break;
            case GLFW_KEY_A:
                input::camera->moveCamera(glm::cross(input::camera->lookAtDirection, input::camera->upVector), -0.1f);
                break;
            case GLFW_KEY_D:
                input::camera->moveCamera(glm::cross(input::camera->lookAtDirection, input::camera->upVector), 0.1f);
                break;
            case GLFW_KEY_F11:
                GLFWmonitor* monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);

                if (!input::isFullscreen) {
                    glfwGetWindowSize(window, &input::oldWidth, &input::oldHeight);
                    glfwGetWindowPos(window, &input::oldX, &input::oldY);
                    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
                } else {
                    glfwSetWindowMonitor(window, NULL, input::oldX, input::oldY, input::oldWidth, input::oldHeight, mode->refreshRate);
                }

                isFullscreen = !isFullscreen;
                break;
        }
    }
}

void input::mouseCallback(GLFWwindow* window, double mouseX, double mouseY) {
    double mouseDeltaX = mouseX - input::oldMouseX;
    double mouseDeltaY = mouseY - input::oldMouseY;

    input::oldMouseX = mouseX;
    input::oldMouseY = mouseY;

    input::camera->rotateCameraY((float)mouseDeltaX * -0.005f);
    input::camera->rotateCameraX((float)mouseDeltaY * -0.005f);
}

void input::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    input::renderer->setPerspectiveMatrix(width, height);
    glViewport(0, 0, width, height);
}