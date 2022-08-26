#pragma once

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
    public:
        Camera(glm::vec3 position, glm::vec3 lookatPoint);
        glm::mat4 getCameraMatrix();
        glm::vec3 position;
        glm::vec3 lookatPoint;
    private:
        glm::mat4 cameraMatrix;
};