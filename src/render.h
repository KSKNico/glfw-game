#pragma once

#include "glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <vector>
#include <array>
#include <algorithm>
#include <glm/matrix.hpp>
#include <iostream>

#include "block.h"
#include "camera.h"
#include "world.h"
// #include "shader.h"

#include <stb_image.h>

class Renderer {
    public:
        Renderer(GLFWwindow& window, Camera& camera, World& world);
        void init();
        void render();
        void createGeometry();


    private:
        GLFWwindow& window;
        Camera& camera;
        World& world;
        std::vector<Block> objects;
        GLuint shader_programme;
        GLuint fs;
        GLuint vs;
        GLuint vertexBuffer;
        GLuint colorBuffer;
        GLuint textureCoordinatesBuffer;
        GLuint indexBuffer;
        GLuint vao;
        GLuint texture;
        std::array<glm::vec3, 8> points;
        std::array<glm::vec3, 8> colors;
        std::array<glm::vec2, 8> textureCoordinates;
        std::array<GLuint, 36> indices;
        glm::mat4 perspectiveMatrix;

};