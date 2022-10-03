#include "camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 lookatPoint) : position(position), lookatPoint(lookatPoint) {
    lookatDirection = lookatPoint - position;
    upVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

// Camera::Camera(glm::vec3 position, glm::vec3 lookatDirection) : position(position), lookatDirection(lookatDirection) {
//     pcamera = this;
//     lookatPoint = position + lookatDirection;
// }


glm::mat4 Camera::getCameraMatrix() {
    this->cameraMatrix = glm::lookAt(position, position+lookatDirection, this->upVector);
    return cameraMatrix;
}

void Camera::moveCamera(const glm::vec3& direction, float distance) {
    position += glm::normalize(direction) * distance;
}

void Camera::rotateCameraY(float angle) {
    lookatDirection = glm::rotate(this->lookatDirection, angle, this->upVector);
}

void Camera::rotateCameraX(float angle) {
    lookatDirection = glm::rotate(this->lookatDirection, angle, glm::cross(lookatDirection, this->upVector));
}