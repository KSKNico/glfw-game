#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 lookatPoint) {
    this->cameraMatrix = glm::lookAt(position, lookatPoint, glm::vec3(0, 1, 0));
}