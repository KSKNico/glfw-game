#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 lookatPoint) : position(position), lookatPoint(lookatPoint) {
    lookAtDirection = glm::normalize(lookatPoint - position);
    upVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

// Camera::Camera(glm::vec3 position, glm::vec3 lookAtDirection) : position(position), lookAtDirection(lookAtDirection) {
//     pcamera = this;
//     lookatPoint = position + lookAtDirection;
// }

bool Camera::isChunkInView(const Chunk& chunk) const {
    // TODO: better algorithm to check whether chunk is in view by checking if the chunk is in the frustum

    // check if at least one of the vertices is in the camera view by taking the camera direction vector and the vector from the camera to the vertex
    // and checking if the angle between them is less than 90 degrees
    for (const auto& vertex : chunk.chunkVertices) {
        glm::vec3 cameraToVertex = glm::vec3(vertex) - position;
        if (glm::dot(lookAtDirection, cameraToVertex) > 0.0f) {
            return true;
        }
    }
    return false;
}

glm::mat4 Camera::getCameraMatrix() {
    this->cameraMatrix = glm::lookAt(position, position + lookAtDirection, this->upVector);
    return cameraMatrix;
}

void Camera::moveCamera(const glm::vec3& direction, float distance) {
    position += glm::normalize(direction) * distance;
}

void Camera::rotateCameraY(float angle) {
    lookAtDirection = glm::normalize(glm::rotate(this->lookAtDirection, angle, this->upVector));
}

void Camera::rotateCameraX(float angle) {
    glm::vec3 tempLookAtDirection = lookAtDirection;
    lookAtDirection = glm::normalize(glm::rotate(this->lookAtDirection, angle, glm::cross(lookAtDirection, this->upVector)));

    // prevent the camera from flipping upside down
    if (glm::abs(glm::dot(lookAtDirection, this->upVector)) > 0.99f) {
        lookAtDirection = tempLookAtDirection;
    }
}