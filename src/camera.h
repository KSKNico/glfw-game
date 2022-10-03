#pragma once

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>



class Camera {
    public:
        Camera(glm::vec3 position, glm::vec3 lookatPoint);
        void moveCamera(const glm::vec3& direction, float distance);
        void rotateCameraY(float angle);
        void rotateCameraX(float angle);
        glm::mat4 getCameraMatrix();
        glm::vec3 position;
        glm::vec3 lookatPoint;
        glm::vec3 lookatDirection;
        glm::vec3 upVector;
    private:
        glm::mat4 cameraMatrix;
};

extern Camera* pcamera;