#pragma once

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <vector>
#include <array>
#include <algorithm>
#include <glm/matrix.hpp>

#include "block.h"
#include "camera.h"
// #include "shader.h"

class Renderer {
    public:
        Renderer(GLFWwindow& window, Camera& camera);
        void init();
        void render();
        void createGeometry();


    private:
        GLFWwindow& window;
        Camera& camera;
        std::vector<Block> objects;
        GLuint shader_programme;
        GLuint fs;
        GLuint vs;
        GLuint vertexBuffer;
        GLuint colorBuffer;
        GLuint indexBuffer;
        GLuint vao;
        std::array<glm::vec3, 8> points;
        std::array<glm::vec3, 8> colors;
        std::array<GLuint, 36> indices;

};