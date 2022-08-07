#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 lookatPoint) : position(position), lookatPoint(lookatPoint) {
}

glm::mat4 Camera::getCameraMatrix() {
    this->cameraMatrix = glm::lookAt(position, lookatPoint, glm::vec3(0.0f, 1.0f, 0.0f));
    return cameraMatrix;
}